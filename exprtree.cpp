/*
 * exprtree.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: radoslav
 */

#include "exprtree.h"
#include "arithmetic.h"
#include "token.h"
#include "operator.h"
#include "lambda.h"
#include "integer.h"
#include "string.h"
#include "identifier.h"
#include "strtok.h"
#include "call.h"
#include "environment.h"
#include "value.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <stack>


void ExpressionTreeNode::copy(const ExpressionTreeNode& other)
{
	value = other.value->clone();
	left = other.left;
	right = other.right;
}

void ExpressionTreeNode::destroy()
{
	delete value;
}

ExpressionTreeNode::ExpressionTreeNode(const ExpressionTreeNode& other)
{
	copy(other);
}

ExpressionTreeNode& ExpressionTreeNode::operator=(const ExpressionTreeNode& other)
{
	if (&other != this)
	{
		destroy();
		copy(other);
	}

	return *this;
}

ExpressionTreeNode::~ExpressionTreeNode()
{
	destroy();
}


void ExpressionTree::createTree(std::stack<ExpressionTree>& rstack, char op)
{
	ExpressionTree rtree = rstack.top();
	rstack.pop();
	ExpressionTree ltree = rstack.top();
	rstack.pop();

	rstack.push(ExpressionTree(new Operator(op), ltree, rtree));
}

Value* ExpressionTree::evaluateExpression(ExpressionTreeConstIterator it, Environment& environment)
{
	switch ((*it)->getTokenType())
	{
	case TOKEN_VALUE:
		// листо
		switch (((Value*) (*it))->getValueType())
		{
		case VALUE_IDENTIFIER:
			return environment.at(*((Identifier*) (*it)))->clone();

		case VALUE_CALL:
			return ((Call*) (*it))->execute(environment);

		default:
			return ((Value*) *it)->clone();
		}
		break;

	case TOKEN_OPERATOR:
		{
			Value *lhs = evaluateExpression(++it, environment), *rhs = evaluateExpression(it++, environment);

			if (lhs->getValueType() == VALUE_INTEGER && rhs->getValueType() == VALUE_INTEGER)
			{
				int lhs_val = ((Integer*) lhs)->getInteger(), rhs_val = ((Integer*) rhs)->getInteger();

				switch (((Operator*) (*it))->getOperator())
				{
					case '+': return new Integer(lhs_val + rhs_val);
					case '-': return new Integer(lhs_val - rhs_val);
					case '*': return new Integer(lhs_val * rhs_val);
					case '/': return new Integer(lhs_val / rhs_val);
					case '^': return new Integer(ipow(lhs_val, rhs_val));
					default:  return NULL;
				}
			}
			else if (lhs->getValueType() == VALUE_STRING && rhs->getValueType() == VALUE_STRING)
			{
				std::string lhs_val = ((String*) lhs)->getString(), rhs_val = ((String*) rhs)->getString();
				return ((Operator*) (*it))->getOperator() == '+' ? new String(lhs_val + rhs_val) : new String();
			}

			delete lhs;
			delete rhs;
		}
		break;

	default:;
	}

	return NULL;
}

ExpressionTree ExpressionTree::createExpressionTree(const char*& expr)
{
	std::stack<char> opstack;
	std::stack<ExpressionTree> rstack;

	if (!expr)
		return ExpressionTree();

	while (*expr && *expr != ';' && *expr != ',' && *expr != ')')
    {
		if (*expr >= '0' && *expr <= '9' || *expr == '-' && expr + 1 && *(expr + 1) >= '0' && *(expr + 1) <= '9')
			rstack.push(ExpressionTree(new Integer(expr)));
		else if (*expr == '"')
			rstack.push(ExpressionTree(new String(expr)));
		else if (*expr == '\\')
			rstack.push(ExpressionTree(new Lambda(expr)));
		else if (*expr >= 'A' && *expr <= 'Z' || *expr >= 'a' && *expr <= 'z')
		{
			const char* name = expr;
			Identifier id(name);

			if (!id.getName().empty())
				rstack.push(ExpressionTree(gotoToken(name, " \t\n\r") && *name == '(' ? (Token*) new Call(expr) : (Token*) new Identifier(expr)));
		}
		else
		{
			if (*expr == '(')
				opstack.push(*expr);
			else if (*expr == ')')
			{
				char op = opstack.top();
				opstack.pop();

				while (op != '(')
				{
					createTree(rstack, op);
					op = opstack.top();
					opstack.pop();
				}
			}
			else if (*expr != ' ' && *expr != '\t' && *expr != '\n' && *expr != '\r')
			{
				// *expr е операция
				// първо: вадим всички операции
				// с по-висок или равен приоритет
				while (!opstack.empty() && priority(opstack.top()) >= priority(*expr))
				{
					createTree(rstack, opstack.top());
					opstack.pop();
				}

				opstack.push(*expr);
			}

			expr++;
		}
	}

	while (!opstack.empty())
	{
		createTree(rstack, opstack.top());
		opstack.pop();
	}

	return rstack.top();
}

void ExpressionTree::deleteNode(ExpressionTreeNode* node)
{
	if (node)
	{
		deleteNode(node->left);
		deleteNode(node->right);
		delete node;
	}
}

ExpressionTreeNode* ExpressionTree::copyNode(ExpressionTreeNode* node)
{
	return node ? new ExpressionTreeNode(node->value, true, copyNode(node->left), copyNode(node->right)) : NULL;
}

ExpressionTree::ExpressionTree(Token* value, ExpressionTree& left, ExpressionTree& right, bool doClone)
{
	root = new ExpressionTreeNode(value, doClone);
	adoptLeft(left.root);
	adoptRight(right.root);
}

ExpressionTree::ExpressionTree(const ExpressionTree& other)
{
	root = copyNode(other.root);
}

ExpressionTree& ExpressionTree::operator=(const ExpressionTree& other)
{
	if (&other != this)
	{
		deleteNode(root);
		root = copyNode(other.root);
	}

	return *this;
}

ExpressionTree::~ExpressionTree()
{
	deleteNode(root);
}

