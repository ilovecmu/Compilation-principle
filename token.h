#ifndef __TOKEN__H
#define __TOKEN__H
#define TOKEN_F	0
#define TOKEN_I 1
#define TOKEN_S 2

struct Token{
	int tag;
	float value_f;
	int value_i;
	char * value_s;
	//int type;
};

enum{
AND   = 256,  BASIC = 257,	BREAK = 258,  DO   = 259, ELSE	= 260,
	 EQ    = 261,  FALSE = 262,  GE    = 263,  ID	= 264, IF	 = 265,
	 INDEX = 266,  LE	 = 267,  MINUS = 268,  NE	= 269, NUM	 = 270,
	 OR    = 271,  REAL  = 272,  TEMP  = 273,  TRUE = 274, WHILE = 275
};

struct Token* scan();

#endif
