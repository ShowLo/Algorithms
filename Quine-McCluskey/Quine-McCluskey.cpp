#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

//蕴含项类
class Implicant
{
public:
	char *binary_code;                       //各项的二进制编码
	int bitNumber;                           //变量数
	int numOf1;                              //二进制编码中"1"的个数
	int numOf_;                              //二进制编码中"-"的个数，为方便排序，实际上用"*"代替了"-"
	string output_result;                    //最终的输出结果
	Implicant();
	Implicant(int bitNum);
	bool combine_whether(Implicant &imp);    //判断两项是否能否合并
	Implicant result_of_combine(Implicant &imp);//合并两项
	bool cover_whether(int num);             //判断能否覆盖编号为num的项
	bool operator==(Implicant &imp);         //重载==运算符
	Implicant& operator =(Implicant &imp);   //重载=运算符
	friend ostream& operator <<(ostream&, Implicant&);//重载"<<"运算符
	void toString();                         //将二进制码映射为最小项表达式
};

//构造函数定义
Implicant::Implicant()
{
	binary_code = NULL;
	bitNumber = 0;
	numOf1 = 0;
	numOf_ = 0;
	output_result = "";
}
Implicant::Implicant(int bitNum):bitNumber(bitNum)
{
	binary_code = new char[bitNum];
	numOf1 = 0;
	numOf_ = 0;
}
//判断两项是否能结合
bool Implicant::combine_whether(Implicant &imp)
{
	if (abs(numOf1 - imp.numOf1) != 1||numOf_!=imp.numOf_)
		return false;                        //"1"的个数不是相差1或者"-"的个数不等
	int difference_count = 0;                //不同的个数
	for (int i = 0;i < bitNumber;++i)
	{
		//首先考虑"-"的位置不一样的情况，先排除
		if ((binary_code[i] == '*'&&imp.binary_code[i] != '*') || (binary_code[i] != '*'&&imp.binary_code[i] == '*'))
			return false;
		if (binary_code[i] != imp.binary_code[i])
			++difference_count;
	}
	return (difference_count == 1);          //只有一位不同的时候返回true
}
//合并两项，返回合并结果
Implicant Implicant::result_of_combine(Implicant &imp)
{
	Implicant result;
	result.binary_code = new char[bitNumber];
	result.bitNumber = bitNumber;
	for (int i = 0;i < bitNumber;++i)        //相同位保留，不同位换为"-"
	{
		if (binary_code[i] == imp.binary_code[i])
			result.binary_code[i] = binary_code[i];
		else
			result.binary_code[i] = '*';
	}
	if (imp.numOf1 > numOf1)                 //1的个数跟合并前两项中的1个数较小者相同
		result.numOf1 = numOf1;
	else
		result.numOf1 = imp.numOf1;
	result.numOf_ = numOf_ + 1;              //"-"的个数加1
	return result;
}
//判断能否覆盖编号为num的项
bool Implicant::cover_whether(int num)
{
	char *be_covered = new char[bitNumber];
	for (int i = 0;i < bitNumber;++i)        //计算二进制码
	{
		be_covered[bitNumber - i - 1] = int(num % 2) + 48;
		num = num / 2;
	}
	for (int i = 0;i < bitNumber;++i)        //比较是否能覆盖
		if (be_covered[i] != binary_code[i] && binary_code[i] != '*')
			return false;
	return true;
}
//重载==运算符，用于比较两个二进制码是否一样
bool Implicant::operator==(Implicant &imp)
{
	if (imp.bitNumber == 0 || bitNumber == 0)
		return false;
	for (int i = 0;i < bitNumber;++i)         //逐位比较
		if (binary_code[i] != imp.binary_code[i])
			return false;
	return true;
}
//重载=运算符
Implicant& Implicant::operator =(Implicant &imp)
{
	for (int i = 0;i < imp.bitNumber;++i)
		binary_code[i] = imp.binary_code[i];
	bitNumber = imp.bitNumber;
	numOf1 = imp.numOf1;
	numOf_ = imp.numOf_;
	return *this;
}
//重载"<<"运算符
ostream& operator <<(ostream& output, Implicant& imp)
{
	output << imp.output_result;
	return output;
}
//定义toString
void Implicant::toString()
{
	for (int i = 0;i < bitNumber;++i)
	{
		if (binary_code[i] != '*')
		{
			char out = i + 'A';
			output_result += out;
			if (binary_code[i] == '0')
				output_result += "'";
		}
	}
}

