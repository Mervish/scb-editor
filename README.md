# scb-editor
A program for managing .scb files (script files).

The SCB file is an archive file used in Namco's "The iDOLM@STER" to store the game's spoken text(subtitles). Main purpose of this utility is to parse the MSG-section, extract the text strings into the Json-format and re-inject it back from the same Json.

Once the SCB reconstruction is complete, the utility will be integrated into the [BNAManager](https://github.com/Mervish/bna-manager). The project mostly serves for the testing purposes, hence it contains direct links to the BNAManager files.

# Build
Requires Qt6, Boost and BNAManager.

# Current state
- [x] SCB-parsing
- [x] MSG-parsing
- [x] String injection
- [x] MSG-reconstruction
- [ ] SCB-reconstruction