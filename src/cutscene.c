#include "global.h"
#include "gba/gba.h"
#include "script.h"
#include "task.h"
#include "main.h"

struct cutscene
{
    const u8 *scriptCutscene;
};

extern const u8 PetalburgCity_EventScript_CutsceneOne[];
extern const u8 PetalburgCity_EventScript_CutsceneOneEnd[];
extern const u8 PetalburgCity_EventScript_CutsceneTwoEnd[];

const struct cutscene sCutsceneScripts[] =
{
    [1] = {PetalburgCity_EventScript_CutsceneOneEnd},
    [2] = {PetalburgCity_EventScript_CutsceneTwoEnd}
};
#define tCutscene data[0]
static void Task_CutsceneTask(u8 taskId)
{
    u32 cutscene = gTasks[taskId].tCutscene;
    if (JOY_NEW(START_BUTTON))
    {
        DebugPrintf("Button Pressed");
        ScriptContext_SetupScript(sCutsceneScripts[cutscene].scriptCutscene);
    }
}

void CreateCutsceneTask(struct ScriptContext *ctx)
{
    u32 cutscene = ScriptReadByte(ctx);
    u8 taskId = CreateTask(Task_CutsceneTask, 64);
    gTasks[taskId].tCutscene = cutscene;
}

void DestroyCutsceneTask(void)
{
    if (FindTaskIdByFunc(Task_CutsceneTask) != TASK_NONE)
        DestroyTask(FindTaskIdByFunc(Task_CutsceneTask));
}

void CheckCutsceneTask(void)
{
    if (FindTaskIdByFunc(Task_CutsceneTask) != TASK_NONE)
        DebugPrintf("Active");
    else
        DebugPrintf("Inactive");
}
