/*
 * environment.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: radoslav
 */

#include "environment.h"

void Environment::cloneAll()
{
	for (Environment::iterator it = begin(); it != end(); it++)
		if ((*it).second)
			(*it).second = (*it).second->clone();
}

void Environment::destroy()
{
	for (Environment::iterator it = begin(); it != end(); it++)
		delete (*it).second;
}

Environment::Environment(): std::map<Identifier, Value*>() {}

Environment::Environment(const Environment& other): std::map<Identifier, Value*>(other)
{
	cloneAll();
}

Environment& Environment::operator=(const Environment& other)
{
	if (&other != this)
	{
		destroy();
		std::map<Identifier, Value*>::operator=(other);
		cloneAll();
	}

	return *this;
}

Environment::~Environment()
{
	destroy();
}

