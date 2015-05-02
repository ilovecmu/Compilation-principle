#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "id.h"
#include "env.h"
#include "util.h"


void push2(struct TokenList *list,struct Token *tok){
	if(list->head==NULL ){
	//	log("push2 malloc\r\n");
		list->head = malloc(sizeof(struct Token *) * 20);
		list->cap  =20;
		list->cnt = 0;
	}
	if(list->cnt == list->cap){
		list->head = realloc(list->head,sizeof(struct Token *) * list->cnt*2);
		list->cap = list->cnt*2;
	}
	list->head[list->cnt] = tok;
	list->cnt++;
	//log("push2 tok->values=%s,list=%d\r\n",tok->value_s,list);
}

struct Token * get2(struct TokenList *ori,struct TokenList *list,struct Token *tok){
	int i=0;
	if(list->head ==NULL) error("get2 cant be null");
	for(;list>=ori;)
	{
		for(i=0;i<list->cnt;i++){
		
			//log("list->head[i]=%s,%d\r\n",list->head[i]->value_s,list);
			if(list->head[i]->tag == tok->tag && strcmp (list->head[i]->value_s , tok->value_s)==0){
				//log("get2 find %s\r\n",list->head[i]->value_s);
				return list->head[i];
			}
		}
		list--;
	}
	error("get2 find nothing %s %d\r\n",tok->value_s,tok->tag);
	return NULL;
}

