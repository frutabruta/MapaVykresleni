# MapaVykresleni

## Changelog:
- 2024-07-10
    - viapoints now have blue markers (requirement from RopidXmlTools development)
        - MapaBod
            - new element barva
        - MapaVykresleni
            - filling of color element to json
        - MapyApiStops::seznamStopPointDestinationToSeznamMapaBod
            - fill blue color to viapoint
        - all css and hrml files moved to folder copy_to_program_directory

- 2024-06-07_2
    - cestaMapa renamed to mHtmlResultPath
- 2024-06-07
    - new submodule 
        - VDV301DataStructures
    - new class MapyApiStops
        - inherits from MapaVykresleni
        - uses VDV301DataStructures
    - MapaVykresleni
        - removed all functions dependent on VDV301DataStructures
- 2023-10-30
    - using VDV301DataStructures instead of VDV301struktury
- 2023-08-16
    - oprava vykreslení dat - escapování speciálních znaků

