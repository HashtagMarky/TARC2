#include "global.h"
#include "gba/defines.h"
#include "data.h"
#include "decompress.h"
#include "sprite.h"
#include "script.h"
#include "event_data.h"
#include "field_message_box.h"
#include "field_mugshot.h"
#include "field_weather.h"
#include "follower_helper.h"
#include "menu.h"
#include "task.h"
#include "window.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/field_mugshots.h"
#include "data/field_mugshots.h"

static EWRAM_DATA u8 sFieldMugshotSpriteIds[2] = {};
static EWRAM_DATA u8 sIsFieldMugshotActive = 0;
static EWRAM_DATA u8 sFieldMugshotSlot = 0;

#define MUGSHOT_NPC_X           200
#define MUGSHOT_NPC_Y           72
#define MUGSHOT_PMD_X           192
#define MUGSHOT_PMD_Y           72

static const struct WindowTemplate sMugshotWindowNPC =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 5,
    .width = 8,
    .height = 8,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sMugshotWindowPokemon =
{
    .bg = 0,
    .tilemapLeft = 23,
    .tilemapTop = 8,
    .width = 5,
    .height = 5,
    .paletteNum = 15,
    .baseBlock = 1,
};

static void SpriteCB_FieldMugshot(struct Sprite *s);
static void Task_MugshotWindow(u8 taskId);

static const struct OamData sFieldMugshot_Oam = {
    .size = SPRITE_SIZE(64x64),
    .shape = SPRITE_SHAPE(64x64),
    .priority = 0,
};

static const struct SpriteTemplate sFieldMugshotMsgBox_SpriteTemplate = {
    .tileTag = TAG_MUGSHOT,
    .paletteTag = TAG_MUGSHOT,
    .oam = &sFieldMugshot_Oam,
    .callback = SpriteCB_FieldMugshot,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};

static const struct SpriteTemplate sFieldMugshotSprite_SpriteTemplate = {
    .tileTag = TAG_MUGSHOT,
    .paletteTag = TAG_MUGSHOT,
    .oam = &sFieldMugshot_Oam,
    .callback = SpriteCallbackDummy,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};

static void SpriteCB_FieldMugshot(struct Sprite *s)
{
    if (s->data[0] == TRUE)
    {
        s->invisible = FALSE;
    }
    else
    {
        s->invisible = TRUE;
    }
}

struct MugshotDetails gActiveMugshotDetails;

void SetMugshotDetails(u8 mugshotType, u16 mugshotId, u8 mugshotEmotion, s16 x, s16 y, u8 windowType)
{
    gActiveMugshotDetails.mugshotType = mugshotType;
    gActiveMugshotDetails.mugshotId = mugshotId;
    gActiveMugshotDetails.mugshotEmotion = mugshotEmotion;
    gActiveMugshotDetails.x = x;
    gActiveMugshotDetails.y = y;
    gActiveMugshotDetails.windowType = windowType;
}

struct MugshotDetails GetMugshotDetails(void)
{
    return gActiveMugshotDetails;
}

static void Task_MugshotWindow(u8 taskId) {}

#define tWindowId   data[0]

void RemoveFieldMugshotAndWindow(void)
{
    RemoveFieldMugshot(FALSE);
}

void RemoveFieldMugshot(bool8 retainDetails)
{
    u8 windowTask = FindTaskIdByFunc(Task_MugshotWindow);  // Find the task responsible for the window
    u8 windowId = WINDOW_NONE;

    ResetPreservedPalettesInWeather();
    if (sFieldMugshotSpriteIds[0] != SPRITE_NONE)
    {
        // Clear and remove the window if valid
        if (windowTask != TASK_NONE && retainDetails != TRUE)
            windowId = gTasks[windowTask].tWindowId;
            ClearStdWindowAndFrame(windowId, TRUE);
            RemoveWindow(windowId);
            DestroyTask(windowTask);

        DestroySpriteAndFreeResources(&gSprites[sFieldMugshotSpriteIds[0]]);
        sFieldMugshotSpriteIds[0] = SPRITE_NONE;
    }
    if (sFieldMugshotSpriteIds[1] != 0xFF)
    {
        // Clear and remove the window if valid
        if (windowTask != TASK_NONE && retainDetails != TRUE)
            windowId = gTasks[windowTask].tWindowId;
            ClearStdWindowAndFrame(windowId, TRUE);
            RemoveWindow(windowId);
            DestroyTask(windowTask);

        DestroySpriteAndFreeResources(&gSprites[sFieldMugshotSpriteIds[1]]);
        sFieldMugshotSpriteIds[1] = SPRITE_NONE;
    }
    sIsFieldMugshotActive = FALSE;
}

