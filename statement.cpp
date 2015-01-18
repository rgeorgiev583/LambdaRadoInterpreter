/*
 * statement.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: radoslav
 */

#include "statement.h"
#include "compound.h"
#include "simple.h"
#include "strtok.h"
#include "exprtree.h"
#include <string>
#include <cstring>


Statement* Statement::create(const char*& code)
{
	return gotoToken(code, " \t\n\r") && *code == '{' ?
			(Statement*) new CompoundStatement(code) : (Statement*) new SimpleStatement(code);
}

