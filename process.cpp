#include"process.h"
#include<algorithm>
#include <cstdint>

using namespace std;
process::process(string in,string out,int type){
	inname=in;
	outname=out;
	if(type==1){ //压缩文件
		encode();
		writedat();
	}
	else decompress();//解压缩文件
}
void process::readtxt()
{
	ifstream fin(inname);
	while(!fin.eof()){ //判断文件是否读到尾
		char le;
		le = fin.get();
		//更新频率表
		if(fremap.find(le)==fremap.end())
			fremap[le]=1; 
		else
			fremap[le]++;
	}
	//对map进行遍历，将遍历出的pair的两个元素作为参数构造一个treenode对象push进list容器内
	for(map<char,int>::iterator iter=fremap.begin();iter!=fremap.end();++iter){
		nodelist.push_back(treenode(iter->first,iter->second));
	}
	nodelist.sort();//根据frequence大小进行排序
	fin.close();
	

}
void  process::treecover(treenode &node,string i){
	if(node.left==NULL&&node.right==NULL){
		node.huffcode = i + node.huffcode;
		codetable[node.letter] = node.huffcode;//动态维护这张codetable
	}
	else{
		//递归操作，对其左右子树的相应叶节点的huffmancode都加i
		treecover(*(node.left),i);
		treecover(*(node.right),i);
	 }


}
void  process::encode()
{
	readtxt();
	treenode first;
	treenode  second;
	while(nodelist.size()>1){//当list中节点只剩一个时停止循环
	first = nodelist.front();//频率最小的节点
	nodelist.pop_front();
	treecover(first,"0");//所有子树叶节点huffmancode加0
	second = nodelist.front();//频率第二小的节点
	nodelist.pop_front();
	treecover(second,"1");//所有子树叶节点huffmancode加1
	treenode *newnode=new treenode;//构造一个新的节点
	newnode->left = new treenode;
	newnode->right = new treenode;
	//将first指向的内容赋给newnode的left子节点，second指向的内容赋给right子节点
	newnode->left->frequence=first.frequence;
	newnode->left->huffcode=first.huffcode;
	newnode->left->left=first.left;
	newnode->left->letter=first.letter;
	newnode->left->right=first.right;
	newnode->right->frequence=second.frequence;
	newnode->right->huffcode=second.huffcode;
	newnode->right->left=second.left;
	newnode->right->letter=second.letter;
	newnode->right->right=second.right;
	newnode->frequence = first.frequence+second.frequence;
	nodelist.push_front(*(newnode));//将newnode插入list中
	nodelist.sort();//再排序
	delete newnode;
	}
	
}
void process::writedat()
{
	int totalbit=0;
	//计算将来出现的那个超长string将会有多少位
	for(map<char,int>::iterator iter=fremap.begin();iter!=fremap.end();++iter){
		totalbit+=codetable[iter->first].length()*(iter->second);
	}

	ifstream fin(inname);
	ofstream fout(outname, ios::binary);
	int tablesize;
	tablesize = codetable.size();
	//写入频率表的大小（即有多少对char，int）
	fout.write((char *)(&tablesize), sizeof(tablesize));
	for(map<char,int>::iterator iter = fremap.begin();iter!=fremap.end();++iter)
	{
		int fre = iter->second;
		fout<<iter->first;
		fout.write((char *)(&fre), sizeof(fre));	
		//fout.write((char *)(&(iter->first)), sizeof(iter->first));
	}
	//将totoalbit写入文件
	fout.write((char *)(&totalbit), sizeof(totalbit));
	//此处使用int8_t 是一个8位的int其实效果跟char是一样的。。。
	int8_t b8=0x00;
	int count=0;
	char *bset = new char[totalbit/8+1];
	int pos=0;
	//这里接下来的具体操作已经在文档里写得很清楚就不赘述了
	while(!fin.eof()){
		char le;
		le = fin.get();
		string huffcode = codetable[le];
	    int length = huffcode.length();
		for(unsigned int i=0;i<huffcode.length();i++){
		if(huffcode[i]=='1'){
		  // bitset.push_back(1);
			   b8 = b8<<1;
		       b8 = (b8|(0x01));
			   count++;
			   if(count%8==0){
				   //fout.write((char *)(&b8), sizeof(b8));
				   bset[pos]=b8;
				   b8=0x00;
				   pos++;
			   }
		}
		else{
			//bitset.push_back(0);
			b8 = b8<<1;
			b8 = b8 | 0x00;
			count++;
			if(count%8==0){
				 bset[pos]=b8;
			     //fout.write((char *)(&b8), sizeof(b8));
			     b8=0x00;
				 pos++;
			 }
		}
		   
	    }
	}
	int offset = 8-count%8;
	b8=b8<<offset;//这里有一个移位操作，因为总的string位数不一定是8的倍数，所以最后剩下的那个b8要右移8-count%8位形成一个8位的int再插入数组中作为最后一个元素
	if(count%8!=0) bset[pos]=b8;
	fout.write(bset,(totalbit/8+1));
	fin.close();
	//int setsize=sizeof(bitset);	
	//fout.write((char *)(&setsize), sizeof(setsize));
	//fout.write((char *)(&bitset), sizeof(bitset));
	fout.close();
}
void process::decompress()
{
	ifstream fin(inname,ios::binary);
	ofstream fout(outname);
	int length;
	//读出那张频率表的总大小
	fin.read((char *)(&length),sizeof(length)); 
	map<char,int> fremap;
	
	//构建出频率表
	for(int i=1;i<=length;i++)
	{
		int fre;
	    char le;
		string huffcode;
		le=fin.get(); 
		fin.read((char *)(&fre),sizeof(fre)); 
		fremap[le]=fre;

	}
	//构建出nodelist
	for(map<char,int>::iterator iter=fremap.begin();iter!=fremap.end();++iter){
		nodelist.push_back(treenode(iter->first,iter->second));
	}
	nodelist.sort();
	treenode first;
	treenode  second;
	//复制了生成哈弗曼编码的那段代码，其实想得到的只是nodelist中剩下的最后一个节点。。。
	while(nodelist.size()>1){
	first = nodelist.front();
	nodelist.pop_front();
	treecover(first,"0");
	second = nodelist.front();
	nodelist.pop_front();
	treecover(second,"1");
	treenode *newnode=new treenode;
	newnode->left = new treenode;
	newnode->right = new treenode;
	newnode->left->frequence=first.frequence;
	newnode->left->huffcode=first.huffcode;
	newnode->left->left=first.left;
	newnode->left->letter=first.letter;
	newnode->left->right=first.right;
	newnode->right->frequence=second.frequence;
	newnode->right->huffcode=second.huffcode;
	newnode->right->left=second.left;
	newnode->right->letter=second.letter;
	newnode->right->right=second.right;
	newnode->frequence = first.frequence+second.frequence;
	nodelist.push_front(*(newnode));
	nodelist.sort();
	delete newnode;
	}
	//得到根节点
	treenode root = nodelist.front();
	int totalbit;
	int8_t b8;
	string strset="";
	fin.read((char *)(&totalbit),sizeof(totalbit));	
	int count=totalbit/8 + 1;
	char *bset = new char[count];
	fin.read(bset, count);
	//读出char数组后对每个char进行的操作，详细也在文档里写明了
	for(int i=1;i<=count;i++){
	//fin.read((char *)(&b8), sizeof(b8));
	 b8=bset[i-1];
	 for(int j=7;j>=0;j--){
		int8_t b = b8 >> j;
		b = b & 0x01;
		if(b==0x01) strset.append("1");
		else strset.append("0");
	 }
	}
	string realset="";
	//因为bset中的最后一个字符不一定8位都是有效的，所以取strset的前totalbit位构造一个realset
	realset.insert(0,strset,0,totalbit);
	//fin.read((char *)(&bitset),size); 
	fin.close();
	/*string strset="";
	unsigned int i=1;
	for(deque<bool>::iterator iter=bitset.begin();i<=bitset.size();++iter){
		if((*iter)==1)strset = strset + "1";
		else strset = strset + "0";
		i++;
	}
	*/
	string code ="";
	//让treeptr指向根节点
	treenode* treeptr = &root;
	const char* setreal = realset.c_str();
	for(unsigned int i=0;i<realset.length();i++){
		if(setreal[i]=='0') treeptr = treeptr->left;//指向左节点
		//treeptr指向右节点
		else treeptr = treeptr->right;
		if(treeptr->left==NULL&&treeptr->right==NULL){
			fout<<(treeptr->letter);
			treeptr = &root;//找到一个char后treeptr重新指向根节点
		}
	}
	/*for(unsigned int i=0;i<realset.length();i++){
		code = code + realset[i];
		if(map.find(code)!=map.end()){//性能瓶颈所在，由于未实现树结构的序列化，map是利用二叉树，要对所有节点进行遍历
			fout<<map[code];
			code = "";
		}
	}*/
	//fout<<outs;
	
	fout.close();
}