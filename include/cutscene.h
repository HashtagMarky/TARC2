#ifndef GUARD_CUTSCENE_H
#define GUARD_CUTSCENE_H

struct CutscenesSkips
{
    const u8 *scriptCutsceneSkips;
};

void EndSkippableCutscene(void);

#endif // GUARD_CUTSCENE_H