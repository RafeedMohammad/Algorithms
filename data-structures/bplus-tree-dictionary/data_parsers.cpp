#include "data_parsers.h"

#include <cctype>
#include <fstream>
#include <iterator>

namespace
{
    void trim(std::string &s)
    {
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
            s.erase(s.begin());
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
            s.pop_back();
    }

    bool parse_json_string(const std::string &content, size_t start, size_t &end, std::string &out)
    {
        out.clear();
        if (start >= content.size() || content[start] != '"')
            return false;

        size_t i = start + 1;
        while (i < content.size())
        {
            char c = content[i];
            if (c == '\\')
            {
                if (i + 1 >= content.size())
                    return false;
                char next = content[i + 1];
                if (next == '"' || next == '\\' || next == '/')
                    out.push_back(next);
                else if (next == 'b')
                    out.push_back('\b');
                else if (next == 'f')
                    out.push_back('\f');
                else if (next == 'n')
                    out.push_back('\n');
                else if (next == 'r')
                    out.push_back('\r');
                else if (next == 't')
                    out.push_back('\t');
                else
                    out.push_back(next);
                i += 2;
            }
            else if (c == '"')
            {
                end = i + 1;
                return true;
            }
            else
            {
                out.push_back(c);
                ++i;
            }
        }
        return false;
    }
}

bool load_dict_json(const std::string &path, std::vector<std::pair<std::string, std::string>> &entries)
{
    std::ifstream in(path);
    if (!in)
        return false;

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    size_t pos = 0;

    while (true)
    {
        size_t enPos = content.find("\"en\"", pos);
        if (enPos == std::string::npos)
            break;

        size_t colon = content.find(':', enPos);
        if (colon == std::string::npos)
            break;

        size_t q1 = content.find('"', colon);
        if (q1 == std::string::npos)
            break;

        std::string en;
        size_t endPos;
        if (!parse_json_string(content, q1, endPos, en))
            break;

        size_t synPos = content.find("\"bn_syns\"", endPos);
        if (synPos == std::string::npos)
            break;

        size_t openBracket = content.find('[', synPos);
        if (openBracket == std::string::npos)
            break;

        size_t closeBracket = content.find(']', openBracket);
        if (closeBracket == std::string::npos)
            break;

        std::string bn;
        size_t stringStart = content.find('"', openBracket);
        if (stringStart != std::string::npos && stringStart < closeBracket)
        {
            size_t stringEnd;
            if (parse_json_string(content, stringStart, stringEnd, bn) && stringEnd <= closeBracket)
            {
                trim(bn);
            }
        }

        trim(en);
        if (!en.empty() && !bn.empty())
            entries.emplace_back(en, bn);

        pos = closeBracket + 1;
    }

    return true;
}
