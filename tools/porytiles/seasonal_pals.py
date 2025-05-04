import os
import csv
import sys

def is_valid_rgb(value):
    try:
        parts = list(map(int, value.strip().split()))
        return len(parts) == 3 and all(0 <= c <= 255 for c in parts)
    except ValueError:
        return False

def load_seasonal_palette(csv_path):
    if not os.path.exists(csv_path):
        print(f"Error: seasonal.csv file ('{csv_path}') does not exist.")
        sys.exit(1)

    expected_headers = ['Spring', 'Summer', 'Autumn', 'Winter']
    seasonal_map = {season: [] for season in expected_headers}

    with open(csv_path, newline='') as csvfile:
        reader = csv.DictReader(csvfile)

        # Validate headers
        if reader.fieldnames is None or sorted(reader.fieldnames) != sorted(expected_headers):
            print(f"Error: seasonal.csv must contain exactly these headers: {', '.join(expected_headers)}")
            sys.exit(1)

        for row_index, row in enumerate(reader, start=1):
            for season in expected_headers:
                value = row[season].strip()
                if not is_valid_rgb(value):
                    print(f"Error: Invalid RGB value '{value}' in row {row_index}, column '{season}'.")
                    sys.exit(1)
                color = tuple(map(int, value.split()))
                seasonal_map[season].append(color)

    return seasonal_map

def read_jasc_pal(filepath):
    with open(filepath, 'r') as f:
        lines = f.readlines()
    if lines[0].strip() != "JASC-PAL" or lines[1].strip() != "0100":
        raise ValueError(f"{filepath} is not a valid JASC PAL file.")
    count = int(lines[2].strip())
    colors = [tuple(map(int, line.strip().split())) for line in lines[3:3+count]]
    return colors

def write_jasc_pal(filepath, colors):
    with open(filepath, 'w') as f:
        f.write("JASC-PAL\n0100\n{}\n".format(len(colors)))
        for color in colors:
            f.write(f"{color[0]} {color[1]} {color[2]}\n")

def replace_spring_colors(original_colors, spring_colors, target_colors):
    changed = False
    replaced = []
    for color in original_colors:
        if color in spring_colors:
            index = spring_colors.index(color)
            new_color = target_colors[index]
            replaced.append(new_color)
            if new_color != color:
                changed = True
        else:
            replaced.append(color)
    return replaced, changed

def process_palettes(input_folder, csv_path):
    seasonal_map = load_seasonal_palette(csv_path)
    spring_colors = seasonal_map['Spring']

    for filename in os.listdir(input_folder):
        if filename.endswith(".pal"):
            original_path = os.path.join(input_folder, filename)
            original_colors = read_jasc_pal(original_path)

            for season in ['Summer', 'Autumn', 'Winter']:
                modified_colors, changed = replace_spring_colors(original_colors, spring_colors, seasonal_map[season])
                if changed:
                    season_folder = os.path.join(input_folder, season.lower())
                    os.makedirs(season_folder, exist_ok=True)
                    output_path = os.path.join(season_folder, filename)
                    write_jasc_pal(output_path, modified_colors)
                    # print(f"Saved (modified): {output_path}")
                # else:
                    # print(f"Skipped (no change for {season}): {filename}")


# -------- Main Execution --------
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python convert_palettes.py <palette_folder> <seasonal_csv>")
        sys.exit(1)

    palette_folder = sys.argv[1]
    seasonal_csv = sys.argv[2]

    process_palettes(palette_folder, seasonal_csv)
