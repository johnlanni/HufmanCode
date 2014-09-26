#include<iostream>
#include<map>
#include<fstream>
using namespace std;
class treenode{
public:
	treenode* left;
	treenode* right;
	char letter;//字符
	int frequence;//频率
	string huffcode;//用字符串暂时存储编码
	//定义判断一个treenode对象的大小规则
	friend bool operator <(const treenode& a,const treenode& b){
		return a.frequence<b.frequence;
	};
	//拷贝构造
	treenode(const treenode& node){
		left=node.left;
		right=node.right;
		letter=node.letter;
		frequence=node.frequence;
		huffcode=node.huffcode;
	}
	//赋值构造
	treenode& operator=(const treenode &node)
	{
		left=node.left;
		right=node.right;
		letter=node.letter;
		frequence=node.frequence;
		huffcode=node.huffcode;
		return *this;

	}
	treenode():left(NULL),right(NULL),letter(NULL),frequence(0),huffcode(""){}
	treenode(char le,int fre):left(NULL),right(NULL),letter(le),frequence(fre),huffcode(""){}

};

