#!/usr/bin/env python3
# file: run_with_palette.py

import os
import subprocess
from pathlib import Path

def rgb_to_hex(rgb_line: str) -> str:
    r, g, b = map(int, rgb_line.strip().split())
    return f"#{r:02x}{g:02x}{b:02x}"

def extract_two_colors(pal_path: Path) -> tuple[str, str]:
    line_color1 = 16
    line_color2 = 17
    with pal_path.open() as f:
        lines = f.readlines()
        if len(lines) < 5:
            raise ValueError(f"{pal_path} doesn't contain enough color entries.")
        color1 = rgb_to_hex(lines[line_color1])
        color2 = rgb_to_hex(lines[line_color2])
        return color1, color2

def main():
    script_dir = Path(__file__).resolve().parent
    palette_dir = script_dir / "graphics/text_window/ikigai_palettes/message_box"
    pal_files = list(palette_dir.glob("*.pal"))
    process_script = script_dir / "window_gradient_pals.py"

    if not process_script.exists():
        print("Missing process.py in the same directory.")
        return

    for pal_file in pal_files:
        try:
            color1, color2 = extract_two_colors(pal_file)
            name = pal_file.stem
            subprocess.run([
                "python3", str(process_script), color1, color2, name
            ], check=True)
        except Exception as e:
            print(f"Error processing {pal_file.name}: {e}")

if __name__ == "__main__":
    main()
