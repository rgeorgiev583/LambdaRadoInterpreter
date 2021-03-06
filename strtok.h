/*
 * strtok.h
 *
 *  Created on: Jan 14, 2015
 *      Author: radoslav
 */

#ifndef STRTOK_H_
#define STRTOK_H_

#include <string>

std::string getToken(const char*&, const char*);
std::string peekToken(const char*&, const char*);
bool gotoToken(const char*&, const char*);
bool ignoreToken(const char*&, const char*);
std::string peekToken(const std::string&, const char*);


#endif /* STRTOK_H_ */
