# The stage overlays

Each stage overlay, AAA/PRO/WSTAG###.PRO, is one C file, `wstag###.c`, in
the folder of the area that FIELDSTG names when the player enters it
(FIELDSTG_stages gives a stage its modes, FIELDSTG_areaNames a mode its
area, a string of text file 0xAA). `tools/stage_areas.py` moves them and
writes this list.

- `common/` holds the code that several stages share, which they include.
- `wstag260.c`, the story events' scripts, is the file mode 528 (Asuka
  City's Cargo Tower) loads, but FIELDSTG starts that mode with its own
  FIELDSTG_createStoryEvents, so the stage has no code and stays here.

The cities have two copies of their stages, one for each server (Asuka and
Amaterasu, Seiryu and Qing Long, Suzaku and Zhu Que, Byakko and Bai Hu),
and the European version adds stages of its own (920 and up) to the areas.

Each stage's file starts with a comment that says which map or event it is
(`tools/stage_names_doc.py` writes them, and the names below).

| Folder | Area | Stages | WSTAG |
|---|---|---|---|
| `central_sector/` | Central Sector | 23 | 200, 201, 202, 203, 205, 206, 218, 219, 330, 331, 335, 336, 340, 341, 345, 346, 920, 921, 922, 940, 941, 942, 943 |
| `asuka_city/` | Asuka City | 24 | 210, 212, 220, 225, 230, 232, 235, 237, 240, 245, 250, 255, 270, 275, 280, 285, 290, 295, 300, 305, 310, 315, 320, 325 |
| `amaterasu_city/` | Amaterasu City | 41 | 211, 221, 226, 231, 233, 236, 238, 241, 246, 251, 256, 261, 271, 276, 281, 286, 291, 296, 301, 306, 311, 316, 321, 326, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939 |
| `east_sector/` | East Sector | 40 | 350, 351, 355, 356, 360, 361, 365, 366, 370, 371, 375, 376, 380, 381, 385, 386, 395, 396, 400, 401, 405, 406, 410, 411, 415, 420, 421, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956 |
| `seiryu_city/` | Seiryu City | 3 | 425, 430, 435 |
| `qing_long_city/` | Qing Long City | 6 | 426, 431, 436, 957, 958, 959 |
| `south_sector/` | South Sector | 32 | 440, 441, 445, 446, 450, 451, 455, 456, 460, 465, 466, 470, 471, 475, 476, 480, 481, 485, 486, 490, 491, 495, 496, 500, 501, 525, 526, 530, 531, 535, 537, 538 |
| `suzaku_city/` | Suzaku City | 2 | 505, 520 |
| `zhu_que_city/` | Zhu Que City | 2 | 506, 521 |
| `network_break/` | Network Break | 2 | 540, 545 |
| `west_sector/` | West Sector | 45 | 550, 551, 555, 556, 560, 561, 565, 566, 570, 571, 575, 576, 580, 581, 585, 586, 590, 591, 595, 596, 600, 601, 605, 606, 610, 611, 615, 616, 620, 621, 625, 630, 631, 635, 636, 640, 641, 645, 646, 650, 651, 655, 656, 660, 661 |
| `byakko_city/` | Byakko City | 3 | 675, 680, 685 |
| `bai_hu_city/` | Bai Hu City | 2 | 676, 686 |
| `north_sector/` | North Sector | 29 | 690, 691, 695, 696, 700, 701, 705, 706, 710, 711, 715, 716, 720, 721, 725, 726, 730, 731, 735, 736, 740, 741, 745, 746, 750, 755, 756, 760, 761 |
| `kusanagi_city/` | Kusanagi City | 3 | 780, 785, 790 |
| `undersea_base/` | Undersea Base | 3 | 795, 800, 805 |
| `spy_satellite/` | Spy Satellite | 3 | 810, 815, 820 |
| `underground/` | Underground | 30 | 825, 830, 835, 840, 845, 850, 855, 860, 865, 870, 875, 880, 885, 890, 895, 960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974 |

