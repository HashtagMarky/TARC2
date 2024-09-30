#ifndef GUARD_FIELD_MUGSHOTS_H
#define GUARD_FIELD_MUGSHOTS_H

// Player Mugshots
extern const u32 sFieldMugshotGfx_Kole[];
extern const u32 sFieldMugshotGfx_Anka[];
extern const u16 sFieldMugshotPal_Protagonist[];

void CreateFieldMugshot(u8 mugshotType, u16 mugshotId, u8 mugshotEmotion, s16 x, s16 y, bool8 retainDetails);
void CreateFollowerFieldMugshot(u32 followerSpecies, u32 followerEmotion, bool8 shiny, bool8 emotePMD);
u8 CreateFieldMugshotSprite(u16 mugshotId, u8 mugshotEmotion);
void RemoveFieldMugshot(bool8 retainDetails);
void RemoveFieldMugshotAndWindow(void);
u8 GetFieldMugshotSpriteId(void);
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

extern struct MugshotDetails gActiveMugshotDetails;
struct MugshotDetails GetMugshotDetails(void);

#endif // GUARD_FIELD_MUGSHOTS_H
