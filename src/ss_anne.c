#include "global.h"
#include "task.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "ss_anne.h"
#include "constants/songs.h"
#include "constants/vars.h"

#include "overworld.h"
#include "palette.h"

// Tasks governing the ship's departure after you've gotten HM01 CUT

#define SPRITE_TAG_WAKE  4000
#define SPRITE_TAG_SMOKE 4001

static void Task_SSAnneInit(u8 taskId);
static void Task_SSAnneRun(u8 taskId);
static void Task_SSAnneFinish(u8 taskId);
static void CreateWakeBehindBoat(void);
static void WakeSpriteCallback(struct Sprite *sprite);
static void CreateSmokeSprite(void);
static void SmokeSpriteCallback(struct Sprite *sprite);
static void CreateWakeBehindBoat_SSPathfinder(void);
static void WakeSpriteCallback_SSPathfinder(struct Sprite *sprite);
static void UNUSED SmokeSpriteCallback_SSPathfinder(struct Sprite *sprite);

static const u16 sWakeTiles[] = INCBIN_U16("graphics/ss_anne/wake.4bpp");
static const u16 sSmokeTiles[] = INCBIN_U16("graphics/ss_anne/smoke.4bpp");
static const u16 sWakePaletteData[] = INCBIN_U16("graphics/ss_anne/wake.gbapal");
static const u16 sSmokePaletteData[] = INCBIN_U16("graphics/ss_anne/smoke.gbapal");

static const struct SpriteSheet sSpriteSheets[] = {
    {(const void *)sWakeTiles, sizeof(sWakeTiles), SPRITE_TAG_WAKE},
    {(const void *)sSmokeTiles, sizeof(sSmokeTiles), SPRITE_TAG_SMOKE},
    {0}
};

static const struct SpritePalette sWakePalette = {sWakePaletteData, SPRITE_TAG_WAKE};
static const struct SpritePalette sSmokePalette = {sSmokePaletteData, SPRITE_TAG_SMOKE};

