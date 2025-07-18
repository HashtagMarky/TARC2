#include "task.h"

// Define all dynamic palettes here and in dynamic_palettes.c
extern u16 sDynPalPlayerOverworld[];
extern u16 sDynPalPlayerReflection[];
extern u16 sDynPalPlayerUnderwater[];
extern u16 sDynPalPlayerBattleFront[];
extern u16 sDynPalPlayerBattleBack[];
extern u16 sDynPalPlayerMugshot[];

// Called at game start
void DynPal_InitAllDynamicPalettes();

// Load a dynamic palette in-game (usually overriding a ROM palette)
void DynPal_LoadPaletteByOffset(u16* paletteData, u16 paletteOffset);
void DynPal_LoadPaletteByTag(u16* paletteData, u16 paletteTag);
void DynPal_LoadOverworldSkinPaletteByOffset(u16 paletteOffset);
void DynPal_LoadOverworldSkinPaletteByTag(u16 paletteTag, u16 paletteOffset);

// Menu for intro & script specials
void DynPal_ShowMenuSingleton(s16 dynPalType, u8 taskId, TaskFunc nFuncFinish, TaskFunc nFuncCancel, bool8 isOverworld);
void DynPal_ShowMenuSequence(u8 taskId, TaskFunc nFuncFinish, TaskFunc nFuncCancel, bool8 isOverworld, bool8 characteristicsMenu);
void DynPal_LoadIntroToneIndices(bool8 introReload, u8 loadCycle);

// Script Specials
void DynPal_ShowFullToneMenu(void); // Do complete sequence of 3 menus
void DynPal_ShowToneMenu_Skin(void);
void DynPal_ShowToneMenu_Hair(void);
void DynPal_ShowToneMenu_Clothes(void);

enum DynamicPalettes
{
    DYNPALS_NONE,
    DYNPALS_SKIN,
    DYNPALS_HAIR,
    DYNPALS_CLOTHES,
    DYNPALS_FULL,
    DYNPALS_CHARACTERISTICS,
};
