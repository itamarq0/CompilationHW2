%{
	#include "tokens.h"
	#include <stdio.h>
	#include <string.h>
	tokens printLine(int line, char* token, char* value, int len);
	tokens printString2(int line, char* value,int len);
	tokens printString1(int line, char* value,int len);
	tokens printStream(int line, char* value,int len);
	void badChar(char* bad);
	void errorInString(char* value,int len);
	void seqError(char* seq,int len);
	tokens printEOF(int line);
	void string2bad(char* value, int place);
%}
%option yylineno
%option noyywrap
digit	([0-9])
letter	([a-zA-Z])
hex ([a-fA-F0-9])
whitespace	([ \t\n\r]|\r\n)
newLine ([\r\n]|\r\n)
printable ([ -']|[*-\[]|[\]-~])
escseq ([ntrbf\\\(\)]|[0-7]{3})
notesc ([ceqs])
end ([ \t\n\r<<EOF>>]|\r\n)
endOfFile ([<<EOF>>])
%x STRING1
%x STRING2
%%
obj	printLine(yylineno,"OBJ",yytext, yyleng);
endobj printLine(yylineno,"ENDOBJ",yytext, yyleng);
\[ printLine(yylineno,"LBRACE",yytext, yyleng);
\] printLine(yylineno,"RBRACE",yytext, yyleng);
\<\< printLine(yylineno,"LDICT",yytext, yyleng);
\>\> printLine(yylineno,"RDICT",yytext, yyleng);
%([^\r\n])*/{newLine} printLine(yylineno,"COMMENT",yytext, yyleng);
true printLine(yylineno,"TRUE",yytext, yyleng);
false printLine(yylineno,"FALSE",yytext, yyleng);
[\+-]?{digit}+ printLine(yylineno,"INTEGER",yytext, yyleng);
([\+-]?{digit}+\.{digit}*)|([\+-]?{digit}*\.{digit}+) printLine(yylineno,"REAL",yytext, yyleng);
\/({digit}|{letter})+ printLine(yylineno,"NAME",yytext,yyleng);
stream{newLine}([^e]|e[^n]|en[^d]|end[^s]|ends[^t]|endst[^r]|endstr[^e]|endstr[^e]|endstre[^a]|endstrea[^m]|endstream[^ \t\n\r])*{newLine}endstream/{end} printStream(yylineno, yytext,yyleng);
null printLine(yylineno,"NULL",yytext, yyleng);
\( {BEGIN(STRING1); yymore(); }
<STRING1>\) {BEGIN(INITIAL); printString1(yylineno,yytext,yyleng);}
<STRING1>{printable} yymore();
<STRING1>\\{newLine} yymore();
<STRING1>\\{escseq} yymore();
<STRING1><<EOF>> {printf("Error unclosed string\n"); exit(0);}
<STRING1>\\[^{escseq}] seqError(yytext,yyleng);
<STRING1>\\{notesc} seqError(yytext,yyleng);
<STRING1>(.|\n) errorInString(yytext,yyleng);
\< {BEGIN(STRING2); yymore(); }
<STRING2>\> {BEGIN(INITIAL); printString2(yylineno,yytext,yyleng);}
<STRING2>((({hex}{whitespace}*){2})|{whitespace})* yymore();
<STRING2><<EOF>> {printf("Error unclosed string\n"); exit(0);}
<STRING2>{hex} {printf("Error incomplete byte\n"); exit(0);}
<STRING2>. string2bad(yytext,yyleng);
stream\n[.\n]*\n+|stream\n {printf("Error unclosed stream\n"); exit(0);}
<<EOF>> printEOF(yylineno);
{whitespace} ;
. badChar(yytext);

%%
int getVal(char c){
	if (c <= 'f' && c >= 'a'){
		return 10+c-'a';
	}
	if (c <= 'F' && c >= 'A'){
		return 10+c-'A';
	}
	return c-'0';
}
tokens printLine(int line, tokens token, char* value, int len){
	return token;
}
void badChar(char* bad){
	printf("Error %s\n",bad);
	exit(0);
}
int checkSpace(char c){
	return (c == ' '|| c=='\n' || c=='\t' || c== '\r' || c== '\b');
}
tokens printString2(int line, char* value, int len){
	return STRING;
	printf("%d STRING ",line);
	int i = 1;
	for(i = 1; i<len-2; i++){
		if( checkSpace(value[i])){
			continue;
		}
		int p = getVal(value[i++])*16;
		while(checkSpace(value[i])){
			i++;
		}
		p+=getVal(value[i]);
		printf("%c",p);
	}
	printf("\n");
}
tokens printString1(int line, char* value, int len){
	return STRING;
	int i;
	printf("%d STRING ",line);
	for(i=1;i<len-1;i++){
		if(value[i] == '\\'){
			i++;
			if(value[i]>='0'&&value[i]<='7'){
				int val = (value[i++]-'0')*64;
				val+=(value[i++]-'0')*8;
				val+=(value[i]-'0');
				printf("%c",val);
				continue;
			}else{
				switch(value[i]){
					case 'n':
						printf("\n");
						break;
					case 't':
						printf("\t");
						break;
					case 'r':
						printf("\r");
						break;
					case 'b':
						printf("\b");
						break;
					case 'f':
						printf("\f");
						break;
					case '(':
						printf("(");
						break;
					case ')':
						printf(")");
						break;
					case '\\':
						printf("\\");
						break;
					case '\r':
						if (value[i+1] == '\n'){
							i++;
						}
						break;
				}
			}
		}else{
				printf("%c",value[i]);
			}
	}
	printf("\n");
}
tokens printStream(int line, char* value,int len){
	return STREAM;
	printf("%d STREAM ",line);
	int i = 0, end = len-10;
	if(value[end-1] == '\r'){
		end--;
	}
	while(value[i++] != '\n');
	for (; i< end; i++){
		printf("%c", value[i]);
	}
	printf("\n");
}

void seqError(char* seq,int len){
	int i;
	for(i = 1; i<len; i++){
		if(seq[i] == '\\' && seq[i+1]!='n'&& seq[i+1]!='r'&& seq[i+1]!='t'&& seq[i+1]!='b'&& seq[i+1]!='f'&& seq[i+1]!='('&& seq[i+1]!=')'&& seq[i+1]!='\\'&& seq[i+1]!='\r'&& seq[i+1]!='\n'){
			printf("Error undefined escape sequence %c\n",seq[i+1]);
			exit(0);
		}
	}
}

int checkPrinable(char* value, int place, int max){
	char temp;
	if(place < max-1){
		if (value[place] == '\\'){
			temp = value[place+1];
			if (temp == 'r' || temp == 'n' || temp == 't' || temp == 'b' || temp == 'f'|| temp == ')' || temp == '(' || temp == '\\' || temp == '\n' ){
				return 1;
			}
			if ( place < max - 3 && (temp == '\r' && value[place+2] != '\n')){
				return 1;
			}
			if (temp == '\r'){
				return 2;
			}
			return 0;
		}	
	}
	if((value[place]>= 32 && value[place]<= 39) || (value[place]>= 42 && value[place]<= 91) || (value[place]>= 93 && value[place]<= 126)){
		return 3;
	}
	return 0;
}

void errorInString(char* value,int len){
	int i, res;
	for(i = 1; i<len; i++){
		res = checkPrinable(value, i, len);
		switch(checkPrinable(value, i, len)){
			case(0):
				printf("Error %c\n",value[i]);
				exit(0);
				return;
			case(2):
				i++;
			case(1):
				i++;
			case(3):
				continue;
		}
	}
}

tokens printEOF(int line){
	return EF;
	printf("%d EOF \n", line);
	exit(0);
}

void string2bad(char* value, int place){
	printf("Error %c\n",value[place-1]);
	exit(0);
}
