#include "global.h"
#include "gba/defines.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "malloc.h"
#include "palette.h"
#include "dynamic_palettes.h"
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
#include "constants/species.h"
#include "ikigai_characters.h"

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
    .baseBlock = 220,
};

static const struct WindowTemplate sMugshotWindowPokemon =
{
    .bg = 0,
    .tilemapLeft = 23,
    .tilemapTop = 8,
    .width = 5,
    .height = 5,
    .paletteNum = 15,
    .baseBlock = 220,
};

static void SpriteCB_FieldMugshot(struct Sprite *s);
static void Task_MugshotWindow(u8 taskId);
static enum MugshotIDs GetPokemonMugshotIdFromObjectEvent(struct ObjectEvent *objEvent);

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

static const union AnimCmd sSpriteAnim_Mugshot_Flipped[] =
{
    ANIMCMD_FRAME(0, 32, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

const union AnimCmd *const gSpriteAnimTable_Mugshot_Flipped[] =
{
    sSpriteAnim_Mugshot_Flipped,
};

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

enum MugshotIDs GetMugshotIdFromObjectEvent(struct ObjectEvent *objEvent)
{
    if (IS_OW_MON_OBJ(objEvent))
    {
        return GetPokemonMugshotIdFromObjectEvent(objEvent);
    }
    else
    {
        return ReturnIkigaiMugshotID_ObjectEventGraphicsId(objEvent->graphicsId);
    }
}

static enum MugshotIDs GetPokemonMugshotIdFromObjectEvent(struct ObjectEvent *objEvent)
{
    u32 species = OW_SPECIES(objEvent);
    bool32 shiny = OW_SHINY(objEvent);
    bool32 female = OW_FEMALE(objEvent);

    if (shiny && female)
        return sPokemonMugshots[species].mugshotFemaleShinyId;
    else if (shiny)
        return sPokemonMugshots[species].mugshotShinyId;
    else if (female)
        return sPokemonMugshots[species].mugshotFemaleId;
    else
        return sPokemonMugshots[species].mugshotId;
}

static void Task_MugshotWindow(u8 taskId) {}

#define tWindowId   data[0]

void RemoveFieldMugshotAndWindow(void)
{
    RemoveFieldMugshot(FALSE);

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
}

void RemoveFieldMugshot(bool8 retainDetails)
{
    u8 windowTask = FindTaskIdByFunc(Task_MugshotWindow);  // Find the task responsible for the window
    u8 windowId = WINDOW_NONE;

    if (IndexOfSpritePaletteTag(TAG_MUGSHOT) != 0xFF)
    {
        ResetPreservedPalettesInWeather();

        // Clear and remove the window if valid
        if (windowTask != TASK_NONE && retainDetails != TRUE)
        {
            windowId = gTasks[windowTask].tWindowId;
            ClearStdWindowAndFrame(windowId, TRUE);
            RemoveWindow(windowId);
            DestroyTask(windowTask);
        }

        DestroySprite(&gSprites[sFieldMugshotSpriteId]);
        FreeSpritePaletteByTag(TAG_MUGSHOT);
        FreeSpriteTilesByTag(TAG_MUGSHOT);
        sFieldMugshotSpriteId = SPRITE_NONE;

        sIsFieldMugshotActive = FALSE;
    }
}

void CreateFollowerFieldMugshot(u32 followerSpecies, u32 followerEmotion, bool8 shiny, bool8 emotePMD)
{
    u16 mugshotId;
    u8 mugshotEmotion;
    bool32 female = FALSE;
     
    if (shiny && female)
        mugshotId = sPokemonMugshots[followerSpecies].mugshotFemaleShinyId;
    else if (shiny)
        mugshotId = sPokemonMugshots[followerSpecies].mugshotShinyId;
    else if (female)
        mugshotId = sPokemonMugshots[followerSpecies].mugshotFemaleId;
    else
        mugshotId = sPokemonMugshots[followerSpecies].mugshotId;

    if (emotePMD)
        mugshotEmotion = followerEmotion;
    else
        switch (followerEmotion)
        {
        case FOLLOWER_EMOTION_HAPPY:
            mugshotEmotion = MUGSHOT_EMOTE_HAPPY;
            break;
        case FOLLOWER_EMOTION_SAD:
            mugshotEmotion = MUGSHOT_EMOTE_SAD;
            break;
        case FOLLOWER_EMOTION_UPSET:
            mugshotEmotion = MUGSHOT_EMOTE_TEARY;
            break;
        case FOLLOWER_EMOTION_ANGRY:
            mugshotEmotion = MUGSHOT_EMOTE_ANGRY;
            break;
        case FOLLOWER_EMOTION_PENSIVE:
            mugshotEmotion = MUGSHOT_EMOTE_WORRIED;
            break;
        case FOLLOWER_EMOTION_LOVE:
        case FOLLOWER_EMOTION_MUSIC:
            mugshotEmotion = MUGSHOT_EMOTE_JOYOUS;
            break;
        case FOLLOWER_EMOTION_SURPRISE:
            mugshotEmotion = MUGSHOT_EMOTE_STUNNED;
            break;
        case FOLLOWER_EMOTION_CURIOUS:
            mugshotEmotion = MUGSHOT_EMOTE_WORRIED;
            break;
        case FOLLOWER_EMOTION_POISONED:
            mugshotEmotion = MUGSHOT_EMOTE_PAIN;
            break;
        case FOLLOWER_EMOTION_NEUTRAL:
        default:
            mugshotEmotion = MUGSHOT_EMOTE_NORMAL;
            break;
        }

    CreateFieldMugshot(MUGSHOT_FOLLOWER, mugshotId, mugshotEmotion, 0, 0, FALSE);
}

void CreateFieldMugshot(enum MugshotType mugshotType, enum MugshotIDs mugshotId, enum MugshotEmoteIDs mugshotEmotion, s16 x, s16 y, bool8 retainDetails)
{
    u8 windowTask = TASK_NONE;
    u8 windowId = WINDOW_NONE;
    u8 windowType = WINDOW_NONE;

    if (!retainDetails)
    {
        x = (x > 176) ? 176 : x;
        x += 32;
    
        y = (y > 96) ? 96 : y;
        y += 32;
    }

    // If playerGender has a value set to female, otherwise male
    if (mugshotType == MUGSHOT_PLAYER)
        mugshotId = gSaveBlock2Ptr->playerGender ? MUGSHOT_ANKA : MUGSHOT_KOLE;

    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=TAG_MUGSHOT };
    struct SpritePalette pal = { .tag = sheet.tag };

    RemoveFieldMugshot(retainDetails);

    if (mugshotEmotion >= MUGSHOT_EMOTE_COUNT)
        mugshotEmotion = MUGSHOT_EMOTE_NORMAL;

    if ((mugshotId >= NELEMS(gFieldMugshots)
        || (gSaveBlock2Ptr->optionsFollowerMugshots == MUGSHOT_FOLLOWER_PLACEHOLDER && mugshotType == MUGSHOT_FOLLOWER)))
        mugshotId = MUGSHOT_SUBSTITUTE_DOLL;

    if (mugshotType == MUGSHOT_PLAYER) {} // so MUGSHOT_PLAYER does not get checked for following conditions
    else if (mugshotId >= NELEMS(gFieldMugshots)
        || FlagGet(FLAG_SUPPRESS_MUGSHOT)
        || (gSaveBlock2Ptr->optionsSuppressNPCMugshots == TRUE && mugshotType != MUGSHOT_FOLLOWER)
        || (gSaveBlock2Ptr->optionsFollowerMugshots == MUGSHOT_FOLLOWER_OFF && mugshotType == MUGSHOT_FOLLOWER))
            return;

    if ((gFieldMugshots[mugshotId][mugshotEmotion].gfx != NULL && gFieldMugshots[mugshotId][mugshotEmotion].pal != NULL))
    {
        sheet.data = gFieldMugshots[mugshotId][mugshotEmotion].gfx;
        pal.data = gFieldMugshots[mugshotId][mugshotEmotion].pal;
    }
    else if ((gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].gfx != NULL && gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].pal != NULL))
    {
        sheet.data = gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].gfx;
        pal.data = gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].pal;
    }
    // else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx != NULL && gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal != NULL))
    // {
    //     sheet.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx;
    //     pal.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal;
    // }
    // else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].gfx != NULL && gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].pal != NULL))
    // {
    //     sheet.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].gfx;
    //     pal.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].pal;
    // }
    else if (mugshotType == MUGSHOT_FOLLOWER && gSaveBlock2Ptr->optionsFollowerMugshots == MUGSHOT_FOLLOWER_PLACEHOLDER)
    {
        sheet.data = gFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].gfx;
        pal.data = gFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].pal;
    }
    else
    {
        return;
    }
    
    if (mugshotType != MUGSHOT_DEFINED && mugshotType != MUGSHOT_NPC && mugshotType != MUGSHOT_PLAYER)
    {
        windowTask = CreateTask(Task_MugshotWindow, 0);

        if (mugshotType == MUGSHOT_NPC || mugshotType == MUGSHOT_PLAYER)
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

        gTasks[windowTask].tWindowId = windowId;
        DrawStdWindowFrame(windowId, FALSE);
        CopyWindowToVram(windowId, 3);
    }

    LoadSpritePalette(&pal);
    if (mugshotId == MUGSHOT_KOLE || mugshotId == MUGSHOT_ANKA)
        DynPal_LoadPaletteByTag(sDynPalPlayerMugshot, TAG_MUGSHOT);
    WaitForFreshVBlank();
    LoadCompressedSpriteSheet(&sheet);

    sFieldMugshotSpriteId = CreateSprite(&sFieldMugshotMsgBox_SpriteTemplate, x, y, 0);
    if (sFieldMugshotSpriteId == SPRITE_NONE)
    {
        windowType = WINDOW_NONE;
        RemoveWindow(windowId);
        DestroyTask(windowTask);
        return;
    }

    SetMugshotDetails(mugshotType, mugshotId, mugshotEmotion, x, y, windowType);
    PreservePaletteInWeather(gSprites[sFieldMugshotSpriteId].oam.paletteNum + 0x10);
    gSprites[sFieldMugshotSpriteId].data[0] = FALSE;
    sIsFieldMugshotActive = TRUE;
}