static const union AnimCmd sWakeAnim[] = {
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(8, 12),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sWakeAnimTable[] = {
    sWakeAnim
};

static const struct OamData sWakeOamData = {
    .shape = ST_OAM_V_RECTANGLE,
    .size = 2
};

static const struct SpriteTemplate sWakeSpriteTemplate = {
    SPRITE_TAG_WAKE,
    SPRITE_TAG_WAKE,
    &sWakeOamData,
    sWakeAnimTable,
    NULL,
    gDummySpriteAffineAnimTable,
    WakeSpriteCallback
};

static const struct SpriteTemplate sWakeSpriteTemplate_SSPathfinder = {
    SPRITE_TAG_WAKE,
    SPRITE_TAG_WAKE,
    &sWakeOamData,
    sWakeAnimTable,
    NULL,
    gDummySpriteAffineAnimTable,
    WakeSpriteCallback_SSPathfinder
};

static const union AnimCmd sSmokeAnim[] = {
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_FRAME( 4, 20),
    ANIMCMD_FRAME( 8, 20),
    ANIMCMD_FRAME(12, 30),
    ANIMCMD_END
};

static const union AnimCmd *const sSmokeAnimTable[] = {
    sSmokeAnim
};

static const struct OamData sSmokeOamData = {
    .shape = ST_OAM_SQUARE,
    .size = 1
};

static const struct SpriteTemplate sSmokeSpriteTemplate = {
    SPRITE_TAG_SMOKE,
    SPRITE_TAG_SMOKE,
    &sSmokeOamData,
    sSmokeAnimTable,
    NULL,
    gDummySpriteAffineAnimTable,
    SmokeSpriteCallback
};

static const struct SpriteTemplate sSmokeSpriteTemplate_SSPathfinder = {
    SPRITE_TAG_SMOKE,
    SPRITE_TAG_SMOKE,
    &sSmokeOamData,
    sSmokeAnimTable,
    NULL,
    gDummySpriteAffineAnimTable,
    SmokeSpriteCallback
};

void DoSSAnneDepartureCutscene(void)
{
    u8 taskId;

    PlaySE(SE_RG_SS_ANNE_HORN);
    taskId = CreateTask(Task_SSAnneInit, 8);
    gTasks[taskId].data[0] = 50;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
}

static void Task_SSAnneInit(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (--data[0] == 0)
    {
        LoadSpriteSheets(sSpriteSheets);
        LoadSpritePalette(&sWakePalette);
        LoadSpritePalette(&sSmokePalette);
        CreateWakeBehindBoat();
        gTasks[taskId].func = Task_SSAnneRun;
    }
}

static void Task_SSAnneRun(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    u8 objectEventId;
    struct ObjectEvent * boatObject;
    s16 x;

    data[1]++;
    data[2]++;
    if (data[1] == 70)
    {
        data[1] = 0;
        CreateSmokeSprite();
    }
    TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_0x8004, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
    boatObject = &gObjectEvents[objectEventId];
    if (gSprites[boatObject->spriteId].x + gSprites[boatObject->spriteId].x2 < -100)
    {
        PlaySE(SE_RG_SS_ANNE_HORN);
        gTasks[taskId].func = Task_SSAnneFinish;
    }
    else
    {
        x = data[2] / 5;
        gSprites[boatObject->spriteId].x2 = -x;
    }
}

static void Task_SSAnneFinish(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (++data[3] == 40)
    {
        FreeSpriteTilesByTag(SPRITE_TAG_WAKE);
        FreeSpriteTilesByTag(SPRITE_TAG_SMOKE);
        FreeSpritePaletteByTag(SPRITE_TAG_WAKE);
        FreeSpritePaletteByTag(SPRITE_TAG_SMOKE);
        DestroyTask(taskId);
        ScriptContext_Enable();
    }
}

#define tTimer   data[0]
static void Task_CreateSmokeSprite(u8 taskId)
{
    gTasks[taskId].tTimer++;
    
    if (gTasks[taskId].tTimer == 70)
    {
        gTasks[taskId].tTimer = 0;
        CreateSmokeSprite_SSPathfinder();
    }
}

void DestroySSPathfinderSmokeTask(void)
{
    DestroyTask(FindTaskIdByFunc(Task_CreateSmokeSprite));
}

void DoSSPathfinderCutscene(void)
{
    u8 taskId;
    LoadSpriteSheets(sSpriteSheets);
    LoadSpritePalette(&sWakePalette);
    LoadSpritePalette(&sSmokePalette);
    CreateWakeBehindBoat_SSPathfinder();
    taskId = CreateTask(Task_CreateSmokeSprite, 0);
    gTasks[taskId].tTimer = 69;
}
#undef tTimer

static void CreateWakeBehindBoat(void)
{
    u8 objectEventId;
    struct ObjectEvent * boatObject;
    u16 x;
    u8 spriteId;

    TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_0x8004, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
    boatObject = &gObjectEvents[objectEventId];
    x = gSprites[boatObject->spriteId].x + gSprites[boatObject->spriteId].x2 + 48;
    spriteId = CreateSprite(&sWakeSpriteTemplate, x, 109, 0xFF);
    gSprites[spriteId].oam.priority = 2;
    UpdatePalettesWithTime(PALETTES_ALL);
}

static void WakeSpriteCallback(struct Sprite *sprite)
{
    u8 objectEventId;
    struct ObjectEvent * boatObject;
    u16 x;

    TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_0x8004, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
    boatObject = &gObjectEvents[objectEventId];
    x = gSprites[boatObject->spriteId].x + gSprites[boatObject->spriteId].x2 + 48;
    sprite->x = x;
    if (sprite->data[0] / 6 < 22)
        sprite->data[0]++;
    sprite->x2 = sprite->data[0] / 6;
    if (sprite->x + sprite->x2 < -18)
        DestroySprite(sprite);
}

static void CreateSmokeSprite(void)
{
    u8 objectEventId;
    struct ObjectEvent * boatObject;
    u16 x;
    u8 UNUSED spriteId;

    TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_0x8004, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
    boatObject = &gObjectEvents[objectEventId];
    x = gSprites[boatObject->spriteId].x + gSprites[boatObject->spriteId].x2 + 25;
    if ((s16)x >= -32)
        CreateSprite(&sSmokeSpriteTemplate, x, 78, 8);
    UpdatePalettesWithTime(PALETTES_ALL);
}

static void SmokeSpriteCallback(struct Sprite *sprite)
{
    sprite->data[0]++;
    sprite->x2 = sprite->data[0] / 4;
    if (sprite->animEnded)
        DestroySprite(sprite);
}

void SSPathfinder_SetPassengers(void)
{
    VarSet(VAR_OBJ_GFX_ID_0, (Random() % 6) + 15);

    VarSet(VAR_OBJ_GFX_ID_1, (Random() % 6) + 15);
    do {
        VarSet(VAR_OBJ_GFX_ID_1, (Random() % 6) + 15);
    } while (VarGet(VAR_OBJ_GFX_ID_1) == VarGet(VAR_OBJ_GFX_ID_0));

    VarSet(VAR_OBJ_GFX_ID_2, (Random() % 4) + 23);

    VarSet(VAR_OBJ_GFX_ID_3, (Random() % 5) + 31);

    VarSet(VAR_OBJ_GFX_ID_4, (Random() % 5) + 31);
    do {
        VarSet(VAR_OBJ_GFX_ID_4, (Random() % 5) + 31);
    } while (VarGet(VAR_OBJ_GFX_ID_4) == VarGet(VAR_OBJ_GFX_ID_3));

    VarSet(VAR_OBJ_GFX_ID_5, (Random() % 5) + 44);
}

static void CreateWakeBehindBoat_SSPathfinder(void)
{
    u8 objectEventId;
    struct ObjectEvent *boatObject;
    u16 x;
    u8 spriteId;

    TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_0x8004, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
    boatObject = &gObjectEvents[objectEventId];
    x = gSprites[boatObject->spriteId].x + gSprites[boatObject->spriteId].x2 + 72;
    spriteId = CreateSprite(&sWakeSpriteTemplate_SSPathfinder, x, 77, 0xFF); // Decreased y by 16
    gSprites[spriteId].oam.priority = 2;
    gSprites[spriteId].x2 = 0; // Set to final position, no movement
    UpdatePalettesWithTime(PALETTES_ALL);
}

static void WakeSpriteCallback_SSPathfinder(struct Sprite *sprite)
{
    if (sprite->x + sprite->x2 < -18)
        DestroySprite(sprite);
}

void CreateSmokeSprite_SSPathfinder(void)
{
    u8 objectEventId;
    struct ObjectEvent *boatObject;
    u16 x;
    u8 UNUSED spriteId;

    TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_0x8004, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
    boatObject = &gObjectEvents[objectEventId];
    x = gSprites[boatObject->spriteId].x + gSprites[boatObject->spriteId].x2 + 25;
    if ((s16)x >= -32)
        CreateSprite(&sSmokeSpriteTemplate_SSPathfinder, x, 46, 8); // Decreased y by 16
    UpdatePalettesWithTime(PALETTES_ALL);
}

static void UNUSED SmokeSpriteCallback_SSPathfinder(struct Sprite *sprite)
{
    if (sprite->animEnded)
        DestroySprite(sprite);
}