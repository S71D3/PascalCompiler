#include "Lexer.h"
Lexer::Lexer() {}

void Lexer::Fill(const char* file_path) {
    try {
        code.open(file_path);
    }
    catch (const std::exception& exp) {
        std::string lel(exp.what());
        std::string what = "<E> Lexer: Catch exception in constructor:";
        throw std::runtime_error(what + lel);
    }
}

std::vector<Lexem> Lexer::ScanCode()
{
    try {
        if (!code.is_open()) {
            std::cerr << "<E> Can't open file" << std::endl;
            return lex_table;
        }

        while (!code.eof()) {
            Lexem tempLex = GetLex();
            lex_table.emplace_back(tempLex);
            std::cout << tempLex.GetDescr() << std::endl;
        }

        return lex_table;
    }
    catch (const std::exception& exp) {
        std::cerr << "<E> Catch exception in " << __func__ << ": " << exp.what() << std::endl;
        return lex_table;
    }
}

Lexem Lexer::ScanLex(bool skipTemp)
{
    if (this->tempLex.empty() || skipTemp)
    {
        Lexem singleLex; //проверять на null
        try {
            if (!code.is_open()) {
                std::cerr << "<E> Can't open file" << std::endl;
            }

            if (!code.eof()) {
                singleLex = GetLex();
                std::cout << singleLex.GetDescr() << std::endl;
            }

        }
        catch (const std::exception& exp) {
            std::cerr << "<E> Catch exception in " << __func__ << ": " << exp.what() << std::endl;
        }

        return singleLex;
    }
    else
    {
        Lexem toReturn = this->tempLex.at(0);
        this->tempLex.erase(tempLex.begin());
        return toReturn;
    }
}

Lexer::~Lexer() {
    code.close();
}

Lexem Lexer::GetLex()
{

    try {
        auto ch = GetCurrentCurs();
        while (ch == -1 || ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t') {
            if (ch == '\n') line++;

            ch = GetChar();

            if (code.eof()) return Lexem(std::move("EOF"), eof_tk, line); // if end of file
        }

        auto isId = [](char ch) {
            return std::isalpha(static_cast<unsigned char>(ch)) ||
                std::isdigit(static_cast<unsigned char>(ch));
        };

        std::string lex;
        if (std::isdigit(static_cast<unsigned char>(ch))) { // Constants (Numbers)
            bool flagIsDottet = false;
            while (std::isdigit(static_cast<unsigned char>(ch)))
            {
                lex += ch;
                ch = GetChar();
            }

            return Lexem(std::move(lex), constant_tk, line);
        }
        else if (std::isalpha(static_cast<unsigned char>(ch))) { // Identificators
            while (isId(ch)) {
                lex += ch;
                ch = GetChar();
            }
            if (KeyWordsMap.find(lex) == KeyWordsMap.end())
                return Lexem(std::move(lex), id_tk, line); //ID
            else
                return Lexem(std::move(lex), KeyWordsMap.at(lex), line);
        }
        else if (std::ispunct(static_cast<unsigned char>(ch))) { // Other symbols
            tokens tok{ unknown_tk };
            std::string tString(1, ch);
            if (KeyWordsMap.find(tString) == KeyWordsMap.end())
            {
                std::cerr << "<E> Unknown token " << ch << std::endl;
                tok = unknown_tk;
            }
            else
                tok = KeyWordsMap.at(tString);
            lex += ch;

            switch (tok) // Check double op-s
            {
                // ':='
            case ddt_tk:
                ch = GetChar();
                if (ch == '=') {
                    lex += ch;
                    tok = ass_tk;
                    GetChar();
                }
                break;

                // '..'
            case dot_tk:
                ch = GetChar();
                if (!code.eof())
                    if (ch == '.') {
                        lex += ch;
                        tok = dots_arr_tk;
                        GetChar();
                    }
                break;

                // '=='
            case eqv_tk:
                ch = GetChar();
                if (ch == '=') {
                    lex += ch;
                    tok = bool_eqv_tk;
                    GetChar();
                }
                break;

                // '>='
            case bool_bigger_tk:
                ch = GetChar();
                if (ch == '=') {
                    lex += ch;
                    tok = bool_bigeqv_tk;
                    GetChar();
                }
                break;

                // '<='
            case bool_less_tk:
                ch = GetChar();
                if (ch == '=') {
                    lex += ch;
                    tok = bool_leseqv_tk;
                    GetChar();
                }
                else if (ch == '>') {
                    lex += ch;
                    tok = bool_noneqv_tk;
                    GetChar();
                }
                break;

            default:
                GetChar(); // kostyl for \n
                break;
            }

            return Lexem(std::move(lex), tok, line);
        }
        else {
            std::cerr << "<E> Unknown token " << ch << std::endl;
        }

        return Lexem(std::move(""), unknown_tk, line);
    }
    catch (const std::exception&) {
        return Lexem(std::move(""), unknown_tk, line);
    }
}

char Lexer::GetChar()
{
    if (code.fail()) {
        std::cerr << "<E> Can't read from the file" << std::endl;
        throw std::runtime_error("File doesn't available");
    }

    if (!code.eof()) {
            code >> std::noskipws >> cursor;
    }
    else {
        std::cerr << "<E> File is EOF early" << std::endl;
        throw std::runtime_error("File is EOF early");
    }

    return cursor;
}

