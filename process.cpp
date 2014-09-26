#include"process.h"
#include<algorithm>
#include <cstdint>

using namespace std;
process::process(string in,string out,int type){
	inname=in;
	outname=out;
	if(type==1){ //ѹ���ļ�
		encode();
		writedat();
	}
	else decompress();//��ѹ���ļ�
}
void process::readtxt()
{
	ifstream fin(inname);
	while(!fin.eof()){ //�ж��ļ��Ƿ����β
		char le;
		le = fin.get();
		//����Ƶ�ʱ�
		if(fremap.find(le)==fremap.end())
			fremap[le]=1; 
		else
			fremap[le]++;
	}
	//��map���б���������������pair������Ԫ����Ϊ��������һ��treenode����push��list������
	for(map<char,int>::iterator iter=fremap.begin();iter!=fremap.end();++iter){
		nodelist.push_back(treenode(iter->first,iter->second));
	}
	nodelist.sort();//����frequence��С��������
	fin.close();
	

}
void  process::treecover(treenode &node,string i){
	if(node.left==NULL&&node.right==NULL){
		node.huffcode = i + node.huffcode;
		codetable[node.letter] = node.huffcode;//��̬ά������codetable
	}
	else{
		//�ݹ����������������������ӦҶ�ڵ��huffmancode����i
		treecover(*(node.left),i);
		treecover(*(node.right),i);
	 }


}
void  process::encode()
{
	readtxt();
	treenode first;
	treenode  second;
	while(nodelist.size()>1){//��list�нڵ�ֻʣһ��ʱֹͣѭ��
	first = nodelist.front();//Ƶ����С�Ľڵ�
	nodelist.pop_front();
	treecover(first,"0");//��������Ҷ�ڵ�huffmancode��0
	second = nodelist.front();//Ƶ�ʵڶ�С�Ľڵ�
	nodelist.pop_front();
	treecover(second,"1");//��������Ҷ�ڵ�huffmancode��1
	treenode *newnode=new treenode;//����һ���µĽڵ�
	newnode->left = new treenode;
	newnode->right = new treenode;
	//��firstָ������ݸ���newnode��left�ӽڵ㣬secondָ������ݸ���right�ӽڵ�
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
	nodelist.push_front(*(newnode));//��newnode����list��
	nodelist.sort();//������
	delete newnode;
	}
	
}
void process::writedat()
{
	int totalbit=0;
	//���㽫�����ֵ��Ǹ�����string�����ж���λ
	for(map<char,int>::iterator iter=fremap.begin();iter!=fremap.end();++iter){
		totalbit+=codetable[iter->first].length()*(iter->second);
	}

	ifstream fin(inname);
	ofstream fout(outname, ios::binary);
	int tablesize;
	tablesize = codetable.size();
	//д��Ƶ�ʱ�Ĵ�С�����ж��ٶ�char��int��
	fout.write((char *)(&tablesize), sizeof(tablesize));
	for(map<char,int>::iterator iter = fremap.begin();iter!=fremap.end();++iter)
	{
		int fre = iter->second;
		fout<<iter->first;
		fout.write((char *)(&fre), sizeof(fre));	
		//fout.write((char *)(&(iter->first)), sizeof(iter->first));
	}
	//��totoalbitд���ļ�
	fout.write((char *)(&totalbit), sizeof(totalbit));
	//�˴�ʹ��int8_t ��һ��8λ��int��ʵЧ����char��һ���ġ�����
	int8_t b8=0x00;
	int count=0;
	char *bset = new char[totalbit/8+1];
	int pos=0;
	//����������ľ�������Ѿ����ĵ���д�ú�����Ͳ�׸����
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
	b8=b8<<offset;//������һ����λ��������Ϊ�ܵ�stringλ����һ����8�ı������������ʣ�µ��Ǹ�b8Ҫ����8-count%8λ�γ�һ��8λ��int�ٲ�����������Ϊ���һ��Ԫ��
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
	//��������Ƶ�ʱ���ܴ�С
	fin.read((char *)(&length),sizeof(length)); 
	map<char,int> fremap;
	
	//������Ƶ�ʱ�
	for(int i=1;i<=length;i++)
	{
		int fre;
	    char le;
		string huffcode;
		le=fin.get(); 
		fin.read((char *)(&fre),sizeof(fre)); 
		fremap[le]=fre;

	}
	//������nodelist
	for(map<char,int>::iterator iter=fremap.begin();iter!=fremap.end();++iter){
		nodelist.push_back(treenode(iter->first,iter->second));
	}
	nodelist.sort();
	treenode first;
	treenode  second;
	//���������ɹ�����������Ƕδ��룬��ʵ��õ���ֻ��nodelist��ʣ�µ����һ���ڵ㡣����
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
	//�õ����ڵ�
	treenode root = nodelist.front();
	int totalbit;
	int8_t b8;
	string strset="";
	fin.read((char *)(&totalbit),sizeof(totalbit));	
	int count=totalbit/8 + 1;
	char *bset = new char[count];
	fin.read(bset, count);
	//����char������ÿ��char���еĲ�������ϸҲ���ĵ���д����
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
	//��Ϊbset�е����һ���ַ���һ��8λ������Ч�ģ�����ȡstrset��ǰtotalbitλ����һ��realset
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
	//��treeptrָ����ڵ�
	treenode* treeptr = &root;
	const char* setreal = realset.c_str();
	for(unsigned int i=0;i<realset.length();i++){
		if(setreal[i]=='0') treeptr = treeptr->left;//ָ����ڵ�
		//treeptrָ���ҽڵ�
		else treeptr = treeptr->right;
		if(treeptr->left==NULL&&treeptr->right==NULL){
			fout<<(treeptr->letter);
			treeptr = &root;//�ҵ�һ��char��treeptr����ָ����ڵ�
		}
	}
	/*for(unsigned int i=0;i<realset.length();i++){
		code = code + realset[i];
		if(map.find(code)!=map.end()){//����ƿ�����ڣ�����δʵ�����ṹ�����л���map�����ö�������Ҫ�����нڵ���б���
			fout<<map[code];
			code = "";
		}
	}*/
	//fout<<outs;
	
	fout.close();
}