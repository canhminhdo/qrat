## [2.0] - 2026-01-19

### Added
- support the `psearch` command for probabilistic reachability analysis of quantum programs.
    + `psearch in <progId> with <searchType> such that <condition> .`
    + `show prob of basis <basisState> in state <stateID> .`
    + `show amp of basis <basisState> in state <stateID> .`

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