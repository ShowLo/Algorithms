#include <iostream>
using namespace std;
//预处理函数
void Next(char *Pattern, int next[])
{
	int length = strlen(Pattern);             //模式串长度
	int i = 1, j = 0;
	next[0] = 0;                              //位置0处的部分匹配串长度为0
	for (; i < length;)
	{
		if (Pattern[i] == Pattern[j])         //已匹配j+1个字符
		{
			next[i] = j + 1;
			++i;
			++j;
		}
		else if (j > 0)                       //移动到更小的部分匹配串上去
			j = next[j - 1];
		else
			next[i++] = 0;                    //j=0
	}
}
//KMP算法实现函数
int KMP_Match(char *Target, char *Pattern)
{
	int Target_length = strlen(Target);       //目标串长度
	int Pattern_length = strlen(Pattern);
	int i = 0, j = 0;
	int *next=new int[Pattern_length];
	Next(Pattern, next);                      //作预处理
	for (; i < Target_length;)
	{
		if (Target[i] == Pattern[j])          //已匹配j+1个字符
		{
			if (j == Pattern_length - 1)
			{
				delete[] next;
				return (i - j);               //全部匹配成功
			}
			else
			{
				++i;
				++j;
			}
		}
		else if (j > 0)
			j = next[j - 1];                  //失配，移动到部分匹配串
		else
			++i;
	}
	delete[] next;
	return -1;                                //匹配失败
}
//测试函数
int main()
{
	char *T = "abacaabaccabacabaabb";
	char *P = "abacab";
	int result = KMP_Match(T, P);
	if (result == -1)
		cout << "没有匹配的位置！" << endl;
	else
		cout << "匹配的位置为：" << result + 1 << endl;
	return 0;
}