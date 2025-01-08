#include "global.h"
#include "menu.h"
#include "string_util.h"
#include "task.h"
#include "event_data.h"
#include "text.h"
#include "match_call.h"
#include "field_message_box.h"
#include "field_mugshot.h"
#include "pokedex.h"
#include "script.h"
#include "sprite.h"
#include "text_window.h"
#include "constants/event_objects.h"
#include "constants/species.h"

#include "ikigai_characters.h"
#include "text.h"

static EWRAM_DATA u8 sFieldMessageBoxMode = 0;
EWRAM_DATA u8 gWalkAwayFromSignpostTimer = 0;
EWRAM_DATA const u8* gSpeakerName = NULL;

static void ExpandStringAndStartDrawFieldMessage(const u8 *, bool32);
static void StartDrawFieldMessage(void);

void InitFieldMessageBox(void)
{
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
    gTextFlags.canABSpeedUpPrint = FALSE;
    gTextFlags.useAlternateDownArrow = FALSE;
    gTextFlags.autoScroll = FALSE;
    gTextFlags.forceMidTextSpeed = FALSE;
}

#define tState data[0]

static void Task_DrawFieldMessage(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
        case 0:
            if (gMsgIsSignPost)
                LoadSignPostWindowFrameGfx();
            else
                LoadMessageBoxAndBorderGfx();
            task->tState++;
            break;
           task->tState++;
           break;
        case 1:
            DrawDialogueFrame(0, TRUE);
            if (gSpeakerName != NULL && !FlagGet(FLAG_SUPPRESS_SPEAKER_NAME))
            {
                DrawNamePlate(WIN_NAME_PLATE, FALSE);
            }
            task->tState++;
           break;
        case 2:
            if (RunTextPrintersAndIsPrinter0Active() != TRUE)
            {
                sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
                DestroyTask(taskId);
            }
    }
}

#undef tState

static void CreateTask_DrawFieldMessage(void)
{
    CreateTask(Task_DrawFieldMessage, 0x50);
}

static void DestroyTask_DrawFieldMessage(void)
{
    u8 taskId = FindTaskIdByFunc(Task_DrawFieldMessage);
    if (taskId != TASK_NONE)
        DestroyTask(taskId);
}

bool8 ShowFieldMessage(const u8 *str)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    ExpandStringAndStartDrawFieldMessage(str, TRUE);
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_NORMAL;
    return TRUE;
}

static void Task_HidePokenavMessageWhenDone(u8 taskId)
{
    if (!IsMatchCallTaskActive())
    {
        sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
        DestroyTask(taskId);
    }
}

bool8 ShowPokenavFieldMessage(const u8 *str)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    StringExpandPlaceholders(gStringVar4, str);
    CreateTask(Task_HidePokenavMessageWhenDone, 0);
    StartMatchCallFromScript(str);
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_NORMAL;
    return TRUE;
}

bool8 ShowFieldAutoScrollMessage(const u8 *str)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_AUTO_SCROLL;
    ExpandStringAndStartDrawFieldMessage(str, FALSE);
    return TRUE;
}

static bool8 UNUSED ForceShowFieldAutoScrollMessage(const u8 *str)
{
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_AUTO_SCROLL;
    ExpandStringAndStartDrawFieldMessage(str, TRUE);
    return TRUE;
}

// Same as ShowFieldMessage, but instead of accepting a
// string arg it just prints whats already in gStringVar4
bool8 ShowFieldMessageFromBuffer(void)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_NORMAL;
    StartDrawFieldMessage();
    return TRUE;
}

extern void FillDialogFramePlate();
extern int GetDialogFramePlateWidth();
enum FontColor
{
    FONT_WHITE,
    FONT_GRAY,
    FONT_RED,
    FONT_GREEN,
    FONT_BLUE,
    FONT_LIGHT_BLUE,
};
static const u8 sFontColorTable[][3] =
{
    [FONT_WHITE]        = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_GRAY]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_RED]          = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_GREEN]        = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_GREEN,      TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
    [FONT_LIGHT_BLUE]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_BLUE, TEXT_COLOR_LIGHT_GRAY},
};
static void ExpandStringAndStartDrawFieldMessage(const u8 *str, bool32 allowSkippingDelayWithButtonPress)
{
    u8 fgColor = IkigaiCharacter_ReturnMessageBoxPersonalityPalette(
        ReturnIkigaiCharacter_SelectedObject()
    );

    if (gSpeakerName != NULL && !FlagGet(FLAG_SUPPRESS_SPEAKER_NAME))
    {
        int strLen = GetStringWidth(FONT_SMALL, gSpeakerName, -1);
        if (strLen > 0)
        {
            strLen = GetDialogFramePlateWidth()/2 - strLen/2;
            gNamePlateBuffer[0] = EXT_CTRL_CODE_BEGIN;
            gNamePlateBuffer[1] = EXT_CTRL_CODE_CLEAR_TO;
            gNamePlateBuffer[2] = strLen;
            StringExpandPlaceholders(&gNamePlateBuffer[3], gSpeakerName);
        }
        else
        {
            StringExpandPlaceholders(&gNamePlateBuffer[0], gSpeakerName);
        }
        FillDialogFramePlate();
        AddTextPrinterParameterizedNamePlate(WIN_NAME_PLATE, FONT_SMALL, gNamePlateBuffer, 0, NULL, fgColor, 0, 3);
        PutWindowTilemap(WIN_NAME_PLATE);
        CopyWindowToVram(WIN_NAME_PLATE, COPYWIN_FULL);
    }
    StringExpandPlaceholders(gStringVar4, str);
    AddTextPrinterForMessage(allowSkippingDelayWithButtonPress);
    CreateTask_DrawFieldMessage();
    if (IsFieldMugshotActive())
    {
        gSprites[GetFieldMugshotSpriteId()].data[0] = TRUE;
    }
}

