#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

// Exported type declarations

// Exported RAM declarations

// Exported ROM declarations
void CB2_InitCopyrightScreenAfterBootup(void);
void CB2_InitCopyrightScreenAfterTitleScreen(void);
void PanFadeAndZoomScreen(u16 screenX, u16 screenY, u16 zoom, u16 alpha);
void HandleAfterCopyright();

#if SKIP_INTRO_AFTER_COPYRIGHT == FALSE
void MainCB2_Intro(void);
void Task_Scene1_Load(u8);
#endif


#endif // GUARD_INTRO_H
