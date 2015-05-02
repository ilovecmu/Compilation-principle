#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "util.h"
#include "env.h"
#include "id.h"
#include "parse.h"
#include "stmt.h"
#include "expr.h"

struct Token *look;

struct TokenList env[100]={0};
struct TokenList *topEnv = &env[0];
int envId =-1;
int label =0;

#if 1
#undef log
#define log 
#endif

int tempLabel = 0;
int newTemp(){
	return tempLabel++;
}

int newLabel(){
	return label++;
}
void move(){
	look = scan();
}

bool match(int  tag){
	
	if(look->tag == tag){
		move();
		return true;
	}
	log("expcet %d(%c),fact is %d(%c)(%s)\r\n",tag,tag,look->tag,look->tag,look->value_s);
	
	return false;
}
void gen(struct Stmt *b);
void program(){
	
	move();
	struct Stmt *b =blocks();
	gen(b);
}
struct Expr* exprreduce(struct Expr *exp){
	printf("%s\r\n",__func__);
	struct Expr * e1,*e2;
	
	if(exp->type == EXPR_ACTION){
		#if 0
		if(exp->action.expr1->type == EXPR_TAG && exp->action.expr2->type == EXPR_TAG){
			printf("555555555 %d,%d\r\n",exp->action.expr1->type,exp->action.expr2->type);
			printf("5555555555555  %s,%d\r\n",exp->action.expr1->tag->value_s,exp->action.expr2->tag->value_i);
			return exp;
		}
		#endif
		if(exp->action.expr1->type == EXPR_ACTION){
			printf("1111111\r\n");
			e1 = exprreduce(exp->action.expr1);			
		}
		else{			
			printf("2222222222\r\n");
			e1 = exp->action.expr1;
		}
		if(exp->action.expr2->type == EXPR_ACTION){		
			printf("333333333\r\n");
			e2 = exprreduce(exp->action.expr2);
		}
		else{			
			printf("44444\r\n");
			e2 = exp->action.expr2;
		}

		struct Expr *e3 = malloc(sizeof(struct Expr));
		memset(e3,0,sizeof(struct Expr));
		struct Token *t3 = malloc(sizeof(struct Token));
		memset(t3,0,sizeof(struct Token));
		t3->value_s = malloc(3);
		memset(t3,0,3);
		sprintf(t3->value_s,"t%d",newTemp());
		
		e3->type=EXPR_TAG;
		e3->tag = t3;

		printf("%s=",t3->value_s);
		if(e1->tag->value_s !=NULL)
			printf("%s",e1->tag->value_s);
		else
			printf("%d",e1->tag->value_i);
		printf("%c",exp->action.op->tag);
		if(e2->tag->value_s !=NULL)
			printf("%s\r\n",e2->tag->value_s);
		else
			printf("%d\r\n",e2->tag->value_i);
		return e3;
	}else{
		printf("xxxxxxxxx\r\n");
		return exp;
	}
}