//计算变量数
int bitNum(int maxNum)
{
	int bitNum = 0;
	for (;maxNum != 0;)
	{
		maxNum = maxNum / 2;
		++bitNum;
	}
	if (bitNum == 0)
		return 1;
	else
		return bitNum;
}

//合并函数，进行一组合并
int combine(Implicant *&imp, int total_num)
{
	int room_needed = total_num*(total_num + 1) / 2;  //计算所需最大空间
	Implicant *next_imp = new Implicant[room_needed]; //存放得到的合并项及不能合并的本原蕴涵项
	bool *cover_whether = new bool[total_num];        //记录是否被覆盖
	for (int i = 0;i < total_num;++i)
		cover_whether[i] = false;
	int bitNumber = imp[0].bitNumber;
	for (int i = 0;i < room_needed;++i)               //初始化操作
	{
		next_imp[i].binary_code = new char[bitNumber];
		next_imp[i].bitNumber = bitNumber;
	}
	int count = 0;                                    //记录next_imp的长度
	for (int i = 0;i < total_num - 1;++i)
	{
		for (int j = i + 1;j < total_num;++j)
			if (imp[i].combine_whether(imp[j]))       //如果可以合并，将合并结果存至next_imp中
			{
				next_imp[count] = imp[i].result_of_combine(imp[j]);
				++count;
				cover_whether[i] = true;              //置合并标志
				cover_whether[j] = true;
			}
	}
	for (int i = 0;i < count - 1;++i)
	{
		for (int j = i + 1;j < count;++j)
			if (next_imp[i] == next_imp[j])           //合并项一样，删去后一个并将最后一个前移，长度减一
			{
				--count;
				next_imp[j] = next_imp[count];
				next_imp[count].bitNumber = 0;
				--j;
			}
	}
	for (int i = 0;i < total_num;++i)
	{
		if (!cover_whether[i])                        //未被覆盖的加入next_imp
			next_imp[count++] = imp[i];
	}
	delete[] cover_whether;
	delete[] imp;
	imp = next_imp;                                   //将next_imp传给传入的参数
	next_imp = NULL;
	return count;
}

//搜索所有本原蕴含项，返回最终项数
int prime_implicant(Implicant *&imp, int Nm, int Nd,int times)
{
	int total_num = Nm + Nd;
	for (int i = 0;i < times;++i)
		total_num = combine(imp, total_num);
	return total_num;
}

//排序辅助函数
int compare(const void *p1, const void *p2)
{
	Implicant *a = (Implicant*)p1;
	Implicant *b = (Implicant*)p2;
	return (strcmp(b->binary_code,a->binary_code));//按A,A'无A的方式排序
}