## Central Sector (`central_sector/`)

- `wstag200`: Asuka City, Central Sector: the streets by the bridge
- `wstag201`: Amaterasu City, Central Sector: the streets by the bridge
- `wstag202`: Asuka City, Central Sector: the streets by the Cargo Tower
- `wstag203`: Amaterasu City, Central Sector: the streets by the Cargo Tower
- `wstag205`: Asuka Bridge, Central Sector
- `wstag206`: Amaterasu Bridge, Central Sector
- `wstag218`: Asuka City, Central Sector: the streets by the Admin Center
- `wstag219`: Amaterasu City, Central Sector: the streets by the Admin Center
- `wstag330`: Central Park, Central Sector
- `wstag331`: Central Park, Central Sector
- `wstag335`: Wire Forest Entrance, Central Sector
- `wstag336`: Wire Forest Entrance, Central Sector
- `wstag340`: Shell Beach, Central Sector
- `wstag341`: Shell Beach, Central Sector
- `wstag345`: Plug Cape, Central Sector
- `wstag346`: Plug Cape, Central Sector
- `wstag920`: Amaterasu City, Central Sector: the streets by the bridge, in the extra chapter
- `wstag921`: Amaterasu City, Central Sector: the streets by the Cargo Tower, in the extra chapter
- `wstag922`: Amaterasu Bridge, Central Sector, in the extra chapter
- `wstag940`: Central Park, Central Sector, in the extra chapter
- `wstag941`: Wire Forest Entrance, Central Sector, in the extra chapter
- `wstag942`: Shell Beach, Central Sector, in the extra chapter
- `wstag943`: Plug Cape, Central Sector, in the extra chapter

## Asuka City (`asuka_city/`)

- `wstag210`: Main Lobby, Asuka City
- `wstag212`: Main Lobby, Asuka City: the story events' copy
- `wstag220`: Digimon Lab, Asuka City
- `wstag225`: Login Room, Asuka City
- `wstag230`: Arena Front Desk, Asuka City
- `wstag232`: Digimon Arena, Asuka City
- `wstag235`: Asuka Inn 1F, Asuka City
- `wstag237`: Underground Path, Asuka City
- `wstag240`: Asuka Inn 2F, Asuka City
- `wstag245`: Smith's Shop, Asuka City
- `wstag250`: Junk Shop, Asuka City
- `wstag255`: Lamb Chop, Asuka City
- `wstag270`: Yellow Cruiser, Asuka City
- `wstag275`: Water Tunnel, Asuka City
- `wstag280`: El Dorado, Asuka City
- `wstag285`: Admin Center 1F, Asuka City
- `wstag290`: Basement Stairs, Asuka City
- `wstag295`: Prison Tower, Asuka City
- `wstag300`: Admin Center 2F, Asuka City
- `wstag305`: Master Room, Asuka City
- `wstag310`: A.o.A Headquarters, Asuka City
- `wstag315`: Admin Center B1F, Asuka City
- `wstag320`: Asuka Sewers, Asuka City
- `wstag325`: Control Room, Asuka City

## Amaterasu City (`amaterasu_city/`)

