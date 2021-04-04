#pragma once
#include <fstream>      
#include <iostream>     
#include <list>
#include <string>
#include <vector>
#include "Lexer.h"
#include "Syntax.h"
//#include "GenCode.h"

int Compile(const std::string& file_path);
