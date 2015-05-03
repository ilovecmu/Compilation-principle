#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "util.h"
#include <sys/stat.h>
#include <fcntl.h>

int fd;
char *buff;
int fileLen;
char *pbuff;
int len=0;

struct Token ifToken = {IF,0,0,"if"};
struct Token elseToken = {ELSE,0,0,"else"};
struct Token doToken = {DO,0,0,"do"};
struct Token whileToken = {WHILE,0,0,"while"};
struct Token breakToken = {BREAK,0,0,"break"};
struct Token intToken = {BASIC,0,0,"int"};
struct Token floatToken = {BASIC,0,0,"float"};
struct Token boolToken = {BASIC,0,0,"bool"};
struct Token trueToken = {TRUE,0,0,"true"};
struct Token falseToken = {FALSE,0,0,"false"};

struct Token* tokenReserve[]={&ifToken,&elseToken,&doToken,&whileToken,&breakToken,
	&intToken,&floatToken,&boolToken,&trueToken,&falseToken,NULL};

void readFile(char *name){
	if(name==NULL){error("no file name"); return;}
	fd = open(name,O_RDONLY);
	struct stat st;
	fstat(fd,&st);
	fileLen  = st.st_size;

	buff= malloc(fileLen);
	if(buff==NULL){
		error("malloc false");
		return;
	}
	read(fd,buff,fileLen);	
	pbuff = buff;
}
char peekCh(){
	//printf("%c\r\n",*pbuff);
	return *pbuff;
}
void nextCh(){
	pbuff++;
	len++;
}

bool isDigital(char c){
	if(c>='0' &&c<='9'){
		return true;
	}
	return false;
}

bool isCharater(char c){
	if(c>='a' &&c<='z'){
		return true;
	}
	return false;
}


struct Token * findId(char * str){
	
	int i=0;
	while(tokenReserve[i]!=NULL){
		if(strcmp (tokenReserve[i]->value_s ,str)!=0){
			i++;
		}else{
			//printf("find id is %s \r\n",tokenReserve[i]->value_s);
			return tokenReserve[i];
		}
	}
	return NULL;
}
struct Token* scan(){
	char c = peekCh();
	int tmp = 0;
	struct Token *tok;
	bool isdig = false;
	bool isreal = false;
	//printf("scan %d %d\r\n",len,fileLen);
	//skip blank
	while(c ==' ' || c=='\n' || c=='	') {
		nextCh();
		c = peekCh();
	//	printf("skip bank\r\n");
	}
	if(len == fileLen) return NULL;
	//token is digital
	if(isDigital(c)){
		while(true){
			tmp = tmp*10 + c - '0';	
			nextCh();
			c = peekCh();		
			if(!isDigital(c))	 
				break;	
		}
		tok = malloc(sizeof(struct Token)) ;
		if(peekCh()!='.'){
			tok->tag = NUM;
			tok->value_i= tmp;
		}else{
			
			int i=10;
			float f_tmp = tmp;
			while(true){
				nextCh();//skip .
				c = peekCh();
				//printf("%c %f\r\n",c,f_tmp);
				if(!isDigital(c))break;
				f_tmp  = f_tmp + (float)(c-'0')/i;
				i=i*10;
			}
			tok->tag = REAL;
			tok->value_f = f_tmp;
		}
		
		//nextCh();
		return tok;
	}
	if(c=='&'  || c=='|' || c=='!' || c=='<' || c=='>' || c=='='){
		tok = malloc(sizeof(struct Token)) ;
		nextCh();
		switch(c){
			case '&':						
				if(peekCh()== '&') {
					nextCh();
					tok->tag = AND;
				}
				else{
					tok->tag = '&';
				}
				break;
			case '<':
				
	 			if(peekCh()== '=') {
					
					nextCh();
					tok->tag = LE;
				}
				else{
					tok->tag = '<';
				}
				break;
			case '>':
	 			if(peekCh()== '=') {
					nextCh();
					tok->tag = GE;
				}
				else{
					tok->tag = '>';
				}
				break;
			case '=':
	 			if(peekCh()== '=') {
					nextCh();
					tok->tag = EQ;
				}
				else{
					tok->tag = '=';
				}
				break;
			case '!':
	 			if(peekCh()== '=') {
					nextCh();
					tok->tag = NE;
				}
				else{
					tok->tag = '!';
				}
				break;
			case '|':
	 			if(peekCh()== '|') {
					nextCh();
					tok->tag = OR;
				}
				else{
					tok->tag = '|';
				}
				break;
			default:
				break;
		}
		return tok;
	}
	 
	//tok is id
	//fix me
	char *id = malloc(100);
	memset(id,0,sizeof(id));
	int i =0;
	if((c >= '0' && c <='9' ) || (c>='a' && c<='z')){
		while((c >= '0' && c <='9' ) || (c>='a' && c<='z')){		
			id[i++] = c;
			nextCh();
			c=peekCh();
		}
		if(tok = findId(id)){
			//printf("already id %s\r\n",id);
			return tok;
		}else{
			//printf("id is%s\r\n",id);
			tok = malloc(sizeof(struct Token)) ;
			tok->tag = ID;
			tok->value_s = id;
		}
		return tok;
	}
	
	tok = malloc(sizeof(struct Token)) ;
	tok->tag = c;
	
	nextCh();
	return tok;
}

#if 0
int main(int argc,char *argv[]){
	readFile("expr3.t");
	struct Token *tok;
	while(tok= scan()){
		if(tok->tag == NUM){ printf("NUM: %d\r\n",tok->value_i);}
		else if(tok->tag == ID) {printf("ID: %s\r\n",tok->value_s);}
		else if(tok->tag == REAL) {printf("REAL:%f\r\n",tok->value_f);}
		else if(tok->tag == AND) {printf("&&\r\n");}
		else if(tok->tag == OR) {printf("|| \r\n");}
		else if(tok->tag == NE) {printf("!=\r\n");}
		else if(tok->tag == LE) {printf("<=\r\n");}
		else if(tok->tag == GE) {printf(">=\r\n");}
		else if(tok->tag == EQ) {printf("==\r\n");}
		else if(tok->tag == BASIC){printf("%s\r\n",tok->value_s);}
		else if(tok->tag == TRUE){printf("%s\r\n",tok->value_s);}
		else if(tok->tag == FALSE){printf("%s\r\n",tok->value_s);}
		else if(tok->tag == IF){printf("%s\r\n",tok->value_s);}
		else if(tok->tag == ELSE){printf("%s\r\n",tok->value_s);}
		else if(tok->tag == DO){printf("%s\r\n",tok->value_s);}
		else if(tok->tag == WHILE){printf("%s\r\n",tok->value_s);}		
		else printf("%c\r\n",tok->tag);
	}
}
#endif
