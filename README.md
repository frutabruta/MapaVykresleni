# MapaVykresleni
Qt library to generate HTML map file from supplied coordinates, using Leaflet and OSM.
Supports receiving real-time drawing of current position using websocket on port 12345. 

## Changelog:
- 20250601_1803
    - new class GNSSWebSocketServer
    - new class TrajectoryJumper
    - mapa.js
        - now supports drawing GNSS position received from websocket
- 20250531
    - new version of - VDV301DataStructures (prevents crashes in VDV301tester)
- 20250128
    - switch to Leaflet + OSM from Mapy.cz
    - moved JS files to a folder
- 20240710
    - viapoints now have blue markers (requirement from RopidXmlTools development)
        - MapaBod
            - new element barva
        - MapaVykresleni
            - filling of color element to json
        - MapyApiStops::seznamStopPointDestinationToSeznamMapaBod
            - fill blue color to viapoint
        - all css and hrml files moved to folder copy_to_program_directory

- 20240607_2
    - cestaMapa renamed to mHtmlResultPath
- 20240607
    - new submodule 
        - VDV301DataStructures
    - new class MapyApiStops
        - inherits from MapaVykresleni
        - uses VDV301DataStructures
    - MapaVykresleni
        - removed all functions dependent on VDV301DataStructures
- 20231030
    - using VDV301DataStructures instead of VDV301struktury
- 20230816
    - oprava vykreslení dat - escapování speciálních znaků

