#ifndef GUARD_FIELD_MESSAGE_BOX_H
#define GUARD_FIELD_MESSAGE_BOX_H

#define NAME_UNKNOWN    NUM_OBJ_EVENT_GFX

enum
{
    FIELD_MESSAGE_BOX_HIDDEN,
    FIELD_MESSAGE_BOX_UNUSED,
    FIELD_MESSAGE_BOX_NORMAL,
    FIELD_MESSAGE_BOX_AUTO_SCROLL,
};

extern const u8* gSpeakerName;
#define WIN_NAME_PLATE 1

bool8 ShowFieldMessage(const u8 *str);
bool8 ShowPokenavFieldMessage(const u8 *str);
bool8 ShowFieldMessageFromBuffer(void);
bool8 ShowFieldAutoScrollMessage(const u8 *str);
void HideFieldMessageBox(void);
bool8 IsFieldMessageBoxHidden(void);
u8 GetFieldMessageBoxMode(void);
void StopFieldMessage(void);
void InitFieldMessageBox(void);
void SetSpeakerName(const u8* name);
void SetSpeakerAuto(u16 graphicsId);
void ReprintSpeakerName(void);

extern u8 gWalkAwayFromSignpostTimer;

#endif // GUARD_FIELD_MESSAGE_BOX_H
