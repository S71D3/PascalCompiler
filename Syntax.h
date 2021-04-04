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

	// ����������
	void CompileProgram(bool& isSuccesed); // <���������>::=program <���>;<����>.

	std::string CompileName(bool &isSuccesed); // <���>::=<�����>{<�����>|<�����>}
	std::string CompileString(bool& isSuccesed); // <������>::='<�����>{<�����>|<�����>}'

	std::string CompileConst(bool& isSuccesed); // <���������>::=<����><����� ��� �����>|<����><��� ���������>|<��������� ��� �����>
	std::string CompileUnsignedConst(bool& isSuccesed); // <��������� ��� �����>::=<�����>|<������>|<��� ���������>
	std::string CompileSign(bool& isSuccesed); // <����>::=+|-

	void CompileBlock(bool& isSuccesed); // <����>::=<������ ��������><������ �����><������ ����������><������ �������� � �������><������ ����������>

	void CompileTypeSection(bool& isSuccesed); // <������ �����>::=type <����������� ����>;{<����������� ����>;}|<�����>
	void CompileTypeDeclaration(bool& isSuccesed); // <����������� ����>::=<���>=<���>
	Type* CompileType(bool& isSuccesed); // <���>::=<������� ���>|<��������� ���>

	Type* CompileSimpleType(bool& isSuccesed); // <������� ���>::=<������������ ���>|<��� ����>
	Type* CompileBoundedType(bool& isSuccesed); // <������������ ���>::=<���������>..<���������>
	Type* CompileTypeName(bool& isSuccesed); // <��� ����>::=<���>

	Type* CompileCompoundType(bool& isSuccesed); // <��������� ���>::<���������� ���>|<��������������� ���>
	Type* CompileArrayType(bool& isSuccesed); // <���������� ���>::=array[<������� ���>{,<������� ���>}] of <���>
	Type* CompileRecordType(bool& isSuccesed); // <��������������� ���>::=record <������ �����> end
	std::vector<Field> CompileFieldList(bool& isSuccesed); // <������ �����>::=<������ ������>{;<������ ������>}
	std::vector<Field> CompileField(bool& isSuccesed); // <������ ������>::=<���>{,<���>}:<���>|<�����>

	void CompileVarSection(bool& isSuccesed); // <������ ����������>::=var <�������� ���������� ����������>;{<�������� ���������� ����������>;}|<�����>
	void CompileVarDeclaration(bool& isSuccesed); // <�������� ���������� ����������>::=<���>{,<���>}:<���>

	void CompileStatementSection(bool& isSuccesed); // <������ ����������>::=<��������� ��������>
	void CompileCompoundStatement(bool& isSuccesed); // <��������� ��������>::=begin <��������>{;<��������>} end
	void CompileStatement(bool& isSuccesed); // <��������>::=<������� ��������>|<������� ��������>

	void CompileSimpleStatement(bool& isSuccesed); // <������� ��������>::=<�������� ������������>|<�����>
	void CompileAssignmentStatement(bool& isSuccesed); // <�������� ������������>::=<����������>:=<���������>
	Variable CompileVariable(bool& isSuccesed); // <����������>::=<���>{[<���������>{,<���������>}]|.<���>}

	std::string CompileExpression(bool& isSuccesed, std::string& type); // <���������>::=<������� ���������>|<������� ���������><�������� ���������><������� ���������>
	std::string CompileSimpleExpression(bool& isSuccesed, std::string& type); // <������� ���������>::=<����><���������>{<���������� ��������><���������>}
	std::string CompileTerm(bool& isSuccesed, std::string& type); // <���������>::=<���������>{<����������������� ��������><���������>}
	std::string CompileFactor(bool& isSuccesed, std::string& type); // <���������>::=<����������>|<��������� ��� �����>|(<���������>)|not <���������>
	std::string CompileRelationOperation(bool& isSuccesed); // <�������� ���������>:==<>|=|<|<=|>|>=|in
	std::string CompileAdditiveOperation(bool& isSuccesed); // <���������� ��������>::=+|-|or
	std::string CompileMultiplicativeOperation(bool& isSuccesed); // <����������������� ��������>::=*|/|div|mod|and

	void CompileComplexStatement(bool& isSuccesed); // <������� ��������>::=<��������� ��������>|<���������� ��������>|<�������� �����>|<�������� �������������>
	void CompileChooseStatement(bool& isSuccesed); // <���������� ��������>::=<�������� ��������>
	void CompileCycleStatement(bool& isSuccesed); // <�������� �����>::=<���� � ������������>
	void CompileWithStatement(bool& isSuccesed); // <�������� �������������>::=with <������ ����������> do <��������>
	void CompileConditionalStatement(bool& isSuccesed); // <�������� ��������>::=if <���������> then <��������>|if <���������> then <��������> else <��������>
	void CompileWhileCycle(bool& isSuccesed); // <���� � ������������>::=while <���������> do <��������>
	std::vector<Variable> CompileVarList(bool& isSuccesed); // <������ ����������>::=<����������>{,<����������>}

	static bool IsVariableEqual(Variable c1, Variable c2);
	static bool IsTypesEqual(Type c1, Type c2);
};