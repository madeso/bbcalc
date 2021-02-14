add_library(calculator STATIC
    calc/calc.cc calc/calc.h
    calc/output.cc calc/output.h
    calc/errorhandler.cc calc/errorhandler.h
    calc/token.cc calc/token.h
    calc/input.h
    calc/ints.cc calc/ints.h
    calc/lexer.cc calc/lexer.h
    calc/ast.cc calc/ast.h
    calc/parser.cc calc/parser.h
    calc/binary.cc calc/binary.h
)
target_include_directories(calculator
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(calculator
    PRIVATE
    project_options
    project_warnings
)
