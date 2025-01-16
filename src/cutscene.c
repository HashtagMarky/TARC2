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
        DestroyTask(taskId);
    }
}

void CheckCutsceneTask(void)
{
    if (FindTaskIdByFunc(Task_CutsceneTask) != TASK_NONE)
        DebugPrintf("Active");
    else
        DebugPrintf("Inactive");
}
