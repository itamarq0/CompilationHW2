#include "tokens.h"
#include "grammar.h"
#include <iostream>

void print_token(tokens tok) {
	switch(tok) {
	case OBJ: std::cout << "OBJ"; break;
	case ENDOBJ: std::cout << "ENDOBJ"; break;
	case LBRACE: std::cout << "LBRACE"; break;
	case RBRACE: std::cout << "RBRACE"; break;
	case LDICT: std::cout << "LDICT"; break;
	case RDICT: std::cout << "RDICT"; break;
	case TRUE: std::cout << "TRUE"; break;
	case FALSE: std::cout << "FALSE"; break;
	case INTEGER: std::cout << "INTEGER"; break;
	case REAL: std::cout << "REAL"; break;
	case STRING: std::cout << "STRING"; break;
	case NAME: std::cout << "NAME"; break;
	case STREAM: std::cout << "STREAM"; break;
	case NUL: std::cout << "NULL"; break;
	case EF: std::cout << "EOF"; break;

	};
}

void print_nonterminal(nonterminal var) {
	switch(var) {
	case Obj: std::cout << "Obj"; break;
	case Body: std::cout << "Body"; break;
	case Dict: std::cout << "Dict"; break;
	case Arr: std::cout << "Arr"; break;
	case KVList: std::cout << "KVList"; break;
	case KV: std::cout << "KV"; break;
	case Exp: std::cout << "Exp"; break;
	case EList: std::cout << "EList"; break;

	};
}

void print_nullable(const std::vector<bool>& vec) {
	for(int i = 0; i < vec.size(); ++i) {
		print_nonterminal((nonterminal)(i));
		std::cout << "=";
		if (vec[i]) std::cout << "true"; else std::cout << "false";
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}

void print_first(const std::vector<std::set<tokens> >& vec) {
        for(int i = 0; i < vec.size(); ++i) {
		print_nonterminal((nonterminal)(i));
		std::cout << "={";
                for(std::set<tokens>::const_iterator it = vec[i].begin(); it != vec[i].end(); ++it) {
			if (it != vec[i].begin()) std::cout << ",";
			print_token(*it);
		}
		std::cout << "}";
                std::cout << std::endl;
        }
}




std::vector<grammar_rule> make_grammar() {
	std::vector<grammar_rule> res;
	int rule0[] = {INTEGER,INTEGER,OBJ,Body,ENDOBJ};
	res.push_back(grammar_rule(Obj,std::vector<int>(rule0,rule0+5)));

	res.push_back(grammar_rule(Body,std::vector<int>(1,Dict)));
	res.push_back(grammar_rule(Body,std::vector<int>(1,Arr)));
	
	int rule3[] = {LDICT, KVList, RDICT};
	res.push_back(grammar_rule(Dict, std::vector<int>(rule3,rule3 + 3)));

	int rule4[] = {KV, KVList};
	res.push_back(grammar_rule(KVList,std::vector<int>(rule4,rule4+2)));
	res.push_back(grammar_rule(KVList,std::vector<int>()));

	int rule6[] = {NAME, Exp};
	res.push_back(grammar_rule(KV,std::vector<int>(rule6,rule6+2)));

	res.push_back(grammar_rule(Exp,std::vector<int>(1,INTEGER)));
	res.push_back(grammar_rule(Exp,std::vector<int>(1,REAL)));
	res.push_back(grammar_rule(Exp,std::vector<int>(1,TRUE)));
	res.push_back(grammar_rule(Exp,std::vector<int>(1,FALSE)));
	res.push_back(grammar_rule(Exp,std::vector<int>(1,STRING)));
	res.push_back(grammar_rule(Exp,std::vector<int>(1,NUL)));

	int rule13[] = {LBRACE, EList, RBRACE};
	res.push_back(grammar_rule(Arr,std::vector<int>(rule13,rule13+3)));

	int rule14[] = {Exp, EList};
	res.push_back(grammar_rule(EList,std::vector<int>(rule14,rule14+2)));
	res.push_back(grammar_rule(EList,std::vector<int>()));
	return res;
}

std::vector<grammar_rule> grammar = make_grammar();
