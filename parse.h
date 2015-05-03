#ifndef __PARSE__H
#define __PARSE__H
struct Stmt* blocks();
void decls();
struct Expr * expr();
struct Stmt * assign();
struct Expr *term() ;
struct Expr * factor();
struct Stmt *stmts();
struct Expr *rel();
struct Stmt * logical();
struct Stmt *stmt();

#endif
