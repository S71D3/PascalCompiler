#pragma once
#ifndef LEXEM_H
#define LEXEM_H

#include <string>
#include <map>

enum tokens {
    unknown_tk = -1,    // we get unknown token
    program_tk = 0,     // 'program'
    var_tk,             // 'var'
    begin_tk,           // 'begin'
    end_tk,             // 'end'
    type_tk,            // 'type'
    id_tk = 8,          // any [aA-zZ][0-9]
    constant_tk = 9,    // 0-9
    arr_tk,             // 'array'
    of_tk,              // 'of'
    if_tk,              // 'if'
    then_tk,            // 'then'
    else_tk,            // 'else'
    do_tk,              // 'do'
    for_tk,             // 'for'
    to_tk,              // 'to'
    downto_tk,          // 'downto'
    bool_false_tk,      // 'false'
    bool_true_tk,       // 'true'
    break_tk,           // 'break'
    dot_tk,             // '.'
    dots_arr_tk,        // '..'
    comma_tk,           // ','
    ddt_tk,             // ':'
    semi_tk,            // ';'
    eqv_tk,             // '='
    ass_tk,             // ':='
    plus_tk,            // '+'
    minus_tk,           // '-'
    mul_tk,             // '*'
    split_tk,           // '/'
    div_tk,             // 'div'
    opb_tk,             // '('
    cpb_tk,             // ')'
    osb_tk,             // '['
    csb_tk,             // ']'
    or_tk,              // 'or'
    and_tk,             // 'and'
    xor_tk,             // 'xor'
    bool_eqv_tk,        // '=='
    bool_noneqv_tk,     // '<>'
    bool_bigger_tk,     // '>'
    bool_less_tk,       // '<'
    bool_bigeqv_tk,     // '>='
    bool_leseqv_tk,     // '<='
    comp_tk,            // 'comparison signs'
    eof_tk,             // 'end of file'
    case_tk,            // 'case'
    const_tk,           // 'const'
    fun_tk,             // 'function'
    goto_tk,            // 'goto'
    label_tk,           // 'label'
    single_quote_tk,		// ' ' '
    mod_tk,
    nil_tk,
    not_tk,
    in_tk,
    proc_tk,
    rec_tk,
    repeat_tk,
    set_tk,
    until_tk,
    while_tk,
    with_tk,
    undefined_var_tk,
    integer_var_tk,
    real_var_tk,
    bool_var_tk,
    char_var_tk,
    string_var_tk,
    packed_tk,
};

static std::map<std::string, tokens> KeyWordsMap =
{
    //{ " ", tokens::Ident },
    { "unknown", tokens::unknown_tk },
    { "program", tokens::program_tk },
    { "var", tokens::var_tk },
    { "begin", tokens::begin_tk },
    { "end", tokens::end_tk },
    { "type", tokens::type_tk },
    { "name", tokens::id_tk },
    { "digit", tokens::constant_tk },
    { "array", tokens::arr_tk },
    { "of", tokens::of_tk },
    { "if", tokens::if_tk },
    { "then", tokens::then_tk },
    { "else", tokens::else_tk },
    { "do", tokens::do_tk },
    { "for", tokens::for_tk },
    { "to", tokens::to_tk },
    { "downto", tokens::downto_tk },
    { "false", tokens::bool_false_tk },
    { "true", tokens::bool_true_tk },
    { "break", tokens::break_tk },
    { ".", tokens::dot_tk },
    { "..", tokens::dots_arr_tk },
    { ",", tokens::comma_tk },
    { ":", tokens::ddt_tk },
    { ";", tokens::semi_tk },
    { "=", tokens::eqv_tk },
    { ":=", tokens::ass_tk },
    { "+", tokens::plus_tk },
    { "-", tokens::minus_tk },
    { "*", tokens::mul_tk },
    { "/", tokens::split_tk},
    { "div", tokens::div_tk },
    { "(", tokens::opb_tk },
    { ")", tokens::cpb_tk },
    { "[", tokens::osb_tk },
    { "]", tokens::csb_tk },
    { "or", tokens::or_tk },
    { "and", tokens::and_tk },
    { "xor", tokens::xor_tk },
    { "==", tokens::bool_eqv_tk },
    { "<>", tokens::bool_noneqv_tk },
    { ">", tokens::bool_bigger_tk },
    { "<", tokens::bool_less_tk },
    { ">=", tokens::bool_bigeqv_tk },
    { "<=", tokens::bool_leseqv_tk },
    { " ' ", tokens::single_quote_tk},
    { "compration", tokens::comp_tk },
    { "end of file", tokens::eof_tk },
    { "case", tokens::case_tk },
    { "const", tokens::const_tk },
    { "function", tokens::fun_tk },
    { "goto", tokens::goto_tk },
    { "label", tokens::label_tk },
    { "mod", tokens::mod_tk },
    { "nil", tokens::nil_tk },
    { "not", tokens::not_tk },
    { "in", tokens::in_tk },
    { "procedure", tokens::proc_tk },
    { "record", tokens::rec_tk },
    { "repeat", tokens::repeat_tk },
    { "set", tokens::set_tk },
    { "until", tokens::until_tk },
    { "while", tokens::while_tk },
    { "with", tokens::with_tk },
    { "packed", tokens::packed_tk},
};