int main()
{
	int Nq;                                 //查询数目
	int Nm, Nd;                             //最小项及无关项数目
	int maxNum = 0;                         //最大编号

	vector<int> minterm;                    //最小项
	vector<int> dnterm;                     //无关项

	minterm.resize(1024);
	dnterm.resize(1024);

	cin >> Nq;
	for (int times = 0;times < Nq;++times)
	{
		//每次开始查询之前先清空缓存
		minterm.clear();
		dnterm.clear();

		cin >> Nm >> Nd;
		minterm.resize(Nm);                 //重新分配空间
		dnterm.resize(Nd);

		for (int i = 0;i < Nm;++i)
		{
			cin >> minterm[i];
			if (minterm[i] > maxNum)        //记录最大编号
				maxNum = minterm[i];
		}
		for (int i = 0;i < Nd;++i)
		{
			cin >> dnterm[i];
			if (dnterm[i] > maxNum)         //记录最大编号
				maxNum = dnterm[i];
		}

		//边界情况
		if (Nm == 0 && Nd == 0)
		{
			cout << 0 << endl;
			continue;
		}
		if (Nm == 0 && Nd == 1)
		{
			cout << 0 << endl;
			continue;
		}
		if (Nm == 1 && Nd == 0)
		{
			cout << 1 << endl;
			continue;
		}

		Implicant *implicant = new Implicant[Nm + Nd];
		Implicant *essential_prime = new Implicant[Nm + Nd];
		Implicant *essential_prime_implicant = new Implicant[Nm + Nd];
		int bitNumber = bitNum(maxNum);     //计算变量数
		for (int i = 0;i < Nm;++i)          //计算最小项的二进制码
		{
			int num = minterm[i];
			implicant[i].binary_code = new char[bitNumber];
			essential_prime[i].binary_code = new char[bitNumber];
			essential_prime_implicant[i].binary_code = new char[bitNumber];
			implicant[i].bitNumber = bitNumber;
			for (int j = 0;j < bitNumber;++j)
			{
				implicant[i].binary_code[bitNumber - j - 1] = int(num % 2) + 48;
				if (num % 2 == 1)
					++implicant[i].numOf1;  //累计1的个数
				num = num / 2;
			}
		}
		for (int i = Nm;i < Nm + Nd;++i)    //计算无关项的二进制码
		{
			int num = dnterm[i - Nm];
			implicant[i].binary_code = new char[bitNumber];
			essential_prime[i].binary_code = new char[bitNumber];
			essential_prime_implicant[i].binary_code = new char[bitNumber];
			implicant[i].bitNumber = bitNumber;
			for (int j = 0;j < bitNumber;++j)
			{
				implicant[i].binary_code[bitNumber - j - 1] = int(num % 2) + 48;
				if (num % 2 == 1)
					++implicant[i].numOf1;  //累计1的个数
				num = num / 2;
			}
		}

		int *cover_times = new int[Nm]();   //记录每个最小项被覆盖次数，初始化为0
		int *cover_term = new int[Nm]();    //记录每个最小项的最后一个本原蕴含项序号
		bool *cover_whether = new bool[Nm]; //记录每个最小项是否已被覆盖
		for (int i = 0;i < Nm;++i)
			cover_whether[i] = false;
		int count_rest = Nm;                //记录尚未被覆盖的最小项项数
		int count_ess_prime = 0;            //记录本质本原蕴含项项数
		int count_prime_imp = 0;            //记录本原蕴含项项数
		//先求最终本原蕴含项的个数
		int num = prime_implicant(implicant, Nm, Nd, bitNumber);
		//搜索本质本原蕴含项
		for (int i = 0;i < num;++i)
		{
			for (int j = 0;j < Nm;++j)
				if (implicant[i].cover_whether(minterm[j]))
				{
					++cover_times[j];       //每找到一个能被覆盖的最小项则覆盖次数加一
					cover_term[j] = i;      //并且记录覆盖了这个最小项的本原蕴含项
				}
		}
		for (int i = 0;i < Nm;++i)
		{
			if (cover_times[i] == 1)        //每找到一个本质本原蕴含项，将其存进预先准备好的对象容器里
			{
				essential_prime[count_ess_prime] = implicant[cover_term[i]];
				++count_ess_prime;          //本质本原蕴含项项数加一
			}
		}
		//删除重复的本质本原蕴含项
		for (int i = 0;i < count_ess_prime - 1;++i)
		{
			for (int j = i + 1;j < count_ess_prime;++j)
			{
				if (essential_prime[i] == essential_prime[j])
				{
					--count_ess_prime;
					essential_prime[j] = essential_prime[count_ess_prime];
					essential_prime[count_ess_prime].bitNumber = 0;
					--j;
				}
			}
		}
		//标记本质本源蕴含项覆盖到的最小项
		for (int i = 0;i < count_ess_prime;++i)
		{
			for (int j = 0;j < Nm;++j)
				if (essential_prime[i].cover_whether(minterm[j]) && cover_whether[j] == false)
				{
					cover_whether[j] = true;//置覆盖标志
					--count_rest;           //未覆盖项数减一
				}
		}

		int maxNum_cover_minterm = 0;       //记录非本质本原蕴含项最大覆盖项数
		int *num_cover_term = new int[num]; //记录非本质本原蕴含项所蕴含的最小项项数
		for (;count_rest > 0;)
		{
			maxNum_cover_minterm = 0;
			for (int i = 0;i < num;++i)
			{
				num_cover_term[i] = 0;
				for (int j = 0;j < Nm;++j)  //排除被本质本原蕴含项覆盖的最小项
				{
					if (implicant[i].cover_whether(minterm[j]) && cover_whether[j] == false)
						++num_cover_term[i];//非本质本原蕴含项覆盖的项数累加1
				}
				if (num_cover_term[i] > maxNum_cover_minterm)
					maxNum_cover_minterm = num_cover_term[i]; //更新
			}
			for (int i = 0;i < num;++i)
			{
				if (num_cover_term[i] == maxNum_cover_minterm)//找出覆盖最多项的非本质本原蕴含项
				{
					essential_prime_implicant[count_prime_imp] = implicant[i];//记录，项数加1
					++count_prime_imp;
					for (int j = 0;j < Nm;++j)
					{
						if (implicant[i].cover_whether(minterm[j]) && cover_whether[j] == false)
						{
							cover_whether[j] = true;          //被这个非本质本原蕴含项覆盖的最小项都置覆盖标志
							--count_rest;                     //未覆盖的最小项项数减一
						}
					}
					break;
				}
			}
		}
		//删除重复的非本质本原蕴含项
		for (int i = 0;i < count_prime_imp - 1;++i)
		{
			for (int j = i + 1;j < count_prime_imp;++j)
				if (essential_prime_implicant[i] == essential_prime_implicant[j])
				{
					--count_prime_imp;
					essential_prime_implicant[j] = essential_prime_implicant[count_prime_imp];
					essential_prime_implicant[count_prime_imp].bitNumber = 0;
					--j;
				}
		}
		//排序
		qsort(essential_prime, count_ess_prime, sizeof(essential_prime[0]), compare);
		qsort(essential_prime_implicant, count_prime_imp, sizeof(essential_prime_implicant[0]), compare);

		//输出
		if (times == 0)
			cout << "陈佳榕 2014011050" << endl;

		for (int i = 0;i < count_ess_prime;++i)
			essential_prime[i].toString();
		for (int i = 0;i < count_prime_imp;++i)
			essential_prime_implicant[i].toString();

		if (essential_prime == 0 && essential_prime_implicant == 0)
		{
			cout << 0 << endl;
			continue;
		}
		for (int i = 0;i < count_ess_prime - 1;++i)
			cout << essential_prime[i] << "+";
		if (count_ess_prime != 0 && count_prime_imp == 0)
			cout << essential_prime[count_ess_prime - 1];
		else
			if (count_ess_prime != 0 && count_prime_imp != 0)
				cout << essential_prime[count_ess_prime - 1] << "+";

		for (int i = 0;i < count_prime_imp - 1;++i)
			cout << essential_prime_implicant[i] << "+";
		if (count_prime_imp != 0)
			cout << essential_prime_implicant[count_prime_imp - 1];
		cout << endl;

		delete[] implicant;
		delete[] essential_prime;
		delete[] essential_prime_implicant;
	}
}