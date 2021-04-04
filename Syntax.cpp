#include "Syntax.h"

void Syntax::SyntaxParse()
{
	bool isSuccesed;
	CompileProgram(isSuccesed);
	if (!this->errorsStack.empty())
	{
		std::cerr << "unsuccesed!" << std::endl;
		PrintErrorStack();
	}
	else
		std::cerr << "successed!" << std::endl;
}

Lexem Syntax::GetLex(bool skipTemp)
{
	return this->lexer.ScanLex(skipTemp);
}

bool Syntax::AddError(Lexem lex, errors error)
{
	this->errorsStack.push_back(std::make_pair(lex, error)); //реализация для стэка ошибок
	return false;
}

bool Syntax::PrintError(Lexem lex, std::string name)
{
	std::cerr << "<E> " << name << " [" << lex.GetDescr() << "]" << std::endl;
	return false;
}

void Syntax::PrintErrorStack()
{
	for (auto error : this->errorsStack)
	{
		std::cerr << "<E> " + Lexem::GetErrorStr(error.second) + ": string " + std::to_string(error.first.GetLine() + 1) + ", token " + error.first.GetName() << std::endl;
	}
}

void Syntax::CompileProgram(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	if (tLex.GetToken() == program_tk)
	{
		CompileName(isSuccesed);
		Lexem tLex = GetLex();
		if (tLex.GetToken() == semi_tk)
		{
			CompileBlock(isSuccesed);
			tLex = GetLex();
			if (tLex.GetToken() != dot_tk)
				isSuccesed = AddError(tLex, MUST_BE_DOT);//PrintError(tLex, "Program2");
		}
		else
			isSuccesed = AddError(tLex, MUST_BE_SEMI); //PrintError(tLex, "Program1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_PROG); //PrintError(tLex, "Program");
}
// <программа>::=program <имя>;<блок>.ы

std::string Syntax::CompileName(bool& isSuccesed)
{
	std::string name;
	Lexem tLex = GetLex();
	if (tLex.GetToken() == id_tk)
	{
		name = tLex.GetName();
		isSuccesed = true;
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_ID); //PrintError(tLex, "Name");

	return name;
}
// <имя>::=<буква>{<буква>|<цифра>}

std::string Syntax::CompileConst(bool& isSuccesed)
{
	std::string tString = "";

	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case plus_tk:
	case minus_tk:
		tString += tLex.GetToken() == plus_tk ? "+" : "-";

		tLex = GetLex();
		switch (tLex.GetToken())
		{
		case constant_tk:
			tString += tLex.GetName();
			isSuccesed = true;
			break;
		case id_tk:
			this->lexer.SetTempLex(tLex);
			tString += CompileName(isSuccesed)[0]; //????
		default:
			isSuccesed = AddError(tLex, MUST_BE_SIGN); //PrintError(tLex, "Const");
		}
		break;
	default:
		this->lexer.SetTempLexBegin(tLex);
		tString += CompileUnsignedConst(isSuccesed);
	}

	return tString;
}
// <константа>::=<знак><число без знака>|<знак><имя константы>|<константа без знака>
std::string Syntax::CompileUnsignedConst(bool& isSuccesed)
{
	std::string tString = "";

	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case constant_tk:
		tString += tLex.GetName();
		tLex = GetLex();
		if (tLex.GetToken() == dot_tk)
		{
			tString += ".";
			tLex = GetLex();
			tString += tLex.GetName();
		}
		else
			this->lexer.SetTempLex(tLex);
		break;
	case single_quote_tk:
		this->lexer.SetTempLex(tLex);
		tString += CompileString(isSuccesed);
		break;
	case id_tk:
		tString += CompileName(isSuccesed);
		break;
	default:
		isSuccesed = AddError(tLex, MUST_BE_CONST); //PrintError(tLex, "UnsignedConst");
	}

	return tString;
}
// <константа без знака>::=<число>|<строка>|<имя константы>
std::string Syntax::CompileString(bool& isSuccesed)
{
	std::string tString = "";
	Lexem tLex = GetLex();
	if (tLex.GetToken() == single_quote_tk)
	{
		tLex = GetLex();
		if (tLex.GetToken() == id_tk)
		{
			tString = tLex.GetName();
			tLex = GetLex();
			if (tLex.GetToken() != single_quote_tk)
				isSuccesed = AddError(tLex, MUST_BE_SINGLE_QOUTE); //PrintError(tLex, "String");
		}
		isSuccesed = AddError(tLex, MUST_BE_ID); //PrintError(tLex, "CompileString1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_SINGLE_QOUTE); //PrintError(tLex, "CompileString");
	return tString;
}
std::string Syntax::CompileSign(bool& isSuccesed)
{
	bool status = true;
	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	if (tLex.GetToken() != plus_tk && tLex.GetToken() != minus_tk)
	{
		isSuccesed = false;
		return "";
	}
	return tLex.GetName();
}
// <знак>::=+|-

void Syntax::CompileBlock(bool& isSuccesed)
{
	CompileTypeSection(isSuccesed);
	CompileVarSection(isSuccesed);
	CompileStatementSection(isSuccesed);
}
// <блок>::=<раздел констант><раздел типов><раздел переменных><раздел процедур и функций><раздел операторов>

void Syntax::CompileTypeSection(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case var_tk:  //т.к. секция типов может отсутствовать
		this->lexer.SetTempLex(tLex);
		break;
	case type_tk:
		CompileTypeDeclaration(isSuccesed);
		do
		{
			tLex = GetLex();
			if (tLex.GetToken() == semi_tk) // закостылить обработку, если не var и не typedecl
			{
				tLex = GetLex();
				this->lexer.SetTempLex(tLex);
				if (tLex.GetToken() != var_tk)
					CompileTypeDeclaration(isSuccesed);
			}
			else
			{
				this->lexer.SetTempLex(tLex);
				break;
			}
		} while (true);
		break;
	default:
		isSuccesed = AddError(tLex, MUST_BE_TYPE); //PrintError(tLex, "TypeSection");
	}
}
// <раздел типов>::=type <определение типа>;{<определение типа>;}|<пусто>
void Syntax::CompileTypeDeclaration(bool& isSuccesed)
{
	std::string name;
	Type* parentType;

	name = CompileName(isSuccesed);
	Lexem tLex = GetLex();
	if (tLex.GetToken() != eqv_tk)
		isSuccesed = AddError(tLex, MUST_BE_EQUAL); //PrintError(tLex, "TypeDecl");
	else
		parentType = CompileType(isSuccesed);
	if (isSuccesed)
	{
		if (name == "")
			isSuccesed = AddError(tLex, INCORRECT_TYPE);
		else
			if (std::any_of(types.begin(), types.end(), [&](Type* elem) { return elem->name == parentType->name; }))
				//for (auto name : names)
				types.push_back(new SimpleType(name, *parentType, parentType->typeDef));
			else if (parentType->typeDef == "record")
			{
				RecordType* tTypet = static_cast<RecordType*>(parentType);
				types.push_back(new RecordType(name, tTypet->fields, "record"));
			}
			else
				isSuccesed = AddError(tLex, INCORRECT_TYPE);
	}
}
// <определение типа>::=<имя>=<тип>
Type* Syntax::CompileType(bool& isSuccesed)
{
	Type* type;
	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case arr_tk:
	case rec_tk:
		this->lexer.SetTempLex(tLex);
		type = CompileCompoundType(isSuccesed);
		break;
	default:
		this->lexer.SetTempLex(tLex);
		type = CompileSimpleType(isSuccesed);
		break;
	}
	return type;
}
// <тип>::=<простой тип>|<составной тип>