enum errors {
    UNKNOWN_LEXEM,          // Unknown lexeme
    EOF_ERR,                // End of file error
    MUST_BE_ID,             // Error: Must be identifier
    MUST_BE_SEMI,           // Error: Must be ';'
    MUST_BE_PROG,           // Error: Must be 'program'
    MUST_BE_COMMA,          // Error: Must be ','
    MUST_BE_CONST,          // Error: Must be constant
    MUST_BE_COMP,           // Error: Must be comparison sign
    MUST_BE_SIGN,           // Error: Must be + or - sign
    MUST_BE_TYPE,           // Error: Must be type of identifier
    MUST_BE_EQUAL,          // Error: Must be '='
    MUST_BE_SINGLE_QOUTE,   // Error: Must be '''
    MUST_BE_DOT,            // Error: Must be '.'
    MUST_BE_DOTS_ARR,       // Error: Must be '..'
    MUST_BE_ASS,            // Error: Must be ':='
    MUST_BE_BRACKET,        // Error: Must be '('
    MUST_BE_BRACKET_END,    // Error: Must be ')'
    MUST_BE_ARRBRACKET,     // Error: Must be '['
    MUST_BE_ARRBRACKET_END, // Error: Must be ']'
    MUST_BE_DO,             // Error: Must be 'do'
    MUST_BE_TO,             // Error: Must be 'to'
    MUST_BE_IF,             // Error: Must be 'if'
    MUST_BE_THEN,           // Error: Must be 'then'
    MUST_BE_OF,             // Error: Must be 'of'
    MUST_BE_COMPOUND,       // Error: Must be 'array' or 'record'
    MUST_BE_ARRAY,          // Error: Must be 'array'
    MUST_BE_RECORD,         // Error: Must be 'record'
    MUST_BE_BEGIN,          // Error: Must be 'begin'
    MUST_BE_END,            // Error: Must be 'end'
    MUST_BE_COMPLEX,        // Error: Must be begining of complex statement
    MUST_BE_WHILE,          // Error: Must be 'while'
    MUST_BE_WITH,           // Error: Must be 'with'
    DUPL_ID_ERR,            // Error: Duplicate declaration on identifier
    UNKNOWN_ID,             // Error: Undefined identifier
    INCORRECT_TYPE,         // Error: Incorrent type
    INCORRECT_RANGE,        // Error: Out of array range
    //Add other types of error
};