void exprreduce1(char *pre,struct Expr *exp){
	struct Expr * e1,*e2;
	
	if(exp->type == EXPR_ACTION){
		if(exp->action.expr1->type == EXPR_TAG && exp->action.expr2->type == EXPR_TAG ){
			printf("%s=",pre);
			if(exp->action.expr1->tag->value_s)
				printf("%s",exp->action.expr1->tag->value_s);
			else
				printf("%d",exp->action.expr1->tag->value_i);	
			printf("%c",exp->action.op->tag);
			if(exp->action.expr2->tag->value_s)
				printf("%s",exp->action.expr2->tag->value_s);
			else
				printf("%d",exp->action.expr2->tag->value_i);	
			printf("\r\n");
			return;
		}
		
		char *t3=NULL;	
		char *t4=NULL;

		if(exp->action.expr1->type == EXPR_ACTION){
			t3=malloc(3);		
			memset(t3,0,3);
			sprintf(t3,"t%d",newTemp());
			//printf("%s\r\n",t3);
			exprreduce1(t3,exp->action.expr1);			
		}
		if(exp->action.expr2->type == EXPR_ACTION){		
			t4=malloc(3);
			memset(t4,0,3);
			sprintf(t4,"t%d",newTemp());
			exprreduce1(t4,exp->action.expr2);
		}
		printf("%s=",pre);
		if(t3!=NULL)
			printf("%s",t3);
		else{
			if(exp->action.expr1->tag->value_s)
				printf("%s",exp->action.expr1->tag->value_s);
			else
				printf("%d",exp->action.expr1->tag->value_i);
		}
		printf("%c",exp->action.op->tag);
		if(t4!=NULL)
			printf("%s",t4);
		else{
			if(exp->action.expr2->tag->value_s)
				printf("%s",exp->action.expr2->tag->value_s);
			else
				printf("%d",exp->action.expr2->tag->value_i);
			}
		printf("\r\n");
	}else{
		printf("%s=",pre);
		
			if(exp->tag->value_s)
				printf("%s",exp->tag->value_s);
			else
				printf("%d",exp->tag->value_i);
			
		printf("\r\n");
	}
}

void exprgen(struct Expr *exp){
	
	printf("#####%s,%d\r\n",__func__,exp->type);
	switch(exp->type ){
		case EXPR_ACTION:
			
			exprgen(exp->action.expr1);
			
			printf("%c",exp->action.op->tag);
			
			exprgen(exp->action.expr2);
			break;
		case EXPR_TAG:
			
			if(exp->tag->value_s!=NULL)
				printf("%s",exp->tag->value_s);
			else
				printf("%d",exp->tag->value_i);
			break;
	}
}
void gen(struct Stmt *b){
	
	//printf("%s ,%d\r\n",__func__,b);
	if(b==NULL) return;
	struct Stmt *s1;
	switch(b->type){
		case STMT_IF:
			break;
		case STMT_SEQ:	
			
	//		printf("L%d: ",newLabel());
			gen(b->Seq.s1);		
//			printf("L%d: ",newLabel());
			
			gen(b->Seq.s2);
			break;
		case STMT_SET:
			
			printf("L%d: ",newLabel());
			#if 0
			if(b->Set.expr->type == EXPR_ACTION){
				int tmp = newTemp();
				printf("t%d =",);
			}
			#endif
			struct Expr *e;
			//if(b->Set.expr->type == EXPR_ACTION)
			//	e = exprreduce(b->Set.expr);
		
			//printf("%s=",b->Set.tok->value_s);
			//exprgen(b->Set.expr);
			exprreduce1(b->Set.tok->value_s,b->Set.expr);
			printf("\r\n");
			break;
		case STMT_WHILE:
			break;
	}
	
}
struct Stmt* blocks(){
	struct Stmt *s;
	printf("%s\r\n",__func__);
	match('{');
	
	envId++;
	//saveEnv = topEnv;
	topEnv = &env[envId];
	
	decls();

	s = stmts();

	match('}');
	printf("exit blocks\r\n");
	free(topEnv->head);
	topEnv->head= NULL;
	envId--;
	printf("env id is %d\r\n",envId);
	topEnv = &env[envId];
	
	return s;
}

void decls(){
	while(look->tag == BASIC){
		//get tok		
		match(BASIC);		
		struct Token *tok = look;
		match(ID);
		push2(topEnv,tok);
		//put tok		
		//next tok;
		//move();

		match(';');
	}
}

struct Stmt * assign(){
	
	log("%s\r\n",__func__);
	
	struct Token *tok = look;
	match(ID);
	struct Token *id = get2(&env[0],topEnv,tok);
	match('=');
	struct Stmt *stmt = malloc(sizeof(struct Stmt));
	memset(stmt,0,sizeof(struct Stmt));
	stmt->Set.tok = id;
	stmt->Set.expr = expr();
	stmt->type=STMT_SET;
	match(';');
	log("exit assign\r\n");
	//log("%d,%s\r\n",id->tag,id->value_s);
	//log("",stmt->Set.expr);
	return stmt;
}

