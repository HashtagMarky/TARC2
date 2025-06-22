import os
import argparse
from PIL import Image

if not os.path.exists("Makefile"):
    print("Please run this script from your root folder.")
    quit()

IMAGE_SIZE = (40, 40)
CANVAS_SIZE = (64, 64)
MAGENTA = (255, 0, 255)


def process_image(path, filename, output_dir, shiny):
    img = Image.open(path).convert('RGBA')
    if img.size != IMAGE_SIZE:
        print(f"Skipping {filename}: size {img.size} is not 40x40")
        return

    flipped = img.transpose(Image.FLIP_LEFT_RIGHT)

    canvas = Image.new('RGBA', CANVAS_SIZE, MAGENTA)
    canvas.paste(flipped, (24, 24), flipped)

    indexed = canvas.convert('RGB').quantize(colors=16, method=Image.MEDIANCUT)

    palette = indexed.getpalette()
    magenta_index = None
    for i in range(16):
        r, g, b = palette[i*3:i*3+3]
        if (r, g, b) == MAGENTA:
            magenta_index = i
            break

    if magenta_index != 0:
        if magenta_index is None:
            palette[0:3] = MAGENTA
        else:
            palette[0:3], palette[magenta_index*3:magenta_index*3+3] = \
                palette[magenta_index*3:magenta_index*3+3], palette[0:3]
        indexed.putpalette(palette)

        if magenta_index is not None:
            px = indexed.load()
            for y in range(indexed.height):
                for x in range(indexed.width):
                    if px[x, y] == 0:
                        px[x, y] = magenta_index
                    elif px[x, y] == magenta_index:
                        px[x, y] = 0

    os.makedirs(output_dir, exist_ok=True)
    base, ext = os.path.splitext(filename.lower().replace('-', '_'))
    if shiny:
        base += '_shiny'
    out_path = os.path.join(output_dir, base + '.png')
    indexed.save(out_path, format='PNG')
    return base  # return processed base name


