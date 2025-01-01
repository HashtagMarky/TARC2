#include "pokesphere.h"

#include "gba/types.h"
#include "gba/defines.h"
#include "global.h"
#include "main.h"
#include "bg.h"
#include "text_window.h"
#include "window.h"
#include "constants/characters.h"
#include "palette.h"
#include "task.h"
#include "overworld.h"
#include "malloc.h"
#include "gba/macro.h"
#include "menu_helpers.h"
#include "menu.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "constants/rgb.h"
#include "decompress.h"
#include "constants/songs.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "pokemon_icon.h"
#include "graphics.h"
#include "data.h"
#include "pokedex.h"
#include "gpu_regs.h"
#include "international_string_util.h"

#include "field_mugshot.h"
#include "ikigai_characters.h"

struct PokeSphereState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 mode;
    u8 characterId;
    u8 characterMugshotSpriteId;
    u8 partnerIconSpriteId;
};

enum WindowIds
{
    WIN_UI_CONTROLS,
    WIN_CHARACTER_NAME,
    WIN_CHARACTER_RELATIONSHIPS,
    WIN_CHARACTER_PROFILE,
    WIN_CHARACTER_MUGSHOT,
};

enum Modes
{
    MODE_PROFILE,
    MODE_POSTS,
    MODE_COUNT,
};

#define CHARACTER_MUGSHOT_X     19
#define CHARACTER_MUGSHOT_Y     19
#define CHARACTER_PARTNER_X     80
#define CHARACTER_PARTNER_Y     50

static EWRAM_DATA struct PokeSphereState *sPokeSphereState = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;

static const u8 *const sModeNames[MODE_COUNT] = {
    [MODE_PROFILE]      = COMPOUND_STRING("Profile:"),
    [MODE_POSTS]        = COMPOUND_STRING("Posts:"),
};

static const struct BgTemplate sPokeSphereBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 1
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 2
    }
};

static const struct WindowTemplate sPokeSphereWindowTemplates[] =
{
    [WIN_UI_CONTROLS] =
    {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 1,
        .width = 9,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_CHARACTER_NAME] =
    {
        .bg = 0,
        .tilemapLeft = 13,
        .tilemapTop = 2,
        .width = 6,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 1 + (9 * 5)
    },
    [WIN_CHARACTER_RELATIONSHIPS] =
    {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 7,
        .width = 8,
        .height = 12,
        .paletteNum = 15,
        .baseBlock = 46 + (3 * 6)
    },
    [WIN_CHARACTER_PROFILE] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 13,
        .width = 17,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 64 + (8 * 12)
    },
    [WIN_CHARACTER_MUGSHOT] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 13,
        .width = 17,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 160 + (17 * 6)
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sPokeSphereTiles[] = INCBIN_U32("graphics/pokesphere/tiles.4bpp.lz");

static const u32 sPokeSphereTilemap[] = INCBIN_U32("graphics/pokesphere/tilemap.bin.lz");

static const u16 sPokeSpherePalette[] = INCBIN_U16("graphics/pokesphere/tiles.gbapal");

enum FontColor
{
    FONT_WHITE,
    FONT_GRAY,
    FONT_RED,
    FONT_GREEN,
    FONT_BLUE,
};
static const u8 sPokeSphereWindowFontColors[][3] =
{
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_GRAY]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_RED]    = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_GREEN]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_GREEN,      TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
};

// Callbacks for the sample UI
static void PokeSphere_SetupCB(void);
static void PokeSphere_MainCB(void);
static void PokeSphere_VBlankCB(void);

// Sample UI tasks
static void Task_PokeSphereWaitFadeIn(u8 taskId);
static void Task_PokeSphereMainInput(u8 taskId);
static void Task_PokeSphereWaitFadeAndBail(u8 taskId);
static void Task_PokeSphereWaitFadeAndExitGracefully(u8 taskId);

