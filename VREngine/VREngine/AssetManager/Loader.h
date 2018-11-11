#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

namespace MyLoader {
	class Node {
	public:
		Node() :child(), name(""), value(""),lineNum(0) {
		}
		void destroy() {
			for (int i = 0; i < child.size(); i++) {
				child[i]->destroy();
				delete child[i];
			}
		}
		//递归寻找名字为str的节点，没有则返回NULL
		Node *getNodef(string str) {
			if (this->name.compare(str) == 0)return this;
			Node* tnode = NULL;
			for (int i = 0; i < child.size(); i++) {
				tnode = child[i]->getNodef(str);
				if (tnode != NULL)return tnode;
			}
			return tnode;
		}
		//寻找本节点儿子中名字为str的节点，没有返回NULL
		Node * getNode(string str) {
			for (int i = 0; i < child.size();i++) {
				if (child[i]->name.compare(str) == 0)
					return child[i];
			}
			return NULL;
		}
		//寻找本节点儿子中所有名字为str的节点，返回一个vector，没有的话vector的size为0
		vector<Node*> getNodes(string str) {
			vector<Node*> v;
			for (int i = 0; i < child.size(); i++) {
				if (child[i]->name.compare(str) == 0)
					v.push_back(child[i]);
			}
			return v;
		}
		vector<Node*> child;
		string name;
		string value;
		int lineNum;
	};
	class Loader {
	public:
		~Loader() {
			this->destroy();
		}
		void load(string fileName);
		void destroy() { root.destroy(); }
		Node root;
	};
}