def generate_mugshot_data(pokemon_name):
    lower_name = pokemon_name.lower()
    capitalized_name = pokemon_name.capitalize()
    mugshot_data = f"""
// {capitalized_name}
static const u32 sFieldMugshotGfx_{capitalized_name}Normal[]            = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/normal.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}NormalShiny[]       = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/normal_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Angry[]             = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/angry.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}AngryShiny[]        = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/angry_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Crying[]            = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/crying.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}CryingShiny[]       = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/crying_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Determined[]        = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/determined.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}DeterminedShiny[]   = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/determined_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Dizzy[]             = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/dizzy.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}DizzyShiny[]        = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/dizzy_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Happy[]             = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/happy.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}HappyShiny[]        = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/happy_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Inspired[]          = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/inspired.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}InspiredShiny[]     = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/inspired_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Joyous[]            = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/joyous.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}JoyousShiny[]       = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/joyous_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Pain[]              = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/pain.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}PainShiny[]         = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/pain_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Sad[]               = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/sad.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}SadShiny[]          = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/sad_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Shouting[]          = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/shouting.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}ShoutingShiny[]     = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/shouting_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Sigh[]              = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/sigh.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}SighShiny[]         = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/sigh_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Stunned[]           = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/stunned.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}StunnedShiny[]      = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/stunned_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Surprised[]         = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/surprised.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}SurprisedShiny[]    = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/surprised_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Teary[]             = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/teary_eyed.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}TearyShiny[]        = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/teary_eyed_shiny.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}Worried[]           = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/worried.4bpp.smol");
static const u32 sFieldMugshotGfx_{capitalized_name}WorriedShiny[]      = INCBIN_U32("graphics/field_mugshots/pokemon/{lower_name}/worried_shiny.4bpp.smol");

static const u16 sFieldMugshotPal_{capitalized_name}Normal[]            = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/normal.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}NormalShiny[]       = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/normal_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Angry[]             = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/angry.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}AngryShiny[]        = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/angry_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Crying[]            = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/crying.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}CryingShiny[]       = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/crying_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Determined[]        = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/determined.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}DeterminedShiny[]   = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/determined_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Dizzy[]             = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/dizzy.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}DizzyShiny[]        = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/dizzy_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Happy[]             = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/happy.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}HappyShiny[]        = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/happy_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Inspired[]          = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/inspired.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}InspiredShiny[]     = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/inspired_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Joyous[]            = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/joyous.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}JoyousShiny[]       = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/joyous_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Pain[]              = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/pain.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}PainShiny[]         = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/pain_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Sad[]               = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/sad.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}SadShiny[]          = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/sad_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Shouting[]          = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/shouting.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}ShoutingShiny[]     = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/shouting_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Sigh[]              = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/sigh.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}SighShiny[]         = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/sigh_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Stunned[]           = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/stunned.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}StunnedShiny[]      = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/stunned_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Surprised[]         = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/surprised.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}SurprisedShiny[]    = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/surprised_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Teary[]             = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/teary_eyed.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}TearyShiny[]        = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/teary_eyed_shiny.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}Worried[]           = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/worried.gbapal");
static const u16 sFieldMugshotPal_{capitalized_name}WorriedShiny[]      = INCBIN_U16("graphics/field_mugshots/pokemon/{lower_name}/worried_shiny.gbapal");


    [MUGSHOT_{capitalized_name.upper()}] =
    {{
        [EMOTE_NORMAL] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Normal,
            .pal = sFieldMugshotPal_{capitalized_name}Normal,
        }},

        [EMOTE_ANGRY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Angry,
            .pal = sFieldMugshotPal_{capitalized_name}Angry,
        }},
        
        [EMOTE_CRYING] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Crying,
            .pal = sFieldMugshotPal_{capitalized_name}Crying,
        }},
        
        [EMOTE_DETERMINED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Determined,
            .pal = sFieldMugshotPal_{capitalized_name}Determined,
        }},
        
        [EMOTE_DIZZY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Dizzy,
            .pal = sFieldMugshotPal_{capitalized_name}Dizzy,
        }},
        
        [EMOTE_HAPPY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Happy,
            .pal = sFieldMugshotPal_{capitalized_name}Happy,
        }},
        
        [EMOTE_INSPIRED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Inspired,
            .pal = sFieldMugshotPal_{capitalized_name}Inspired,
        }},
        
        [EMOTE_JOYOUS] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Joyous,
            .pal = sFieldMugshotPal_{capitalized_name}Joyous,
        }},
        
        [EMOTE_PAIN] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Pain,
            .pal = sFieldMugshotPal_{capitalized_name}Pain,
        }},
        
        [EMOTE_SAD] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Sad,
            .pal = sFieldMugshotPal_{capitalized_name}Sad,
        }},
        
        [EMOTE_SHOUTING] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Shouting,
            .pal = sFieldMugshotPal_{capitalized_name}Shouting,
        }},
        
        [EMOTE_SIGH] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Sigh,
            .pal = sFieldMugshotPal_{capitalized_name}Sigh,
        }},
        
        [EMOTE_STUNNED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Stunned,
            .pal = sFieldMugshotPal_{capitalized_name}Stunned,
        }},
        
        [EMOTE_SURPRISED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Surprised,
            .pal = sFieldMugshotPal_{capitalized_name}Surprised,
        }},
        
        [EMOTE_TEARY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Teary,
            .pal = sFieldMugshotPal_{capitalized_name}Teary,
        }},
        
        [EMOTE_WORRIED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}Worried,
            .pal = sFieldMugshotPal_{capitalized_name}Worried,
        }},
    }},

    [MUGSHOT_{capitalized_name.upper()}_SHINY] =
    {{
        [EMOTE_NORMAL] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}NormalShiny,
            .pal = sFieldMugshotPal_{capitalized_name}NormalShiny,
        }},

        [EMOTE_ANGRY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}AngryShiny,
            .pal = sFieldMugshotPal_{capitalized_name}AngryShiny,
        }},
        
        [EMOTE_CRYING] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}CryingShiny,
            .pal = sFieldMugshotPal_{capitalized_name}CryingShiny,
        }},
        
        [EMOTE_DETERMINED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}DeterminedShiny,
            .pal = sFieldMugshotPal_{capitalized_name}DeterminedShiny,
        }},
        
        [EMOTE_DIZZY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}DizzyShiny,
            .pal = sFieldMugshotPal_{capitalized_name}DizzyShiny,
        }},
        
        [EMOTE_HAPPY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}HappyShiny,
            .pal = sFieldMugshotPal_{capitalized_name}HappyShiny,
        }},
        
        [EMOTE_INSPIRED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}InspiredShiny,
            .pal = sFieldMugshotPal_{capitalized_name}InspiredShiny,
        }},
        
        [EMOTE_JOYOUS] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}JoyousShiny,
            .pal = sFieldMugshotPal_{capitalized_name}JoyousShiny,
        }},
        
        [EMOTE_PAIN] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}PainShiny,
            .pal = sFieldMugshotPal_{capitalized_name}PainShiny,
        }},
        
        [EMOTE_SAD] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}SadShiny,
            .pal = sFieldMugshotPal_{capitalized_name}SadShiny,
        }},
        
        [EMOTE_SHOUTING] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}ShoutingShiny,
            .pal = sFieldMugshotPal_{capitalized_name}ShoutingShiny,
        }},
        
        [EMOTE_SIGH] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}SighShiny,
            .pal = sFieldMugshotPal_{capitalized_name}SighShiny,
        }},
        
        [EMOTE_STUNNED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}StunnedShiny,
            .pal = sFieldMugshotPal_{capitalized_name}StunnedShiny,
        }},
        
        [EMOTE_SURPRISED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}SurprisedShiny,
            .pal = sFieldMugshotPal_{capitalized_name}SurprisedShiny,
        }},
        
        [EMOTE_TEARY] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}TearyShiny,
            .pal = sFieldMugshotPal_{capitalized_name}TearyShiny,
        }},
        
        [EMOTE_WORRIED] =
        {{
            .gfx = sFieldMugshotGfx_{capitalized_name}WorriedShiny,
            .pal = sFieldMugshotPal_{capitalized_name}WorriedShiny,
        }},
    }},
"""
    return mugshot_data

def main():
    parser = argparse.ArgumentParser(description="Process 40x40 images to 64x64 indexed format.")
    parser.add_argument('input_dir', type=str, help='Absolute path to input directory containing 40x40 images')
    parser.add_argument('output_name', type=str, help='Name to use for output folder')
    parser.add_argument('--generate-header', action='store_true', help='Print header C code to terminal')
    parser.add_argument('--shiny', action='store_true', help='Flag to append _shiny to output filenames')
    args = parser.parse_args()

    input_dir = args.input_dir
    output_name = args.output_name
    shiny = args.shiny
    output_dir = os.path.abspath(os.path.join('graphics/field_mugshots/pokemon', output_name))

    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.gif')):
            path = os.path.join(input_dir, filename)
            process_image(path, filename, output_dir, shiny)

    if args.generate_header:
        header = generate_mugshot_data(output_name)
        print(header)


if __name__ == '__main__':
    main()