// Sample UI helper functions
static void PokeSphere_Init(MainCallback callback);
static void PokeSphere_ResetGpuRegsAndBgs(void);
static bool8 PokeSphere_InitBgs(void);
static void PokeSphere_FadeAndBail(void);
static bool8 PokeSphere_LoadGraphics(void);
static void PokeSphere_InitWindows(void);
static void PokeSphere_DrawCharacterMusghot(void);
static void PokeSphere_DrawPartnerMonIcon(void);
static void PokeSphere_PrintUIControls(void);
static void PokeSphere_ReloadText(void);
static void PokeSphere_PrintNames(void);
static void PokeSphere_PrintRelationships(void);
static void PokeSphere_PrintProfile(void);
static void PokeSphere_FreeResources(void);

// Declared in sample_ui.h
void Task_OpenPokeSphere(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        PokeSphere_Init(CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

static void PokeSphere_Init(MainCallback callback)
{
    sPokeSphereState = AllocZeroed(sizeof(struct PokeSphereState));
    if (sPokeSphereState == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sPokeSphereState->loadState = 0;
    sPokeSphereState->savedCallback = callback;

    SetMainCallback2(PokeSphere_SetupCB);
}

// Credit: Jaizu, pret
static void PokeSphere_ResetGpuRegsAndBgs(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    ChangeBgX(2, 0, BG_COORD_SET);
    ChangeBgY(2, 0, BG_COORD_SET);
    ChangeBgX(3, 0, BG_COORD_SET);
    ChangeBgY(3, 0, BG_COORD_SET);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WIN1H, 0);
    SetGpuReg(REG_OFFSET_WIN1V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    CpuFill16(0, (void *)VRAM, VRAM_SIZE);
    CpuFill32(0, (void *)OAM, OAM_SIZE);
}

static void PokeSphere_SetupCB(void)
{
    switch (gMain.state)
    {
    case 0:
        PokeSphere_ResetGpuRegsAndBgs();
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 2:
        if (PokeSphere_InitBgs())
        {
            sPokeSphereState->loadState = 0;
            gMain.state++;
        }
        else
        {
            PokeSphere_FadeAndBail();
            return;
        }
        break;
    case 3:
        if (PokeSphere_LoadGraphics() == TRUE)
        {
            gMain.state++;
        }
        break;
    case 4:
        PokeSphere_InitWindows();
        gMain.state++;
        break;
    case 5:
        sPokeSphereState->characterId = MAIN_CHARACTER_SAMUEL;
        // FreeMonIconPalettes();
        // LoadMonIconPalettes();
        // PokeSphere_DrawPartnerMonIcon();
        // PokeSphere_DrawCharacterMusghot(void);
        PokeSphere_PrintUIControls();
        PokeSphere_PrintNames();
        PokeSphere_PrintRelationships();
        PokeSphere_PrintProfile();
        CreateTask(Task_PokeSphereWaitFadeIn, 0);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(PokeSphere_VBlankCB);
        SetMainCallback2(PokeSphere_MainCB);
        break;
    }
}

static void PokeSphere_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void PokeSphere_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_PokeSphereWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_PokeSphereMainInput;
    }
}

static void Task_PokeSphereMainInput(u8 taskId)
{
    if (JOY_REPEAT(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        if (sPokeSphereState->characterId == MAIN_CHARACTER_COUNT - 1)
        {
            sPokeSphereState->characterId++;
            sPokeSphereState->characterId++;
        }
        else if (sPokeSphereState->characterId == CHARACTER_COUNT_TOTAL - 1)
        {
            sPokeSphereState->characterId = CHARACTER_DEFAULT + 1;
        }
        else
        {
            sPokeSphereState->characterId++;
        }
        PokeSphere_PrintNames();
        PokeSphere_ReloadText();
    }
    if (JOY_REPEAT(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        if (sPokeSphereState->characterId == CHARACTER_DEFAULT + 1)
        {
            sPokeSphereState->characterId = CHARACTER_COUNT_TOTAL - 1;
        }
        else if (sPokeSphereState->characterId == MAIN_CHARACTER_COUNT + 1)
        {
            sPokeSphereState->characterId--;
            sPokeSphereState->characterId--;
        }
        else
        {
            sPokeSphereState->characterId--;
        }
        PokeSphere_PrintNames();
        PokeSphere_ReloadText();
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_PokeSphereWaitFadeAndExitGracefully;
    }
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
    }
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
    }
}

static void Task_PokeSphereWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sPokeSphereState->savedCallback);
        PokeSphere_FreeResources();
        DestroyTask(taskId);
    }
}

