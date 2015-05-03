#ifndef __STMT__#define __STMT__#define STMT_SET 0#define STMT_IF 1#define STMT_WHILE 2#define STMT_SEQ 3#define STMT_ELSE 4struct Stmt{
		//a=a+1	struct {		struct Token *tok;		struct Expr *expr;	}Set;		//if(expr) stmt	struct {		struct Expr *expr1;//expr  a
		struct Stmt *stmt1;//stmt  null	}If;	struct{		struct Expr *expr1;		struct Stmt *stmt1;		struct Stmt *stmt2;	}Else;	//while(expr) stmt
	struct {		struct Expr *expr1;		struct Stmt *stmt1;	}While;	struct{		struct Stmt *s1;		struct Stmt *s2;	}Seq;
	int type ;	
};
#endif