Type* Syntax::CompileSimpleType(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	tLex = GetLex(true);
	this->lexer.SetTempLex(tLex);

	Type* type;
	type = tLex.GetToken() == dots_arr_tk ? CompileBoundedType(isSuccesed) : CompileTypeName(isSuccesed);
	return type;
}
// <простой тип>::=<ограниченный тип>|<имя типа>
Type* Syntax::CompileBoundedType(bool& isSuccesed)
{
	Type* type = new Type();
	std::string boundUp, boundDown;
	boundDown = CompileConst(isSuccesed);
	Lexem tLex = GetLex();
	if (tLex.GetToken() == dots_arr_tk)
	{
		boundUp = CompileConst(isSuccesed);
		if (isSuccesed)
			type = new BoundedType(boundUp, boundDown, "bounded");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_DOTS_ARR); //PrintError(tLex, "BoundedType");
	return type;
}
// <ограниченный тип>::=<константа>..<константа>
Type* Syntax::CompileTypeName(bool& isSuccesed)
{
	Type* type = new Type();
	Lexem tLex = GetLex();
	if (tLex.GetToken() != id_tk)
		isSuccesed = AddError(tLex, MUST_BE_ID); //PrintError(tLex, "TypeName");
	else
	{
		for (auto tType : types)
			if (tType->name == tLex.GetName())
			{
				type = tType;
				break;
			}
		//if (!type)
			//status = AddError(tLex, INCORRECT_TYPE);
	}
	return type;
}
// <имя типа>::=<имя>

