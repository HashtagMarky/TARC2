/*      --  Credits & Requirements  --

This Rotom Start Menu was built upon two public resources.
Vol's Start Menu and the Sample UI (Sliding Panel) provided
by Ghoulslash & Grant-Lucas both provided a basis for the two
aspects of this start menu Many thanks, and a lot of credit
must go to them all.
Vol Start Menu:
    https://github.com/volromhacking/pokeemerald/tree/start_menu_1
Ghoulslash & Grant-Lucas Sample UI:
    https://github.com/grunt-lucas/pokeemerald-expansion/tree/sample-ui

Another aspect that I must provide credit for was Bivurnum's
guide on Title Screen Easy Fade Colors. This implementation
provided a code efficient way to create the cursor highlighting.
    https://github.com/Bivurnum/decomps-resources/wiki/Title-Screen-Easy-Fade-Colors

The Rotom Start Menu uses ShantyTown's ComfyAnim Library,
many thanks to them for creating this public resource.
It will need to be added to your project in order to use
this if not already:
    https://github.com/huderlem/pokeemerald/tree/comfy_anims



        --    Tips & Stipulations   --

1.  In RotomPhone_StartMenu_Open, if using pokeemerald-expansion
    v1.11.0 or later, ResetDexNavSearch is called. This is due
    to it causing window issues when the start menu is called
    and the DexNav is acive. If your project uses the DexNav but
    is not on pokeemerald-expansion v1.11.0+, adjust the preproc
    surrounding ResetDexNavSearch as necessary.
2.  No two overworld icons that appear on screen at the same
    time should have the same colour.
3.  A shortcut option can be selected by using the function
    called RotomPhone_StartMenu_GetShortcutOption, and can be
    more easily accessed by RP_GET_SHORTCUT_OPTION. These can
    be set by the developer, or the function can be made to
    dynamically change based on player selection.



        --   Configuration Options  --

In rotom_start_menu.h (this file) and rotom_start_menu.c there
are multiple defines that are used to control differing aspects
of the start menu. The defines in rotom_start_menu.c are able to
be tweaked by the developer to adjust how the menu works, but
some of the defines in rotom_start_menu.h are made to allow for
player adjustment. This can be done in game through a scripting
flag, or save game options.

    RP_CONFIG_USE_ROTOM_PHONE
    Whether or not to use the Rotom Phone or a Generic Flip
    Phone, potentially an early game option due to story
    purposes.

    RP_CONFIG_MONOCHROME_ICONS
    Whether each icon uses it's individual colours when on the
    menu. Works better with RP_CONFIG_PALETTE_BUFFER set to TRUE.

    RP_CONFIG_PALETTE_BUFFER
    Uses two bytes of EWRAM in order to store the background and
    sprite palettes used in the Rotom Phone. It is recommended to
    turn keep this on if you want to have various colour phones or
    change the monochrome sprite colour. When doing so you will
    only have to make adjustments to these palettes once. To see
    how the palettes are initialised, see these functions,
    particularly the ones with bool32 firstLoad as a parameter:
        RotomPhone_OverworldMenu_LoadIconSpritePalette
        RotomPhone_OverworldMenu_LoadBgPalette
        RotomPhone_RotomRealityMenu_LoadIconSpritePalette
        RotomPhone_RotomRealityMenu_LoadBgPalette

    RP_CONFIG_FULL_SCREEN_SHORTCUT
    Allows for the START BUTTON to be used on the rotom reality
    menu to automatically open the shortcut. This menu option
    is denoted with a star.

    RP_CONFIG_24_HOUR_MODE
    A boolean option to display 24 hour time when TRUE, and
    12 hour time when FALSE.

    RP_CONFIG_NUM_MINUTES_TO_UPDATE
    An integer of how many in game minutes to update the time.
    In the overworld, this time update is interrupted by a
    message from Rotom exactly halfway through, unless
    RP_CONFIG_UPDATE_MESSAGE is set to FALSE. Due to task data
    being saved as an s16, this has to have a value less than:
        (32767 / (60 * FakeRtc_GetSecondsRatio()))

    RP_CONFIG_UPDATE_MESSAGE
    A boolean option that allows Rotom's Overworld messages
    if set to TRUE.

    RP_CONFIG_UPDATE_MESSAGE_SOUND
    A boolean option that plays a sound when Rotom provides
    a message or update.

    RP_CONFIG_FACE_UPDATE_PERCENT
    A percent chance to update Rotom's facial expressions
    when giving a message or update. Rotom's facial
    expression is currently guarenteed to change on a
    menu input, but this can easily be changed in
    RotomPhone_StartMenu_UpdateRotomFaceAnim(bool32 input).


    
        --  How to Add a New Option --
*/

#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

#define RP_CONFIG_USE_ROTOM_PHONE         FlagGet(FLAG_SYS_POKEDEX_GET)
#define RP_CONFIG_MONOCHROME_ICONS        FALSE
#define RP_CONFIG_PALETTE_BUFFER          TRUE
#define RP_CONFIG_ROTOM_REALITY_SHORTCUT  TRUE
#define RP_CONFIG_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define RP_CONFIG_NUM_MINUTES_TO_UPDATE   1
#define RP_CONFIG_UPDATE_MESSAGE          TRUE
#define RP_CONFIG_UPDATE_MESSAGE_SOUND    TRUE
#define RP_CONFIG_FACE_UPDATE_PERCENT     100

void RotomPhone_StartMenu_Open(bool32 firstInit);
void RotomPhone_StartMenu_DisableRotomReality(void);

#endif // GUARD_ROTOM_START_MENU_H