- `wstag211`: Main Lobby, Amaterasu City
- `wstag221`: Digimon Lab, Amaterasu City
- `wstag226`: Login Room, Amaterasu City
- `wstag231`: Arena Front Desk, Amaterasu City
- `wstag233`: Digimon Arena, Amaterasu City
- `wstag236`: Amaterasu Inn 1F, Amaterasu City
- `wstag238`: Underground Path, Amaterasu City
- `wstag241`: Amaterasu Inn 2F, Amaterasu City
- `wstag246`: Wedge's Shop, Amaterasu City
- `wstag251`: Junk Shop, Amaterasu City
- `wstag256`: Lamb Chop, Amaterasu City
- `wstag261`: Cargo Tower, Amaterasu City
- `wstag271`: Yellow Cruiser, Amaterasu City
- `wstag276`: Water Tunnel, Amaterasu City
- `wstag281`: El Dorado, Amaterasu City
- `wstag286`: Admin Center 1F, Amaterasu City
- `wstag291`: Basement Stairs, Amaterasu City
- `wstag296`: Prison Tower, Amaterasu City
- `wstag301`: Admin Center 2F, Amaterasu City
- `wstag306`: Master Room, Amaterasu City
- `wstag311`: A.o.A Headquarters, Amaterasu City
- `wstag316`: Admin Center B1F, Amaterasu City
- `wstag321`: Amaterasu Sewer, Amaterasu City
- `wstag326`: Control Room, Amaterasu City
- `wstag923`: Main Lobby, Amaterasu City, in the extra chapter
- `wstag924`: Digimon Lab, Amaterasu City, in the extra chapter
- `wstag925`: Login Room, Amaterasu City, in the extra chapter
- `wstag926`: Arena Front Desk, Amaterasu City, in the extra chapter
- `wstag927`: Digimon Arena, Amaterasu City, in the extra chapter
- `wstag928`: Amaterasu Inn 1F, Amaterasu City, in the extra chapter
- `wstag929`: Underground Path, Amaterasu City, in the extra chapter
- `wstag930`: Amaterasu Inn 2F, Amaterasu City, in the extra chapter
- `wstag931`: Wedge's Shop, Amaterasu City, in the extra chapter
- `wstag932`: Junk Shop, Amaterasu City, in the extra chapter
- `wstag933`: Lamb Chop, Amaterasu City, in the extra chapter
- `wstag934`: Cargo Tower, Amaterasu City, in the extra chapter
- `wstag935`: Yellow Cruiser, Amaterasu City, in the extra chapter
- `wstag936`: Water Tunnel, Amaterasu City, in the extra chapter
- `wstag937`: El Dorado, Amaterasu City, in the extra chapter
- `wstag938`: Amaterasu Sewer, Amaterasu City, in the extra chapter
- `wstag939`: Control Room, Amaterasu City, in the extra chapter

## East Sector (`east_sector/`)

- `wstag350`: West Wire Forest, East Sector
- `wstag351`: West Wire Forest, East Sector
- `wstag355`: East Wire Forest, East Sector
- `wstag356`: East Wire Forest, East Sector
- `wstag360`: Forest Inn, East Sector
- `wstag361`: Forest Inn, East Sector
- `wstag365`: Forest Inn BF, East Sector
- `wstag366`: Forest Inn BF, East Sector
- `wstag370`: Protocol Forest, East Sector
- `wstag371`: Protocol Forest, East Sector
- `wstag375`: Protocol Ruins, East Sector
- `wstag376`: Protocol Ruins, East Sector
- `wstag380`: Divermon's Lake, East Sector
- `wstag381`: Divermon's Lake, East Sector
- `wstag385`: Duel Island, East Sector
- `wstag386`: Duel Island, East Sector
- `wstag395`: Wind Prairie, East Sector
- `wstag396`: Wind Prairie, East Sector
- `wstag400`: Kicking Forest, East Sector
- `wstag401`: Kicking Forest, East Sector
- `wstag405`: Tyranno Valley, East Sector
- `wstag406`: Tyranno Valley, East Sector
- `wstag410`: East Station, East Sector
- `wstag411`: East Station, East Sector
- `wstag415`: Deeper Crevice, East Sector
- `wstag420`: Seiryu City, East Sector
- `wstag421`: Qing Long City, East Sector
- `wstag944`: West Wire Forest, East Sector, in the extra chapter
- `wstag945`: East Wire Forest, East Sector, in the extra chapter
- `wstag946`: Forest Inn, East Sector, in the extra chapter
- `wstag947`: Forest Inn BF, East Sector, in the extra chapter
- `wstag948`: Protocol Forest, East Sector, in the extra chapter
- `wstag949`: Protocol Ruins, East Sector, in the extra chapter
- `wstag950`: Divermon's Lake, East Sector, in the extra chapter
- `wstag951`: Duel Island, East Sector, in the extra chapter
- `wstag952`: Wind Prairie, East Sector, in the extra chapter
- `wstag953`: Kicking Forest, East Sector, in the extra chapter
- `wstag954`: Tyranno Valley, East Sector, in the extra chapter
- `wstag955`: East Station, East Sector, in the extra chapter
- `wstag956`: Qing Long City, East Sector, in the extra chapter

