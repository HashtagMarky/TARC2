EventScript_DisplayPokedexHeader::
    callnative ShowPokedexHeaderMessage
    playfanfare MUS_RG_CAUGHT
    delay 150
    waitfanfare
    callnative HidePokedexHeaderMessage
    return
