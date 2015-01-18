/*
 * call.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: radoslav
 */

#include "call.h"
#include "strtok.h"
#include "exprtree.h"
#include "identifier.h"
#include "value.h"
#include "environment.h"
#include "lambda.h"
#include <map>
#include <cstring>


Call::Call(const char*& code): Value(VALUE_CALL)
{
	name = Identifier(code);

	if (!gotoToken(code, " \t\n\r") || *code != '(')
	{
		type = VALUE_INVALID;
		return;
	}

	code++;

	while (gotoToken(code, ", \t\n\r") && *code != ')')
		arguments.push_back(ExpressionTree::createExpressionTree(code));

	if (*code != ')')
		type = VALUE_INVALID;
	else
		code++;
}

Value* Call::execute(Environment& environment) const
{
	Environment::iterator eit = environment.find(name);
	Lambda* lambda = NULL;

	if (eit != environment.end())
	{
		Value* value = (*eit).second;

		if (value->getTokenType() == TOKEN_VALUE && ((Value*) value)->getValueType() == VALUE_LAMBDA)
			lambda = (Lambda*) value;
	}

	if (!lambda)
		return NULL;

	Environment child_environment(environment);
	std::vector<Identifier>::const_iterator pit;
	std::vector<ExpressionTree>::const_iterator ait;

	for (pit = lambda->getSignatureIterator(), ait = arguments.begin();
		 pit != lambda->getSignatureEndIterator() && ait != arguments.end();
		 pit++, ait++)

		child_environment[(*pit).getName()] = (*ait).evaluate(environment);

	Value* retval = lambda->getBody()->execute(child_environment);

	for (Environment::iterator it = environment.begin(); it != environment.end(); it++)
	{
		Value* value = child_environment[(*it).first];

		if ((*it).second != value)
			(*it).second = value->clone();
	}

	return retval;
}

