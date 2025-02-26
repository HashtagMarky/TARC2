#ifndef GUARD_FIELD_MUGSHOTS_H
#define GUARD_FIELD_MUGSHOTS_H

#include "constants/field_mugshots.h"

// Player Mugshot Palette
extern const u16 sFieldMugshotPal_Protagonist[];

enum MugshotIDs GetMugshotIdFromObjectEvent(struct ObjectEvent *objEvent);
void CreateFieldMugshot(enum MugshotType mugshotType, enum MugshotIDs mugshotId, enum MugshotEmoteIDs mugshotEmotion, s16 x, s16 y, bool8 retainDetails);
void CreateFollowerFieldMugshot(u32 followerSpecies, u32 followerEmotion, bool8 shiny, bool8 emotePMD);
u8 CreateFieldMugshotSprite(enum MugshotIDs mugshotId, enum MugshotEmoteIDs mugshotEmotion, bool8 typeMon, bool8 shinyMon, u8 mugshotNum);
void DestroyFieldMugshotSprite(u8 sprite, u8 mugshotNum);
void RemoveFieldMugshot(bool8 retainDetails);
void RemoveFieldMugshotAndWindow(void);
u8 GetFieldMugshotSpriteId(void);
u16 CreatePlayerMugshotTrainerCardSprite(u8 gender, enum MugshotEmoteIDs mugshotEmotion, u16 destX, u16 destY, u8 paletteSlot, u8 windowId);
u8 IsFieldMugshotActive(void);
void SetMugshotDetails(u8 mugshotType, u16 mugshotId, u8 mugshotEmotion, s16 x, s16 y, u8 windowType);
void SetFieldMugshotSpriteId(u32 value);

struct MugshotDetails
{
    u8 mugshotType;        // Type of the mugshot
    u16 mugshotId;         // ID of the mugshot
    u8 mugshotEmotion;     // Emotion of the character
    s16 x;                 // X position of the mugshot
    s16 y;                 // Y position of the mugshot
    u8 windowType;         // Type of window (NPC, Follower, etc.)
};

struct PokemonMugshot
{
    u16 mugshotId;
    u16 mugshotFemaleId;
    u16 mugshotShinyId;
    u16 mugshotFemaleShinyId;
};

struct MugshotGfx
{
    const u32 *gfx;
    const u16 *pal;
};

extern const union AnimCmd *const gSpriteAnimTable_Mugshot_Flipped[];
extern const struct MugshotGfx gFieldMugshots[][MUGSHOT_EMOTE_COUNT];
extern struct MugshotDetails gActiveMugshotDetails;
struct MugshotDetails GetMugshotDetails(void);

#endif // GUARD_FIELD_MUGSHOTS_H