static void Task_PokeSphereWaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sPokeSphereState->savedCallback);
        PokeSphere_FreeResources();
        DestroyTask(taskId);
    }
}
#define TILEMAP_BUFFER_SIZE (1024 * 2)
static bool8 PokeSphere_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    if (sBg1TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sPokeSphereBgTemplates, NELEMS(sPokeSphereBgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    ShowBg(0);
    ShowBg(1);

    return TRUE;
}
#undef TILEMAP_BUFFER_SIZE

static void PokeSphere_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_PokeSphereWaitFadeAndBail, 0);
    SetVBlankCallback(PokeSphere_VBlankCB);
    SetMainCallback2(PokeSphere_MainCB);
}

static bool8 PokeSphere_LoadGraphics(void)
{
    switch (sPokeSphereState->loadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sPokeSphereTiles, 0, 0, 0);
        sPokeSphereState->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sPokeSphereTilemap, sBg1TilemapBuffer);
            sPokeSphereState->loadState++;
        }
        break;
    case 2:
        LoadPalette(sPokeSpherePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        sPokeSphereState->loadState++;
    default:
        sPokeSphereState->loadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void PokeSphere_InitWindows(void)
{
    InitWindows(sPokeSphereWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    
    FillWindowPixelBuffer(WIN_UI_CONTROLS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_NAME, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    PutWindowTilemap(WIN_UI_CONTROLS);
    PutWindowTilemap(WIN_CHARACTER_NAME);
    PutWindowTilemap(WIN_CHARACTER_RELATIONSHIPS);
    PutWindowTilemap(WIN_CHARACTER_PROFILE);
    
    CopyWindowToVram(WIN_UI_CONTROLS, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_NAME, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_PROFILE, COPYWIN_FULL);
}

static void PokeSphere_PrintUIControls(void)
{
    FillWindowPixelBuffer(WIN_UI_CONTROLS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    AddTextPrinterParameterized4(WIN_UI_CONTROLS, FONT_SMALL_NARROWER, 5, 1, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        COMPOUND_STRING("{DPAD_LEFTRIGHT} Change Profile"));
    AddTextPrinterParameterized4(WIN_UI_CONTROLS, FONT_SMALL_NARROWER, 5, 12, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        COMPOUND_STRING("{A_BUTTON} Change View"));
    AddTextPrinterParameterized4(WIN_UI_CONTROLS, FONT_SMALL_NARROWER, 5, 23, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        COMPOUND_STRING("{B_BUTTON} Exit"));

    CopyWindowToVram(WIN_UI_CONTROLS, COPYWIN_GFX);
}

static void PokeSphere_ReloadText(void)
{
    PokeSphere_PrintRelationships();
    PokeSphere_PrintProfile();
}

static void PokeSphere_PrintNames(void)
{   
    u8 x;

    FillWindowPixelBuffer(WIN_CHARACTER_NAME, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        gIkigaiCharactersInfo[sPokeSphereState->characterId].name,
        48
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_NAME, FONT_SMALL_NARROWER, x, 0, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        gIkigaiCharactersInfo[sPokeSphereState->characterId].name
    );
    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        gSpeciesInfo[gIkigaiCharactersInfo[sPokeSphereState->characterId].partnerPokemon].speciesName,
        48
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_NAME, FONT_SMALL_NARROWER, x, 9, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        gSpeciesInfo[gIkigaiCharactersInfo[sPokeSphereState->characterId].partnerPokemon].speciesName
    );

    CopyWindowToVram(WIN_CHARACTER_NAME, COPYWIN_GFX);
}

static void PokeSphere_PrintRelationships(void)
{
    u8 x, relationship;
    u8 y, i = 0;
    const u8 *stringBuffer;

    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    stringBuffer = COMPOUND_STRING("Tagged Profiles");
    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        stringBuffer,
        64
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS, FONT_SMALL_NARROWER, x, 2, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        stringBuffer
    );

    y = 5;
    for (i = 0; i < MAX_RELATIONSHIPS; i++)
    {
        relationship = gIkigaiCharactersInfo[sPokeSphereState->characterId].relationships[i].characterId;
        stringBuffer = gIkigaiCharactersInfo[sPokeSphereState->characterId].relationships[i].title;

        if (relationship != CHARACTER_DEFAULT)
        {
            y += 11;
            x = GetStringRightAlignXOffset(FONT_SMALL_NARROWER,
                gIkigaiCharactersInfo[relationship].name,
                64
            );
            AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS, FONT_SMALL_NARROWER, x - 5, y, 0, 0,
                sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
                gIkigaiCharactersInfo[relationship].name
            );
            y += 11;
            x = GetStringRightAlignXOffset(FONT_SMALL_NARROWER,
                stringBuffer,
                64
            );
            AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS, FONT_SMALL_NARROWER, x - 5, y, 0, 0,
                sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
                stringBuffer
            );
            y += 3;
        }
    }
    
    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS, COPYWIN_GFX);
}

