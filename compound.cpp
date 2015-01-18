/*
 * compound.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: radoslav
 */

#include "compound.h"
#include "strtok.h"
#include "environment.h"
#include "value.h"
#include <cstring>
#include <string>


void CompoundStatement::copy(const CompoundStatement& other)
{
	for (std::vector<Statement*>::const_iterator i = other.body.begin(); i != other.body.end(); i++)
		body.push_back((*i)->clone());
}

void CompoundStatement::destroy()
{
	while (!body.empty())
	{
		delete body.back();
		body.pop_back();
	}
}

CompoundStatement::CompoundStatement(const char*& code): Statement(STATEMENT_COMPOUND)
{
	if (!gotoToken(code, " \t\n\r") || *code != '{')
	{
		type = STATEMENT_INVALID;
		return;
	}

	code++;

	while (gotoToken(code, "; \t\n\r") && *code != '}')
		body.push_back(Statement::create(code));

	if (*code != '}')
		type = STATEMENT_INVALID;
	else
		code++;
}

CompoundStatement::CompoundStatement(const CompoundStatement& other): Statement(other)
{
	copy(other);
}

CompoundStatement& CompoundStatement::operator=(const CompoundStatement& other)
{
	if (&other != this)
	{
		destroy();
		Statement::operator=(other);
		copy(other);
	}

	return *this;
}

CompoundStatement::~CompoundStatement()
{
	destroy();
}

Value* CompoundStatement::execute(Environment& environment) const
{
	Environment child_environment(environment);

	for (std::vector<Statement*>::const_iterator it = body.begin(); it != body.end(); it++)
	{
		Value* retval = (*it)->execute(child_environment);

		for (Environment::iterator it = environment.begin(); it != environment.end(); it++)
		{
			Value* value = child_environment[(*it).first];

			if ((*it).second != value)
				(*it).second = value->clone();
		}

		if (retval)
			return retval;
	}

	return NULL;
}

