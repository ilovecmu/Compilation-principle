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

#if 0
#undef log
#define log 
#endif

int tempLabel = 0;
int newTemp(){
	return tempLabel++;
}

char * getNewTemp(){
	
	char *t4=malloc(10);
	memset(t4,0,10);
	sprintf(t4,"t%d",newTemp());
	return t4;
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
	char *p=0; *p=0;
	return false;
}
void gen(struct Stmt *b);
void program(){
	
	move();
	struct Stmt *b =blocks();
	gen(b);
}
void printTag(struct Expr *exp){
	switch(exp->logical.op->tag){
	case NE:
		printf("!=");
		break;
	case EQ:
		printf("==");
		break;
	case GE:
		printf(">=");
		break;
	case LE:
		printf("<=");
		break;
	default:
		printf("%c",exp->logical.op->tag);
		break;
	}
}
struct Expr* exprreduce(struct Expr *exp){
	printf("%s\r\n",__func__);
	struct Expr * e1,*e2;
	
	if(exp->type == EXPR_ACTION){
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
		return exp;
	}
}


void exprreduce1(char *pre,struct Expr *exp,int labelb){
	struct Expr * e1,*e2;
	static bool isNot = false;
	//printf("%s %d,%d\r\n",__func__,exp->type,stmt2);
	if(isNot==false)
	printf("L%d: ",newLabel());
	if(exp->type == EXPR_ACTION){
		if(exp->action.expr1->type == EXPR_TAG && exp->action.expr2->type == EXPR_TAG ){
			printf("%s=",pre);
			if(exp->action.expr1->tag->value_s)
				printf("%s",exp->action.expr1->tag->value_s);
			else if(exp->action.expr1->tag->tag == NUM)
				printf("%d",exp->action.expr1->tag->value_i);	
			else if(exp->action.expr1->tag->tag == REAL)
				printf("%f",exp->action.expr1->tag->value_f);
			printf("%c",exp->action.op->tag);
			if(exp->action.expr2->tag->value_s)
				printf("%s",exp->action.expr2->tag->value_s);
			else if(exp->action.expr2->tag->tag == NUM)
				printf("%d",exp->action.expr2->tag->value_i);			
			else if(exp->action.expr2->tag->tag == REAL)
				printf("%f",exp->action.expr2->tag->value_f);
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
			exprreduce1(t3,exp->action.expr1,-1);			
		}
		if(exp->action.expr2->type == EXPR_ACTION){		
			t4=malloc(3);
			memset(t4,0,3);
			sprintf(t4,"t%d",newTemp());
			exprreduce1(t4,exp->action.expr2,-1);
		}
		printf("%s=",pre);
		if(t3!=NULL)
			printf("%s",t3);
		else{
			if(exp->action.expr1->tag->value_s)
				printf("%s",exp->action.expr1->tag->value_s);
			else if(exp->action.expr1->tag->tag == NUM)
				printf("%d",exp->action.expr1->tag->value_i);
			
			else if(exp->action.expr1->tag->tag == REAL)
				printf("%f",exp->action.expr1->tag->value_f);
		}
		printf("%c",exp->action.op->tag);
		if(t4!=NULL)
			printf("%s",t4);
		else{
			if(exp->action.expr2->tag->value_s)
				printf("%s",exp->action.expr2->tag->value_s);
			else if(exp->action.expr2->tag->tag==NUM)
				printf("%d",exp->action.expr2->tag->value_i);
			else if(exp->action.expr2->tag->tag==REAL)
				printf("%f",exp->action.expr2->tag->value_f);
			}
		printf("\r\n");
	}else if(exp->type == EXPR_LOGICAL){
		if(isNot)
			printf("if ");
		else 
			printf("iffalse ");
		isNot = false;
		if(exp->logical.expr1->type==EXPR_ACTION){	
			exprreduce1(getNewTemp(),exp->logical.expr1,-1);
		}else{
		
			printf("%s",exp->logical.expr1->tag->value_s);
		}
		printTag(exp);
		if(exp->logical.expr2->type==EXPR_ACTION){
		
			printf("$$$$%d,%d\r\n",exp->logical.expr1->type,exp->logical.expr2->type);			
			exprreduce1(getNewTemp(),exp->logical.expr2,-1);
		}else{
			printf("%s   ",exp->logical.expr2->tag->value_s);
		}
		char *tmp = getNewTemp();
		if(labelb==-1){	
			int tmp1  = newLabel();
			int tmp2  = newLabel();
			printf("goto L%d\r\n",tmp1);	
			printf("%s=true\r\n",tmp);
			printf("goto L%d\r\n",tmp2);
			printf("L%d:",tmp1);
			printf("%s=false\r\n",tmp);	
			if(pre)
			printf("L%d: %s=%s",tmp2,pre,tmp);
		}else{
			printf("goto L%d\r\n",labelb);	
		}
	}	//printf("uuuuuuuuuuuu\r\n");
	else if(exp->type == EXPR_NOT){
		//printf("iffalse");
		isNot = true;
		exprreduce1(pre,exp->not.expr1,labelb);
	}else{
			if(pre)printf("%s=",pre);
		
			if(exp->tag->value_s)
				printf("%s",exp->tag->value_s);
			else if(exp->tag->tag == NUM)
				printf("%d",exp->tag->value_i);
			else if(exp->tag->tag == REAL)
				printf("%f",exp->tag->value_f);
			
		printf("\r\n");
	}
}