void _RemoveFieldMugshot(u8 slot)
{
    ResetPreservedPalettesInWeather();
    if (sFieldMugshotSpriteIds[slot ^ 1] != SPRITE_NONE)
    {
        gSprites[sFieldMugshotSpriteIds[slot ^ 1]].data[0] = FALSE; // same as setting visibility
    }
    if (sFieldMugshotSpriteIds[slot] != SPRITE_NONE)
    {
        gSprites[sFieldMugshotSpriteIds[slot]].data[0] = FALSE; // same as setting visibility
        DestroySpriteAndFreeResources(&gSprites[sFieldMugshotSpriteIds[slot]]);
        sFieldMugshotSpriteIds[slot] = SPRITE_NONE;
    }
}

void CreateFollowerFieldMugshot(u32 followerSpecies, u32 followerEmotion, bool8 shiny, bool8 emotePMD)
{
    u16 mugshotId;
    u8 mugshotEmotion;
     
    mugshotId = followerSpecies + OBJ_EVENT_GFX_SPECIES(NONE);
    if (shiny)
        mugshotId += SPECIES_SHINY_TAG;

    if (emotePMD)
        mugshotEmotion = followerEmotion;
    else
        switch (followerEmotion)
        {
        case FOLLOWER_EMOTION_HAPPY:
            mugshotEmotion = EMOTE_HAPPY;
            break;
        case FOLLOWER_EMOTION_SAD:
            mugshotEmotion = EMOTE_SAD;
            break;
        case FOLLOWER_EMOTION_UPSET:
            mugshotEmotion = EMOTE_TEARY;
            break;
        case FOLLOWER_EMOTION_ANGRY:
            mugshotEmotion = EMOTE_ANGRY;
            break;
        case FOLLOWER_EMOTION_PENSIVE:
            mugshotEmotion = EMOTE_WORRIED;
            break;
        case FOLLOWER_EMOTION_LOVE:
        case FOLLOWER_EMOTION_MUSIC:
            mugshotEmotion = EMOTE_JOYOUS;
            break;
        case FOLLOWER_EMOTION_SURPRISE:
            mugshotEmotion = EMOTE_STUNNED;
            break;
        case FOLLOWER_EMOTION_CURIOUS:
            mugshotEmotion = EMOTE_WORRIED;
            break;
        case FOLLOWER_EMOTION_POISONED:
            mugshotEmotion = EMOTE_PAIN;
            break;
        case FOLLOWER_EMOTION_NEUTRAL:
        default:
            mugshotEmotion = EMOTE_NORMAL;
            break;
        }

    CreateFieldMugshot(MUGSHOT_FOLLOWER, mugshotId, mugshotEmotion, 0, 0, FALSE);
}

void CreateFieldMugshot(u8 mugshotType, u16 mugshotId, u8 mugshotEmotion, s16 x, s16 y, bool8 retainDetails)
{
    u8 windowTask = TASK_NONE;
    u8 windowId = WINDOW_NONE;
    u8 windowType = WINDOW_NONE;
    u32 slot = sFieldMugshotSlot;

    // Verification that sprite isn't placed offscreen.
    // The +32 makes it so the defined x & y position are the top left.
    if (!retainDetails)
    {
        x = (x > 176) ? 176 : x;
        x += 32;
    
        y = (y > 96) ? 96 : y;
        y += 32;
    }

    struct SpriteTemplate temp = sFieldMugshotSprite_SpriteTemplate;
    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=slot+TAG_MUGSHOT };
    struct SpritePalette pal = { .tag = sheet.tag };

    _RemoveFieldMugshot(slot ^ 1);
    if (sIsFieldMugshotActive)
    {
        FreeSpriteTilesByTag(slot + TAG_MUGSHOT);
        FreeSpritePaletteByTag(slot + TAG_MUGSHOT);
    }

    if ((mugshotId >= NELEMS(sFieldMugshots)
        && gSaveBlock2Ptr->optionsFollowerMugshotPlaceholder == TRUE && mugshotType == MUGSHOT_FOLLOWER))
        mugshotId = MUGSHOT_SUBSTITUTE_DOLL;

    if (mugshotId >= NELEMS(sFieldMugshots)
        || FlagGet(FLAG_SUPPRESS_MUGSHOT)
        || (gSaveBlock2Ptr->optionsSuppressNPCMugshots == TRUE && mugshotType != MUGSHOT_FOLLOWER)
        || (gSaveBlock2Ptr->optionsSuppressFollowerMugshots == TRUE && mugshotType == MUGSHOT_FOLLOWER))
            return;

    temp.tileTag = sheet.tag;
    temp.paletteTag = sheet.tag;

    if ((sFieldMugshots[mugshotId][mugshotEmotion].gfx != NULL && sFieldMugshots[mugshotId][mugshotEmotion].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[mugshotId][mugshotEmotion].pal;
    }
    else if ((sFieldMugshots[mugshotId][EMOTE_NORMAL].gfx != NULL && sFieldMugshots[mugshotId][EMOTE_NORMAL].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId][EMOTE_NORMAL].gfx;
        pal.data = sFieldMugshots[mugshotId][EMOTE_NORMAL].pal;
    }
    else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx != NULL && sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal;
    }
    else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].gfx != NULL && sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].gfx;
        pal.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].pal;
    }
    else if (mugshotType == MUGSHOT_FOLLOWER && gSaveBlock2Ptr->optionsFollowerMugshotPlaceholder == TRUE)
    {
        sheet.data = sFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].pal;
    }
    else
    {
        return;
    }
    
    // Create windows based on the mugshot type
    if (mugshotType != MUGSHOT_DEFINED)
    {
        windowTask = CreateTask(Task_MugshotWindow, 0);  // Create a new task and get its task ID

        if (mugshotType == MUGSHOT_NPC)
        {
            windowId = AddWindow(&sMugshotWindowNPC);
            x = MUGSHOT_NPC_X;
            y = MUGSHOT_NPC_Y;
            windowType = MUGSHOT_NPC;
        }
        else if (mugshotType == MUGSHOT_PMD || mugshotType == MUGSHOT_FOLLOWER)
        {
            windowId = AddWindow(&sMugshotWindowPokemon);
            x = MUGSHOT_PMD_X;
            y = MUGSHOT_PMD_Y;
            windowType = MUGSHOT_PMD;
        }

        gTasks[windowTask].tWindowId = windowId;  // Store windowId in task's data
        DrawStdWindowFrame(windowId, FALSE);
        CopyWindowToVram(windowId, 3);
    }

    // Logic for loading and creating the sprite
    LoadSpritePalette(&pal);
    LoadCompressedSpriteSheet(&sheet);

    sFieldMugshotSpriteIds[slot] = CreateSprite(&temp, x, y, 0);
    if (sFieldMugshotSpriteIds[slot] == SPRITE_NONE)
    {
        windowType = WINDOW_NONE;
        RemoveWindow(windowId);  // Clean up if sprite creation fails
        DestroyTask(windowTask);  // Destroy the task if sprite creation fails
        return;
    }

    SetMugshotDetails(mugshotType, mugshotId, mugshotEmotion, x, y, windowType);
    PreservePaletteInWeather(gSprites[sFieldMugshotSpriteIds[slot]].oam.paletteNum + 0x10);
    gSprites[sFieldMugshotSpriteIds[slot]].data[0] = FALSE;
    sIsFieldMugshotActive = TRUE;
    sFieldMugshotSlot ^= 1;
}

