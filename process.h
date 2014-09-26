#include<list>
#include<string>
#include"treenode.h"
using namespace std;
class process{
	public:
		string inname;//读入文件路径
		string outname;//输出文件路径
		//int totalbit;//总位数的累计和
		//int tablesize;//映射表的的大小
		list<treenode> nodelist;
		map<char,int> fremap;
		map<char,string> codetable;//字符对应编码的一个映射表
		process(string in,string out,int type);//type=0是压缩 type=1是解压缩
		void readtxt();//读一个TXT文件返回一串按频率从小到大排列的treenode
		void encode(); //用nodelist来构建霍夫曼树,并返回一个映射表
		void treecover(treenode &node,string i);//一个用来遍历节点并处理编码的递归函数
		void writedat();//根据codetable将inname中读出的字符进行转换，再将string转换为bitset，同时计算totalbit和tablesize，一起写入outname中
	    void decompress();//用一个新的对象调用此函数，将读入dat文件，得到一个反转的codetable，据此解压到指定的outname中



};