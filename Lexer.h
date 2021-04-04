#pragma once
#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include "Lexem.h"

class Lexer
{
public:
	Lexer();
	void Fill(const char* file_path);
	std::vector<Lexem>	ScanCode();
	Lexem ScanLex( bool skipTemp = false);
	~Lexer();
	void SetTempLex(Lexem lex) { this->tempLex.push_back(lex); }
	void SetTempLexBegin(Lexem lex) { this->tempLex.push_front(lex); }
private:
	std::ifstream		code;
	char				cursor{ -1 };
	int					line{ 0 };
	std::vector<Lexem>	lex_table;
	std::deque<Lexem>	tempLex;

	Lexem				GetLex();
	char				GetChar();

	inline char			GetCurrentCurs() { return cursor; }

};

#endif // !LEXER_H

