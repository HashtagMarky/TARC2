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


def main():
    parser = argparse.ArgumentParser(description="Process 40x40 images to 64x64 indexed format.")
    parser.add_argument('input_dir', type=str, help='Absolute path to input directory containing 40x40 images')
    parser.add_argument('output_name', type=str, help='Name to use for output folder')
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


if __name__ == '__main__':
    main()
