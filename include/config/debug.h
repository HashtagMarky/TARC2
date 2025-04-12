#ifndef GUARD_CONFIG_DEBUG_H
#define GUARD_CONFIG_DEBUG_H

// Overworld Debug
#define DEBUG_OVERWORLD_MENU            DEV_BUILD                // Enables an overworld debug menu to change flags, variables, giving pokemon and more, accessed by holding R and pressing START while in the overworld by default.
#define DEBUG_OVERWORLD_HELD_KEYS       (R_BUTTON)          // The keys required to be held to open the debug menu.
#define DEBUG_OVERWORLD_TRIGGER_EVENT   pressedStartButton  // The event that opens the menu when holding the key(s) defined in DEBUG_OVERWORLD_HELD_KEYS.
#define DEBUG_OVERWORLD_IN_MENU         FALSE               // Replaces the overworld debug menu button combination with a start menu entry (above Pokédex).

// Battle Debug Menu
#define DEBUG_BATTLE_MENU               DEV_BUILD    // If set to TRUE, enables a debug menu to use in battles by pressing the Select button.
#define DEBUG_AI_DELAY_TIMER            FALSE   // If set to TRUE, displays the number of frames it takes for the AI to choose a move. Replaces the "What will PKMN do" text. Useful for devs or anyone who modifies the AI code and wants to see if it doesn't take too long to run.

// Pokémon Debug
#define DEBUG_POKEMON_SPRITE_VISUALIZER DEV_BUILD    // Enables a debug menu for Pokémon sprites and icons, accessed by pressing Select in the summary screen.

// Options Menu
#define DEBUG_OPTIONS_MENU_GYM_INTERFACE            FALSE   // Enable this to allow for changing Gym Type via the Options menu.
#define DEBUG_OPTIONS_MENU_MAIN_MENU_SCREENSHOTS    FALSE   // Enable this to randomise the DynPals and Gender of the player as well as modify the starter when leaving the options menu.

#endif // GUARD_CONFIG_DEBUG_H