#undef  tWindowId

u8 GetFieldMugshotSpriteId(void)
{
    return sFieldMugshotSpriteId;
}

u8 IsFieldMugshotActive(void)
{
    return sIsFieldMugshotActive;
}

u8 CreateFieldMugshotSprite(enum MugshotIDs mugshotId, enum MugshotEmoteIDs mugshotEmotion, bool8 typeMon, bool8 shinyMon, u8 mugshotNum)
{
    u32 mugshotTag = TAG_MUGSHOT;
    u8 spriteId;

    mugshotTag += mugshotNum;

    struct CompressedSpriteSheet sheet = { .size = 0x1000, .tag = mugshotTag };
    struct SpritePalette pal = { .tag = sheet.tag };
    struct SpriteTemplate spriteTemplate = sFieldMugshotSprite_SpriteTemplate;

    spriteTemplate.tileTag = mugshotTag;
    spriteTemplate.paletteTag = mugshotTag;

    if (typeMon == TRUE)
    {
        bool32 female = FALSE;
        
        if (shinyMon == TRUE && female)
            mugshotId = sPokemonMugshots[mugshotId].mugshotFemaleShinyId;
        else if (shinyMon == TRUE)
            mugshotId = sPokemonMugshots[mugshotId].mugshotShinyId;
        else if (female)
            mugshotId = sPokemonMugshots[mugshotId].mugshotFemaleId;
        else
            mugshotId = sPokemonMugshots[mugshotId].mugshotId;
    }

    if (mugshotEmotion >= MUGSHOT_EMOTE_COUNT)
        mugshotEmotion = MUGSHOT_EMOTE_NORMAL;

    if (mugshotId >= NELEMS(gFieldMugshots))
        mugshotId = MUGSHOT_SUBSTITUTE_DOLL;

    if ((gFieldMugshots[mugshotId][mugshotEmotion].gfx != NULL && gFieldMugshots[mugshotId][mugshotEmotion].pal != NULL))
    {
        sheet.data = gFieldMugshots[mugshotId][mugshotEmotion].gfx;
        pal.data = gFieldMugshots[mugshotId][mugshotEmotion].pal;
    }
    else if ((gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].gfx != NULL && gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].pal != NULL))
    {
        sheet.data = gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].gfx;
        pal.data = gFieldMugshots[mugshotId][MUGSHOT_EMOTE_NORMAL].pal;
    }
    // else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx != NULL && gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal != NULL))
    // {
    //     sheet.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].gfx;
    //     pal.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][mugshotEmotion].pal;
    // }
    // else if (mugshotId > (OBJ_EVENT_GFX_SPECIES_SHINY(NONE)) && (gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].gfx != NULL && gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].pal != NULL))
    // {
    //     sheet.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].gfx;
    //     pal.data = gFieldMugshots[mugshotId - SPECIES_SHINY_TAG][MUGSHOT_EMOTE_NORMAL].pal;
    // }
    else
    {
        sheet.data = gFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].gfx;
        pal.data = gFieldMugshots[MUGSHOT_SUBSTITUTE_DOLL][mugshotEmotion].pal;
    }

    LoadSpritePalette(&pal);
    WaitForFreshVBlank();
    LoadCompressedSpriteSheet(&sheet);

    if (mugshotId == MUGSHOT_KOLE || mugshotId == MUGSHOT_ANKA)
        DynPal_LoadPaletteByTag(sDynPalPlayerMugshot, mugshotTag);

    spriteId = CreateSprite(&spriteTemplate, 0, 0, 0);
    if (spriteId == SPRITE_NONE)
    {
        return SPRITE_NONE;
    }
    PreservePaletteInWeather(gSprites[spriteId].oam.paletteNum + 0x10);
    return spriteId;
}

