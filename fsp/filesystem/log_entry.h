#include <string>

namespace fsp::fs
{
    struct LogEntry 
    {
        LogEntry() = default;
        LogEntry(std::streamoff line_number_, std::string text_, std::string file_name_)
        : line_number(line_number_), text(text_), file_name(file_name_){}

        std::streamoff line_number;
        std::string text;
        std::string file_name;
    };
}