static void PokeSphere_PrintProfile(void)
{
    const u8 *stringBuffer = COMPOUND_STRING("Highlights");

    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    AddTextPrinterParameterized4(WIN_CHARACTER_PROFILE, FONT_SMALL_NARROWER,
        GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
                stringBuffer,
                136
        ),
        1, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        stringBuffer
    );

    AddTextPrinterParameterized4(WIN_CHARACTER_PROFILE, FONT_SMALL_NARROWER, 5, 13, 0, 1,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        gIkigaiCharactersInfo[sPokeSphereState->characterId].highlights
    );

    CopyWindowToVram(WIN_CHARACTER_PROFILE, COPYWIN_GFX);
}

static void PokeSphere_DrawCharacterMusghot(void)
{
    u32 character = sPokeSphereState->characterId;
    u16 mughsotId = gIkigaiCharactersInfo[character].mugshotId;
    u8 mugshotEmotion = EMOTE_NORMAL;
    u8 windowColour = TEXT_COLOR_TRANSPARENT;

    switch (gIkigaiCharactersInfo[character].personality)
    {
    case ATTITUDE_CYNICAL:
        windowColour = TEXT_COLOR_LIGHT_BLUE;
        break;
        
    case ATTITUDE_DOMINANT:
        windowColour = TEXT_COLOR_RED;
        break;
        
    case ATTITUDE_HUMBLE:
        windowColour = TEXT_COLOR_GREEN;
        break;
        
    case ATTITUDE_INSPIRED:
        windowColour = TEXT_COLOR_BLUE;
        break;
    }
    FillWindowPixelBuffer(WIN_CHARACTER_MUGSHOT, PIXEL_FILL(windowColour));

    sPokeSphereState->characterMugshotSpriteId = CreateFieldMugshotSprite(mughsotId, mugshotEmotion);
    gSprites[sPokeSphereState->characterMugshotSpriteId].oam.priority = 0;
    gSprites[sPokeSphereState->characterMugshotSpriteId].x = CHARACTER_MUGSHOT_X;
    gSprites[sPokeSphereState->characterMugshotSpriteId].y = CHARACTER_MUGSHOT_Y;
}

static void PokeSphere_DrawPartnerMonIcon(void)
{
    u32 character = sPokeSphereState->characterId;
    u16 speciesId = gIkigaiCharactersInfo[character].partnerPokemon;

    sPokeSphereState->partnerIconSpriteId = CreateMonIcon(speciesId, SpriteCB_MonIcon, CHARACTER_PARTNER_X, CHARACTER_PARTNER_Y, 4, 0);
    gSprites[sPokeSphereState->partnerIconSpriteId].oam.priority = 0;
}

static void PokeSphere_FreeResources(void)
{
    if (sPokeSphereState != NULL)
    {
        Free(sPokeSphereState);
    }
    if (sBg1TilemapBuffer != NULL)
    {
        Free(sBg1TilemapBuffer);
    }
    FreeAllWindowBuffers();
    ResetSpriteData();
}
