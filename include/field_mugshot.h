#ifndef GUARD_FIELD_MUGSHOTS_H
#define GUARD_FIELD_MUGSHOTS_H

void CreateFieldMugshot(u8 mugshotType, u16 mugshotId, u8 mugshotEmotion, s16 x, s16 y);
void CreateFollowerFieldMugshot(u32 followerSpecies, u32 followerEmotion, bool8 shiny);
u8 CreateFieldMugshotSprite(u16 mugshotId, u8 mugshotEmotion);
void RemoveFieldMugshot(void);

#endif // GUARD_FIELD_MUGSHOTS_H
