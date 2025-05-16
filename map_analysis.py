import pandas as pd
import re

def parse_map_file(map_file):
    """Parses an entire .map file and extracts address, size, and symbol/file."""
    entries = []
    prev_address = None

    with open(map_file, "r") as f:
        for line in f:
            parts = line.split()

            # Match lines with section name, address, size, and file
            if len(parts) >= 3 and re.match(r"\.[a-zA-Z0-9_.]+", parts[0]) and re.match(r"0x[0-9A-Fa-f]+", parts[1]):
                try:
                    section = parts[0]
                    address = int(parts[1], 16)
                    size = int(parts[2], 16)
                    file_or_symbol = parts[-1] if len(parts) > 3 else "(no symbol)"

                    entries.append([section, hex(address), size, file_or_symbol])
                    prev_address = address
                except ValueError:
                    continue

            # Match symbol-only lines: address + symbol
            elif len(parts) == 2 and re.match(r"0x[0-9A-Fa-f]+", parts[0]):
                try:
                    address = int(parts[0], 16)
                    symbol = parts[1]

                    # Size is difference from previous address
                    size = address - prev_address if prev_address and address > prev_address else 0
                    entries.append(["symbol", hex(address), size, symbol])
                    prev_address = address
                except ValueError:
                    continue

    return pd.DataFrame(entries, columns=["Section", "Address", "Size (bytes)", "Symbol/File"])

# Parse single .map file
df = parse_map_file("pokeikigai.map")

# Save to CSV instead of Excel
output_file = "pokeikigai_map_analysis.csv"
df.to_csv(output_file, index=False)
