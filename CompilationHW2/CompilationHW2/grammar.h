#ifndef __GRAMMAR__
#define __GRAMMAR__

#include "tokens.h"
#include <vector>
#include <set>

enum nonterminal {
	Obj,
	Body,
	Dict,
	Arr,
	KVList,
	KV,
	Exp,
	EList,
	NONTERMINAL_ENUM_SIZE
};

struct grammar_rule {
	nonterminal lhs;
	std::vector<int> rhs; //nonterminal and token
	grammar_rule(nonterminal _lhs, std::vector<int> _rhs) :lhs(_lhs), rhs(_rhs) {}
};

extern std::vector<grammar_rule> grammar;

void print_nullable(const std::vector<bool>& nullables);
void print_first(const std::vector<std::set<tokens> >& firsts);

#endif //__GRAMMAR__
