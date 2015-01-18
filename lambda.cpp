/*
 * lambda.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: radoslav
 */

#include "lambda.h"
#include "strtok.h"
#include "identifier.h"
#include <cstring>


void Lambda::copy(const Lambda& other)
{
	signature = other.signature;
	body = other.body->clone();
}

void Lambda::destroy()
{
	delete body;
}

Lambda::Lambda(const char*& code): Value(VALUE_LAMBDA)
{
	if (!gotoToken(code, " \t\n\r") || *code != '\\')
	{
		type = VALUE_INVALID;
		return;
	}

	code++;

	if (!gotoToken(code, " \t\n\r") || *code != '(')
	{
		type = VALUE_INVALID;
		return;
	}

	code++;

	while (gotoToken(code, ", \t\n\r") && *code != ')')
		signature.push_back(Identifier(code));

	if (*code != ')')
		type = VALUE_INVALID;
	else
	{
		code++;
		body = Statement::create(code);
	}
}

Lambda::Lambda(const Lambda& other): Value(other)
{
	copy(other);
}

Lambda& Lambda::operator=(const Lambda& other)
{
	if (&other != this)
	{
		destroy();
		Value::operator=(other);
		copy(other);
	}

	return *this;
}

Lambda::~Lambda()
{
	destroy();
}