#undef  tWindowId

u8 GetFieldMugshotSpriteId(void)
{
    return sFieldMugshotSpriteIds[sFieldMugshotSlot ^ 1];
}

u8 IsFieldMugshotActive(void)
{
    return sIsFieldMugshotActive;
}

u8 CreateFieldMugshotSprite(u16 mugshotId, u8 mugshotEmotion)
{
    u32 slot = sFieldMugshotSlot;
    
    struct SpriteTemplate temp = sFieldMugshotSprite_SpriteTemplate;
    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=slot+TAG_MUGSHOT };
    struct SpritePalette pal = { .tag = sheet.tag };

    mugshotId += OBJ_EVENT_GFX_SPECIES(NONE);
    if (gSpecialVar_0x8005 == 1)
        mugshotId += SPECIES_SHINY_TAG;

    RemoveFieldMugshotAndWindow();

    if (mugshotId >= NELEMS(sFieldMugshots))
        mugshotId = MUGSHOT_SUBSTITUTE_DOLL;

    temp.tileTag = sheet.tag;
    temp.paletteTag = sheet.tag;

    if ((sFieldMugshots[mugshotId][mugshotEmotion].gfx != NULL && sFieldMugshots[mugshotId][mugshotEmotion].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[mugshotId][mugshotEmotion].pal;
    }
    else if ((sFieldMugshots[mugshotId][EMOTE_NORMAL].gfx != NULL && sFieldMugshots[mugshotId][EMOTE_NORMAL].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId][EMOTE_NORMAL].gfx;
        pal.data = sFieldMugshots[mugshotId][EMOTE_NORMAL].pal;
    }
    else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx != NULL && sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal;
    }
    else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].gfx != NULL && sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].pal != NULL))
    {
        sheet.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].gfx;
        pal.data = sFieldMugshots[mugshotId - SPECIES_SHINY_TAG][EMOTE_NORMAL].pal;
    }
    else
    {
        sheet.data = sFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].pal;
    }

    LoadSpritePalette(&pal);
    LoadCompressedSpriteSheet(&sheet);

    sFieldMugshotSpriteIds[slot] = CreateSprite(&temp, 0, 0, 0);
    if (sFieldMugshotSpriteIds[slot] == SPRITE_NONE)
    {
        return NULL;
    }
    PreservePaletteInWeather(gSprites[sFieldMugshotSpriteIds[slot]].oam.paletteNum + 0x10);
    return sFieldMugshotSpriteIds[slot];
}

void SetFieldMugshotSpriteId(u32 value)
{
    sFieldMugshotSpriteIds[0] = value;
    sFieldMugshotSpriteIds[1] = value;
}