static std::map<std::string, errors> ErrorsWordsMap =
{
    { "Unknown lexeme", errors::UNKNOWN_LEXEM },
    { "End of file error", errors::EOF_ERR },
    { "Error: Must be identifier", errors::MUST_BE_ID },
    { "Error: Must be ';'", errors::MUST_BE_SEMI },
    { "Error: Must be 'program'", errors::MUST_BE_PROG },
    { "Error: Must be ','", errors::MUST_BE_COMMA },
    { "Error: Must be constant", errors::MUST_BE_CONST },
    { "Error: Must be comparison sign", errors::MUST_BE_COMP },
    { "Error: Must be + or - sign", errors::MUST_BE_SIGN },
    { "Error: Must be type of identifier", errors::MUST_BE_ID },
    { "Error: Must be '='", errors::MUST_BE_EQUAL },
    { "Error: Must be '''", errors::MUST_BE_SINGLE_QOUTE },
    { "Error: Must be '.'", errors::MUST_BE_DOT },
    { "Error: Must be '..'", errors::MUST_BE_DOTS_ARR },
    { "Error: Must be ':='", errors::MUST_BE_ASS },
    { "Error: Must be '('", errors::MUST_BE_BRACKET },
    { "Error: Must be ')'", errors::MUST_BE_BRACKET_END },
    { "Error: Must be '['", errors::MUST_BE_ARRBRACKET },
    { "Error: Must be ']'", errors::MUST_BE_ARRBRACKET_END },
    { "Error: Must be 'do'", errors::MUST_BE_DO },
    { "Error: Must be 'to'", errors::MUST_BE_TO },
    { "Error: Must be 'if'", errors::MUST_BE_IF },
    { "Error: Must be 'then'", errors::MUST_BE_THEN },
    { "Error: Must be 'of'", errors::MUST_BE_OF },
    { "Error: Must be 'array' or 'record'", errors::MUST_BE_COMP },
    { "Error: Must be 'array'", errors::MUST_BE_ARRAY },
    { "Error: Must be 'record'", errors::MUST_BE_RECORD },
    { "Error: Must be 'begin'", errors::MUST_BE_BEGIN },
    { "Error: Must be 'end'", errors::MUST_BE_END },
    { "Error: Must be begining of complex statement", errors::MUST_BE_COMPLEX },
    { "Error: Must be 'while'", errors::MUST_BE_WHILE },
    { "Error: Must be 'with'", errors::MUST_BE_WITH },
    { "Error: Duplicate declaration on identifier", errors::DUPL_ID_ERR },
    { "Error: Undefined identifier", errors::UNKNOWN_ID },
    { "Error: Incorrent type", errors::INCORRECT_TYPE },
    { "Error: Out of array range", errors::INCORRECT_RANGE },
};

class Lexem
{
public:
    Lexem() = default;
    Lexem(const std::string& t_name, tokens token_tk, int t_line) : name(t_name), token(token_tk), line(t_line) {};

    int                 GetLine() { return line; }
    tokens              GetToken() { return token; }
    const std::string&  GetName() { return name; }
    
    std::string GetDescr()
    {
        return this->GetName() +" | " + this->GetTokenStr(KeyWordsMap.begin(), KeyWordsMap.end()) + " | " + std::to_string(this->GetLine());
    }
    bool IsNull()
    {
        return (this->GetName() == "" && this->token == unknown_tk && this->line == 0);
    }
    static std::string GetErrorStr(errors error)
    {
        std::map<std::string, errors>::iterator begin = ErrorsWordsMap.begin();
        std::map<std::string, errors>::iterator end = ErrorsWordsMap.end();
        while (begin != end && begin->second != error) ++begin;
        return begin->first;
    }

private:
    std::string     name{ "" };
    tokens          token{ unknown_tk };
    int             line{ 0 };

    std::string         GetTokenStr(std::map<std::string, tokens>::iterator begin, std::map<std::string, tokens>::iterator end)
    {
        tokens nToken = this->GetToken();
        while (begin != end && begin->second != nToken) ++begin;
        return begin->first;
    }
};


#endif // !LEXEM_H

