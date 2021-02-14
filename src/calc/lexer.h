#ifndef CALC_LEXER_H
#define CALC_LEXER_H

#include <vector>
#include <string>

#include "calc/token.h"

struct ErrorHandler;


std::vector<Token>
RunLexer(const std::string& source, ErrorHandler* errors);

#endif  // CALC_LEXER_H

