import argparse
import os


if not os.path.exists("Makefile"):
    print("Please run this script from your root folder.")
    quit()

def hex_to_rgb(hex_color):
    hex_color = hex_color.lstrip('#')
    return tuple(int(hex_color[i:i+2], 16) for i in (0, 2, 4))


def interpolate_color(c1, c2, t):
    return tuple(int(c1[i] + (c2[i] - c1[i]) * t) for i in range(3))


def brightness(rgb):
    return 0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2]


def generate_gradient(c1, c2, steps):
    return [interpolate_color(c1, c2, i / (steps - 1)) for i in range(steps)]


def format_palette(colors):
    return "\n".join(f"{r} {g} {b}" for r, g, b in colors)

def save_palette(filepath, colors):
    filepath = f"graphics/{filepath}"
    os.makedirs(os.path.dirname(filepath), exist_ok=True)

    header = ["JASC-PAL", "0100", "16"]
    lines = header + [f"{r} {g} {b}" for r, g, b in colors]
    content = "\r\n".join(lines) + "\r\n"  # Final line ends with CRLF

    with open(filepath, 'wb') as f:
        f.write(content.encode('utf-8'))


def main():
    parser = argparse.ArgumentParser(description="Generate JASC-PAL files with gradients.")
    parser.add_argument('color1', type=str, help='First hex color (e.g., #ff0000)')
    parser.add_argument('color2', type=str, help='Second hex color (e.g., #0000ff)')
    parser.add_argument('name', type=str, help='Name of Pal File')
    args = parser.parse_args()

    c1 = hex_to_rgb(args.color1)
    c2 = hex_to_rgb(args.color2)

    # Ensure c1 is the darker color
    if brightness(c1) > brightness(c2):
        c1, c2 = c2, c1

    # Generate gradients
    gradient2 = sorted(generate_gradient(c1, c2, 2), key=brightness)     # 2 in-between, 2 total
    gradient4 = sorted(generate_gradient(c1, c2, 4), key=brightness)     # 4 in-between, 4 total
    gradient_full = sorted(generate_gradient(c1, c2, 6), key=brightness) # 6 in-between, 6 total

    # Palette 1
    palette1 = [
        (112, 200, 160),
        (248, 248, 248),
        (96, 96, 96),
        (208, 208, 200),
        (224, 8, 8),
        (248, 184, 112),
        (32, 152, 8),
        (144, 240, 144),
        (48, 80, 200),
        (160, 192, 240),
        (248, 248, 248),
        (224, 232, 224),
        (248, 248, 248),
        *reversed(gradient4[:2]),
        (72, 112, 160),
    ]
    save_palette(f"text_window/ikigai_palettes/message_box/{args.name}.pal", palette1)

    # Palette 2
    palette2 = [
        (0, 0, 0),
        (255, 255, 255),
        (255, 0, 0),
        (82, 98, 156),
        (106, 139, 189),
        (255, 255, 255),
        (106, 90, 115),
        (213, 205, 213),
        (65, 74, 123),
        (72, 72, 72),
        (255, 255, 255),
        *gradient4[:4],
        (144, 144, 144),
    ]
    save_palette(f"battle_interface/ikigai_palettes/textbox/{args.name}.pal", palette2)

    # Palette 3
    palette3 = [
        (112, 200, 160),
        (255, 255, 255),
        (222, 213, 222),
        *gradient_full[:6],
        (0, 0, 0),
        (0, 0, 0),
        (0, 0, 0),
        (0, 0, 0),
        (0, 0, 0),
        (255, 255, 255),
        (74, 65, 82),
    ]
    save_palette(f"text_window/ikigai_palettes/window_frame/{args.name}.pal", palette3)

    print(f'✅ All palettes generated!\n'
          f'static const u16 gBattleTextboxPalette_{args.name.capitalize()}[] = INCBIN_U16("graphics/battle_interface/ikigai_palettes/textbox/{args.name}.gbapal.lz");\n'
          f'static const u16 gTextWindowFrame_{args.name.capitalize()}Pal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/{args.name}.gbapal");\n'
          f'INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/{args.name}.gbapal"),')


if __name__ == '__main__':
    main()
