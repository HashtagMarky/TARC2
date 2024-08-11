static const u32 sFieldMugshotGfx_Brendan[] = INCBIN_U32("graphics/field_mugshots/people/brendan/brendan.4bpp.lz");
static const u16 sFieldMugshotPal_Brendan[] = INCBIN_U16("graphics/field_mugshots/people/brendan/brendan.gbapal");

static const u32 sFieldMugshotGfx_May[] = INCBIN_U32("graphics/field_mugshots/people/may/may.4bpp.lz");
static const u16 sFieldMugshotPal_May[] = INCBIN_U16("graphics/field_mugshots/people/may/may.gbapal");

static const u32 sFieldMugshotGfx_SubstituteDollNormal[] = INCBIN_U32("graphics/field_mugshots/pokemon/substitute_doll/normal.4bpp.lz");
static const u16 sFieldMugshotPal_SubstituteDollNormal[] = INCBIN_U16("graphics/field_mugshots/pokemon/substitute_doll/normal.gbapal");

#include "constants/event_objects.h"
#include "constants/species.h"

struct MugshotGfx
{
    const u32 *gfx;
    const u16 *pal;
};

static const struct MugshotGfx sFieldMugshots[][EMOTE_COUNT] =
{
    [MUGSHOT_BRENDAN] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_Brendan,
            .pal = sFieldMugshotPal_Brendan,
        },
    },

    [MUGSHOT_MAY] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_May,
            .pal = sFieldMugshotPal_May,
        },
    },

    [MUGSHOT_SUBSTITUTE_DOLL] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_SubstituteDollNormal,
            .pal = sFieldMugshotPal_SubstituteDollNormal,
        },
    },

    [MUGSHOT_SUBSTITUTE_DOLL_SHINY] =
    {
        [EMOTE_NORMAL] =
        {
            .gfx = sFieldMugshotGfx_SubstituteDollNormal,
            .pal = sFieldMugshotPal_SubstituteDollNormal,
        },
    }
};