void exprgen(struct Expr *exp){
	
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
	
	int labela,labelb ;
	switch(b->type){
		case STMT_IF:
			labela= newLabel();
			labelb = newLabel();
			exprreduce1(NULL,b->If.expr1,labelb);
			gen(b->If.stmt1);
			printf("L%d:\r\n",labelb);
			break;
		case STMT_ELSE:
			labela= newLabel();
			labelb = newLabel();
			exprreduce1(NULL,b->Else.expr1,labelb);
			gen(b->Else.stmt1);			
			printf("goto L%d\r\n",labela);
			printf("L%d:",labelb);
			gen(b->Else.stmt2);
			printf("L%d:\r\n",labela);
			break;
		case STMT_SEQ:				
			gen(b->Seq.s1);		
			gen(b->Seq.s2);
			break;
		case STMT_SET:
			printf("\r\n");
			struct Expr *e;
			exprreduce1(b->Set.tok->value_s,b->Set.expr,-1);
			printf("\r\n");
			break;
		case STMT_WHILE:
			break;
	}
	
}
struct Stmt* blocks(){
	struct Stmt *s;
	log("%s\r\n",__func__);
	match('{');
	
	envId++;
	//saveEnv = topEnv;
	topEnv = &env[envId];
	
	decls();

	s = stmts();

	match('}');
	log("exit blocks\r\n");
	free(topEnv->head);
	topEnv->head= NULL;
	envId--;
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
	
	log("#############%s\r\n",id->value_s);
	match('=');
	struct Stmt *stmt = malloc(sizeof(struct Stmt));
	memset(stmt,0,sizeof(struct Stmt));
	stmt->Set.tok = id;
	//stmt->Set.expr = expr();
	stmt->Set.expr = rel();
	stmt->type=STMT_SET;
	match(';');
	log("exit assign\r\n");
	return stmt;
}

struct Expr *rel(){
	static int cnt=0;
		
	log("%s			%d\r\n",__func__,cnt);
	cnt++;
	struct Expr *exp1 ,*exp2,*exp3,exp4;
	bool isNot =false;
	struct Token *tok;
	while(look->tag == '!') {isNot=!isNot;move();}
	if(isNot){
		exp1 = malloc(sizeof(struct Expr));
		memset(exp1,0,sizeof(struct Expr));
		exp1->type = EXPR_NOT;
		exp1->not.expr1 = expr();
		return exp1;
	}
	exp1 = expr();
	
	if(look->tag == '<' ||
		look->tag ==LE||
		look->tag ==GE||
		look->tag ==EQ||
		look->tag ==NE||
		look->tag =='>'){
		tok = look;
		move();		
	 	exp2 = expr();
		//if(isNot)
		exp3 = malloc(sizeof(struct Expr));
		memset(exp3,0,sizeof(struct Expr));
		exp3->type = EXPR_LOGICAL;
		exp3->logical.expr1 = exp1;
		exp3->logical.expr2 = exp2;
		exp3->logical.op = tok;
		log("exit rel			%d\r\n",cnt);
		cnt--;
		return exp3;
	}
	
