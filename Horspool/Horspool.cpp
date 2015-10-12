#include <iostream>
using namespace std;
//求较小者函数
int min(int a, int b)
{
	return ((a < b) ? a : b);
}
//预处理函数
#define NUM 128
void Last_Occurrence(char *Pattern,int *L)
{
	int length_P = strlen(Pattern);              //模式串长度
	for (int i = 0; i < NUM; i++)
		L[i] = -1;                               //初始化
	for (int i = 0; i < length_P; i++)
		L[(int)Pattern[i]] = i;                  //预处理
}
//Boyer-Moore算法的简化算法--Horspool算法的实现函数
int Horspool(char *Target,char *Pattern)
{
	int length_T = strlen(Target);               //目标串长度
	int length_P = strlen(Pattern);
	int L[NUM];
	Last_Occurrence(Pattern, L);
	int i = length_P - 1;                        //初始化匹配位置
	int j = i;
	int k;
	for (; i < length_T;)
	{
		if (Target[i] == Pattern[j])
		{
			if (j == 0)
				return i;                        //匹配成功
			else                                 //比较前一个字符
			{
				--i;
				--j;
			}
		}
		else
		{
			k = L[(int)Target[i]];               //Last_Occurrence值
			i += length_P - min(j, k + 1);       //下一步比较位置
		}
	}
	return -1;                                   //匹配失败
}
//测试函数
int main()
{
	char *T = "a pattern matching algorithm";
	char *P = "rithm";
	int result = Horspool(T, P);
	if (result == -1)
		cout << "匹配失败！" << endl;
	else
		cout << "匹配位置为:" << result + 1 << endl;
	return 0;
}