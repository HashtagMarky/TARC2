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
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/field_mugshots.h"
#include "data/field_mugshots.h"

static EWRAM_DATA u8 sFieldMugshotSpriteId = 0;
static EWRAM_DATA u8 sIsFieldMugshotActive = 0;

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

#define taskId        NUM_TASKS - 1
#define tWindowId     data[0]

void RemoveFieldMugshot(void)
{
    u8 windowTask = taskId;
    u8 windowId = 0;
    
    if (IndexOfSpritePaletteTag(TAG_MUGSHOT) != 0xFF)
    {
        ResetPreservedPalettesInWeather();
        /*
        windowId = AddWindow(&sMugshotWindowNPC);
        ClearStdWindowAndFrame(windowId, TRUE);
        RemoveWindow(windowId);
        windowId = AddWindow(&sMugshotWindowPokemon);
        ClearStdWindowAndFrame(windowId, TRUE);
        RemoveWindow(windowId);
        */
        DestroySprite(&gSprites[sFieldMugshotSpriteId]);
        FreeSpritePaletteByTag(TAG_MUGSHOT);
        FreeSpriteTilesByTag(TAG_MUGSHOT);
        sFieldMugshotSpriteId = SPRITE_NONE;

        if (GetTaskCount() < (NUM_TASKS - 1))
            windowId = gTasks[windowTask].tWindowId;
            ClearStdWindowAndFrame(windowId, TRUE);
            RemoveWindow(windowId);
            DestroyTask(windowTask);

        sIsFieldMugshotActive = FALSE;
    }
};

void CreateFollowerFieldMugshot(u32 followerSpecies, u32 followerEmotion, bool8 shiny)
{
    u16 mugshotId;
    u8 mugshotEmotion;
     
    mugshotId = followerSpecies + OBJ_EVENT_GFX_SPECIES(NONE);
    if (shiny)
        mugshotId += SPECIES_SHINY_TAG;

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

    CreateFieldMugshot(MUGSHOT_FOLLOWER, mugshotId, mugshotEmotion, 0, 0);
}

void CreateFieldMugshot(u8 mugshotType, u16 mugshotId, u8 mugshotEmotion, s16 x, s16 y)
{
    u8 windowTask = taskId;
    u8 windowId = 0;

    // Verification that sprite isn't placed offscreen.
    // The +32 makes it so the defined x & y position are the top left.
    x = (x > 176) ? 176 : x;
    x = x + 32;
    
    y = (y > 96) ? 96 : y;
    y = y + 32;

    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=TAG_MUGSHOT };
    struct SpritePalette pal = { .tag = sheet.tag };

    RemoveFieldMugshot();

    if ((mugshotId >= NELEMS(sFieldMugshots)
        && gSaveBlock2Ptr->optionsFollowerMugshotPlaceholder == TRUE && mugshotType == MUGSHOT_FOLLOWER))
        mugshotId = MUGSHOT_SUBSTITUTE_DOLL;

    if (mugshotId >= NELEMS(sFieldMugshots)
        || FlagGet(FLAG_SUPPRESS_MUGSHOT)
        || (gSaveBlock2Ptr->optionsSuppressNPCMugshots == TRUE && mugshotType != MUGSHOT_FOLLOWER)
        || (gSaveBlock2Ptr->optionsSuppressFollowerMugshots == TRUE && mugshotType == MUGSHOT_FOLLOWER))
            return;

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
    
    if (mugshotType != MUGSHOT_DEFINED)
    {
        if (mugshotType == MUGSHOT_NPC)
        {
            windowId = AddWindow(&sMugshotWindowNPC);
            x = MUGSHOT_NPC_X;
            y = MUGSHOT_NPC_Y;
        }
        else if (mugshotType == MUGSHOT_PMD || mugshotType == MUGSHOT_FOLLOWER)
        {
            windowId = AddWindow(&sMugshotWindowPokemon);
            x = MUGSHOT_PMD_X;
            y = MUGSHOT_PMD_Y;
        }
        
        if (GetTaskCount() < (NUM_TASKS - 1))
            gTasks[windowTask].tWindowId = windowId;
            DrawStdWindowFrame(windowId, FALSE);
            CopyWindowToVram(windowId, 3);
            DestroyTask(windowTask);
    }

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&pal);

    sFieldMugshotSpriteId = CreateSprite(&sFieldMugshotMsgBox_SpriteTemplate, x, y, 0);
    if (sFieldMugshotSpriteId == SPRITE_NONE)
    {
        return;
    }
    PreservePaletteInWeather(gSprites[sFieldMugshotSpriteId].oam.paletteNum + 0x10);
    gSprites[sFieldMugshotSpriteId].data[0] = FALSE;
    sIsFieldMugshotActive = TRUE;
}

u8 GetFieldMugshotSpriteId(void)
{
    return sFieldMugshotSpriteId;
}

u8 IsFieldMugshotActive(void)
{
    return sIsFieldMugshotActive;
}

#undef tWindowId

u8 CreateFieldMugshotSprite(u16 mugshotId, u8 mugshotEmotion)
{
    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=TAG_MUGSHOT };
    struct SpritePalette pal = { .tag = sheet.tag };

    mugshotId += OBJ_EVENT_GFX_SPECIES(NONE);
    if (gSpecialVar_0x8005 == 1)
        mugshotId += SPECIES_SHINY_TAG;

    RemoveFieldMugshot();

    if (mugshotId >= NELEMS(sFieldMugshots))
        mugshotId = MUGSHOT_SUBSTITUTE_DOLL;

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

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&pal);

    sFieldMugshotSpriteId = CreateSprite(&sFieldMugshotSprite_SpriteTemplate, 0, 0, 0);
    if (sFieldMugshotSpriteId == SPRITE_NONE)
    {
        return NULL;
    }
    PreservePaletteInWeather(gSprites[sFieldMugshotSpriteId].oam.paletteNum + 0x10);
    return sFieldMugshotSpriteId;
}