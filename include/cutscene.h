#ifndef GUARD_CUTSCENE_H
#define GUARD_CUTSCENE_H

#define CUTSCENE_FLAG_TRACKING TRUE
// By default this requires a flag to be defined for each cutscene that is created.
// I just prefered the more cursed route of saving the bits directly in the saveblock,
// so decided to keep the functionality in. Preproc is used for the minor space savings
// in ROM or the SaveBlock depending on which method you want to use.

struct CutscenesSkips
{
    const u8 *scriptCutsceneSkipPtr;
#if CUTSCENE_FLAG_TRACKING == TRUE
    u16 flag;
#endif
};

void EndSkippableCutscene(void);

#endif // GUARD_CUTSCENE_H