Type* Syntax::CompileCompoundType(bool& isSuccesed)
{
	Type* type = new Type();
	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case arr_tk:
		this->lexer.SetTempLex(tLex);
		type = CompileArrayType(isSuccesed);
		break;
	case rec_tk:
		this->lexer.SetTempLex(tLex);
		type = CompileRecordType(isSuccesed);
		break;
	default:
		isSuccesed = AddError(tLex, MUST_BE_COMPOUND); //PrintError(tLex, "CompoundType");
	}
	return type;
}
// <составной тип>::<регулярный тип>|<комбинированный тип>
Type* Syntax::CompileArrayType(bool& isSuccesed)
{
	Type* type = new Type();

	Lexem tLex = GetLex();
	if (tLex.GetToken() == arr_tk)
	{
		tLex = GetLex();
		if (tLex.GetToken() == osb_tk)
		{
			std::vector<Type*> inTypes;
			Type* tType;
			tType = CompileSimpleType(isSuccesed);
			inTypes.push_back(tType);
			do
			{
				tLex = GetLex();
				if (tLex.GetToken() == comma_tk)
				{
					tType = new Type();
					tType = CompileSimpleType(isSuccesed);
					inTypes.push_back(tType);
				}
				else
				{
					this->lexer.SetTempLex(tLex);
					break;
				}
			} while (true);

			tLex = GetLex();
			if (tLex.GetToken() == csb_tk && this->lexer.ScanLex().GetToken() == of_tk)
			{
				Type parentType;
				parentType = *CompileType(isSuccesed);
				if (isSuccesed)
				{
					type = new ArrayType(inTypes, parentType, "array");
				}
			}
			else
				isSuccesed = AddError(tLex, MUST_BE_ARRBRACKET_END); //PrintError(tLex, "ArrayType2");
		}
		else
			isSuccesed = AddError(tLex, MUST_BE_ARRBRACKET); //PrintError(tLex, "ArrayType1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_ARRAY); //PrintError(tLex, "ArrayType");

	return type;
}
// <регулярный тип>::=array[<простой тип>{,<простой тип>}] of <тип>
Type* Syntax::CompileRecordType(bool& isSuccesed)
{
	Type* type = new Type();

	Lexem tLex = GetLex();
	if (tLex.GetToken() == rec_tk)
	{
		std::vector<Field> fieldsList;
		fieldsList = CompileFieldList(isSuccesed);

		tLex = GetLex();
		if (tLex.GetToken() != end_tk)
			isSuccesed = AddError(tLex, MUST_BE_END); //PrintError(tLex, "RecordType1");
		else
			type = new RecordType("", fieldsList, "record");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_RECORD); //PrintError(tLex, "RecordType");

	return type;
}
// <комбинированный тип>::=record <список полей> end
std::vector<Field> Syntax::CompileFieldList(bool& isSuccesed)
{
	std::vector<Field> fieldsList;
	std::vector<Field> tFields = CompileField(isSuccesed);

	for (auto field : tFields)
		fieldsList.push_back(field);

	Lexem tLex;
	do
	{
		tLex = GetLex();
		if (tLex.GetToken() == semi_tk)
		{
			tFields = CompileField(isSuccesed);
			for (auto field : tFields)
				fieldsList.push_back(field);
		}
		else
		{
			this->lexer.SetTempLex(tLex);
			break;
		}
	} while (true);

	return fieldsList;
}
// <список полей>::=<секция записи>{;<секция записи>}
std::vector<Field> Syntax::CompileField(bool& isSuccesed)
{
	std::vector<std::string> names;
	std::vector<Field> fields;

	names.push_back(CompileName(isSuccesed));
	Lexem tLex;
	do
	{
		tLex = GetLex();
		if (tLex.GetToken() == comma_tk)
			names.push_back(CompileName(isSuccesed));
		else
		{
			this->lexer.SetTempLex(tLex);
			break;
		}
	} while (true);
	tLex = GetLex();
	if (tLex.GetToken() == ddt_tk)
	{
		Type* tType;
		tType = CompileType(isSuccesed);
		tLex = GetLex();
		if (tLex.GetToken() != semi_tk)
			isSuccesed = AddError(tLex, MUST_BE_SEMI); //PrintError(tLex, "Field");
		else
			for (auto name : names)
				fields.push_back(Field(name, tType)); //костыль, добавить обработку перечисления полей //добавил
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_DOTS_ARR); //PrintError(tLex, "CompileField");
	return fields;
}
// <секция записи>::=<имя>{,<имя>}:<тип>|<пусто>

void Syntax::CompileVarSection(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	CompileVarDeclaration(isSuccesed);
	do
	{
		tLex = GetLex();
		if (tLex.GetToken() == semi_tk)
		{
			tLex = GetLex();
			this->lexer.SetTempLex(tLex);
			if (tLex.GetToken() == begin_tk)
				break;
			else
				CompileVarDeclaration(isSuccesed);
		}
		else
		{
			this->lexer.SetTempLex(tLex);
			break;
		}
	} while (true);
}
// <раздел переменных>::=var <описание однотипных переменных>;{<описание однотипных переменных>;}|<пусто>
void Syntax::CompileVarDeclaration(bool& isSuccesed)
{
	std::vector<std::string> names = {};
	Type* type;

	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	names.push_back(CompileName(isSuccesed));
	if (isSuccesed)
	{
		do
		{
			tLex = GetLex();
			if (tLex.GetToken() == comma_tk)
				names.push_back(CompileName(isSuccesed));
			else
			{
				this->lexer.SetTempLex(tLex);
				break;
			}
		} while (true);

		tLex = GetLex();
		if (tLex.GetToken() == ddt_tk)
		{
			type = CompileType(isSuccesed);
			if (isSuccesed)
			{
				for (auto name : names)
					this->variables.push_back(Variable(name, type)); //Добавление переменных в VarScope
			}
		}
		else
			isSuccesed = AddError(tLex, MUST_BE_DOTS_ARR); //PrintError(tLex, "VarDecl1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_ID); //PrintError(tLex, "VarDecl");
}
// <описание однотипных переменных>::=<имя>{,<имя>}:<тип>

void Syntax::CompileStatementSection(bool& isSuccesed)
{
	CompileCompoundStatement(isSuccesed);
}
// <раздел операторов>::=<составной оператор>
void Syntax::CompileCompoundStatement(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	if (tLex.GetToken() == begin_tk)
	{
		CompileStatement(isSuccesed);
		do
		{
			tLex = GetLex();
			if (tLex.GetToken() == semi_tk)
			{
				tLex = GetLex();
				this->lexer.SetTempLex(tLex);
				if (tLex.GetToken() == end_tk)
					break;
				else
					CompileStatement(isSuccesed);
			}
			else
			{
				this->lexer.SetTempLex(tLex);
				break;
			}
		} while (true);

		tLex = GetLex();
		if (tLex.GetToken() != end_tk)
			isSuccesed = AddError(tLex, MUST_BE_END); //PrintError(tLex, "CompoundStatement1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_BEGIN); //PrintError(tLex, "CompoundStatement");
}
// <составной оператор>::=begin <оператор>{;<оператор>} end
void Syntax::CompileStatement(bool& isSuccesed)
{
	bool status = true;
	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case begin_tk:
	case if_tk:
	case while_tk:
	case with_tk:
		this->lexer.SetTempLex(tLex);
		CompileComplexStatement(isSuccesed);
		break;
	default:
		this->lexer.SetTempLex(tLex);
		CompileSimpleStatement(isSuccesed);
	}
}
// <оператор>::=<простой оператор>|<сложный оператор>

void Syntax::CompileSimpleStatement(bool& isSuccesed)
{
	CompileAssignmentStatement(isSuccesed);
}
// <простой оператор>::=<оператор присваивания>|<пусто>
void Syntax::CompileAssignmentStatement(bool& isSuccesed)
{
	std::string tString = "";
	Variable tVariable = CompileVariable(isSuccesed);
	Lexem tLex = GetLex();
	if (tLex.GetToken() == ass_tk)
	{
		std::string tExprType;
		tString = CompileExpression(isSuccesed, tExprType);
		auto itVar = std::find_if(std::begin(variables), std::end(variables), [&](Variable elem) { return  elem.name == tVariable.name; });
		if (itVar != variables.end())
			tVariable = *itVar;

		std::string typeName;
		if (tVariable.type->typeDef == "array")
		{
			ArrayType* arrayTemp = static_cast<ArrayType*>(tVariable.type);
			typeName = arrayTemp->parentType.name;
		}
		else if (tVariable.type->typeDef == "record")
		{
			RecordType* recordTemp = static_cast<RecordType*>(tVariable.type);
			typeName = recordTemp->GetFieldByName(tVariable.name).type->name;
			//typeName = recordTemp->typeDef;
		}
		else
			typeName = tVariable.type->name;

		if (typeName != tExprType && (!(typeName == "integer" || typeName == "real") && tExprType == "const") )
			isSuccesed = AddError(tLex, INCORRECT_TYPE);
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_ASS); //PrintError(tLex, "AssignmentStatement");
}
// <оператор присваивания>::=<переменная>:=<выражение>
Variable Syntax::CompileVariable(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	std::string tString = CompileName(isSuccesed);
	Type* tType;
	Variable tVariable(tString);

	auto itVar = std::find_if(std::begin(variables), std::end(variables), [&](Variable elem) { return  elem.name == tString; });
	if (itVar != variables.end())
	{
		do
		{
			Lexem tLex = GetLex();

			if (tLex.GetToken() == osb_tk || tLex.GetToken() == dot_tk)
			{
				std::string tString = "";
				std::string rub;
				switch (tLex.GetToken())
				{
				case dot_tk:
					tType = (itVar)->type;
					if (tType->typeDef == "record")
					{
						auto itType = std::find_if(std::begin(types), std::end(types), [&](Type* elem) { return  elem->name == tType->name; });
						RecordType* tRecType = static_cast<RecordType*>(*itType);
						std::string tName = CompileName(isSuccesed);
						if (!std::any_of(tRecType->fields.begin(), tRecType->fields.end(), [&](Field elem) { return elem.name == tName; }))
							isSuccesed = AddError(tLex, UNKNOWN_ID);
						else
							tVariable = Variable(tName, tRecType);
					}
					else
						isSuccesed = AddError(tLex, INCORRECT_TYPE);
					break;
				case osb_tk:
					tString = CompileExpression(isSuccesed, rub);
					do
					{
						tLex = GetLex();
						if (tLex.GetToken() == comma_tk)
							tString += CompileExpression(isSuccesed, rub);
						else
						{
							this->lexer.SetTempLex(tLex);
							break;
						}
					} while (true);
					tLex = GetLex();
					if (tLex.GetToken() != csb_tk)
						isSuccesed = AddError(tLex, MUST_BE_ARRBRACKET_END); //PrintError(tLex, "CompileVariable1");
					else
					{
						ArrayType* tArrType = static_cast<ArrayType*>((*itVar).type);
						BoundedType* tBoundType = static_cast<BoundedType*>(tArrType->inTypes[0]);
						if (!(tString[0] >= tBoundType->boundDown[0]) || !(tString[0] <= tBoundType->boundTop[0]))
							isSuccesed = AddError(tLex, INCORRECT_RANGE);
					}
					break;
				default:
					isSuccesed = AddError(tLex, MUST_BE_ID); //PrintError(tLex, "CompileVariable");
				}
			}
			else
			{
				this->lexer.SetTempLex(tLex);
				break;
			}

		} while (true);
	}
	else
		isSuccesed = AddError(tLex, UNKNOWN_ID);

	return tVariable;
}
// <переменная>::=<имя>{[<выражение>{,<выражение>}]|.<имя>}

std::string Syntax::CompileExpression(bool& isSuccesed, std::string& type)
{
	std::string tString = "";
	bool status = true;
	tString += CompileSimpleExpression(isSuccesed, type);
	tString += CompileRelationOperation(isSuccesed);
	if (isSuccesed)
	{
		GetLex();
		tString += CompileSimpleExpression(isSuccesed, type);
	}
	return tString;
}
// <выражение>::=<простое выражение>|<простое выражение><операция отношения><простое выражение>
std::string Syntax::CompileSimpleExpression(bool& isSuccesed, std::string& type)
{
	bool tFlag;
	std::string tString = "";

	tString += CompileSign(tFlag);
	if (tFlag)
		GetLex();
	std::string rub;
	tString += CompileTerm(isSuccesed, type);
	do
	{
		tString += CompileAdditiveOperation(tFlag);
		if (tFlag)
		{
			GetLex();
			tString += CompileTerm(isSuccesed, rub);
		}
		else
			break;
	} while (true);
	return tString;
}
// <простое выражение>::=<знак><слагаемое>{<аддитивная операция><слагаемое>}
std::string Syntax::CompileTerm(bool& isSuccesed, std::string& type)
{
	std::string tString = "";

	tString += CompileFactor(isSuccesed, type);
	do
	{
		bool tFlag;
		tString += CompileMultiplicativeOperation(tFlag);
		if (tFlag)
		{
			GetLex();
			tString += CompileTerm(isSuccesed, type);
		}
		else
			break;
	} while (true);

	return tString;
}
// <слагаемое>::=<множитель>{<мультипликативная операция><множитель>}
std::string Syntax::CompileFactor(bool& isSuccesed, std::string& type)
{
	std::string tString = "";

	Lexem tLex = GetLex();
	tString += tLex.GetName();
	std::string rub;

	switch (tLex.GetToken())
	{
	case not_tk:
		//if (!type.empty())
			type = "bool";
		tString += CompileFactor(isSuccesed, rub);
		break;
	case opb_tk:
		tString += CompileExpression(isSuccesed, type);
		tLex = GetLex();
		if (tLex.GetToken() != cpb_tk)
			isSuccesed = AddError(tLex, MUST_BE_BRACKET_END); //PrintError(tLex, "Factor");
		break;
	case constant_tk:
		//if (!type.empty())
			type = "const";
		this->lexer.SetTempLex(tLex);
		CompileUnsignedConst(isSuccesed);
		break;
	default:
		this->lexer.SetTempLex(tLex);
		Variable tVar = CompileVariable(isSuccesed);
		//if (!type.empty())
			type = tVar.name;
	}
	return tString;
}
// <множитель>::=<переменная>|<константа без знака>|(<выражение>)|not <множитель>
std::string Syntax::CompileRelationOperation(bool& isSuccesed)
{
	bool status = true;
	tokens AddOpArr[] = { bool_noneqv_tk, bool_eqv_tk, bool_less_tk, bool_leseqv_tk, bool_bigger_tk, bool_bigeqv_tk, in_tk };
	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	isSuccesed = std::find(std::begin(AddOpArr), std::end(AddOpArr), tLex.GetToken()) != std::end(AddOpArr);
	return isSuccesed ? tLex.GetName() : "";
}
// <операция отношения>:==<>|==|<|<=|>|>=|in
std::string Syntax::CompileAdditiveOperation(bool& isSuccesed)
{
	bool status = true;
	tokens AddOpArr[] = { plus_tk, minus_tk, or_tk };
	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	isSuccesed = std::find(std::begin(AddOpArr), std::end(AddOpArr), tLex.GetToken()) != std::end(AddOpArr);
	return isSuccesed ? tLex.GetName() : "";
}
// <аддитивная операция>::=+|-|or
std::string Syntax::CompileMultiplicativeOperation(bool& isSuccesed)
{
	bool status = true;
	tokens AddOpArr[] = { mul_tk, split_tk, div_tk, mod_tk, and_tk };
	Lexem tLex = GetLex();
	this->lexer.SetTempLex(tLex);
	isSuccesed = std::find(std::begin(AddOpArr), std::end(AddOpArr), tLex.GetToken()) != std::end(AddOpArr);
	return isSuccesed ? tLex.GetName() : "";
}
// <мультипликативная операция>::=*|/|div|mod|and

void Syntax::CompileComplexStatement(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	switch (tLex.GetToken())
	{
	case begin_tk:
		this->lexer.SetTempLex(tLex);
		CompileCompoundStatement(isSuccesed);
		break;
	case while_tk:
		this->lexer.SetTempLex(tLex);
		CompileCycleStatement(isSuccesed);
		break;
	case if_tk:
		this->lexer.SetTempLex(tLex);
		CompileChooseStatement(isSuccesed);
		break;
	case with_tk:
		this->lexer.SetTempLex(tLex);
		CompileWithStatement(isSuccesed);
		break;
	default:
		isSuccesed = AddError(tLex, MUST_BE_COMPLEX); //PrintError(tLex, "ComplexStatement");
	}
}
// <сложный оператор>::=<составной оператор>|<выбирающий оператор>|<оператор цикла>|<оператор присоединения>
void Syntax::CompileChooseStatement(bool& isSuccesed)
{
	CompileConditionalStatement(isSuccesed);
}
// <выбирающий оператор>::=<условный оператор>
void Syntax::CompileCycleStatement(bool& isSuccesed)
{
	CompileWhileCycle(isSuccesed);
}
// <оператор цикла>::=<цикл с предусловием>
void Syntax::CompileWithStatement(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	if (tLex.GetToken() == with_tk)
	{
		std::vector<Variable> tVariables = CompileVarList(isSuccesed);
		for (auto variable : tVariables)
			variables.push_back(variable);

		tLex = GetLex();
		if (tLex.GetToken() == do_tk)
			CompileStatement(isSuccesed);
		else
			isSuccesed = AddError(tLex, MUST_BE_DO); //PrintError(tLex, "WithStatement1");

		for (int i = 0; i < variables.size(); i++)
			for (auto element : tVariables)
				if (IsVariableEqual(variables[i], element))
					variables.erase(variables.begin() + i);
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_WITH); //PrintError(tLex, "WithStatement");
}
// <оператор присоединения>::=with <список переменных> do <оператор>
void Syntax::CompileConditionalStatement(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	if (tLex.GetToken() == if_tk)
	{
		std::string rub;
		CompileExpression(isSuccesed, rub);
		tLex = GetLex();
		if (tLex.GetToken() == then_tk)
		{
			CompileStatement(isSuccesed);
			tLex = GetLex();
			if (tLex.GetToken() == else_tk)
				CompileStatement(isSuccesed);
			else
				this->lexer.SetTempLex(tLex);
		}
		else
			isSuccesed = AddError(tLex, MUST_BE_THEN); //PrintError(tLex, "CondStatement1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_IF); //PrintError(tLex, "CondStatement");
}
// <условный оператор>::=if <выражение> then <оператор>|if <выражение> then <оператор> else <оператор>
void Syntax::CompileWhileCycle(bool& isSuccesed)
{
	Lexem tLex = GetLex();
	if (tLex.GetToken() == while_tk)
	{
		std::string rub;
		CompileExpression(isSuccesed, rub);
		tLex = GetLex();
		if (tLex.GetToken() == do_tk)
			CompileStatement(isSuccesed);
		else
			isSuccesed = AddError(tLex, MUST_BE_DO); //PrintError(tLex, "WhileCycle1");
	}
	else
		isSuccesed = AddError(tLex, MUST_BE_WHILE); //PrintError(tLex, "WhileCycle");
}
// <цикл с предусловием>::=while <выражение> do <оператор>
std::vector<Variable> Syntax::CompileVarList(bool& isSuccesed)
{
	std::vector<Variable> tVariables;
	Variable tVariable = CompileVariable(isSuccesed);

	auto itVariable = std::find_if(std::begin(variables), std::end(variables), [&](Variable elem) { return  elem.name == tVariable.name; });
	auto itType = std::find_if(std::begin(types), std::end(types), [&](Type* elem) { return  elem->name == (*itVariable).type->name; });
	for (auto variable : static_cast<RecordType*>(*itType)->fields)
		tVariables.push_back(Variable(variable.name, variable.type));

	Lexem tLex;
	do
	{
		tLex = GetLex();
		if (tLex.GetToken() == comma_tk)
		{
			tVariable = CompileVariable(isSuccesed);
			auto itVariable = std::find_if(std::begin(variables), std::end(variables), [&](Variable elem) { return  elem.name == tVariable.name; });
			auto itType = std::find_if(std::begin(types), std::end(types), [&](Type* elem) { return  elem->name == (*itVariable).type->name; });
			for (auto variable : static_cast<RecordType*>(*itType)->fields)
				tVariables.push_back(Variable(variable.name, variable.type));
		}
		else
		{
			this->lexer.SetTempLex(tLex);
			break;
		}
	} while (true);

	return tVariables;
}
// <список переменных>::=<переменная>{,<переменная>}

bool Syntax::IsVariableEqual(Variable c1, Variable c2)
{
	return c1.name == c2.name && c1.type == c2.type;// IsTypesEqual(*c1.type, *c2.type);
}

bool Syntax::IsTypesEqual(Type c1, Type c2)
{
	return c1.name == c2.name && c1.typeDef == c2.typeDef && c1.isScalar == c2.isScalar;
}