static void StartDrawFieldMessage(void)
{
    AddTextPrinterForMessage(TRUE);
    CreateTask_DrawFieldMessage();
}

void HideFieldMessageBox(void)
{
    DestroyTask_DrawFieldMessage();
    ClearDialogWindowAndFrame(0, TRUE);
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
    gSpeakerName = NULL;
    if (IsFieldMugshotActive())
    {
        gSprites[GetFieldMugshotSpriteId()].data[0] = FALSE;
        RemoveFieldMugshotAndWindow();
    }
    HidePokedexHeaderMessage();
}

u8 GetFieldMessageBoxMode(void)
{
    return sFieldMessageBoxMode;
}

bool8 IsFieldMessageBoxHidden(void)
{
    if (sFieldMessageBoxMode == FIELD_MESSAGE_BOX_HIDDEN)
        return TRUE;
    return FALSE;
}

static void UNUSED ReplaceFieldMessageWithFrame(void)
{
    DestroyTask_DrawFieldMessage();
    DrawStdWindowFrame(0, TRUE);
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
}

void StopFieldMessage(void)
{
    DestroyTask_DrawFieldMessage();
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
}

bool8 ScrCmd_setspeaker(struct ScriptContext *ctx)
{
    const u8 *name = (const u8 *)ScriptReadWord(ctx);
    bool8 isAuto = ScriptReadByte(ctx);
    bool8 isKnown = ScriptReadByte(ctx);
    bool8 isPlayer = ScriptReadByte(ctx);

    if (!isKnown)
    {
        SetSpeakerAuto(NAME_UNKNOWN);
    }
    else if (isPlayer)
    {
        SetSpeakerName(gSaveBlock2Ptr->playerName);
    }
    else if (isAuto)
    {
        SetSpeakerAuto(gObjectEvents[gSelectedObjectEvent].graphicsId);
    }
    else
    {
        SetSpeakerName(name);
    }
    
    return FALSE;
}

void SetSpeakerName(const u8* name)
{
    gSpeakerName = name;
}

void SetSpeakerAuto(u16 graphicsId)
{
    u8 character = ReturnIkigaiCharacter_ObjectEventGraphicsId(graphicsId);

    if (graphicsId > OBJ_EVENT_GFX_SPECIES(NONE) && graphicsId < OBJ_EVENT_GFX_SPECIES(EGG))
    {
        gSpeakerName = GetSpeciesName(graphicsId - OBJ_EVENT_GFX_SPECIES(NONE));
    }
    else if (graphicsId > OBJ_EVENT_GFX_SPECIES_SHINY(NONE) && graphicsId < OBJ_EVENT_GFX_SPECIES_SHINY(EGG))
    {
        gSpeakerName = GetSpeciesName(graphicsId - OBJ_EVENT_GFX_SPECIES_SHINY(NONE));
    }
    else
    {
        gSpeakerName = gIkigaiCharactersInfo[character].name;
    }
}

void ReprintSpeakerName(void)
{
    if (gSpeakerName != NULL)
    {
        u8 colorBackup[3];
        u8 fgColor = IkigaiCharacter_ReturnMessageBoxPersonalityPalette(
            ReturnIkigaiCharacter_SelectedObject()
        );

        int strLen = GetStringWidth(FONT_SMALL, gSpeakerName, -1);
        if (strLen > 0)
        {
            strLen = GetDialogFramePlateWidth()/2 - strLen/2;
            gNamePlateBuffer[0] = EXT_CTRL_CODE_BEGIN;
            gNamePlateBuffer[1] = EXT_CTRL_CODE_CLEAR_TO;
            gNamePlateBuffer[2] = strLen;
            StringExpandPlaceholders(&gNamePlateBuffer[3], gSpeakerName);
        }
        else
        {
            StringExpandPlaceholders(&gNamePlateBuffer[0], gSpeakerName);
        }
        FillDialogFramePlate();
        SaveTextColors(&colorBackup[0], &colorBackup[1], &colorBackup[2]);
        AddTextPrinterParameterizedNamePlate(WIN_NAME_PLATE, FONT_SMALL, gNamePlateBuffer, 0, NULL, fgColor, 0, 3);
        RestoreTextColors(&colorBackup[0], &colorBackup[1], &colorBackup[2]);
    }
}