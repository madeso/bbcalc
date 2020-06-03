#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include <vector>
#include <memory>

#include "calc/ast.h"
#include "calc/token.h"

struct ErrorHandler;


std::shared_ptr<Node>
RunParser(const std::vector<Token>& tokens, ErrorHandler* errors);


#endif  // CALC_PARSER_H

