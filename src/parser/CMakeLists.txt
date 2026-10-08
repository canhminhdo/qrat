add_subdirectory(
    "${PROJECT_SOURCE_DIR}/extern/mqt-core"
    "extern/mqt-core"
    EXCLUDE_FROM_ALL)

add_subdirectory(
    "${PROJECT_SOURCE_DIR}/extern/exact-dd"
    "extern/exact-dd"
    EXCLUDE_FROM_ALL)

# COPY FILE
configure_file(prog.qw prog.qw COPYONLY)
configure_file(prog-loop.qw prog-loop.qw COPYONLY)
configure_file(lexerAux.cpp lexerAux.cpp COPYONLY)

# FLEX & BISON
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS -Wcounterexamples) # Optional param: DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
flex_target(Scanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.hpp)
add_flex_bison_dependency(Scanner Parser)

add_library(
    Parser STATIC
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    ../core/Token.cpp
    ../utility/StringTable.cpp
    ../core/Interpreter.cpp
    ../core/SyntaxProg.cpp
    ../core/global.cpp
    ../core/SymbolTable.cpp
    ../core/VarSymbol.cpp
    ../core/ConstSymbol.cpp
    ../ast/KetExpNode.cpp
    ../ast/QubitExpNode.cpp
    ../ast/OpExpNode.cpp
    ../ast/ConstExpNode.cpp
    ../ast/NumExpNode.cpp
    ../ast/UnitaryStmNode.cpp
    ../ast/StmSeq.cpp
    ../ast/SkipStmNode.cpp
    ../ast/MeasExpNode.cpp
    ../ast/CondExpNode.cpp
    ../ast/CondStmNode.cpp
    ../ast/WhileStmNode.cpp
    ../dd/SimulationBase.cpp
    ../dd/mqt/DDSimulation.cpp
    ../dd/exact/DwSimulation.cpp
    ../dd/SimulationFactory.cpp
    ../ast/CachedNode.cpp
    ../ast/EndStmNode.cpp
    ../core/SearchGraph.cpp
    ../core/StateSpaceGraph.cpp
    ../core/StateTransitionGraph.cpp
    ../ast/PropExpNode.cpp
    ../ast/InitExpNode.cpp
    ../ast/BoolExpNode.cpp
    ../utility/Timer.cpp
    ../utility/Tty.cpp
    ../utility/fileReader.cpp
    ../core/PropTable.cpp
    ../utility/FileWriter.cpp
    ../ast/AtomicStmNode.cpp
)

target_include_directories(
    Parser PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

#link the flex library if using default functions yywrap() and main()
#target_link_libraries(Parser PUBLIC ${FLEX_LIBRARIES})
target_link_libraries(Parser PUBLIC MQT::CoreDD ExactDD)