	log("exit rel			%d\r\n",cnt);
	cnt--;
	return exp1;
}
struct Expr * expr(){
	
	log("%s\r\n",__func__);
	struct Expr *x = term();
	struct Expr *y ;
      while( look->tag == '+' || look->tag == '-' ) {
	  	 log("expr look->tag=%c\r\n",look->tag);
         struct Token *tok = look;  move();  
		 y = malloc(sizeof(struct Expr));
		 memset(y,0,sizeof(struct Expr));
		 y->action.expr1 = x;
		 y->action.expr2 = term();
		 y->action.op = tok;
		 y->type = EXPR_ACTION;		 
		 x=y;
      }
	  
	  log("exit expr\r\n");
      return x;
}

struct Expr *term() {
	
	 log("%s\r\n",__func__);
	 struct Expr* x = factor();	 
	 struct Expr *y;
	 		
	 while(look->tag == '*' || look->tag == '/' ) {		
	 	log("term look->tag =%c\r\n",look->tag );
		y= malloc(sizeof(struct Expr));
		memset(y,0,sizeof(struct Expr));
		struct Token *tok = look;  move();
		y->action.expr1 = x;
		y->action.expr2 = factor();		
		y->action.op = tok;
		y->type = EXPR_ACTION;
		x = y;
	 }
	 log("exit term\r\n");
	 return x;
  }
struct Expr * factor(){
	
	log("%s\r\n",__func__);
	struct Expr *exp = malloc(sizeof(struct Expr));
	memset(exp,0,sizeof(struct Expr));
	switch( look->tag ) {
		case ID:		
			exp->tag = get2(&env[0],topEnv,look);
			log("id is %s\r\n",exp->tag->value_s);
			
			exp->type = EXPR_TAG;
			move();
			break;
		case NUM:
			exp->tag = look;
			log("num is %d\r\n",exp->tag->value_i);
			
			exp->type = EXPR_TAG;
			move();
			break;
		case REAL:
			exp->tag = look;
			log("float is %f\r\n",exp->tag->value_f);
			
			exp->type = EXPR_TAG;
			move();
			break;
		case TRUE:
			exp->tag = look;
			log("id is %s\r\n","true");
			
			exp->type = EXPR_TAG;
			move();
			break;
		case FALSE:
			exp->tag = look;
			log("id is %s\r\n","false");
			
			exp->type = EXPR_TAG;
			move();
			break;
		case '(':
			move();
			//exp = expr();
			exp = rel();
			match(')');
			//exp->type = EXPR_ACTION;
			break;
			
	}
	
	//log("factor tag=%d,%d\r\n",expr->tag->tag,,expr->tag->value_i);
	return exp;
}
struct Stmt * logical(){
	log("@@@@@@@@@@@@@@@@%s\r\n",__func__);
	match(IF);
	struct Stmt *s = malloc(sizeof(struct Stmt));
	memset(s,0,sizeof(struct Stmt));

	struct Stmt *s1;
	
	struct Expr *exp = rel();
	s1 = stmt();
	if(look->tag != ELSE){
		log("only if\r\n");
		s->type = STMT_IF;
		s->If.expr1 = exp;
		s->If.stmt1 = s1;
		return s;
	}
	match(ELSE);
	log("else only\r\n");
	s->type = STMT_ELSE;
	s->Else.expr1 = exp;
	s->Else.stmt1 = s1;
	s->Else.stmt2 = stmt();
	return s;
}

	struct Stmt *stmt(){
	log("%s\r\n",__func__);
	struct Stmt *s;
	switch(look->tag){
		case '{':
			s = blocks();
			break;
		case IF:
			s= logical();
			break;
		default:
			s = assign();	
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
		return NULL;	
	}
	
	struct Stmt *s1 =stmt();
	
	struct Stmt *s2 =stmts();
	struct Stmt *seq = malloc(sizeof(struct Stmt));	
	memset(seq,0,sizeof(struct Stmt));
	seq->Seq.s1 = s1;
	seq->Seq.s2 = s2;
	seq->type = STMT_SEQ;
	
	return seq;
}
#if 1
int main(){
	readFile("expr3.t");
	program();
}
#endif
