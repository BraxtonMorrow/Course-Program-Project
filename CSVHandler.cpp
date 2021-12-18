#include <fstream>
#include <sstream>
#include "CSVHandler.hpp"

namespace csv {

    Parser::Parser(const std::string& data, const DataType& type, char sep)
        : _type(type), _sep(sep)
    {
        std::string line;
        if (type == eFILE)
        {
            _file = data;
            std::ifstream ifile(_file.c_str());
            if (ifile.is_open())
            {
                while (ifile.good())
                {
                    getline(ifile, line);
                    if (line != "")
                        _originalFile.push_back(line);
                }
                ifile.close();

                if (_originalFile.size() == 0)
                    throw Error(std::string("No Data in ").append(_file));
                parseContent();
            }
            else
                throw Error(std::string("Failed to open ").append(_file));
        }
        else
        {
            std::istringstream stream(data);
            while (std::getline(stream, line))
                if (line != "")
                    _originalFile.push_back(line);
            if (_originalFile.size() == 0)
                throw Error(std::string("No Data in pure content"));
            parseContent();
        }
    }

    Parser::~Parser(void)
    {
        std::vector<Row*>::iterator it;

        for (it = _content.begin(); it != _content.end(); it++)
            delete* it;
    }

    void Parser::parseContent(void)
    {
        std::vector<std::string>::iterator it;

        it = _originalFile.begin();

        for (; it != _originalFile.end(); it++)
        {
            bool quoted = false;
            int tokenStart = 0;
            unsigned int i = 0;

            Row* row = new Row();

            for (; i != it->length(); i++)
            {
                if (it->at(i) == '"')
                    quoted = ((quoted) ? (false) : (true));
                else if (it->at(i) == ',' && !quoted)
                {
                    row->push(it->substr(tokenStart, i - tokenStart));
                    tokenStart = i + 1;
                }
            }

            //end
            row->push(it->substr(tokenStart, it->length() - tokenStart));

            _content.push_back(row);
        }
    }

    Row& Parser::getRow(unsigned int rowPosition) const
    {
        if (rowPosition < _content.size())
            return *(_content[rowPosition]);
        throw Error("can't return this row (doesn't exist)");
    }

    Row& Parser::operator[](unsigned int rowPosition) const
    {
        return Parser::getRow(rowPosition);
    }

    unsigned int Parser::rowCount(void) const
    {
        return _content.size();
    }

    /*
    ** ROW
    */

    Row::Row() {}

    Row::~Row(void) {}

    unsigned int Row::size(void) const
    {
        return _values.size();
    }

    void Row::push(const std::string& value)
    {
        _values.push_back(value);
    }

    const std::string Row::operator[](unsigned int valuePosition) const
    {
        if (valuePosition < _values.size())
            return _values[valuePosition];
        throw Error("can't return this value (doesn't exist)");
    }

    std::ostream& operator<<(std::ostream& os, const Row& row)
    {
        for (unsigned int i = 0; i != row._values.size(); i++)
            os << row._values[i] << " | ";

        return os;
    }

    std::ofstream& operator<<(std::ofstream& os, const Row& row)
    {
        for (unsigned int i = 0; i != row._values.size(); i++)
        {
            os << row._values[i];
            if (i < row._values.size() - 1)
                os << ",";
        }
        return os;
    }
}