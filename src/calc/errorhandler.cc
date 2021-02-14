#include "calc/errorhandler.h"

#include <fmt/core.h>

#include "calc/output.h"


void
ErrorHandler::Err(const std::string& str)
{
    errors.emplace_back(str);
}


[[nodiscard]] bool
ErrorHandler::HasErr() const
{
    return !errors.empty();
}


void
ErrorHandler::PrintErrors(Output* output)
{
    output->PrintError("Error while parsing:");
    for (const auto& err: errors)
    {
        output->PrintError(fmt::format(" - {}", err));
    }
}

