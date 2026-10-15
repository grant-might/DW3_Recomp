#!/usr/bin/env python3
"""
Writes, at the top of each stage's C file, a comment that says which map or
event the stage is, and the same names in src/field/stages/README.md.

    tools/stage_names_doc.py [-n]

The comment is the block that starts the file, before its first #include:

    /*
     * WSTAG210: Main Lobby, Asuka City.
     */

The script replaces only that block (or adds it) and rewrites the README
through tools/stage_areas.py, so it can be run again at any time, after a
rebase too. -n lists the files it would change, and fails if there are any.

The names come from the game (see STAGES): the banner FIELDSTG shows as the
player enters, "area / place" (FIELDSTG_stages gives a stage its modes,
FIELDSTG_areaNames a mode its strings of text files 0xAA and 0xB8, STAREA and
STNAME), STAGSLCT's list of the scenes, and where the banner is the same for
several stages, their exits, warps and place points (StageSlot and
StagePoint modes), their talk and event texts, and the story point
(GAME.progress) that picks them.
"""
import argparse
import re
import sys
import textwrap

import stage_areas

# {stage: (name, what tells it apart or how it is reached, or "")}. A stage
# number ending in 1 or 6 next to one ending in 0 or 5 is the Amaterasu
# server's copy of an Asuka server map (Seiryu and Qing Long, Suzaku and Zhu
# Que, Byakko and Bai Hu, Genbu and Xuan Wu are the same pairs); 920 and up
# are the European version's extra chapter.
STAGES = {
    200: ("Asuka City, Central Sector: the streets by the bridge",
          "Exits to Asuka Bridge (WSTAG205), the Main Lobby (210), the Digimon "
          "Lab (220), the Inn (235), Smith's Shop (245) and Lamb Chop (255)."),
    201: ("Amaterasu City, Central Sector: the streets by the bridge",
          "On the Amaterasu server; its Asuka server twin is WSTAG200."),
    202: ("Asuka City, Central Sector: the streets by the Cargo Tower",
          "Exits to the Underground Path (WSTAG237), the Yellow Cruiser (270), El "
          "Dorado (280) and the Cargo Tower (mode 528, WSTAG260)."),
    203: ("Amaterasu City, Central Sector: the streets by the Cargo Tower",
          "On the Amaterasu server; its Asuka server twin is WSTAG202."),
    205: ("Asuka Bridge, Central Sector", ""),
    206: ("Amaterasu Bridge, Central Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG205."),
    210: ("Main Lobby, Asuka City", ""),
    211: ("Main Lobby, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG210."),
    212: ("Main Lobby, Asuka City: the story events' copy",
          "STAGSLCT's \"Main Lobby: demo\": the first partner's download, the DO "
          "Guard, Kail asking how to change into Digimon."),
    218: ("Asuka City, Central Sector: the streets by the Admin Center",
          "Exits to the Basement Stairs (WSTAG290), the Prison Tower (295) and "
          "Admin Center 2F (300)."),
    219: ("Amaterasu City, Central Sector: the streets by the Admin Center",
          "On the Amaterasu server; its Asuka server twin is WSTAG218."),
    220: ("Digimon Lab, Asuka City", ""),
    221: ("Digimon Lab, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG220."),
    225: ("Login Room, Asuka City", ""),
    226: ("Login Room, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG225."),
    230: ("Arena Front Desk, Asuka City", ""),
    231: ("Arena Front Desk, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG230."),
    232: ("Digimon Arena, Asuka City", ""),
    233: ("Digimon Arena, Amaterasu City", ""),
    235: ("Asuka Inn 1F, Asuka City", ""),
    236: ("Amaterasu Inn 1F, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG235."),
    237: ("Underground Path, Asuka City", ""),
    238: ("Underground Path, Amaterasu City", ""),
    240: ("Asuka Inn 2F, Asuka City", ""),
    241: ("Amaterasu Inn 2F, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG240."),
    245: ("Smith's Shop, Asuka City", ""),
    246: ("Wedge's Shop, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG245."),
    250: ("Junk Shop, Asuka City", ""),
    251: ("Junk Shop, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG250."),
    255: ("Lamb Chop, Asuka City", ""),
    256: ("Lamb Chop, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG255."),
    260: ("Cargo Tower, Asuka City: the story events' scripts",
          "Mode 528 loads the file, and FIELDSTG_createStoryEvents starts it."),
    261: ("Cargo Tower, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG260."),
    270: ("Yellow Cruiser, Asuka City", ""),
    271: ("Yellow Cruiser, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG270."),
    275: ("Water Tunnel, Asuka City", ""),
    276: ("Water Tunnel, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG275."),
    280: ("El Dorado, Asuka City", ""),
    281: ("El Dorado, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG280."),
    285: ("Admin Center 1F, Asuka City", ""),
    286: ("Admin Center 1F, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG285."),
    290: ("Basement Stairs, Asuka City", ""),
    291: ("Basement Stairs, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG290."),
    295: ("Prison Tower, Asuka City", ""),
    296: ("Prison Tower, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG295."),
    300: ("Admin Center 2F, Asuka City", ""),
    301: ("Admin Center 2F, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG300."),
    305: ("Master Room, Asuka City", ""),
    306: ("Master Room, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG305."),
    310: ("A.o.A Headquarters, Asuka City", ""),
    311: ("A.o.A Headquarters, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG310."),
    315: ("Admin Center B1F, Asuka City", ""),
    316: ("Admin Center B1F, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG315."),
    320: ("Asuka Sewers, Asuka City", ""),
    321: ("Amaterasu Sewer, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG320."),
    325: ("Control Room, Asuka City", ""),
    326: ("Control Room, Amaterasu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG325."),
    330: ("Central Park, Central Sector", ""),
    331: ("Central Park, Central Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG330."),
    335: ("Wire Forest Entrance, Central Sector", ""),
    336: ("Wire Forest Entrance, Central Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG335."),
    340: ("Shell Beach, Central Sector", ""),
    341: ("Shell Beach, Central Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG340."),
    345: ("Plug Cape, Central Sector", ""),
    346: ("Plug Cape, Central Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG345."),
    350: ("West Wire Forest, East Sector", ""),
    351: ("West Wire Forest, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG350."),
    355: ("East Wire Forest, East Sector", ""),
    356: ("East Wire Forest, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG355."),
    360: ("Forest Inn, East Sector", ""),
    361: ("Forest Inn, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG360."),
    365: ("Forest Inn BF, East Sector", ""),
    366: ("Forest Inn BF, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG365."),
    370: ("Protocol Forest, East Sector", ""),
    371: ("Protocol Forest, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG370."),
    375: ("Protocol Ruins, East Sector", ""),
    376: ("Protocol Ruins, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG375."),
    380: ("Divermon's Lake, East Sector", ""),
    381: ("Divermon's Lake, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG380."),
    385: ("Duel Island, East Sector", ""),
    386: ("Duel Island, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG385."),
    395: ("Wind Prairie, East Sector", ""),
    396: ("Wind Prairie, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG395."),
    400: ("Kicking Forest, East Sector", ""),
    401: ("Kicking Forest, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG400."),
    405: ("Tyranno Valley, East Sector", ""),
    406: ("Tyranno Valley, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG405."),
    410: ("East Station, East Sector", ""),
    411: ("East Station, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG410."),
    415: ("Deeper Crevice, East Sector",
          "The gondola ride to South Sector."),
    420: ("Seiryu City, East Sector", ""),
    421: ("Qing Long City, East Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG420."),
    425: ("Zephyr Tower, Seiryu City", ""),
    426: ("Zephyr Tower, Qing Long City",
          "On the Amaterasu server; its Asuka server twin is WSTAG425."),
    430: ("Seiryu Tower, Seiryu City", ""),
    431: ("Qing Long Tower, Qing Long City",
          "On the Amaterasu server; its Asuka server twin is WSTAG430."),
    435: ("Gale Tower, Seiryu City", ""),
    436: ("Gale Tower, Qing Long City",
          "On the Amaterasu server; its Asuka server twin is WSTAG435."),
    440: ("South Station, South Sector", ""),
    441: ("South Station, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG440."),
    445: ("Bulk Swamp, South Sector", ""),
    446: ("Bulk Swamp, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG445."),
    450: ("Bulk Bridge, South Sector", ""),
    451: ("Bulk Bridge, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG450."),
    455: ("Bios Swamp, South Sector", ""),
    456: ("Bios Swamp, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG455."),
    460: ("Reliability Spot, South Sector",
          "Where the Qing Long Chief welcomes the player."),
    465: ("Tranquil Swamp, South Sector", ""),
    466: ("Tranquil Swamp, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG465."),
    470: ("Swamp Inn, South Sector", ""),
    471: ("Swamp Inn, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG470."),
    475: ("Shaman House, South Sector", ""),
    476: ("Shaman House, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG475."),
    480: ("Jungle Grave, South Sector", ""),
    481: ("Jungle Grave, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG480."),
    485: ("Phoenix Bay, South Sector", ""),
    486: ("Phoenix Bay, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG485."),
    490: ("Ether Jungle, South Sector", ""),
    491: ("Ether Jungle, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG490."),
    495: ("South Cape, South Sector", ""),
    496: ("South Cape, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG495."),
    500: ("Suzaku City, South Sector", ""),
    501: ("Zhu Que City, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG500."),
    505: ("Suzaku Inn, Suzaku City", ""),
    506: ("Zhu Que Inn, Zhu Que City",
          "On the Amaterasu server; its Asuka server twin is WSTAG505."),
    520: ("Suzaku Hall, Suzaku City", ""),
    521: ("Zhu Que Hall, Zhu Que City",
          "On the Amaterasu server; its Asuka server twin is WSTAG520."),
    525: ("Suzaku UG Lake, South Sector", ""),
    526: ("Zhu Que UG Lake, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG525."),
    530: ("Jungle Shrine, South Sector", ""),
    531: ("Jungle Shrine, South Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG530."),
    535: ("Catacomb, South Sector: before story point 0x1A",
          "The Jungle Shrine (WSTAG530) leads here (mode 579) until GAME.progress "
          "reaches 0x1A."),
    537: ("Catacomb, South Sector: from story point 0x1A",
          "The Jungle Shrine (WSTAG530) leads here (mode 580) from GAME.progress "
          "0x1A on; it has the way to the Bug Maze (540)."),
    538: ("Catacomb, South Sector, on the Amaterasu server",
          "Off Amaterasu's Jungle Shrine (WSTAG531); STAGSLCT lists it as "
          "Amaterasu's after WSTAG535 and 537."),
    540: ("Bug Maze, Network Break", ""),
    545: ("Bug Maze Pit, Network Break", ""),
    550: ("South Badland, West Sector", ""),
    551: ("South Badland, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG550."),
    555: ("Noise Desert, West Sector", ""),
    556: ("Noise Desert, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG555."),
    560: ("Pelche Oasis, West Sector", ""),
    561: ("Pelche Oasis, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG560."),
    565: ("North Badland W, West Sector", ""),
    566: ("North Badland W, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG565."),
    570: ("North Badland E, West Sector", ""),
    571: ("North Badland E, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG570."),
    575: ("Bullet Valley, West Sector", ""),
    576: ("Bullet Valley, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG575."),
    580: ("Dum Dum Factory, West Sector", ""),
    581: ("Dum Dum Factory, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG580."),
    585: ("Duct Room 01, West Sector", ""),
    586: ("Duct Room 01, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG585."),
    590: ("Duct Room 02, West Sector", ""),
    591: ("Duct Room 02, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG590."),
    595: ("Duct Room 03, West Sector", ""),
    596: ("Duct Room 03, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG595."),
    600: ("Duct Room 04, West Sector", ""),
    601: ("Duct Room 04, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG600."),
    605: ("Operation Room, West Sector", ""),
    606: ("Operation Room, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG605."),
    610: ("Secret Stairs, West Sector", ""),
    611: ("Secret Stairs, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG610."),
    615: ("Sewers, West Sector", ""),
    616: ("Sewers, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG615."),
    620: ("Secret Room, West Sector: the rebels' hideout",
          "Off the Sewers (WSTAG615): Lisa, Kail, Keith and Nick, who looks for "
          "the pass code, and the Byakko Leader."),
    621: ("Secret Room, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG620."),
    625: ("Secret Room, West Sector: the hideout later on",
          "A second copy off the Sewers (WSTAG615), where Lisa sends the player "
          "into Asuka City and the Bai Hu Chief is met."),
    630: ("S Noise Desert, West Sector", ""),
    631: ("S Noise Desert, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG630."),
    635: ("Mobius Desert, West Sector: the way in",
          "Between S Noise Desert (WSTAG630) and the Mirage Tower (645); it leads "
          "on to WSTAG640."),
    636: ("Mobius Desert, West Sector: the way in, on the Amaterasu server",
          "On the Amaterasu server; its Asuka server twin is WSTAG635."),
    640: ("Mobius Desert, West Sector: the looping part",
          "Off WSTAG635: its exits loop back into itself, its other ways lead to "
          "WSTAG630 and 635."),
    641: ("Mobius Desert, West Sector: the looping part, on the Amaterasu "
          "server",
          "On the Amaterasu server; its Asuka server twin is WSTAG640."),
    645: ("Mirage Tower, West Sector", ""),
    646: ("Mirage Tower, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG645."),
    650: ("Mirage Hall, West Sector", ""),
    651: ("Mirage Hall, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG650."),
    655: ("Mirage Room, West Sector", ""),
    656: ("Mirage Room, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG655."),
    660: ("Byakko City, West Sector", ""),
    661: ("Bai Hu City, West Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG660."),
    675: ("Byakko Dome, Byakko City", ""),
    676: ("Bai Hu Dome, Bai Hu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG675."),
    680: ("Storage Room, Byakko City",
          "The basement the player falls into; no Bai Hu copy."),
    685: ("Underground Cave, Byakko City", ""),
    686: ("Underground Cave, Bai Hu City",
          "On the Amaterasu server; its Asuka server twin is WSTAG685."),
    690: ("Boot Mountain, North Sector", ""),
    691: ("Boot Mountain, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG690."),
    695: ("Snow Mountain, North Sector", ""),
    696: ("Snow Mountain, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG695."),
    700: ("Mountain Inn, North Sector", ""),
    701: ("Mountain Inn, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG700."),
    705: ("Freeze Mountain, North Sector", ""),
    706: ("Freeze Mountain, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG705."),
    710: ("Kulon Mine, North Sector", ""),
    711: ("Kulon Mine, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG710."),
    715: ("Lake of Ice, North Sector", ""),
    716: ("Lake of Ice, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG715."),
    720: ("Legendary Gym, North Sector", ""),
    721: ("Legendary Gym, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG720."),
    725: ("Kulon Pit, North Sector", ""),
    726: ("Kulon Pit, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG725."),
    730: ("Kulon Weapons, North Sector", ""),
    731: ("Kulon Weapons, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG730."),
    735: ("Ice Dungeon, North Sector", ""),
    736: ("Ice Dungeon, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG735."),
    740: ("Fire Dungeon, North Sector", ""),
    741: ("Fire Dungeon, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG740."),
    745: ("Dark Dungeon, North Sector", ""),
    746: ("Dark Dungeon, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG745."),
    750: ("Chamber Room, North Sector: the emergency Matrix Chamber",
          "Its gates lead to the Real World and the Ice Dungeon (WSTAG735); the "
          "trip to the Real World (its 180 seconds, the Juggernaut taken) plays "
          "here."),
    755: ("Battle Gate, North Sector", ""),
    756: ("Battle Gate, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG755."),
    760: ("Genbu City, North Sector", ""),
    761: ("Xuan Wu City, North Sector",
          "On the Amaterasu server; its Asuka server twin is WSTAG760."),
    780: ("Street Corner, Kusanagi City (the Real World)",
          "Junior's town outside the MAGAMI Online Center; the news of the "
          "undersea base attack and the ending."),
    785: ("Online Center, Kusanagi City (the Real World)",
          "MAGAMI's Online Center, where Junior registers before the Chamber "
          "Room."),
    790: ("Chamber Room, Kusanagi City (the Real World)",
          "Where Junior logs into the game; Guardromon saves."),
    795: ("Magasta B1F, Undersea Base",
          "The terrorists' base in the Real World, where the 180 seconds run out "
          "and the player is digitized back."),
    800: ("Magasta B2F, Undersea Base",
          "Where the player tries to stop the Juggernaut's launch."),
    805: ("Magasta 1F, Undersea Base",
          "Where the Juggernaut launches."),
    810: ("Gunslinger 1F, Spy Satellite",
          "Reached from Amaterasu's A.o.A Headquarters (WSTAG311); its blocks "
          "open from panels."),
    815: ("Gunslinger 2F, Spy Satellite",
          "Lord Megadeath waits beyond its Warp Gate."),
    820: ("Control Room, Spy Satellite",
          "Where the player faces Lord Megadeath, then Snatchmon."),
    825: ("Seabed, Underground: below the Sewers, Duel Island, Kicking Forest "
          "and the UG Lake",
          "A tunnel under the Asuka and Amaterasu Sewers, Duel Island, Kicking "
          "Forest and the UG Lake; it joins WSTAG830, 835 and 855."),
    830: ("Seabed, Underground: below the bridges",
          "A tunnel under the Asuka and Amaterasu Bridges (WSTAG205, 206); it "
          "joins WSTAG825."),
    835: ("Seabed, Underground: below Divermon's Lake and South and West "
          "Sector",
          "A tunnel under Divermon's Lake, Ether Jungle, Suzaku City, South "
          "Badland, Pelche Oasis and North Badland E; it joins WSTAG825, 850 and "
          "855."),
    840: ("Seabed, Underground: below Shell Beach, Plug Cape, South Cape and "
          "the Lake of Ice",
          "A tunnel under Shell Beach, Plug Cape, South Cape and the Lake of Ice; "
          "it joins WSTAG845 and 855."),
    845: ("Seabed, Underground: Seehomon and Depthmon's forge",
          "A tunnel only the other tunnels reach (WSTAG840, 850, 855, 860), with "
          "Seehomon's and Depthmon's weapon forging."),
    850: ("Seabed, Underground: below Central Park, Bulk Bridge and Phoenix "
          "Bay",
          "A tunnel under Central Park, Bulk Bridge and Phoenix Bay; it joins "
          "WSTAG835, 845, 855 and 860."),
    855: ("Seabed, Underground: the junction of the Seabed tunnels",
          "A tunnel only the other tunnels reach: it joins WSTAG825, 835, 840, "
          "845 and 850."),
    860: ("Seabed, Underground: a dead end off WSTAG845 and 850",
          "A tunnel only WSTAG845 and 850 reach, with chests (the Charisma Chip, "
          "TP Chip 3)."),
    865: ("Circuit Board, Underground: entered from the sectors' maps",
          "A tunnel entered from maps of Central, East, South, West and North "
          "Sector; it joins WSTAG880 to 895."),
    870: ("Circuit Board, Underground: entered from the sectors' maps",
          "A tunnel entered from maps of Central, East, South, West and North "
          "Sector; it joins WSTAG885 to 895."),
    875: ("Circuit Board, Underground: the Black Kingz' den",
          "A tunnel only WSTAG885 to 895 reach: the Black Kingz and their leader, "
          "who has Etemon's Mic."),
    880: ("Circuit Board, Underground: reached only from the other tunnels",
          "A tunnel only WSTAG865 and 885 to 895 reach."),
    885: ("Circuit Board, Underground: a deep tunnel with Black Kingz and "
          "Numemon",
          "A tunnel only the other tunnels reach (WSTAG865 to 895), with Black "
          "Kingz and Numemon battles."),
    890: ("Circuit Board, Underground: a deep tunnel with guides to both "
          "servers",
          "A tunnel only the other tunnels reach (WSTAG865 to 895), with guides "
          "to both servers' exits."),
    895: ("Circuit Board, Underground: a deep tunnel with guides to both "
          "servers",
          "A tunnel only the other tunnels reach (WSTAG865 to 890), with guides "
          "to both servers' exits."),
    920: ("Amaterasu City, Central Sector: the streets by the bridge, in the "
          "extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 624 starts it instead of WSTAG201."),
    921: ("Amaterasu City, Central Sector: the streets by the Cargo Tower, in "
          "the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 625 starts it instead of WSTAG203."),
    922: ("Amaterasu Bridge, Central Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 626 starts it instead of WSTAG206."),
    923: ("Main Lobby, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 627 starts it instead of WSTAG211."),
    924: ("Digimon Lab, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 629 starts it instead of WSTAG221."),
    925: ("Login Room, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 630 starts it instead of WSTAG226."),
    926: ("Arena Front Desk, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 631 starts it instead of WSTAG231."),
    927: ("Digimon Arena, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 632 starts it instead of WSTAG233."),
    928: ("Amaterasu Inn 1F, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 633 starts it instead of WSTAG236."),
    929: ("Underground Path, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 634 starts it instead of WSTAG238."),
    930: ("Amaterasu Inn 2F, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 635 starts it instead of WSTAG241."),
    931: ("Wedge's Shop, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 636 starts it instead of WSTAG246."),
    932: ("Junk Shop, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 637 starts it instead of WSTAG251."),
    933: ("Lamb Chop, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 638 starts it instead of WSTAG256."),
    934: ("Cargo Tower, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 639 starts it instead of WSTAG261."),
    935: ("Yellow Cruiser, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 640 starts it instead of WSTAG271."),
    936: ("Water Tunnel, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 641 starts it instead of WSTAG276."),
    937: ("El Dorado, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 642 starts it instead of WSTAG281."),
    938: ("Amaterasu Sewer, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 650 starts it instead of WSTAG321."),
    939: ("Control Room, Amaterasu City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 651 starts it instead of WSTAG326."),
    940: ("Central Park, Central Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 652 starts it instead of WSTAG331."),
    941: ("Wire Forest Entrance, Central Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 653 starts it instead of WSTAG336."),
    942: ("Shell Beach, Central Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 654 starts it instead of WSTAG341."),
    943: ("Plug Cape, Central Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 655 starts it instead of WSTAG346."),
    944: ("West Wire Forest, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 656 starts it instead of WSTAG351."),
    945: ("East Wire Forest, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 657 starts it instead of WSTAG356."),
    946: ("Forest Inn, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 658 starts it instead of WSTAG361."),
    947: ("Forest Inn BF, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 659 starts it instead of WSTAG366."),
    948: ("Protocol Forest, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 660 starts it instead of WSTAG371."),
    949: ("Protocol Ruins, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 661 starts it instead of WSTAG376."),
    950: ("Divermon's Lake, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 662 starts it instead of WSTAG381."),
    951: ("Duel Island, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 663 starts it instead of WSTAG386."),
    952: ("Wind Prairie, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 664 starts it instead of WSTAG396."),
    953: ("Kicking Forest, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 665 starts it instead of WSTAG401."),
    954: ("Tyranno Valley, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 666 starts it instead of WSTAG406."),
    955: ("East Station, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 667 starts it instead of WSTAG411."),
    956: ("Qing Long City, East Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 668 starts it instead of WSTAG421."),
    957: ("Zephyr Tower, Qing Long City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 669 starts it instead of WSTAG426."),
    958: ("Qing Long Tower, Qing Long City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 670 starts it instead of WSTAG431."),
    959: ("Gale Tower, Qing Long City, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 671 starts it instead of WSTAG436."),
    960: ("Seabed, Underground: below the Sewers, Duel Island, Kicking Forest "
          "and the UG Lake, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 736 starts it instead of WSTAG825."),
    961: ("Seabed, Underground: below the bridges, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 737 starts it instead of WSTAG830."),
    962: ("Seabed, Underground: below Divermon's Lake and South and West "
          "Sector, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 738 starts it instead of WSTAG835."),
    963: ("Seabed, Underground: below Shell Beach, Plug Cape, South Cape and "
          "the Lake of Ice, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 739 starts it instead of WSTAG840."),
    964: ("Seabed, Underground: Seehomon and Depthmon's forge, in the extra "
          "chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 740 starts it instead of WSTAG845."),
    965: ("Seabed, Underground: below Central Park, Bulk Bridge and Phoenix "
          "Bay, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 741 starts it instead of WSTAG850."),
    966: ("Seabed, Underground: the junction of the Seabed tunnels, in the "
          "extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 742 starts it instead of WSTAG855."),
    967: ("Seabed, Underground: a dead end off WSTAG845 and 850, in the extra "
          "chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 743 starts it instead of WSTAG860."),
    968: ("Circuit Board, Underground: entered from the sectors' maps, in the "
          "extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 744 starts it instead of WSTAG865."),
    969: ("Circuit Board, Underground: entered from the sectors' maps, in the "
          "extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 745 starts it instead of WSTAG870."),
    970: ("Circuit Board, Underground: the Black Kingz' den, in the extra "
          "chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 746 starts it instead of WSTAG875."),
    971: ("Circuit Board, Underground: reached only from the other tunnels, "
          "in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 747 starts it instead of WSTAG880."),
    972: ("Circuit Board, Underground: a deep tunnel with Black Kingz and "
          "Numemon, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 748 starts it instead of WSTAG885."),
    973: ("Circuit Board, Underground: a deep tunnel with guides to both "
          "servers, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 749 starts it instead of WSTAG890."),
    974: ("Circuit Board, Underground: a deep tunnel with guides to both "
          "servers, in the extra chapter",
          "The European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), "
          "mode 750 starts it instead of WSTAG895."),
}