void DestroyFieldMugshotSprite(u8 sprite, u8 mugshotNum)
{
    DestroySprite(&gSprites[sprite]);
    FreeSpritePaletteByTag(TAG_MUGSHOT + mugshotNum);
    FreeSpriteTilesByTag(TAG_MUGSHOT + mugshotNum);
}

u16 CreatePlayerMugshotTrainerCardSprite(u8 gender, enum MugshotEmoteIDs mugshotEmotion, u16 destX, u16 destY, u8 paletteSlot, u8 windowId)
{
    enum MugshotIDs mugshotId;
    u8 *mugshotBuffer;
    const u32 *mugshotGfx;
    const u16 *mugshotPal;

    mugshotId = (gender == MALE) ? MUGSHOT_KOLE : MUGSHOT_ANKA;

    mugshotBuffer = Alloc(TRAINER_PIC_SIZE * MAX_TRAINER_PIC_FRAMES);
    if (mugshotBuffer == NULL)
        return 0xFFFF;

    if (mugshotId >= NELEMS(gFieldMugshots) || mugshotEmotion >= MUGSHOT_EMOTE_COUNT)
    {
        Free(mugshotBuffer);
        return 0xFFFF;
    }

    mugshotGfx = gFieldMugshots[mugshotId][mugshotEmotion].gfx;
    mugshotPal = gFieldMugshots[mugshotId][mugshotEmotion].pal;

    DecompressDataWithHeaderWram(mugshotGfx, mugshotBuffer);

    BlitBitmapRectToWindow(windowId, mugshotBuffer, 0, 0, TRAINER_PIC_WIDTH, TRAINER_PIC_HEIGHT, destX, destY, TRAINER_PIC_WIDTH, TRAINER_PIC_HEIGHT);

    LoadPalette(mugshotPal, BG_PLTT_ID(paletteSlot), PLTT_SIZE_4BPP);
    DynPal_LoadPaletteByOffset(sDynPalPlayerMugshot, BG_PLTT_ID(paletteSlot));

    Free(mugshotBuffer);

    return 0;
}

void SetFieldMugshotSpriteId(u32 value)
{
    sFieldMugshotSpriteId = value;
}