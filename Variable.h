#pragma once
#include <string>
#include "Type.h"

class Variable {
public:
	Variable() = default;
	Variable(std::string& t_name, Type* t_type)
	{
		this->name = t_name;
		this->type = t_type;
	}
	Variable(std::string& t_name)
	{
		this->name = t_name;
	}
	~Variable() = default;

	std::string name;
	Type* type;
};