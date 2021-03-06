// CompilationHW2.cpp : Defines the entry point for the console application.
//
#include "AST.h"
#include "tokens.h"

void printErrorMsg() {
	std::cout << "Syntax error" << std::endl;
}

BodyNode* recDesBody() {
	int token;
	return new BodyNode(recDesDict);
}

ObjNode* recDesDict(){
	int token = yylex();
	if (token != LDICT){
		printErrorMsg();
		return nullptr;
	}
	ObjNode* node = new DictNode(KVListNode)
}

ObjNode* recDesObj() {
	int token;
	token = yylex();
	if (token != INTEGER) {
		printErrorMsg();
		return nullptr;
	}
	token = yylex();
	if (token != INTEGER) {
		printErrorMsg();
		return nullptr;
	}
	token = yylex();
	if (token != OBJ){
		printErrorMsg();
		return nullptr;
	}
	ObjNode* node = new ObjNode(recDesBody());
	token = yylex();
	if (token != ENDOBJ) {
		delete node;
		printErrorMsg();
		return nullptr;
	}
	return node;
}



int main()
{
	ASTNode* node = new ObjNode(recDes());
	
}

