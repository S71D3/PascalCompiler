#pragma once
#include <string>

	class Field;
	class Type {
	public:
		Type() {};
		Type(std::string t_typeDef) { typeDef = t_typeDef; };
		~Type() {};

		std::string name{ "" };
		std::string typeDef{ "" };
		bool isScalar{ false };
	};

	class Field {
	public:
		Field() = default;
		Field(std::string& t_name, Type* t_type) { name = t_name; type = t_type; };
		~Field() {};
		Type* type;
		std::string name{ "" };
	};

	class DefaultType : public Type {
	public:
		DefaultType(const std::string& t_name, std::string t_typeDef) : Type(t_typeDef) { name = t_name; isScalar = true; };
		~DefaultType() {};
	};

	class SimpleType : public Type {
		// <имя типа>::=<имя>
	public:
		SimpleType(const std::string& t_name, Type t_type, std::string t_typeDef) : Type(t_typeDef) { name = t_name; parentType = t_type; };
		~SimpleType() {};
		Type parentType;
	};

	class BoundedType : public Type {
		// <ограниченный тип>::=<константа>..<константа>
	public:
		BoundedType(std::string t_boundTop, std::string t_boundDown, std::string t_typeDef) : Type(t_typeDef) { boundTop = t_boundTop; boundDown = t_boundDown; };
		~BoundedType() {};
		std::string boundTop;
		std::string boundDown;
	};

	class ArrayType : public Type {
		// <регулярный тип>::=array[<простой тип>{,<простой тип>}] of <тип>
	public:
		ArrayType(const std::vector<Type*> t_inTypes, Type t_parentType, std::string t_typeDef) : Type(t_typeDef) { inTypes = t_inTypes; parentType = t_parentType; };
		~ArrayType() {};
		std::vector<Type*> inTypes;
		Type parentType;
	};

	class RecordType : public Type {
		// <комбинированный тип>::=record <список полей> end
	public:
		RecordType(const std::string& t_name, std::vector<Field> t_fields, std::string t_typeDef) : Type(t_typeDef) { name = t_name; fields = t_fields; }
		~RecordType() {};
		std::vector<Field> fields;

		Field GetFieldByName(std::string n)
		{
			for (auto field : this->fields)
				if (field.name == n)
					return field;
			return Field();
		}
	};