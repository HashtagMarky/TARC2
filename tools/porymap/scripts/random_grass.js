function randInt(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min;
}

// These are the grass metatiles in pokefirered.
const grassTiles = [0x001, 0x002, 0x003, 0x004, 0x005];

// Porymap callback when a block is painted.
export function onBlockChanged(x, y, prevBlock, newBlock) {
    // Check if the user is painting a grass tile.
    if (grassTiles.indexOf(newBlock.metatileId) != -1) {
        // Choose a random grass tile and paint it on the map.
        const i = randInt(0, grassTiles.length);
        const randomise = randInt(0, 2);
        if (randomise == 1)
            map.setMetatileId(x, y, grassTiles[i]);
        else
            map.setMetatileId(x, y, grassTiles[0]);
    }
}