function randInt(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min;
}

// These are the grass metatiles in Ikigai.
const grassTiles = [
    0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007,
    0x008, 0x009, 0x00A, 0x00B, 0x00C, 0x00D, 0x00E, 0x00F,
    0x013, 0x014, // Flowers
];

// Porymap callback when a block is painted.
export function onBlockChanged(x, y, prevBlock, newBlock) {
    // Check if the user is painting a grass tile.
    if (grassTiles.indexOf(newBlock.metatileId) != -1) {
        // Choose a random grass tile and paint it on the map.
        const i = randInt(0, 2 * grassTiles.length);
        if (i > grassTiles.length)
            i = 0;
        map.setMetatileId(x, y, grassTiles[i]);
    }
}