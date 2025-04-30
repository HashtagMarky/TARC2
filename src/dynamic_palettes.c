#include "global.h"
#include "dynamic_palettes.h"
#include "event_object_movement.h"
#include "list_menu.h"
#include "map_name_popup.h"
#include "menu.h"
#include "palette.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "field_mugshot.h"
#include "constants/field_mugshots.h"
#include "overworld.h"

// *MODIFY*
// Define all dynamic palettes here and in dynamic_palettes.h
EWRAM_DATA u16 sDynPalPlayerOverworld[16] = {0};
EWRAM_DATA u16 sDynPalPlayerReflection[16] = {0};
EWRAM_DATA u16 sDynPalPlayerUnderwater[16] = {0};
EWRAM_DATA u16 sDynPalPlayerBattleFront[16] = {0};
EWRAM_DATA u16 sDynPalPlayerBattleBack[16] = {0};
EWRAM_DATA u16 sDynPalPlayerMugshot[16] = {0};

// *MODIFY*
// Define palette colors from files
const u16 sDynPal_Base[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/base.gbapal");
const u16 sDynPal_Part_SkinPale[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/skin/pale.gbapal");
const u16 sDynPal_Part_SkinLight[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/skin/light.gbapal");
const u16 sDynPal_Part_SkinMedium[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/skin/medium.gbapal");
const u16 sDynPal_Part_SkinDark[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/skin/dark.gbapal");
const u16 sDynPal_Part_HairBlack[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/black.gbapal");
const u16 sDynPal_Part_HairBrown[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/brown.gbapal");
const u16 sDynPal_Part_HairBlonde[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/blonde.gbapal");
const u16 sDynPal_Part_HairGinger[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/ginger.gbapal");
const u16 sDynPal_Part_HairRed[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/red.gbapal");
const u16 sDynPal_Part_HairBlue[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/blue.gbapal");
const u16 sDynPal_Part_HairGreen[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/green.gbapal");
const u16 sDynPal_Part_HairPink[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/pink.gbapal");
const u16 sDynPal_Part_HairCyan[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/cyan.gbapal");
const u16 sDynPal_Part_HairPurple[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/purple.gbapal");
const u16 sDynPal_Part_HairSilver[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/hair/silver.gbapal");
const u16 sDynPal_Part_ClothesGreen[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/clothes/green.gbapal");
const u16 sDynPal_Part_ClothesBlue[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/clothes/blue.gbapal");
const u16 sDynPal_Part_ClothesOrange[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/clothes/orange.gbapal");
const u16 sDynPal_Part_ClothesPink[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/clothes/pink.gbapal");
const u16 sDynPal_Part_ClothesWhite[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/clothes/white.gbapal");
const u16 sDynPal_Part_ClothesBlack[] = INCBIN_U16("graphics/protagonist_sprites/dynamic_palettes/clothes/black.gbapal");

// *MODIFY*
// Preset lists (indices in these lists are what is saved to game save)
static const struct SpritePalette sDynPalPartAPresets[] = {
    {sDynPal_Part_SkinPale, 0x1301},
    {sDynPal_Part_SkinLight, 0x1302},
    {sDynPal_Part_SkinMedium, 0x1303},
    {sDynPal_Part_SkinDark, 0x1304}
};

// *MODIFY*
static const struct SpritePalette sDynPalPartBPresets[] = {
    {sDynPal_Part_HairBrown, 0x1305},
    {sDynPal_Part_HairBlack, 0x1306},
    {sDynPal_Part_HairBlonde, 0x1307},
    {sDynPal_Part_HairGinger, 0x1308},
    {sDynPal_Part_HairRed, 0x1309},
    {sDynPal_Part_HairBlue, 0x130A},
    {sDynPal_Part_HairGreen, 0x130B},
    {sDynPal_Part_HairPink, 0x130C},
    {sDynPal_Part_HairCyan, 0x130D},
    {sDynPal_Part_HairPurple, 0x130E},
    {sDynPal_Part_HairSilver, 0x130F}
};

// *MODIFY*
static const struct SpritePalette sDynPalPartCPresets[] = {
    {sDynPal_Part_ClothesBlack, 0x1310},
    {sDynPal_Part_ClothesWhite, 0x1311},
    {sDynPal_Part_ClothesGreen, 0x1312},
    {sDynPal_Part_ClothesBlue, 0x1313},
    {sDynPal_Part_ClothesOrange, 0x1314},
    {sDynPal_Part_ClothesPink, 0x1315}
};

// *MODIFY*
// Change to match counts for preceding arrays
#define DYN_PAL_LIST_NAME 1
#define COUNT_PART_A_TONES 4
#define COUNT_PART_B_TONES 11
#define COUNT_PART_C_TONES 6

#define DYNPAL_MENU_MAX_SHOWN 5

// *MODIFY*
// List text definitions for menu
static const struct ListMenuItem sListItems_DynPal_PartATones[] = {
    {COMPOUND_STRING("SKIN TONE"), LIST_HEADER},
    {COMPOUND_STRING("Pale"), 0},
    {COMPOUND_STRING("Light"), 1},
    {COMPOUND_STRING("Medium"), 2},
    {COMPOUND_STRING("Dark"), 3}
};
static const struct ListMenuItem sListItems_DynPal_PartBTones[] = {
    {COMPOUND_STRING("HAIR TYPE"), LIST_HEADER},
    {COMPOUND_STRING("Brown"), 0},
    {COMPOUND_STRING("Black"), 1},
    {COMPOUND_STRING("Blonde"), 2},
    {COMPOUND_STRING("Ginger"), 3},
    {COMPOUND_STRING("Red"), 4},
    {COMPOUND_STRING("Blue"), 5},
    {COMPOUND_STRING("Green"), 6},
    {COMPOUND_STRING("Pink"), 7},
    {COMPOUND_STRING("Cyan"), 8},
    {COMPOUND_STRING("Purple"), 9},
    {COMPOUND_STRING("Silver"), 10}
};
static const struct ListMenuItem sListItems_DynPal_PartCTones[] = {
    {COMPOUND_STRING("CLOTHING"), LIST_HEADER},
    {COMPOUND_STRING("Black"), 0},
    {COMPOUND_STRING("White"), 1},
    {COMPOUND_STRING("Green"), 2},
    {COMPOUND_STRING("Blue"), 3},
    {COMPOUND_STRING("Orange"), 4},
    {COMPOUND_STRING("Pink"), 5}
};

// Dynamic palette definitions are split into 3 groups of 5, starting from palette index 1.
#define DYNPAL_COLOR_GROUP_NORMAL 0
#define DYNPAL_COLOR_GROUP_REFLECTION 5
#define DYNPAL_COLOR_GROUP_UNDERWATER 10

static void DynPal_InitOverworld(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset);
static void DynPal_InitBattleFront(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset);
static void DynPal_InitBattleBack(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset);
static void DynPal_InitFieldMugshot(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset);

static void DynPal_CopySection(const u16* src, u16* dest, int srcInd, int destInd, int groupOffset, int numberOfColors);
static void DynPal_SetToneIndices(u8 partATone, u8 partBTone, u8 partCTone);

static void Task_DynPal_MenuOne(u8 taskId);
static void Task_DynPal_MenuSequence(u8 taskId);
static void Task_DynPal_CharacteristicsMenuSequence(u8 taskId);
static void Task_DynPal_MenuFinish(u8 taskId);
static void Task_DynPal_MenuCancel(u8 taskId);
static void Task_HandleDynPalMultichoiceInput(u8 taskId);

static void DynPal_MenuInit();
static void DynPal_MenuShow(u8 taskId);
static void DynPal_MenuCursorMoved(s32 itemIndex, bool8 onInit, struct ListMenu* list);
static void DynPal_MenuSaveToneIndex(int dynPalType, int tone);
static void DynPal_ReloadToneForMenuByType(int dynPalType, int tone);
static void DynPal_ReloadPlayerPaletteForMenu(u16 paletteTag, u8 partATone, u8 partBTone, u8 partCTone);

// *MODIFY*
// Fill all dynamic palettes with data according to indices in Save Block
void DynPal_InitAllDynamicPalettes()
{
    const u16* partAPalData = sDynPalPartAPresets[min(gSaveBlock2Ptr->dynPalSkinTone, COUNT_PART_A_TONES)].data;
    const u16* partBPalData = sDynPalPartBPresets[min(gSaveBlock2Ptr->dynPalHairTone, COUNT_PART_B_TONES)].data;
    const u16* partCPalData = sDynPalPartCPresets[min(gSaveBlock2Ptr->dynPalClothesTone, COUNT_PART_C_TONES)].data;

    // Player Normal
    DynPal_InitOverworld(sDynPalPlayerOverworld, partAPalData, partBPalData, partCPalData, DYNPAL_COLOR_GROUP_NORMAL);
    // Player Reflection
    DynPal_InitOverworld(sDynPalPlayerReflection, partAPalData, partBPalData, partCPalData, DYNPAL_COLOR_GROUP_REFLECTION);
    // Player Underwater
    DynPal_InitOverworld(sDynPalPlayerUnderwater, partAPalData, partBPalData, partCPalData, DYNPAL_COLOR_GROUP_UNDERWATER);
    // Player Battle Front
    DynPal_InitBattleFront(sDynPalPlayerBattleFront, partAPalData, partBPalData, partCPalData, DYNPAL_COLOR_GROUP_NORMAL);
    // Player Battle Back
    DynPal_InitBattleBack(sDynPalPlayerBattleBack, partAPalData, partBPalData, partCPalData, DYNPAL_COLOR_GROUP_NORMAL);
    // Player Field Mugshot
    DynPal_InitFieldMugshot(sDynPalPlayerMugshot, partAPalData, partBPalData, partCPalData, DYNPAL_COLOR_GROUP_NORMAL);
}

// *MODIFY*
// Load each section of the palette. Your implementation will differ depending on how the sprite palette is arranged.
// For any sections of the palette that should remain constant regardless of parts, use sDynPal_Base as <src>
#define PART_A_INDEX_START 1
#define PART_A_NUMBER_OF_COLOURS 4
#define PART_B_INDEX_START 5
#define PART_B_NUMBER_OF_COLOURS 3
#define PART_C_INDEX_START 8
#define PART_C_NUMBER_OF_COLOURS 5
#define BASE_INDEX_START 13
#define BASE_NUMBER_OF_COLOURS 3
#define IKIAGI_CLOTHES_OFFSET 2

static void DynPal_InitOverworld(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset)
{
    // Change this function to match your palette setup

    // This setup assumes your male and female characters will be using the same base palette
    // If they don't, you can create a second base palette and check against the selected gender

    // Skin & Lips/Pokéball 1-4
    DynPal_CopySection(partAPalData, dest, 1, PART_A_INDEX_START, groupOffset, PART_A_NUMBER_OF_COLOURS);
    // Hair 5-7
    DynPal_CopySection(partBPalData, dest, 1, PART_B_INDEX_START, groupOffset, PART_B_NUMBER_OF_COLOURS);
    // Clothes 8-12
    DynPal_CopySection(partCPalData, dest, 1, PART_C_INDEX_START, groupOffset, PART_C_NUMBER_OF_COLOURS);
    // White, Grey & Black 13-15
    DynPal_CopySection(sDynPal_Base, dest, 1, BASE_INDEX_START, groupOffset, BASE_NUMBER_OF_COLOURS);
}

// *MODIFY*
// Derive battle front sprite palette - first load the overworld palette, then make modifications. Your specific implementation may differ.
static void DynPal_InitBattleFront(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset)
{
    DynPal_InitOverworld(dest, partAPalData, partBPalData, partCPalData, groupOffset);

    //Add any modifications you need here
}

// *MODIFY*
// Derive battle back sprite palette - first load the overworld palette, then make modifications. Your specific implementation may differ.
static void DynPal_InitBattleBack(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset)
{
    DynPal_InitOverworld(dest, partAPalData, partBPalData, partCPalData, groupOffset);
    
    //Add any modifications you need here
}

// *MODIFY*
// Derive field mugshot sprite palette - first load the overworld palette, then make modifications. Your specific implementation may differ.
static void DynPal_InitFieldMugshot(u16* dest, const u16* partAPalData, const u16* partBPalData, const u16* partCPalData, int groupOffset)
{
    DynPal_InitOverworld(dest, partAPalData, partBPalData, partCPalData, groupOffset);
    
    //Add any modifications you need here
}

// *MODIFY*
// If you don't want the character sprites to load with parts {0, 0, 0} in the intro, you can set the preferred palette indices here
void DynPal_LoadIntroToneIndices(bool8 introReload, u8 loadCycle)
{
    u8 koleSkin = Random() % COUNT_PART_A_TONES;
    u8 ankaSkin = Random() % COUNT_PART_A_TONES;
    u8 koleHair = Random() % COUNT_PART_B_TONES;
    u8 ankaHair = Random() % COUNT_PART_B_TONES;
    u8 koleClothes, ankaClothes;
    u8 noKoleReload = 0;
    u8 noAnkaReload = 0;

    // Randomises skin so one of the lighter and darker tones are always shown. 
    while ((koleSkin <= 1 && ankaSkin <= 1) || (koleSkin >= 2 && ankaSkin >= 2))
    {
        ankaSkin = Random() % COUNT_PART_A_TONES;
    }

    // Randomises hair so two different are always shown.
    while (koleHair == ankaHair)
    {
        ankaHair = Random() % COUNT_PART_B_TONES;
    }

    // Randomises clothes when reloading only, if not clothes match the message box.
    if (!introReload)
    {
        koleClothes = gSaveBlock2Ptr->optionsInterfaceColor + IKIAGI_CLOTHES_OFFSET;
        ankaClothes = gSaveBlock2Ptr->optionsInterfaceColor + IKIAGI_CLOTHES_OFFSET;
    }
    else
    {
        koleClothes = Random() % COUNT_PART_C_TONES;
        ankaClothes = Random() % COUNT_PART_C_TONES;

        while (koleClothes == ankaClothes)
        {
            ankaClothes = Random() % COUNT_PART_C_TONES;
        }
    }

    // Prevents reloading after initial load in.
    if ((introReload && loadCycle == 1))
    {
        noKoleReload = 1;
        noAnkaReload = 1;
    }

    // Prevents reloading of displayed sprite when returning to gender menu from pallette menu.
    // Includes complete randomisation of the other sprite pallete.
    if (loadCycle == 2)
    {
        if (gSaveBlock2Ptr->playerGender == MALE)
        {
            noKoleReload = 1;
        }
        else
        {
            noAnkaReload = 1;
        }
    }

    // Prevents reloading of displayed sprite when returning to gender menu from naming screen.
    // Includes randomisation of the other sprite pallete using the same rules as above.
    if (loadCycle == 3)
    {
        if (gSaveBlock2Ptr->playerGender == MALE)
        {
            noKoleReload = 1;

            // Skin Randomisation
            while ((gSaveBlock2Ptr->dynPalSkinTone <= 1 && ankaSkin <= 1) || (gSaveBlock2Ptr->dynPalSkinTone >= 2 && ankaSkin >= 2))
            {
                ankaSkin = Random() % COUNT_PART_A_TONES;
            }

            // Hair Randomisation
            while (gSaveBlock2Ptr->dynPalHairTone == ankaHair)
            {
                ankaHair = Random() % COUNT_PART_B_TONES;
            }

            // Clothes Randomisation
            while (gSaveBlock2Ptr->dynPalClothesTone == ankaClothes)
            {
                ankaClothes = Random() % COUNT_PART_C_TONES;
            }
        }
        else
        {
            noAnkaReload = 1;

            // Skin Randomisation
            while ((gSaveBlock2Ptr->dynPalSkinTone <= 1 && koleSkin <= 1) || (gSaveBlock2Ptr->dynPalSkinTone >= 2 && koleSkin >= 2))
            {
                koleSkin = Random() % COUNT_PART_A_TONES;
            }

            // Hair Randomisation
            while (gSaveBlock2Ptr->dynPalHairTone == koleHair)
            {
                koleHair = Random() % COUNT_PART_B_TONES;
            }

            // Clothes Randomisation
            while (gSaveBlock2Ptr->dynPalClothesTone == koleClothes)
            {
                koleClothes = Random() % COUNT_PART_C_TONES;
            }
        }
    }

    // Prevents reloading of each pallete.
    if (noKoleReload != 1)
    {
        DynPal_ReloadPlayerPaletteForMenu(TRAINER_PIC_KOLE, koleSkin, koleHair, koleClothes);
    }
    if (noAnkaReload != 1)
    {
        DynPal_ReloadPlayerPaletteForMenu(TRAINER_PIC_ANKA, ankaSkin, ankaHair, ankaClothes);
    }
}

// Copies <numberOfColors> values from the ROM palette <src>, to dynamic palette <dest>
static void DynPal_CopySection(const u16* src, u16* dest, int srcInd, int destInd, int groupOffset, int numberOfColors)
{
    CpuCopy16(&src[srcInd + groupOffset], &dest[destInd], numberOfColors * 2);
}

// Load the palette to active palettes using a known offset
void DynPal_LoadPaletteByOffset(u16* paletteData, u16 paletteOffset)
{
    // Standard palette loading expects the palette to be in ROM (const). Circumvent that
    memcpy(&gPlttBufferFaded[paletteOffset], paletteData, PLTT_SIZE_4BPP);
    memcpy(&gPlttBufferUnfaded[paletteOffset], paletteData, PLTT_SIZE_4BPP);
}

// Load the palette to active palettes using a known sprite tag
void DynPal_LoadPaletteByTag(u16* paletteData, u16 paletteTag)
{
    struct SpritePalette dest;

    // Free the palette tag if it is already initialized
    FreeSpritePaletteByTag(paletteTag);
    // Could just memcpy but nominally doing this the 'right' way for no particular reason
    dest.data = paletteData;
    dest.tag = paletteTag;
    LoadSpritePalette(&dest);
}

// Write palette indices to save block (can send 0xFF to exclude that tone list)
static void DynPal_SetToneIndices(u8 partATone, u8 partBTone, u8 partCTone)
{
    if (partATone != 0xFF)
    {
        gSaveBlock2Ptr->dynPalSkinTone = partATone % COUNT_PART_A_TONES;
    }
    if (partBTone != 0xFF)
    {
        gSaveBlock2Ptr->dynPalHairTone = partBTone % COUNT_PART_B_TONES;
    }
    if (partCTone != 0xFF)
    {
        gSaveBlock2Ptr->dynPalClothesTone = partCTone % COUNT_PART_C_TONES;
    }
}

// LIST HANDLER DATA

#define DYNPAL_MENU_ID_CANCEL -1
#define DYNPAL_MENU_ID_PART_A 0
#define DYNPAL_MENU_ID_PART_B 1
#define DYNPAL_MENU_ID_PART_C 2
#define DYNPAL_MENU_ID_FINISH 3

// Dynpal list menu template
static const struct ListMenuTemplate sListTemplate_DynPal =
{
    .moveCursorFunc = DynPal_MenuCursorMoved,
    .itemPrintFunc = NULL,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_SHORT,
    .cursorKind = CURSOR_BLACK_ARROW
};

// Dynpal list flow extra data
static EWRAM_DATA struct {
    s16 menuSeq;
    bool8 isActive;
    bool8 isOverworld;
    TaskFunc funcCancel;
    TaskFunc funcFinish;
    u8 partATone;
    u8 partBTone;
    u8 partCTone;
    u16 scrollOffset;
} sDynPalMenu = {0};

// Task data
#define tDynpalListMenuTask data[0]
#define tDynpalWindowId     data[1]
#define tDynpalNumItems     data[2]
#define tDynpalMaxItems     data[3]
#define tDynpalScrollArrows data[4]
#define tDynpalParentTask   data[5]

// Show just a single menu. Use this if you want text or whatever in between menus
void DynPal_ShowMenuSingleton(s16 dynPalType, u8 taskId, TaskFunc nFuncFinish, TaskFunc nFuncCancel, bool8 isOverworld)
{
    sDynPalMenu.menuSeq = dynPalType;
    sDynPalMenu.funcCancel = nFuncCancel;
    sDynPalMenu.funcFinish = nFuncFinish;
    sDynPalMenu.isOverworld = isOverworld;
    DynPal_MenuInit();
    gTasks[taskId].func = Task_DynPal_MenuOne;
}

// Show all three menus in sequence with nothing in between
void DynPal_ShowMenuSequence(u8 taskId, TaskFunc nFuncFinish, TaskFunc nFuncCancel, bool8 isOverworld, bool8 characteristicsMenu)
{
    sDynPalMenu.menuSeq = DYNPAL_MENU_ID_PART_A;
    sDynPalMenu.funcCancel = nFuncCancel;
    sDynPalMenu.funcFinish = nFuncFinish;
    sDynPalMenu.isOverworld = isOverworld;
    DynPal_MenuInit();
    if (!characteristicsMenu) // If bool8 characteristicsMenu was set to be true in DynPal_ShowMenuSequence, only the skin and hair menu are shown.
        gTasks[taskId].func = Task_DynPal_MenuSequence;
    else
        gTasks[taskId].func = Task_DynPal_CharacteristicsMenuSequence;
}

// Manager task for singleton dynpal menu
static void Task_DynPal_MenuOne(u8 taskId)
{
    if (!sDynPalMenu.isActive)
    {
        DynPal_MenuShow(taskId);
        gTasks[taskId].func = Task_DynPal_MenuFinish;
    }
}

// Manager task for full sequence dynpal menu
static void Task_DynPal_MenuSequence(u8 taskId)
{
    if (!sDynPalMenu.isActive)
    {
        switch (sDynPalMenu.menuSeq)
        {
        case DYNPAL_MENU_ID_FINISH:
            gTasks[taskId].func = Task_DynPal_MenuFinish;
            break;
        case DYNPAL_MENU_ID_CANCEL:
            gTasks[taskId].func = Task_DynPal_MenuCancel;
            break;
        default:
            DynPal_MenuShow(taskId);
            break;
        }
    }
}

// Manager task for characteristics sequence dynpal menu
static void Task_DynPal_CharacteristicsMenuSequence(u8 taskId)
{
    if (!sDynPalMenu.isActive)
    {
        switch (sDynPalMenu.menuSeq)
        {
        case DYNPAL_MENU_ID_FINISH:
        case DYNPAL_MENU_ID_PART_C:
            gTasks[taskId].func = Task_DynPal_MenuFinish;
            break;
        case DYNPAL_MENU_ID_CANCEL:
            gTasks[taskId].func = Task_DynPal_MenuCancel;
            break;
        default:
            DynPal_MenuShow(taskId);
            break;
        }
    }
}

// Set tone indices, init palettes, and exit dynpal menu flow
static void Task_DynPal_MenuFinish(u8 taskId)
{
    // For singleton case, only continue after menu is deactivated again
    if (!sDynPalMenu.isActive)
    {
        DynPal_SetToneIndices(sDynPalMenu.partATone, sDynPalMenu.partBTone, sDynPalMenu.partCTone);
        DynPal_InitAllDynamicPalettes();

        // For overworld use case, reload player palette.
        // This code assumes fixed IDs for player palette, so you may need to change this.
        if (sDynPalMenu.isOverworld)
        {
            DynPal_LoadPaletteByOffset(sDynPalPlayerOverworld, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BRENDAN)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerReflection, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BRENDAN_REFLECTION)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerOverworld, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_MAY)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerReflection, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_MAY_REFLECTION)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerReflection, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER)));
            ScriptContext_Enable();
        }

        if (sDynPalMenu.funcFinish != NULL)
        {
            gTasks[taskId].func = sDynPalMenu.funcFinish;
        }
        else
        {
            DestroyTask(taskId);
        }
    }
}

// Identical to above, minus setting the tone indices from the temp vars
static void Task_DynPal_MenuCancel(u8 taskId)
{
    if (!sDynPalMenu.isActive)
    {
        DynPal_InitAllDynamicPalettes();

        if (sDynPalMenu.isOverworld)
        {
            DynPal_LoadPaletteByOffset(sDynPalPlayerOverworld, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BRENDAN)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerReflection, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BRENDAN_REFLECTION)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerOverworld, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_MAY)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerReflection, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_MAY_REFLECTION)));
            DynPal_LoadPaletteByOffset(sDynPalPlayerReflection, OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER)));
            ScriptContext_Enable();
        }

        if (sDynPalMenu.funcFinish != NULL)
        {
            gTasks[taskId].func = sDynPalMenu.funcCancel;
        }
        else
        {
            DestroyTask(taskId);
        }
    }
}


static void DynPal_MenuInit()
{
    sDynPalMenu.isActive = FALSE;
    sDynPalMenu.partATone = 0xFF;
    sDynPalMenu.partBTone = 0xFF;
    sDynPalMenu.partCTone = 0xFF;
    sDynPalMenu.scrollOffset = 0;
}

// Shows the color menu corresponding to the value in sDynPalMenu.menuSeq
static void DynPal_MenuShow(u8 taskId)
{
    const struct ListMenuItem* menuItems;
    struct WindowTemplate windowTemplate;
    u8 numItems, maxShownItems, listTaskId, windowId;
    switch (sDynPalMenu.menuSeq)
    {
    case DYNPAL_MENU_ID_PART_A:
        menuItems = sListItems_DynPal_PartATones;
        numItems = COUNT_PART_A_TONES + DYN_PAL_LIST_NAME;
        break;
    case DYNPAL_MENU_ID_PART_B:
        menuItems = sListItems_DynPal_PartBTones;
        numItems = COUNT_PART_B_TONES + DYN_PAL_LIST_NAME;
        break;
    default: // prevent compiler from being sad
    case DYNPAL_MENU_ID_PART_C:
        menuItems = sListItems_DynPal_PartCTones;
        numItems = COUNT_PART_C_TONES + DYN_PAL_LIST_NAME;
        break;
    }
    // If the menu has no values, skip past it. If you don't want to use one of the part groups this should handle it
    if (numItems == 0)
    {
        ++sDynPalMenu.menuSeq;
        return;
    }

    maxShownItems = min(DYNPAL_MENU_MAX_SHOWN, numItems);

    HideMapNamePopUpWindow();
    // Create window and menu templates
    u8 top = 2;
    u8 height = (maxShownItems * 2);
    if (gSaveBlock2Ptr->optionsCurrentFont == 0)
    {
        top++;
        height--;
    }
    windowTemplate = CreateWindowTemplate(0, 2, top, 9, height, 15, 0x80);
    windowId = AddWindow(&windowTemplate);
    if (!sDynPalMenu.isOverworld)
    {
        LoadUserWindowBorderGfx_(windowId, 0xF3, BG_PLTT_ID(2));
        DrawTextBorderOuter(windowId, 0xF3, 2);
    }
    else
    {
        DrawStdWindowFrame(windowId, FALSE);
    }

    gMultiuseListMenuTemplate = sListTemplate_DynPal;
    gMultiuseListMenuTemplate.fontId = ReturnNormalTextFont();
    gMultiuseListMenuTemplate.items = menuItems;
    gMultiuseListMenuTemplate.totalItems = numItems;
    gMultiuseListMenuTemplate.maxShowed = maxShownItems;
    gMultiuseListMenuTemplate.windowId = windowId;

    listTaskId = CreateTask(Task_HandleDynPalMultichoiceInput, 0);
    gTasks[listTaskId].tDynpalListMenuTask = ListMenuInit(&gMultiuseListMenuTemplate, 0, 1);
    gTasks[listTaskId].tDynpalWindowId = windowId;
    gTasks[listTaskId].tDynpalNumItems = numItems;
    gTasks[listTaskId].tDynpalMaxItems = maxShownItems;
    gTasks[listTaskId].tDynpalParentTask = taskId;

    sDynPalMenu.scrollOffset = 0;
    sDynPalMenu.isActive = TRUE;

    if (numItems > maxShownItems)
    {
        u8 firstPos = 12;
        if (gSaveBlock2Ptr->optionsCurrentFont == 0)
        {
            firstPos += 8;
        }
        gTasks[listTaskId].tDynpalScrollArrows = AddScrollIndicatorArrowPairParameterized(
            SCROLL_ARROW_UP, 56, firstPos, 100, numItems - 1 - DYN_PAL_LIST_NAME, 2000, 100, &(sDynPalMenu.scrollOffset));
    }
    else
    {
        gTasks[listTaskId].tDynpalScrollArrows = -1;
    }

    CopyWindowToVram(windowId, COPYWIN_MAP);
}

// Handler for scrollable list input
static void Task_HandleDynPalMultichoiceInput(u8 taskId)
{
    bool32 done = FALSE;
    s32 input = ListMenu_ProcessInput(gTasks[taskId].tDynpalListMenuTask);
    switch (input)
    {
    case LIST_HEADER:
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        // Return to previous list in sequence, or exit to fallback func
        --sDynPalMenu.menuSeq;
        done = TRUE;
        break;
    default:
        // Selection made
        done = TRUE;

        PlaySE(SE_SELECT);

        DynPal_ReloadToneForMenuByType(sDynPalMenu.menuSeq, input);
        DynPal_MenuSaveToneIndex(sDynPalMenu.menuSeq, input);
        // Prepare for next menu in sequence
        ++sDynPalMenu.menuSeq;
        break;
    }

    //PrintToDynPalNameWindow(gTasks[taskId].tDynpalWindowId);

    if (done)
    {
        // Cleanup
        sDynPalMenu.isActive = FALSE;
        // Make sure these were actually initialized before continuing
        if (gTasks[taskId].tDynpalScrollArrows != -1)
        {
            RemoveScrollIndicatorArrowPair(gTasks[taskId].tDynpalScrollArrows);
        }
        DestroyListMenuTask(gTasks[taskId].tDynpalListMenuTask, NULL, NULL);
        ClearStdWindowAndFrame(gTasks[taskId].tDynpalWindowId, TRUE);
        RemoveWindow(gTasks[taskId].tDynpalWindowId);
        DestroyTask(taskId);
    }
}

// Hot reload displayed player palette on cursor move 
static void DynPal_MenuCursorMoved(s32 itemIndex, bool8 onInit, struct ListMenu* list)
{
    sDynPalMenu.scrollOffset = itemIndex;
    DynPal_ReloadToneForMenuByType(sDynPalMenu.menuSeq, itemIndex);
}

// Write selected tones to temp vars. The vars in save block are not written until end of menu sequence, so backing out will cancel any changes
static void DynPal_MenuSaveToneIndex(int dynPalType, int tone)
{
    switch (dynPalType)
    {
    case DYNPAL_MENU_ID_PART_A:
        sDynPalMenu.partATone = tone;
        break;
    case DYNPAL_MENU_ID_PART_B:
        sDynPalMenu.partBTone = tone;
        break;
    case DYNPAL_MENU_ID_PART_C:
        sDynPalMenu.partCTone = tone;
        break;
    }
}

// Helper for hot reloading player palette in intro menu
static void DynPal_ReloadToneForMenuByType(int dynPalType, int tone)
{
    switch (dynPalType)
    {
        case DYNPAL_MENU_ID_PART_A:
            DynPal_ReloadPlayerPaletteForMenu(PlayerGenderToFrontTrainerPicId(gSaveBlock2Ptr->playerGender), tone, 0xFF, 0xFF);
            break;
        case DYNPAL_MENU_ID_PART_B:
            DynPal_ReloadPlayerPaletteForMenu(PlayerGenderToFrontTrainerPicId(gSaveBlock2Ptr->playerGender), 0xFF, tone, 0xFF);
            break;
        case DYNPAL_MENU_ID_PART_C:
            DynPal_ReloadPlayerPaletteForMenu(PlayerGenderToFrontTrainerPicId(gSaveBlock2Ptr->playerGender), 0xFF, 0xFF, tone);
            break;
    }
}

// *MODIFY*
// Hot reload player palette - Main section should be identical to DynPal_InitOverworld, but split up between each tone
static void DynPal_ReloadPlayerPaletteForMenu(u16 paletteTag, u8 partATone, u8 partBTone, u8 partCTone)
{
    u16 offsetDefault;
    u16 offsetMugshot = OBJ_PLTT_ID(IndexOfSpritePaletteTag(TAG_MUGSHOT));
    if (sDynPalMenu.isOverworld)
    {
        if (gSaveBlock2Ptr->playerGender == MALE)
            offsetDefault = OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BRENDAN));
        else
            offsetDefault = OBJ_PLTT_ID(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_MAY));
    }
    else
    {
        offsetDefault = OBJ_PLTT_ID(IndexOfSpritePaletteTag(paletteTag));
    }

    if (partATone != 0xFF)
    {
        const u16* partAPalData = sDynPalPartAPresets[min(partATone, COUNT_PART_A_TONES)].data;
        DynPal_CopySection(partAPalData, &gPlttBufferUnfaded[offsetDefault], 1, PART_A_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, PART_A_NUMBER_OF_COLOURS);
        if (IsFieldMugshotActive())
            DynPal_CopySection(partAPalData, &gPlttBufferUnfaded[offsetMugshot], 1, PART_A_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, PART_A_NUMBER_OF_COLOURS);
    }
    if (partBTone != 0xFF)
    {
        const u16* partBPalData = sDynPalPartBPresets[min(partBTone, COUNT_PART_B_TONES)].data;
        DynPal_CopySection(partBPalData, &gPlttBufferUnfaded[offsetDefault], 1, PART_B_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, PART_B_NUMBER_OF_COLOURS);
        if (IsFieldMugshotActive())
            DynPal_CopySection(partBPalData, &gPlttBufferUnfaded[offsetMugshot], 1, PART_B_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, PART_B_NUMBER_OF_COLOURS);
    }
    if (partCTone != 0xFF)
    {
        const u16* partCPalData = sDynPalPartCPresets[min(partCTone, COUNT_PART_C_TONES)].data;
        DynPal_CopySection(partCPalData, &gPlttBufferUnfaded[offsetDefault], 1, PART_C_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, PART_C_NUMBER_OF_COLOURS);
        if (IsFieldMugshotActive())
            DynPal_CopySection(partCPalData, &gPlttBufferUnfaded[offsetMugshot], 1, PART_C_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, PART_C_NUMBER_OF_COLOURS);
    }
    DynPal_CopySection(sDynPal_Base, &gPlttBufferUnfaded[offsetDefault], 1, BASE_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, BASE_NUMBER_OF_COLOURS);
    if (IsFieldMugshotActive())
        DynPal_CopySection(sDynPal_Base, &gPlttBufferUnfaded[offsetMugshot], 1, BASE_INDEX_START, DYNPAL_COLOR_GROUP_NORMAL, BASE_NUMBER_OF_COLOURS);

    /*
    if (!sDynPalMenu.isOverworld)
    {
        // Reflect the code in DynPal_InitBattleFront here
    }
    */

    memcpy(&gPlttBufferFaded[offsetDefault], &gPlttBufferUnfaded[offsetDefault], PLTT_SIZE_4BPP);
    if (IsFieldMugshotActive())
        memcpy(&gPlttBufferFaded[offsetMugshot], &gPlttBufferUnfaded[offsetMugshot], PLTT_SIZE_4BPP);

    if (sDynPalMenu.isOverworld)
        UpdatePalettesWithTime(PALETTES_ALL);
}

// SCRIPT SPECIAL WRAPPERS
void DynPal_ShowFullToneMenu(void)
{
    DynPal_ShowMenuSequence(CreateTask(NULL, 0), NULL, NULL, TRUE, FALSE);
}

void DynPal_ShowCharacteristicsToneMenu(void)
{
    DynPal_ShowMenuSequence(CreateTask(NULL, 0), NULL, NULL, TRUE, TRUE);
}

void DynPal_ShowToneMenu_Skin(void)
{
    DynPal_ShowMenuSingleton(DYNPAL_MENU_ID_PART_A, CreateTask(NULL, 0), NULL, NULL, TRUE);
}

void DynPal_ShowToneMenu_Hair(void)
{
    DynPal_ShowMenuSingleton(DYNPAL_MENU_ID_PART_B, CreateTask(NULL, 0), NULL, NULL, TRUE);
}

void DynPal_ShowToneMenu_Clothes(void)
{
    DynPal_ShowMenuSingleton(DYNPAL_MENU_ID_PART_C, CreateTask(NULL, 0), NULL, NULL, TRUE);
}

#undef DYNPAL_MENU_ID_CANCEL
#undef DYNPAL_MENU_ID_PART_A
#undef DYNPAL_MENU_ID_PART_B
#undef DYNPAL_MENU_ID_PART_C
#undef DYNPAL_MENU_ID_FINISH
#undef tDynpalListMenuTask
#undef tDynpalWindowId
#undef tDynpalNumItems
#undef tDynpalMaxItems
#undef tDynpalScrollArrows
#undef tDynpalParentTask
