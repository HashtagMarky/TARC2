#include "global.h"
#include "gba/gba.h"
#include "cutscene.h"
#include "constants/cutscene.h"
#include "data/cutscene.h"
#include "main.h"
#include "script.h"
#include "sprite.h"
#include "task.h"

extern const u8 PetalburgCity_EventScript_CutsceneOne[];

#define SPRITE_TAG_SKIP_BUTTON 5550

static const u16 sSkipButton_Gfx[] = INCBIN_U16("graphics/cutscene/skip_button.4bpp");
static const u16 sSkipButton_Pal[] = INCBIN_U16("graphics/cutscene/skip_button.gbapal");

static const struct SpriteSheet sSkipButtonSpriteSheets[] = {
    {(const void *)sSkipButton_Gfx, sizeof(sSkipButton_Gfx), SPRITE_TAG_SKIP_BUTTON},
    {0}
};

static const union AnimCmd sSkipButtonAnim[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(8, 16),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sSkipButtonAnimTable[] = {
    sSkipButtonAnim
};

static const struct SpritePalette sSkipButtonPalette = {sSkipButton_Pal, SPRITE_TAG_SKIP_BUTTON};

static const struct OamData sSkipButtonOamData = {
    .shape = ST_OAM_H_RECTANGLE,
    .size = 2
};

static const struct SpriteTemplate sSkipButtonTemplate = {
    SPRITE_TAG_SKIP_BUTTON,
    SPRITE_TAG_SKIP_BUTTON,
    &sSkipButtonOamData,
    sSkipButtonAnimTable,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy
};


#define tCutscene data[0]
#define tSpriteId data[1]
static void Task_CutsceneTask(u8 taskId)
{
    u32 cutscene = gTasks[taskId].tCutscene;
    if (JOY_NEW(START_BUTTON))
    {
        ScriptContext_SetupScript(sCutsceneSkipScripts[cutscene].scriptCutsceneSkips);
        DestroyCutsceneTask();
    }
}

bool32 CheckCutsceneFlag(u32 cutscene)
{
    return TRUE;
}

void SetCutsceneFlag(u32 cutscene)
{
    
}

void CreateCutsceneTask(struct ScriptContext *ctx)
{
    u32 cutscene = ScriptReadByte(ctx);

    if (CheckCutsceneFlag(cutscene))
    {
        u8 taskId = CreateTask(Task_CutsceneTask, 64);
        gTasks[taskId].tCutscene = cutscene;
        LoadSpriteSheets(sSkipButtonSpriteSheets);
        LoadSpritePalette(&sSkipButtonPalette);

        u8 spriteId = CreateSprite(&sSkipButtonTemplate, 20, 50, 0xFF);
        gTasks[taskId].tSpriteId = spriteId;
        gSprites[spriteId].oam.priority = 0;
    }
    else
    {
        SetCutsceneFlag(cutscene);
    }
}

void DestroyCutsceneTask(void)
{
    u8 taskId = FindTaskIdByFunc(Task_CutsceneTask);
    if (taskId != TASK_NONE)
    {
        DestroySprite(&gSprites[gTasks[taskId].tSpriteId]);
        FreeSpriteTilesByTag(SPRITE_TAG_SKIP_BUTTON);
        FreeSpritePaletteByTag(SPRITE_TAG_SKIP_BUTTON);
        DestroyTask(taskId);
    }
}
#undef tCutscene
#undef tSpriteId

void CheckCutsceneTask(void)
{
    if (FindTaskIdByFunc(Task_CutsceneTask) != TASK_NONE)
        DebugPrintf("Active");
    else
        DebugPrintf("Inactive");
}
