#ifndef __ENV__H
#define __ENV__H
#include "id.h"
#include "token.h"

struct TokenList{
	struct Token ** head;
	int cnt;
	int cap;
};


void push2(struct TokenList *list,struct Token *tok);

struct Token * get2(struct TokenList *ori,struct TokenList *list,struct Token *tok);

#endif
