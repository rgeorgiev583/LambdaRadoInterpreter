/*
 * statement.h
 *
 *  Created on: Jan 14, 2015
 *      Author: radoslav
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_

#include "value.h"
#include "identifier.h"
#include "environment.h"
#include <map>


enum STATEMENT_TYPE
{
	STATEMENT_INVALID,
	STATEMENT_SIMPLE,
	STATEMENT_CONDITIONAL,
	STATEMENT_COMPOUND
};

class Statement: public Cloneable
{
protected:
	STATEMENT_TYPE type;

public:
	static Statement* create(const char*&);

	Statement(STATEMENT_TYPE _type = STATEMENT_INVALID): type(_type) {}

	virtual Statement* clone() const = 0;

	STATEMENT_TYPE getStatementType() const { return type; }

	virtual Value* execute(Environment&) const = 0;
};


#endif /* STATEMENT_H_ */
