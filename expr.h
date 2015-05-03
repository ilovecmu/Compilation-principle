
#ifndef __EXPR__H
#define __EXPR__H
#include "token.h"

#define EXPR_ACTION 0
#define EXPR_TAG 1
#define EXPR_NOT 2
#define EXPR_LOGICAL 3
#define EXPR_NOTNOT 4

struct Expr{
	// a+1
	struct {
		struct Token *op;// +
		struct Expr *expr1;//a
		struct Expr *expr2;// 1
	} action;
	struct{
		struct Expr* expr1;
	}not;
	struct {
		struct Token *op;
		struct Expr *expr1;
		struct Expr *expr2;
	}logical;
	//change token to expr. int i
	struct Token *tag;
	int type;
};

#endif
