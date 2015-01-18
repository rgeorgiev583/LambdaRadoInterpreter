/*
 * environment.h
 *
 *  Created on: Jan 13, 2015
 *      Author: radoslav
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <map>
#include "identifier.h"
#include "value.h"


class Environment: public std::map<Identifier, Value*>
{
	void cloneAll();
	void destroy();

public:
	Environment();
	Environment(const Environment&);
	Environment& operator=(const Environment&);
	~Environment();
};


#endif /* ENVIRONMENT_H_ */
