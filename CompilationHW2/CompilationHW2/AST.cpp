#include "AST.h"

void ast_print(const ASTNode& node)
{
	 node.prettyPrint();
}


ObjNode::ObjNode(BodyNode * body)
{
	children.push_back(body);
}

void ObjNode::prettyPrint() const
{
	std::cout << "ObjNode {";
	for (std::vector<ASTNode*>::const_iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->prettyPrint();
	}
	std::cout << "} End ObjNode" << std::endl;;
}

BodyNode::BodyNode(DictNode * dict)
{
	children.push_back(dict);
}

void BodyNode::prettyPrint() const 
{
	std::cout << "BodyNode [";
	for (std::vector<ASTNode*>::const_iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->prettyPrint();
	}
	std::cout << "] End BodyNode";
}


DictNode::DictNode(KVListNode * kvList)
{
	children.push_back(kvList);
}

void DictNode::prettyPrint() const 
{
	std::cout << "Dictionary Node <<";
	for (std::vector<ASTNode*>::const_iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->prettyPrint();
	}
	std::cout << ">> End Dictionary";
}



KVListNode::KVListNode(KVNode * kv, KVListNode *list)
{ 
	children.push_back(kv);
	children.push_back(list);
}

KVListNode::KVListNode()
{}

void KVListNode::prettyPrint() const
{
	std::cout << "KVListNode <";
	for (std::vector<ASTNode*>::const_iterator it = children.begin(); it != children.end(); ++it) {
		if (it != children.begin()) std::cout << ", ";
		(*it)->prettyPrint();
	}
	std::cout << "> End KVListNode";
}


KVNode::KVNode(ExpNode *child) //rule 7
{
	children.push_back(child);
}
void KVNode::prettyPrint() const
{
	std::cout << "Key : Value(";
	for (std::vector<ASTNode*>::const_iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->prettyPrint();
	}
	std::cout << ")";
}


void ExpNode::prettyPrint() const
{
	std::cout << "Expr";
}


