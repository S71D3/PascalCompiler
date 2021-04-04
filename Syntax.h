#pragma once
#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <vector>
#include "Lexer.h"
#include "Lexem.h"
#include "Variable.h"
#include "Type.h"

class Syntax {
public:
	Syntax(const std::string& file_path)
	{
		this->lexer.Fill(file_path.c_str());
		this->errorsStack = {}; //empty init

		this->SyntaxParse();
	}
	~Syntax() = default;

	void SyntaxParse();
private:
	Lexer lexer;
	std::vector<Variable> variables;
	std::vector<Type*> types = { new DefaultType("boolean", "default"), new DefaultType("integer", "default"),
		new DefaultType("real", "default"), new DefaultType("char", "default") };
	Lexem GetLex(bool skipTemp = false);

	std::vector<std::pair<Lexem, errors>> errorsStack;
	bool AddError(Lexem lex, errors error);
	static bool PrintError(Lexem lex, std::string name);
	void PrintErrorStack();

	// Грамматики
	void CompileProgram(bool& isSuccesed); // <программа>::=program <имя>;<блок>.

	std::string CompileName(bool &isSuccesed); // <имя>::=<буква>{<буква>|<цифра>}
	std::string CompileString(bool& isSuccesed); // <строка>::='<буква>{<буква>|<цифра>}'

	std::string CompileConst(bool& isSuccesed); // <константа>::=<знак><число без знака>|<знак><имя константы>|<константа без знака>
	std::string CompileUnsignedConst(bool& isSuccesed); // <константа без знака>::=<число>|<строка>|<имя константы>
	std::string CompileSign(bool& isSuccesed); // <знак>::=+|-

	void CompileBlock(bool& isSuccesed); // <блок>::=<раздел констант><раздел типов><раздел переменных><раздел процедур и функций><раздел операторов>

	void CompileTypeSection(bool& isSuccesed); // <раздел типов>::=type <определение типа>;{<определение типа>;}|<пусто>
	void CompileTypeDeclaration(bool& isSuccesed); // <определение типа>::=<имя>=<тип>
	Type* CompileType(bool& isSuccesed); // <тип>::=<простой тип>|<составной тип>

	Type* CompileSimpleType(bool& isSuccesed); // <простой тип>::=<ограниченный тип>|<имя типа>
	Type* CompileBoundedType(bool& isSuccesed); // <ограниченный тип>::=<константа>..<константа>
	Type* CompileTypeName(bool& isSuccesed); // <имя типа>::=<имя>

	Type* CompileCompoundType(bool& isSuccesed); // <составной тип>::<регулярный тип>|<комбинированный тип>
	Type* CompileArrayType(bool& isSuccesed); // <регулярный тип>::=array[<простой тип>{,<простой тип>}] of <тип>
	Type* CompileRecordType(bool& isSuccesed); // <комбинированный тип>::=record <список полей> end
	std::vector<Field> CompileFieldList(bool& isSuccesed); // <список полей>::=<секция записи>{;<секция записи>}
	std::vector<Field> CompileField(bool& isSuccesed); // <секция записи>::=<имя>{,<имя>}:<тип>|<пусто>

	void CompileVarSection(bool& isSuccesed); // <раздел переменных>::=var <описание однотипных переменных>;{<описание однотипных переменных>;}|<пусто>
	void CompileVarDeclaration(bool& isSuccesed); // <описание однотипных переменных>::=<имя>{,<имя>}:<тип>

	void CompileStatementSection(bool& isSuccesed); // <раздел операторов>::=<составной оператор>
	void CompileCompoundStatement(bool& isSuccesed); // <составной оператор>::=begin <оператор>{;<оператор>} end
	void CompileStatement(bool& isSuccesed); // <оператор>::=<простой оператор>|<сложный оператор>

	void CompileSimpleStatement(bool& isSuccesed); // <простой оператор>::=<оператор присваивания>|<пусто>
	void CompileAssignmentStatement(bool& isSuccesed); // <оператор присваивания>::=<переменная>:=<выражение>
	Variable CompileVariable(bool& isSuccesed); // <переменная>::=<имя>{[<выражение>{,<выражение>}]|.<имя>}

	std::string CompileExpression(bool& isSuccesed, std::string& type); // <выражение>::=<простое выражение>|<простое выражение><операция отношения><простое выражение>
	std::string CompileSimpleExpression(bool& isSuccesed, std::string& type); // <простое выражение>::=<знак><слагаемое>{<аддитивная операция><слагаемое>}
	std::string CompileTerm(bool& isSuccesed, std::string& type); // <слагаемое>::=<множитель>{<мультипликативная операция><множитель>}
	std::string CompileFactor(bool& isSuccesed, std::string& type); // <множитель>::=<переменная>|<константа без знака>|(<выражение>)|not <множитель>
	std::string CompileRelationOperation(bool& isSuccesed); // <операция отношения>:==<>|=|<|<=|>|>=|in
	std::string CompileAdditiveOperation(bool& isSuccesed); // <аддитивная операция>::=+|-|or
	std::string CompileMultiplicativeOperation(bool& isSuccesed); // <мультипликативная операция>::=*|/|div|mod|and

	void CompileComplexStatement(bool& isSuccesed); // <сложный оператор>::=<составной оператор>|<выбирающий оператор>|<оператор цикла>|<оператор присоединения>
	void CompileChooseStatement(bool& isSuccesed); // <выбирающий оператор>::=<условный оператор>
	void CompileCycleStatement(bool& isSuccesed); // <оператор цикла>::=<цикл с предусловием>
	void CompileWithStatement(bool& isSuccesed); // <оператор присоединения>::=with <список переменных> do <оператор>
	void CompileConditionalStatement(bool& isSuccesed); // <условный оператор>::=if <выражение> then <оператор>|if <выражение> then <оператор> else <оператор>
	void CompileWhileCycle(bool& isSuccesed); // <цикл с предусловием>::=while <выражение> do <оператор>
	std::vector<Variable> CompileVarList(bool& isSuccesed); // <список переменных>::=<переменная>{,<переменная>}

	static bool IsVariableEqual(Variable c1, Variable c2);
	static bool IsTypesEqual(Type c1, Type c2);
};