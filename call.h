/*
 * call.h
 *
 *  Created on: Jan 15, 2015
 *      Author: radoslav
 */

#ifndef CALL_H_
#define CALL_H_

#include "identifier.h"
#include "exprtree.h"
#include "environment.h"
#include <vector>
#include <map>

class Call: public Value
{
	Identifier name;
	std::vector<ExpressionTree> arguments;

public:
	Call(): Value(VALUE_CALL) {}
	Call(const Identifier& _name, const std::vector<ExpressionTree>& _arguments):
			Value(VALUE_CALL), name(_name), arguments(_arguments) {}
	Call(const char*&);

	Call* clone() const { return new Call(*this); }

	const Identifier& getIdentifier() const { return name; }
	std::vector<ExpressionTree>::const_iterator getArgumentIterator() const { return arguments.begin(); }
	std::vector<ExpressionTree>::const_iterator getArgumentEndIterator() const { return arguments.end(); }

	Value* execute(Environment&) const;
};


#endif /* CALL_H_ */
