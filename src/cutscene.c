#include "global.h"
#include "gba/gba.h"
#include "cutscene.h"
#include "constants/cutscene.h"
#include "data/cutscene.h"
#include "main.h"
#include "script.h"
#include "task.h"

extern const u8 PetalburgCity_EventScript_CutsceneOne[];

#define tCutscene data[0]
static void Task_CutsceneTask(u8 taskId)
{
    u32 cutscene = gTasks[taskId].tCutscene;
    if (JOY_NEW(START_BUTTON))
    {
        DebugPrintf("Button Pressed");
        ScriptContext_SetupScript(sCutsceneSkipScripts[cutscene].scriptCutsceneSkips);
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
