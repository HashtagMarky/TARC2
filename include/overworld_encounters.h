#ifndef GUARD_OVERWORLD_ENCOUNTERS_H
#define GUARD_OVERWORLD_ENCOUNTERS_H

void GetOverworldMonSpecies(void);
void SantizeOverworldMonLevel(void);
void GetOverworldSpeciesCatchRate(void);
u8 OverworldTrappedInBattle(void);
bool8 WillOverworldEncounterRun(void);
u16 ReturnFixedSpeciesEncounter(void);
u16 ReturnHeaderSpeciesEncounter(u8 encounterType, u16 headerId);
bool8 GeneratedOverworldMonShinyRoll(void);

#endif // GUARD_OVERWORLD_ENCOUNTERS_H