## Seiryu City (`seiryu_city/`)

- `wstag425`: Zephyr Tower, Seiryu City
- `wstag430`: Seiryu Tower, Seiryu City
- `wstag435`: Gale Tower, Seiryu City

## Qing Long City (`qing_long_city/`)

- `wstag426`: Zephyr Tower, Qing Long City
- `wstag431`: Qing Long Tower, Qing Long City
- `wstag436`: Gale Tower, Qing Long City
- `wstag957`: Zephyr Tower, Qing Long City, in the extra chapter
- `wstag958`: Qing Long Tower, Qing Long City, in the extra chapter
- `wstag959`: Gale Tower, Qing Long City, in the extra chapter

## South Sector (`south_sector/`)

- `wstag440`: South Station, South Sector
- `wstag441`: South Station, South Sector
- `wstag445`: Bulk Swamp, South Sector
- `wstag446`: Bulk Swamp, South Sector
- `wstag450`: Bulk Bridge, South Sector
- `wstag451`: Bulk Bridge, South Sector
- `wstag455`: Bios Swamp, South Sector
- `wstag456`: Bios Swamp, South Sector
- `wstag460`: Reliability Spot, South Sector
- `wstag465`: Tranquil Swamp, South Sector
- `wstag466`: Tranquil Swamp, South Sector
- `wstag470`: Swamp Inn, South Sector
- `wstag471`: Swamp Inn, South Sector
- `wstag475`: Shaman House, South Sector
- `wstag476`: Shaman House, South Sector
- `wstag480`: Jungle Grave, South Sector
- `wstag481`: Jungle Grave, South Sector
- `wstag485`: Phoenix Bay, South Sector
- `wstag486`: Phoenix Bay, South Sector
- `wstag490`: Ether Jungle, South Sector
- `wstag491`: Ether Jungle, South Sector
- `wstag495`: South Cape, South Sector
- `wstag496`: South Cape, South Sector
- `wstag500`: Suzaku City, South Sector
- `wstag501`: Zhu Que City, South Sector
- `wstag525`: Suzaku UG Lake, South Sector
- `wstag526`: Zhu Que UG Lake, South Sector
- `wstag530`: Jungle Shrine, South Sector
- `wstag531`: Jungle Shrine, South Sector
- `wstag535`: Catacomb, South Sector: before story point 0x1A
- `wstag537`: Catacomb, South Sector: from story point 0x1A
- `wstag538`: Catacomb, South Sector, on the Amaterasu server

## Suzaku City (`suzaku_city/`)

- `wstag505`: Suzaku Inn, Suzaku City
- `wstag520`: Suzaku Hall, Suzaku City

## Zhu Que City (`zhu_que_city/`)

- `wstag506`: Zhu Que Inn, Zhu Que City
- `wstag521`: Zhu Que Hall, Zhu Que City

## Network Break (`network_break/`)

- `wstag540`: Bug Maze, Network Break
- `wstag545`: Bug Maze Pit, Network Break

## West Sector (`west_sector/`)

