#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>
char A[20];//����ջ 
char B[20];//ʣ�മ 
char v1[10] = {'$','#'};//�ս��
char v2[10] = {};//���ս��
int lenth1=2,lenth2=0;//�ս���ͷ��ս�����鳤�� 
int j = 0, b = 0, top = 0, l;//LΪ���봮����
typedef struct type//����ʽ�ṹ�� �� first follow���ṹ�� 
{ 
	char origin;//��д�ַ�
	char array[5];//����ʽ�ұ��ַ� 
	int length;//����ʽ�ұ��ַ����� �������ʽ������ջ 
	char follows[5];//����Щfollow�����й� 
	int flength;
	int isPredict;
}type; 
type s[20];//����ʽ���ʽ 
type first[10],follow[10];//first follow�� 
int sNum=0,array=0;//sNum��¼һ�����ٲ���ʽ��array��¼����ʽ�ұ��ַ����� 
type C[10][10];//Ԥ�������
void print()//�������ջ
{  
	int a;//ָ�� 
	for (a = 0; a <= top + 1; a++)
		printf("%c", A[a]);
	printf("\t\t");
} 
void print1()//���ʣ�മ 
{  
	int j; 
	for (j = 0; j<b; j++)
		printf(" "); 
	for (j = b; j <= l; j++) 
		printf("%c", B[j]); 
	printf("\t\t\t");  
} 
//�ж��Ƿ�Ϊ�Ѵ��ڷ��ս�� 
bool isNoTerminal(char ch){
	for (int j = 0; j < lenth2; j++)
		if (ch == v2[j]){ 
			return true;
		} 
	return false;
}
//�ж��Ƿ�Ϊ�Ѵ����ս�� 
bool isTerminal(char ch){
	for (int j = 0; j < lenth1; j++)
		if (ch == v1[j]){ 
			return true;
		} 
	return false;
}
//���к� ���ս�� 
int forRow(char in){
	for(int i=0;i<lenth2;i++){
		if(in == v2[i])
			return i;
	}
}
//���к� ������$ 
int forLine(char in){
	for(int i=0;i<lenth1;i++){
		if(in == v1[i])
			return i-1;
	}
} 
//��Ŀ���ַ����ĸ��ַ��������ŵڼ��� 
int locat(char in,char s[],int length){
	for(int i=0;i<length;i++){
		if(in == s[i])
			return i;
	}
	return -1;//���߲��� 
} 
//�ж�first�������Ƿ��п�
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
//���뾭����֤���� 
void input(){
	char in;  
	int right = 0;//�ж��Ƿ��ǲ���ʽ�Ҳ� 
	int newFlag = 1;// �ж��Ƿ����µĲ���ʽ 
	do
	{
		scanf("%c", &in);
		//����ʽ���origin��ֵ 
		if(newFlag == 1){
			s[sNum].origin = in;
		}
		newFlag = 0;
		if(in <= 'Z' && in >= 'A'){
			//�ж��ǲ����µķ��ս�����������v2 
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
				right = 1;//>Ϊ���ҷָ� 
				continue;
			}else if(in == ' '){
				s[sNum].length = array;
				sNum++;//�ո�ָ�����µĲ���ʽ 
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
			//���ս����$��ֵ 
			s[sNum].array[array++] = in;
		}
	}while (in != '\n');
}
//��first���� ������֤���� 
void forFirst(){
	int isContinue = 0;
	//��ʼ��first���� 
	for(int i=0;i<lenth2;i++){
		first[i].origin = v2[i]; 
		first[i].length = 0;
	}
	//��һ��ѭ������first����������ʽ�޹ص�first 
	for(int i=0;i<lenth2;i++){
		int firstNum = 0;
		for(int k=0;k<sNum;k++){
			//�������ʽ�Ҳ���һ��Ϊ���ս�� �������ѭ�� 
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
	//Ϊ�˱���ǰ���first��Ҫ����û���������first����Ϊ�յ����
	//����first(A)=first(A) first(A)=first(B) 
	do{
		isContinue = 0;
		//�ڶ��ξͿ��԰����Ƕ��������
		for(int i=0;i<lenth2;i++){
			int firstNum = 0;
			int firstHaveNull = 0;//first�����в����� 
			for(int k=0;k<sNum;k++){
				//��������first���ϳ���>0,˵���ǵ�һ����� ���� 
				int forFirst = 0;
				if(first[i].length > 0)
					break;
				if(first[i].origin == s[k].origin && isTerminal(s[k].array[0])){
					first[i].array[firstNum++] = s[k].array[0];
					first[i].length = firstNum;
					continue;
				}else if(first[i].origin == s[k].origin && isNoTerminal(s[k].array[0])){
					do{
						int haveNull = 0;//first�����в����� 
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
					//������ʽ�Ҳ�Ϊ���ս��ʱһֱѭ�� 
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
//��follow���� ������֤���� 
void forFollow(){
	//��ʼ��follow���� 
	for(int i=0;i<lenth2;i++){
		follow[i].origin = v2[i]; 
		follow[i].length = 0;
		follow[i].flength = 0;
	}
	//#����S��follow���� 
	int length = follow[0].length;
	follow[0].array[length++] = '#';
	follow[0].length = length;
	int all = 0; 
	//��һ��ѭ������follow����������ʽ�޹ص�follow
	for(int i=0;i<lenth2;i++){
		int isEnd = 0;
		int flenth = follow[i].flength;
		int falenth = follow[i].length;
		for(int k=0;k<sNum;k++){
			int lenth=s[k].length;
			for(int x=0;x<lenth;x++){
				if(follow[i].origin == s[k].array[x]){
					//���ǲ���ʽ��β�ķ��ս��  A->BS ����follows[]�� 
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
							//C���Ƴ�$ 
							if(isNext(s[k].array[x])){
								int location = locat(s[k].array[x],v2,lenth2);
								int firstl = first[location].length;
								for(int m=0;m<firstl;m++){
									if(first[location].array[m] == '$')
										continue;
									int length1 = follow[i].length;
									int location1 = locat(first[location].array[m],follow[i].array,length1);
									if(location1 != -1){//����-1��ʾ������follow������ 
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
									if(location1 != -1){//����-1��ʾ������follow������ 
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
							if(location1 != -1){//����-1��ʾ������follow������ 
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
	//��������follow�����������йأ��Ͱ�s���й�followȥ��
	if(all-1 == lenth2){
		follow[0].flength = 0;
	}
	int isContinue = 0;
	do{
		isContinue = 0;
		for(int x=0;x<lenth2;x++){
			int flength = follow[x].flength;
			//follow���Ϻ�������follow�й�ʱ 
			while(flength > 0){
				flength--;
				//��˭�й� 
				int location = locat(follow[x].follows[flength],v2,lenth2);
				int flength1 = follow[location].flength;
				int length1 = follow[location].length;
				//���ǲ��ǻ����������й� 
				if(flength1 > 0){
					flength++;
					isContinue = 1;
					break;
				}else{
					for(int y=0;y<length1;y++){
						int length = follow[x].length;
						int location1 = locat(follow[location].array[y],follow[x].array,length);
						if(location1 != -1){//����-1��ʾ������follow������ 
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
		//SELECT(A �� ��) = FIRST(��) 
		//SELECT(A �� ��) = ( FIRST(��) �C {��} ) �� FOLLOW(A)
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
		//ΪԤ�������ֵ 
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
	//flag�ս����ʶ finish��������ʶ 
	int m, n, k = 1, flag = 0, finish = 0; 
	char ch, x; 
	type cha;//��������C[m][n]  
	//���������
	do
	{
		scanf("%c", &ch); 
		if(!isTerminal(ch)){
			printf("���봮���зǷ��ַ�\n");
			exit(1); 
		}
		B[j] = ch; 
		j++; 
	}while (ch != '#');
	l = j;//����������  
	ch = B[0];//��ǰ�����ַ�  
	A[top] = '#'; 
	A[++top] = 'S';//'#','S'��ջ  
	printf("����\t\t����ջ\t\tʣ���ַ�\t\t���ò���ʽ\n"); 
	do { 
		x = A[top--];//xΪ��ǰջ���ַ�  
		printf("%d", k++); 
		printf("\t\t"); 
		//�ж��Ƿ�Ϊ�ս�� 
		for (j = 0; j < lenth1; j++)
			if (x == v1[j]){ 
				flag = 1; 
				break;  
			} 
		//������ս��  
		if (flag == 1){ 
			if (x == '#') { 
				finish = 1;//�������  
				printf("\t\t\t\t\t����!\n");
				getchar(); 
				getchar(); 
				exit(1); 
			}
			if (x == ch){ 
				print(); 
				print1(); 
				printf("%cƥ��\n", ch); 
				ch = B[++b];//��һ�������ַ�  
				flag = 0;//�ָ����  
			} 
			//������
			else{ 
				print();  print1(); 
				printf("%c����\n", ch);//��������ս��
				exit(1); 
			} 
		}else{//���ս������ 
			//���к� 
			for (j = 0; j < lenth2; j++) 
				if (x == v2[j]) { 
					m = j; 
					break; 
				} 
			//���к� 
			for (j = 1; j < lenth1; j++) 
				if (ch == v1[j]){ 
					n = j-1;  
					break; 
				} 
			cha = C[m][n]; 
			//�ж�Ԥ��������Ƿ�Ϊ��  
			if (cha.origin != 'N'){  
				print();
				print1(); 
				//�������ʽ 
				printf("%c->", cha.origin); 
				for (j = 0; j<cha.length; j++) 
					printf("%c", cha.array[j]);
				printf("\n");
				//����ʽ������ջ 
				for (j = (cha.length - 1); j >= 0; j--)
					A[++top] = cha.array[j];
				if (A[top] == '$')//Ϊ���򲻽�ջ 
					top--; 
			}else { 		//������
				print(); 
				print1(); 
				printf("%c����\n", x);//���������ս��  
				exit(1); 
			} 
		}
	}while (finish == 0);
}
