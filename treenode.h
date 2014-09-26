#include<iostream>
#include<map>
#include<fstream>
using namespace std;
class treenode{
public:
	treenode* left;
	treenode* right;
	char letter;//�ַ�
	int frequence;//Ƶ��
	string huffcode;//���ַ�����ʱ�洢����
	//�����ж�һ��treenode����Ĵ�С����
	friend bool operator <(const treenode& a,const treenode& b){
		return a.frequence<b.frequence;
	};
	//��������
	treenode(const treenode& node){
		left=node.left;
		right=node.right;
		letter=node.letter;
		frequence=node.frequence;
		huffcode=node.huffcode;
	}
	//��ֵ����
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