- `wstag550`: South Badland, West Sector
- `wstag551`: South Badland, West Sector
- `wstag555`: Noise Desert, West Sector
- `wstag556`: Noise Desert, West Sector
- `wstag560`: Pelche Oasis, West Sector
- `wstag561`: Pelche Oasis, West Sector
- `wstag565`: North Badland W, West Sector
- `wstag566`: North Badland W, West Sector
- `wstag570`: North Badland E, West Sector
- `wstag571`: North Badland E, West Sector
- `wstag575`: Bullet Valley, West Sector
- `wstag576`: Bullet Valley, West Sector
- `wstag580`: Dum Dum Factory, West Sector
- `wstag581`: Dum Dum Factory, West Sector
- `wstag585`: Duct Room 01, West Sector
- `wstag586`: Duct Room 01, West Sector
- `wstag590`: Duct Room 02, West Sector
- `wstag591`: Duct Room 02, West Sector
- `wstag595`: Duct Room 03, West Sector
- `wstag596`: Duct Room 03, West Sector
- `wstag600`: Duct Room 04, West Sector
- `wstag601`: Duct Room 04, West Sector
- `wstag605`: Operation Room, West Sector
- `wstag606`: Operation Room, West Sector
- `wstag610`: Secret Stairs, West Sector
- `wstag611`: Secret Stairs, West Sector
- `wstag615`: Sewers, West Sector
- `wstag616`: Sewers, West Sector
- `wstag620`: Secret Room, West Sector: the rebels' hideout
- `wstag621`: Secret Room, West Sector
- `wstag625`: Secret Room, West Sector: the hideout later on
- `wstag630`: S Noise Desert, West Sector
- `wstag631`: S Noise Desert, West Sector
- `wstag635`: Mobius Desert, West Sector: the way in
- `wstag636`: Mobius Desert, West Sector: the way in, on the Amaterasu server
- `wstag640`: Mobius Desert, West Sector: the looping part
- `wstag641`: Mobius Desert, West Sector: the looping part, on the Amaterasu server
- `wstag645`: Mirage Tower, West Sector
- `wstag646`: Mirage Tower, West Sector
- `wstag650`: Mirage Hall, West Sector
- `wstag651`: Mirage Hall, West Sector
- `wstag655`: Mirage Room, West Sector
- `wstag656`: Mirage Room, West Sector
- `wstag660`: Byakko City, West Sector
- `wstag661`: Bai Hu City, West Sector

## Byakko City (`byakko_city/`)

- `wstag675`: Byakko Dome, Byakko City
- `wstag680`: Storage Room, Byakko City
- `wstag685`: Underground Cave, Byakko City

## Bai Hu City (`bai_hu_city/`)

- `wstag676`: Bai Hu Dome, Bai Hu City
- `wstag686`: Underground Cave, Bai Hu City

## North Sector (`north_sector/`)

- `wstag690`: Boot Mountain, North Sector
- `wstag691`: Boot Mountain, North Sector
- `wstag695`: Snow Mountain, North Sector
- `wstag696`: Snow Mountain, North Sector
- `wstag700`: Mountain Inn, North Sector
- `wstag701`: Mountain Inn, North Sector
- `wstag705`: Freeze Mountain, North Sector
- `wstag706`: Freeze Mountain, North Sector
- `wstag710`: Kulon Mine, North Sector
- `wstag711`: Kulon Mine, North Sector
- `wstag715`: Lake of Ice, North Sector
- `wstag716`: Lake of Ice, North Sector
- `wstag720`: Legendary Gym, North Sector
- `wstag721`: Legendary Gym, North Sector
- `wstag725`: Kulon Pit, North Sector
- `wstag726`: Kulon Pit, North Sector
- `wstag730`: Kulon Weapons, North Sector
- `wstag731`: Kulon Weapons, North Sector
- `wstag735`: Ice Dungeon, North Sector
- `wstag736`: Ice Dungeon, North Sector
- `wstag740`: Fire Dungeon, North Sector
- `wstag741`: Fire Dungeon, North Sector
- `wstag745`: Dark Dungeon, North Sector
- `wstag746`: Dark Dungeon, North Sector
- `wstag750`: Chamber Room, North Sector: the emergency Matrix Chamber
- `wstag755`: Battle Gate, North Sector
- `wstag756`: Battle Gate, North Sector
- `wstag760`: Genbu City, North Sector
- `wstag761`: Xuan Wu City, North Sector

