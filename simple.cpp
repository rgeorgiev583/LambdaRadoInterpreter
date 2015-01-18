/*
 * simple.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: radoslav
 */

#include "simple.h"
#include "strtok.h"
#include "identifier.h"
#include "environment.h"
#include "integer.h"
#include "string.h"
#include <iostream>
#include <string>
#include <cstring>


SimpleStatement::SimpleStatement(const char*& code): Statement(STATEMENT_SIMPLE)
{
	std::string lhs_expr = peekToken(code, " \t\n\r");

	if (lhs_expr == "input")
	{
		type = STATEMENT_INPUT;
		ignoreToken(code, " \t\n\r");
		lhs = Identifier(code);
	}
	else
	{
		if (lhs_expr == "return")
		{
			type = STATEMENT_RETURN;
			ignoreToken(code, " \t\n\r");
		}
		else if (lhs_expr == "print")
		{
			type = STATEMENT_OUTPUT;
			ignoreToken(code, " \t\n\r");
		}
		else
		{
			type = STATEMENT_ASSIGNMENT;
			Identifier lhs_assign = Identifier(code);

			if (gotoToken(code, " \t\n\r") && *code == '=')
			{
				lhs = lhs_assign;
				code++;
			}
			else
			{
				type = SIMPLE_STATEMENT_INVALID;
				Statement::type = STATEMENT_INVALID;
				return;
			}
		}

		rhs = ExpressionTree::createExpressionTree(code);
	}

	if (*code != ';')
	{
		type = SIMPLE_STATEMENT_INVALID;
		Statement::type = STATEMENT_INVALID;
	}
}

Value* SimpleStatement::execute(Environment& environment) const
{
	ExpressionTree* etree = (ExpressionTree*) &rhs;

	switch (type)
	{
	case STATEMENT_INPUT:
		{
			std::string raw_input, line;

			do
			{
				getline(std::cin, line);
				raw_input += line + '\n';
			}
			while (!std::cin.eof());

			const char* input_iterator = raw_input.c_str();
			*etree = ExpressionTree::createExpressionTree(input_iterator);
		}

	case STATEMENT_ASSIGNMENT:
		{
			Environment::iterator it = environment.find(lhs);

			if (it != environment.end())
			{
				delete (*it).second;
				(*it).second = etree->evaluate(environment);
			}
			else
				environment[lhs] = etree->evaluate(environment);
		}

		break;

	case STATEMENT_RETURN:
		return rhs.evaluate(environment);

	case STATEMENT_OUTPUT:
		{
			Value* value = rhs.evaluate(environment);

			if (!value)
				return NULL;

			switch (value->getValueType())
			{
			case VALUE_INTEGER:
				std::cout << ((Integer*) value)->getInteger();
				break;

			case VALUE_STRING:
				std::cout << ((String*) value)->getString();
			}

			delete value;
		}
	}

	return NULL;
}

