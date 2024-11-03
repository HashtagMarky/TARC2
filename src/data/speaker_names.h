#include "field_message_box.h"
#include "constants/event_objects.h"

static const u8 *const gSpeakerNamesText[] =
{
    [OBJ_EVENT_GFX_KOLE_NORMAL] = COMPOUND_STRING("KOLE"),
    [OBJ_EVENT_GFX_ANKA_NORMAL] = COMPOUND_STRING("ANKA"),
    [OBJ_EVENT_GFX_SAMUEL] = COMPOUND_STRING("SAMUEL"),
    [OBJ_EVENT_GFX_LAUREN] = COMPOUND_STRING("LAUREN"),
    [NAME_UNKNOWN] = COMPOUND_STRING("??????"),
};