## Kusanagi City (`kusanagi_city/`)

- `wstag780`: Street Corner, Kusanagi City (the Real World)
- `wstag785`: Online Center, Kusanagi City (the Real World)
- `wstag790`: Chamber Room, Kusanagi City (the Real World)

## Undersea Base (`undersea_base/`)

- `wstag795`: Magasta B1F, Undersea Base
- `wstag800`: Magasta B2F, Undersea Base
- `wstag805`: Magasta 1F, Undersea Base

## Spy Satellite (`spy_satellite/`)

- `wstag810`: Gunslinger 1F, Spy Satellite
- `wstag815`: Gunslinger 2F, Spy Satellite
- `wstag820`: Control Room, Spy Satellite

## Underground (`underground/`)

- `wstag825`: Seabed, Underground: below the Sewers, Duel Island, Kicking Forest and the UG Lake
- `wstag830`: Seabed, Underground: below the bridges
- `wstag835`: Seabed, Underground: below Divermon's Lake and South and West Sector
- `wstag840`: Seabed, Underground: below Shell Beach, Plug Cape, South Cape and the Lake of Ice
- `wstag845`: Seabed, Underground: Seehomon and Depthmon's forge
- `wstag850`: Seabed, Underground: below Central Park, Bulk Bridge and Phoenix Bay
- `wstag855`: Seabed, Underground: the junction of the Seabed tunnels
- `wstag860`: Seabed, Underground: a dead end off WSTAG845 and 850
- `wstag865`: Circuit Board, Underground: entered from the sectors' maps
- `wstag870`: Circuit Board, Underground: entered from the sectors' maps
- `wstag875`: Circuit Board, Underground: the Black Kingz' den
- `wstag880`: Circuit Board, Underground: reached only from the other tunnels
- `wstag885`: Circuit Board, Underground: a deep tunnel with Black Kingz and Numemon
- `wstag890`: Circuit Board, Underground: a deep tunnel with guides to both servers
- `wstag895`: Circuit Board, Underground: a deep tunnel with guides to both servers
- `wstag960`: Seabed, Underground: below the Sewers, Duel Island, Kicking Forest and the UG Lake, in the extra chapter
- `wstag961`: Seabed, Underground: below the bridges, in the extra chapter
- `wstag962`: Seabed, Underground: below Divermon's Lake and South and West Sector, in the extra chapter
- `wstag963`: Seabed, Underground: below Shell Beach, Plug Cape, South Cape and the Lake of Ice, in the extra chapter
- `wstag964`: Seabed, Underground: Seehomon and Depthmon's forge, in the extra chapter
- `wstag965`: Seabed, Underground: below Central Park, Bulk Bridge and Phoenix Bay, in the extra chapter
- `wstag966`: Seabed, Underground: the junction of the Seabed tunnels, in the extra chapter
- `wstag967`: Seabed, Underground: a dead end off WSTAG845 and 850, in the extra chapter
- `wstag968`: Circuit Board, Underground: entered from the sectors' maps, in the extra chapter
- `wstag969`: Circuit Board, Underground: entered from the sectors' maps, in the extra chapter
- `wstag970`: Circuit Board, Underground: the Black Kingz' den, in the extra chapter
- `wstag971`: Circuit Board, Underground: reached only from the other tunnels, in the extra chapter
- `wstag972`: Circuit Board, Underground: a deep tunnel with Black Kingz and Numemon, in the extra chapter
- `wstag973`: Circuit Board, Underground: a deep tunnel with guides to both servers, in the extra chapter
- `wstag974`: Circuit Board, Underground: a deep tunnel with guides to both servers, in the extra chapter
