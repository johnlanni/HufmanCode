#include<list>
#include<string>
#include"treenode.h"
using namespace std;
class process{
	public:
		string inname;//�����ļ�·��
		string outname;//����ļ�·��
		//int totalbit;//��λ�����ۼƺ�
		//int tablesize;//ӳ���ĵĴ�С
		list<treenode> nodelist;
		map<char,int> fremap;
		map<char,string> codetable;//�ַ���Ӧ�����һ��ӳ���
		process(string in,string out,int type);//type=0��ѹ�� type=1�ǽ�ѹ��
		void readtxt();//��һ��TXT�ļ�����һ����Ƶ�ʴ�С�������е�treenode
		void encode(); //��nodelist��������������,������һ��ӳ���
		void treecover(treenode &node,string i);//һ�����������ڵ㲢�������ĵݹ麯��
		void writedat();//����codetable��inname�ж������ַ�����ת�����ٽ�stringת��Ϊbitset��ͬʱ����totalbit��tablesize��һ��д��outname��
	    void decompress();//��һ���µĶ�����ô˺�����������dat�ļ����õ�һ����ת��codetable���ݴ˽�ѹ��ָ����outname��



};