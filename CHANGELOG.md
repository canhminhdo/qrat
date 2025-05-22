## [1.0] - 2025-05-01

### Added
- support the `pcheck` command for probabilistic model checking of quantum programs with PRISM and Storm as alternative backends.
    + `pcheck in <progId> with '<property>' {--backend=(PRISM|Storm)} {--save-model=(yes|no)} .` 

## [1.0] - 2025-01-06

### Added

- First stable release of the project
- Support a wide range of quantum gates
- Support a search command for analyzing quantum programs
- Available commands:
    + `load <progFile> .`
    + `search {[ bound {, depth} ]} in <progId> with <searchType> such that <condition> .`
    + `show path <number> .`
    + `set show timing <on|off> .`
    + `set random seed <number> .`
    + `quit .`