# The block this script writes: a comment that starts the file and names it
BLOCK = re.compile(r"\A/\*\n \* WSTAG\d{3}: .*?\*/\n\n?", re.S)


def comment(stage):
    """The comment block for a stage, wrapped to 79 columns"""
    name, detail = STAGES[stage]
    text = f"WSTAG{stage}: {name}." + (f" {detail}" if detail else "")
    lines = textwrap.wrap(text, 76)
    if len(lines) > 3:
        sys.exit(f"WSTAG{stage}: its comment takes {len(lines)} lines, more than 3")
    return "/*\n" + "".join(f" * {line}\n" for line in lines) + " */\n\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[1])
    ap.add_argument("-n", "--dry-run", action="store_true", help="only list what would change")
    args = ap.parse_args()
    files = {int(stage[len("wstag"):]): path
             for stage, paths in stage_areas.stage_files().items()
             for path in paths if path.stem == stage}
    if files.keys() != STAGES.keys():
        sys.exit(f"stages without a name: {sorted(files.keys() - STAGES.keys())}, "
                 f"names without a stage: {sorted(STAGES.keys() - files.keys())}")
    changed = []
    for stage, path in sorted(files.items()):
        text = path.read_text()
        new = comment(stage) + BLOCK.sub("", text, count=1)
        if new != text:
            changed.append(path)
            if not args.dry_run:
                path.write_text(new)
    readme = stage_areas.STAGES / "README.md"
    text = stage_areas.readme(stage_areas.stage_areas())
    if readme.read_text() != text:
        changed.append(readme)
        if not args.dry_run:
            readme.write_text(text)
    for path in changed:
        print(path.relative_to(stage_areas.ROOT))
    if args.dry_run and changed:
        sys.exit(1)


if __name__ == "__main__":
    main()