struct Expr * expr(){
	
	log("%s\r\n",__func__);
	struct Expr *x = term();
	struct Expr *y ;
      while( look->tag == '+' || look->tag == '-' ) {
	  	 log("expr termr\r\n");
         struct Token *tok = look;  move();  
		 y = malloc(sizeof(struct Expr));
		 memset(y,0,sizeof(struct Expr));
		 y->action.expr1 = x;
		 printf("==========x=%d\r\n",x->type);
		 y->action.expr2 = term();
		 y->action.op = tok;
		 y->type = EXPR_ACTION;		 
		 x=y;
      }
	  
	 // exprreduce1("s",x);
	  if(x->type == EXPR_TAG){printf("tag is %s,%d\r\n",x->tag->value_s,x->tag->value_i);}
	  else printf("expr is \r\n");
	  
	  log("exit expr\r\n");
      return x;
}

struct Expr *term() {
	 struct Expr* x = factor();	 
	 struct Expr *y;
	 log("%s\r\n",__func__);
	 		
	 while(look->tag == '*' || look->tag == '/' ) {			
		y= malloc(sizeof(struct Expr));
		memset(y,0,sizeof(struct Expr));
		struct Token *tok = look;  move();
		y->action.expr1 = x;
		y->action.expr2 = factor();
		y->action.op = tok;
		y->type = EXPR_ACTION;
		x = y;
	 }
	 log("exit term ,x=%d,%s,%d\r\n",x->tag->tag,x->tag->value_s,x->tag->value_i);
	 log("###############x=%d,%d\r\n",x,x->tag);
	 return x;
  }
struct Expr * factor(){
	
	log("%s\r\n",__func__);
	struct Expr *expr = malloc(sizeof(struct Expr));
	memset(expr,0,sizeof(struct Expr));
	switch( look->tag ) {
		case ID:		
			expr->tag = get2(&env[0],topEnv,look);
			log("id is %s\r\n",expr->tag->value_s);
			break;
		case NUM:
			expr->tag = look;
			log("num is %d\r\n",expr->tag->value_i);
			break;
	}
	expr->type = EXPR_TAG;
	move();
	//log("factor tag=%d,%d\r\n",expr->tag->tag,,expr->tag->value_i);
	return expr;
}
struct Stmt *stmt(){
	log("%s\r\n",__func__);
	struct Stmt *s;
	switch(look->tag){
		case '{':
			s = blocks();
			break;
		default:
			s = assign();	
			log("fsfsfsfsf\r\n");
			break;
		//	log("tag is %d,%c,value is %s\r\n",s->Set.tok->tag,s->Set.tok->tag,s->Set.tok->value_s);
	}
	
	//gen(s);
	return s;
}
struct Stmt *stmts(){
//	log("stms look->tag=%d ,%s\r\n",look->tag,look->value_s);
	
	log("%s\r\n",__func__);
	
	if(look->tag == '}')
	{
		log("222222222222222\r\n");
		return NULL;	
	}
	
	struct Stmt *s1 =stmt();
	
	//log("s1 is %d,%s\r\n",s1->Set.tok->tag,s1->Set.tok->value_s);
	struct Stmt *s2 =stmts();
	struct Stmt *seq = malloc(sizeof(struct Stmt));	
	memset(seq,0,sizeof(struct Stmt));
	seq->Seq.s1 = s1;
	seq->Seq.s2 = s2;
	seq->type = STMT_SEQ;
	
	//log("s2 is %d,%s",s2->Set.tok->tag,s2->Set.tok->value_s);
	return seq;
}
#if 1
int main(){
	readFile("block1.t");
	program();
}
#endif
