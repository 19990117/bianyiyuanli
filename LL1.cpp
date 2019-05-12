#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>
char A[20];//分析栈 
char B[20];//剩余串 
char v1[10] = {'$','#'};//终结符
char v2[10] = {};//非终结符
int lenth1=2,lenth2=0;//终结符和非终结符数组长度 
int j = 0, b = 0, top = 0, l;//L为输入串长度
typedef struct type//产生式结构体 和 first follow集结构体 
{ 
	char origin;//大写字符
	char array[5];//产生式右边字符 
	int length;//产生式右边字符个数 方便产生式逆序入栈 
	char follows[5];//与哪些follow集合有关 
	int flength;
	int isPredict;
}type; 
type s[20];//产生式表达式 
type first[10],follow[10];//first follow集 
int sNum=0,array=0;//sNum记录一共多少产生式，array记录产生式右边字符个数 
type C[10][10];//预测分析表
void print()//输出分析栈
{  
	int a;//指针 
	for (a = 0; a <= top + 1; a++)
		printf("%c", A[a]);
	printf("\t\t");
} 
void print1()//输出剩余串 
{  
	int j; 
	for (j = 0; j<b; j++)
		printf(" "); 
	for (j = b; j <= l; j++) 
		printf("%c", B[j]); 
	printf("\t\t\t");  
} 
//判断是否为已存在非终结符 
bool isNoTerminal(char ch){
	for (int j = 0; j < lenth2; j++)
		if (ch == v2[j]){ 
			return true;
		} 
	return false;
}
//判断是否为已存在终结符 
bool isTerminal(char ch){
	for (int j = 0; j < lenth1; j++)
		if (ch == v1[j]){ 
			return true;
		} 
	return false;
}
//求行号 非终结符 
int forRow(char in){
	for(int i=0;i<lenth2;i++){
		if(in == v2[i])
			return i;
	}
}
//求列号 不包括$ 
int forLine(char in){
	for(int i=0;i<lenth1;i++){
		if(in == v1[i])
			return i-1;
	}
} 
//求目标字符在哪个字符集里面排第几个 
int locat(char in,char s[],int length){
	for(int i=0;i<length;i++){
		if(in == s[i])
			return i;
	}
	return -1;//或者不在 
} 
//判断first集合中是否有空
bool isNext(char s){
	for(int x=0;x<lenth1;x++){
		if(first[x].origin == s){
			int length = first[x].length;
			for(int y=0;y<length;y++)
				if(first[x].array[y] == '$')
					return true;
			break;
		}
	}
	return false;
} 
//输入经过验证，稳 
void input(){
	char in;  
	int right = 0;//判断是否是产生式右部 
	int newFlag = 1;// 判断是否是新的产生式 
	do
	{
		scanf("%c", &in);
		//产生式左边origin赋值 
		if(newFlag == 1){
			s[sNum].origin = in;
		}
		newFlag = 0;
		if(in <= 'Z' && in >= 'A'){
			//判断是不是新的非终结符，是则加入v2 
			if (!isNoTerminal(in)){
				v2[lenth2++] = in;
			}
			if(right == 1){
				s[sNum].array[array++] = in;
			}
			continue;
		}else{
			if(in == '-'){
				continue;
			}else if(in == '>'){
				right = 1;//>为左右分割 
				continue;
			}else if(in == ' '){
				s[sNum].length = array;
				sNum++;//空格分割，产生新的产生式 
				array = 0;
				right = 0;
				newFlag = 1;
				continue;
			}else if(in == '\n'){
				s[sNum++].length = array;
				array = 0;
				right = 0;
				newFlag = 1;
				break;
			}else if(!isTerminal(in)){
				v1[lenth1++] = in;
			}
			//将终结符和$赋值 
			s[sNum].array[array++] = in;
		}
	}while (in != '\n');
}
//求first集合 经过验证，稳 
void forFirst(){
	int isContinue = 0;
	//初始化first集合 
	for(int i=0;i<lenth2;i++){
		first[i].origin = v2[i]; 
		first[i].length = 0;
	}
	//第一次循环先求first与其他产生式无关的first 
	for(int i=0;i<lenth2;i++){
		int firstNum = 0;
		for(int k=0;k<sNum;k++){
			//如果产生式右部第一个为非终结符 结束这次循环 
			if(first[i].origin == s[k].origin && isNoTerminal(s[k].array[0])){
				for(int inFirst=0;inFirst<firstNum;inFirst++){
					first[i].array[inFirst] = ' ';
				}
				first[i].length = 0;
				break;
			}else if(first[i].origin == s[k].origin){
				first[i].array[firstNum++] = s[k].array[0];
				first[i].length = firstNum;
			} 
		}
	}
	//为了避免前面的first需要后面没有求出来的first导致为空的情况
	//比如first(A)=first(A) first(A)=first(B) 
	do{
		isContinue = 0;
		//第二次就可以把它们都求出来了
		for(int i=0;i<lenth2;i++){
			int firstNum = 0;
			int firstHaveNull = 0;//first集合中不含空 
			for(int k=0;k<sNum;k++){
				//如果求出的first集合长度>0,说明是第一种情况 不求 
				int forFirst = 0;
				if(first[i].length > 0)
					break;
				if(first[i].origin == s[k].origin && isTerminal(s[k].array[0])){
					first[i].array[firstNum++] = s[k].array[0];
					first[i].length = firstNum;
					continue;
				}else if(first[i].origin == s[k].origin && isNoTerminal(s[k].array[0])){
					do{
						int haveNull = 0;//first集合中不含空 
						firstHaveNull = 0;
						int location = locat(s[k].array[forFirst],v2,lenth2);
						if(first[location].length == 0){
								isContinue = 1;
						}
						for(int x=0;x<first[location].length;x++){
							if(first[location].array[x] == '$'){
								haveNull = 1;
								firstHaveNull = 1;
								continue;
							}
							first[i].array[firstNum++] = first[location].array[x];
							first[i].length = firstNum;
						}
						forFirst++;
						if(haveNull == 0)
							break;
					//当产生式右部为非终结符时一直循环 
					}while(forFirst < s[k].length && isNoTerminal(s[k].array[forFirst]));
					if(firstHaveNull == 1){
						first[i].array[firstNum++] = '$';
						first[i].length = firstNum;
					}
				} 
			}
		} 
	}while(isContinue == 1);
}
//求follow集合 经过验证，稳 
void forFollow(){
	//初始化follow集合 
	for(int i=0;i<lenth2;i++){
		follow[i].origin = v2[i]; 
		follow[i].length = 0;
		follow[i].flength = 0;
	}
	//#加入S的follow集中 
	int length = follow[0].length;
	follow[0].array[length++] = '#';
	follow[0].length = length;
	int all = 0; 
	//第一次循环先求follow与其他产生式无关的follow
	for(int i=0;i<lenth2;i++){
		int isEnd = 0;
		int flenth = follow[i].flength;
		int falenth = follow[i].length;
		for(int k=0;k<sNum;k++){
			int lenth=s[k].length;
			for(int x=0;x<lenth;x++){
				if(follow[i].origin == s[k].array[x]){
					//这是产生式结尾的非终结符  A->BS 加入follows[]中 
					if(x == lenth-1){
						all++; 
						if(follow[i].origin != s[k].origin){
							follow[i].follows[flenth++] = s[k].origin;
							follow[i].flength = flenth; 
						}  
						break;
					}
					while(x < lenth-1){
						//S->BSC
						x++;
						if(isNoTerminal(s[k].array[x])){
							//C能推出$ 
							if(isNext(s[k].array[x])){
								int location = locat(s[k].array[x],v2,lenth2);
								int firstl = first[location].length;
								for(int m=0;m<firstl;m++){
									if(first[location].array[m] == '$')
										continue;
									int length1 = follow[i].length;
									int location1 = locat(first[location].array[m],follow[i].array,length1);
									if(location1 != -1){//等于-1表示它不在follow集合中 
										continue;
									}else{
										follow[i].array[falenth++] = first[location].array[m];
										follow[i].length = falenth;
									} 
								}
								if(x == lenth-1){
									all++; 
									if(follow[i].origin != s[k].origin){
										follow[i].follows[flenth++] = s[k].origin;
										follow[i].flength = flenth; 
									}  
									break;
								}
							}else{
								int location = locat(s[k].array[x],v2,lenth2);
								int firstl = first[location].length;
								for(int m=0;m<firstl;m++){
									int length1 = follow[i].length;
									int location1 = locat(first[location].array[m],follow[i].array,length1);
									if(location1 != -1){//等于-1表示它不在follow集合中 
										continue;
									}else{
										follow[i].array[falenth++] = first[location].array[m];
										follow[i].length = falenth;
									} 
								}
								break;
							}
						}else{
							int length1 = follow[i].length;
							int location1 = locat(s[k].array[x],follow[i].array,length1);
							if(location1 != -1){//等于-1表示它不在follow集合中 
								continue;
							}else{
								follow[i].array[falenth++] = s[k].array[x];
								follow[i].length = falenth;
							} 
							break;
						} 
					}
					break;
				}
			}
		}
	}
	//如果求出的follow都和其他的有关，就把s的有关follow去掉
	if(all-1 == lenth2){
		follow[0].flength = 0;
	}
	int isContinue = 0;
	do{
		isContinue = 0;
		for(int x=0;x<lenth2;x++){
			int flength = follow[x].flength;
			//follow集合和其他的follow有关时 
			while(flength > 0){
				flength--;
				//和谁有关 
				int location = locat(follow[x].follows[flength],v2,lenth2);
				int flength1 = follow[location].flength;
				int length1 = follow[location].length;
				//它是不是还和其他的有关 
				if(flength1 > 0){
					flength++;
					isContinue = 1;
					break;
				}else{
					for(int y=0;y<length1;y++){
						int length = follow[x].length;
						int location1 = locat(follow[location].array[y],follow[x].array,length);
						if(location1 != -1){//等于-1表示它不在follow集合中 
							continue;
						}else{
							follow[x].array[length++] = follow[location].array[y];
							follow[x].length = length;
						} 
					}
					follow[x].flength = flength;
				}
			}
		}
	}while(isContinue == 1);
} 
void predict(){
	for(int x=0;x<lenth2;x++){
		for(int y=0;y<lenth1;y++){
			C[x][y].isPredict = 0; 
		}
	}
	for(int x=0;x<sNum;x++){
		char array[10]={};
		int alength = 0; 
		//SELECT(A → α) = FIRST(α) 
		//SELECT(A → α) = ( FIRST(α) – {ε} ) ∪ FOLLOW(A)
		if(isTerminal(s[x].array[0])){
			if(s[x].array[0] == '$'){
				int location = locat(s[x].origin,v2,lenth2);
				int length = follow[location].length;
				for(int y=0;y<length;y++){
					array[alength++] = follow[location].array[y];
				}
			}else{
				array[alength++] = s[x].array[0];
			}
		}else{
			if(isNext(s[x].array[0])){
				int location = locat(s[x].array[0],v2,lenth2);//first
				int location1 = locat(s[x].origin,v2,lenth2);//follow
				int firstl = first[location].length;
				int followl = follow[location1].length;
				for(int y=0;y<firstl;y++){
					if(first[location].array[y] == '$')
						continue;
					array[alength++] = first[location].array[y];
				}
				for(int y=0;y<followl;y++){
					int location2 = locat(follow[location1].array[y],array,alength);
					if(location2 != -1)
						continue;
					array[alength++] = follow[location1].array[y];
				}
			}else{
				int location = locat(s[x].array[0],v2,lenth2);//first
				int firstl = first[location].length;
				for(int y=0;y<firstl;y++){
					array[alength++] = first[location].array[y];
				}
			}
		}
		//为预测分析表赋值 
		for(int y=0;y<alength;y++){
			int row = forRow(s[x].origin);
			int line = forLine(array[y]);
			C[row][line] = s[x];
			C[row][line].isPredict = 1;
		}
	}
	for(int x=0;x<lenth1;x++){
		printf("\t%c",v1[x+1]);
	}
	printf("\n");
	for(int x=0;x<lenth2;x++){
		printf("%c",v2[x]);
		for(int y=0;y<lenth1;y++){
			printf("\t");
			if(C[x][y].isPredict != 0){
				int pLength = C[x][y].length;
				for(int z=0;z<pLength;z++){
					printf("%c",C[x][y].array[z]);
				}
			}	
		}
		printf("\n");
	}
}
void test(){
	printf("first:\n");
	for(int m=0;m<lenth2;m++){
		printf("%c:",first[m].origin);
		int length = first[m].length;
		for(int n=0;n<length;n++){
			printf("%c ",first[m].array[n]);
		}
		printf("\n");
	}
	printf("follow:\n");
	for(int m=0;m<lenth2;m++){
		printf("%c:",follow[m].origin);
		int length = follow[m].length;
		for(int n=0;n<length;n++){
			printf("%c ",follow[m].array[n]);
		}
		printf("\n");
	}
} 
int main()  
{  
	input();
	forFirst();
	forFollow();
	predict();
	test();
	//flag终结符标识 finish结束符标识 
	int m, n, k = 1, flag = 0, finish = 0; 
	char ch, x; 
	type cha;//用来接受C[m][n]  
	//读入分析串
	do
	{
		scanf("%c", &ch); 
		if(!isTerminal(ch)){
			printf("输入串中有非法字符\n");
			exit(1); 
		}
		B[j] = ch; 
		j++; 
	}while (ch != '#');
	l = j;//分析串长度  
	ch = B[0];//当前分析字符  
	A[top] = '#'; 
	A[++top] = 'S';//'#','S'进栈  
	printf("步骤\t\t分析栈\t\t剩余字符\t\t所用产生式\n"); 
	do { 
		x = A[top--];//x为当前栈顶字符  
		printf("%d", k++); 
		printf("\t\t"); 
		//判断是否为终结符 
		for (j = 0; j < lenth1; j++)
			if (x == v1[j]){ 
				flag = 1; 
				break;  
			} 
		//如果是终结符  
		if (flag == 1){ 
			if (x == '#') { 
				finish = 1;//结束标记  
				printf("\t\t\t\t\t接受!\n");
				getchar(); 
				getchar(); 
				exit(1); 
			}
			if (x == ch){ 
				print(); 
				print1(); 
				printf("%c匹配\n", ch); 
				ch = B[++b];//下一个输入字符  
				flag = 0;//恢复标记  
			} 
			//出错处理
			else{ 
				print();  print1(); 
				printf("%c出错\n", ch);//输出出错终结符
				exit(1); 
			} 
		}else{//非终结符处理 
			//求行号 
			for (j = 0; j < lenth2; j++) 
				if (x == v2[j]) { 
					m = j; 
					break; 
				} 
			//求列号 
			for (j = 1; j < lenth1; j++) 
				if (ch == v1[j]){ 
					n = j-1;  
					break; 
				} 
			cha = C[m][n]; 
			//判断预测分析表是否为空  
			if (cha.origin != 'N'){  
				print();
				print1(); 
				//输出产生式 
				printf("%c->", cha.origin); 
				for (j = 0; j<cha.length; j++) 
					printf("%c", cha.array[j]);
				printf("\n");
				//产生式逆序入栈 
				for (j = (cha.length - 1); j >= 0; j--)
					A[++top] = cha.array[j];
				if (A[top] == '$')//为空则不进栈 
					top--; 
			}else { 		//出错处理
				print(); 
				print1(); 
				printf("%c出错\n", x);//输出出错非终结符  
				exit(1); 
			} 
		}
	}while (finish == 0);
}
