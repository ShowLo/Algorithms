#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

//�̺�����
class Implicant
{
public:
	char *binary_code;                       //����Ķ����Ʊ���
	int bitNumber;                           //������
	int numOf1;                              //�����Ʊ�����"1"�ĸ���
	int numOf_;                              //�����Ʊ�����"-"�ĸ�����Ϊ��������ʵ������"*"������"-"
	string output_result;                    //���յ�������
	Implicant();
	Implicant(int bitNum);
	bool combine_whether(Implicant &imp);    //�ж������Ƿ��ܷ�ϲ�
	Implicant result_of_combine(Implicant &imp);//�ϲ�����
	bool cover_whether(int num);             //�ж��ܷ񸲸Ǳ��Ϊnum����
	bool operator==(Implicant &imp);         //����==�����
	Implicant& operator =(Implicant &imp);   //����=�����
	friend ostream& operator <<(ostream&, Implicant&);//����"<<"�����
	void toString();                         //����������ӳ��Ϊ��С����ʽ
};

//���캯������
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
//�ж������Ƿ��ܽ��
bool Implicant::combine_whether(Implicant &imp)
{
	if (abs(numOf1 - imp.numOf1) != 1||numOf_!=imp.numOf_)
		return false;                        //"1"�ĸ����������1����"-"�ĸ�������
	int difference_count = 0;                //��ͬ�ĸ���
	for (int i = 0;i < bitNumber;++i)
	{
		//���ȿ���"-"��λ�ò�һ������������ų�
		if ((binary_code[i] == '*'&&imp.binary_code[i] != '*') || (binary_code[i] != '*'&&imp.binary_code[i] == '*'))
			return false;
		if (binary_code[i] != imp.binary_code[i])
			++difference_count;
	}
	return (difference_count == 1);          //ֻ��һλ��ͬ��ʱ�򷵻�true
}
//�ϲ�������غϲ����
Implicant Implicant::result_of_combine(Implicant &imp)
{
	Implicant result;
	result.binary_code = new char[bitNumber];
	result.bitNumber = bitNumber;
	for (int i = 0;i < bitNumber;++i)        //��ͬλ��������ͬλ��Ϊ"-"
	{
		if (binary_code[i] == imp.binary_code[i])
			result.binary_code[i] = binary_code[i];
		else
			result.binary_code[i] = '*';
	}
	if (imp.numOf1 > numOf1)                 //1�ĸ������ϲ�ǰ�����е�1������С����ͬ
		result.numOf1 = numOf1;
	else
		result.numOf1 = imp.numOf1;
	result.numOf_ = numOf_ + 1;              //"-"�ĸ�����1
	return result;
}
//�ж��ܷ񸲸Ǳ��Ϊnum����
bool Implicant::cover_whether(int num)
{
	char *be_covered = new char[bitNumber];
	for (int i = 0;i < bitNumber;++i)        //�����������
	{
		be_covered[bitNumber - i - 1] = int(num % 2) + 48;
		num = num / 2;
	}
	for (int i = 0;i < bitNumber;++i)        //�Ƚ��Ƿ��ܸ���
		if (be_covered[i] != binary_code[i] && binary_code[i] != '*')
			return false;
	return true;
}
//����==����������ڱȽ��������������Ƿ�һ��
bool Implicant::operator==(Implicant &imp)
{
	if (imp.bitNumber == 0 || bitNumber == 0)
		return false;
	for (int i = 0;i < bitNumber;++i)         //��λ�Ƚ�
		if (binary_code[i] != imp.binary_code[i])
			return false;
	return true;
}
//����=�����
Implicant& Implicant::operator =(Implicant &imp)
{
	for (int i = 0;i < imp.bitNumber;++i)
		binary_code[i] = imp.binary_code[i];
	bitNumber = imp.bitNumber;
	numOf1 = imp.numOf1;
	numOf_ = imp.numOf_;
	return *this;
}
//����"<<"�����
ostream& operator <<(ostream& output, Implicant& imp)
{
	output << imp.output_result;
	return output;
}
//����toString
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

//���������
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

//�ϲ�����������һ��ϲ�
int combine(Implicant *&imp, int total_num)
{
	int room_needed = total_num*(total_num + 1) / 2;  //�����������ռ�
	Implicant *next_imp = new Implicant[room_needed]; //��ŵõ��ĺϲ�����ܺϲ��ı�ԭ�̺���
	bool *cover_whether = new bool[total_num];        //��¼�Ƿ񱻸���
	for (int i = 0;i < total_num;++i)
		cover_whether[i] = false;
	int bitNumber = imp[0].bitNumber;
	for (int i = 0;i < room_needed;++i)               //��ʼ������
	{
		next_imp[i].binary_code = new char[bitNumber];
		next_imp[i].bitNumber = bitNumber;
	}
	int count = 0;                                    //��¼next_imp�ĳ���
	for (int i = 0;i < total_num - 1;++i)
	{
		for (int j = i + 1;j < total_num;++j)
			if (imp[i].combine_whether(imp[j]))       //������Ժϲ������ϲ��������next_imp��
			{
				next_imp[count] = imp[i].result_of_combine(imp[j]);
				++count;
				cover_whether[i] = true;              //�úϲ���־
				cover_whether[j] = true;
			}
	}
	for (int i = 0;i < count - 1;++i)
	{
		for (int j = i + 1;j < count;++j)
			if (next_imp[i] == next_imp[j])           //�ϲ���һ����ɾȥ��һ���������һ��ǰ�ƣ����ȼ�һ
			{
				--count;
				next_imp[j] = next_imp[count];
				next_imp[count].bitNumber = 0;
				--j;
			}
	}
	for (int i = 0;i < total_num;++i)
	{
		if (!cover_whether[i])                        //δ�����ǵļ���next_imp
			next_imp[count++] = imp[i];
	}
	delete[] cover_whether;
	delete[] imp;
	imp = next_imp;                                   //��next_imp��������Ĳ���
	next_imp = NULL;
	return count;
}

//�������б�ԭ�̺��������������
int prime_implicant(Implicant *&imp, int Nm, int Nd,int times)
{
	int total_num = Nm + Nd;
	for (int i = 0;i < times;++i)
		total_num = combine(imp, total_num);
	return total_num;
}

//����������
int compare(const void *p1, const void *p2)
{
	Implicant *a = (Implicant*)p1;
	Implicant *b = (Implicant*)p2;
	return (strcmp(b->binary_code,a->binary_code));//��A,A'��A�ķ�ʽ����
}

int main()
{
	int Nq;                                 //��ѯ��Ŀ
	int Nm, Nd;                             //��С��޹�����Ŀ
	int maxNum = 0;                         //�����

	vector<int> minterm;                    //��С��
	vector<int> dnterm;                     //�޹���

	minterm.resize(1024);
	dnterm.resize(1024);

	cin >> Nq;
	for (int times = 0;times < Nq;++times)
	{
		//ÿ�ο�ʼ��ѯ֮ǰ����ջ���
		minterm.clear();
		dnterm.clear();

		cin >> Nm >> Nd;
		minterm.resize(Nm);                 //���·���ռ�
		dnterm.resize(Nd);

		for (int i = 0;i < Nm;++i)
		{
			cin >> minterm[i];
			if (minterm[i] > maxNum)        //��¼�����
				maxNum = minterm[i];
		}
		for (int i = 0;i < Nd;++i)
		{
			cin >> dnterm[i];
			if (dnterm[i] > maxNum)         //��¼�����
				maxNum = dnterm[i];
		}

		//�߽����
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
		int bitNumber = bitNum(maxNum);     //���������
		for (int i = 0;i < Nm;++i)          //������С��Ķ�������
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
					++implicant[i].numOf1;  //�ۼ�1�ĸ���
				num = num / 2;
			}
		}
		for (int i = Nm;i < Nm + Nd;++i)    //�����޹���Ķ�������
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
					++implicant[i].numOf1;  //�ۼ�1�ĸ���
				num = num / 2;
			}
		}

		int *cover_times = new int[Nm]();   //��¼ÿ����С����Ǵ�������ʼ��Ϊ0
		int *cover_term = new int[Nm]();    //��¼ÿ����С������һ����ԭ�̺������
		bool *cover_whether = new bool[Nm]; //��¼ÿ����С���Ƿ��ѱ�����
		for (int i = 0;i < Nm;++i)
			cover_whether[i] = false;
		int count_rest = Nm;                //��¼��δ�����ǵ���С������
		int count_ess_prime = 0;            //��¼���ʱ�ԭ�̺�������
		int count_prime_imp = 0;            //��¼��ԭ�̺�������
		//�������ձ�ԭ�̺���ĸ���
		int num = prime_implicant(implicant, Nm, Nd, bitNumber);
		//�������ʱ�ԭ�̺���
		for (int i = 0;i < num;++i)
		{
			for (int j = 0;j < Nm;++j)
				if (implicant[i].cover_whether(minterm[j]))
				{
					++cover_times[j];       //ÿ�ҵ�һ���ܱ����ǵ���С���򸲸Ǵ�����һ
					cover_term[j] = i;      //���Ҽ�¼�����������С��ı�ԭ�̺���
				}
		}
		for (int i = 0;i < Nm;++i)
		{
			if (cover_times[i] == 1)        //ÿ�ҵ�һ�����ʱ�ԭ�̺��������Ԥ��׼���õĶ���������
			{
				essential_prime[count_ess_prime] = implicant[cover_term[i]];
				++count_ess_prime;          //���ʱ�ԭ�̺���������һ
			}
		}
		//ɾ���ظ��ı��ʱ�ԭ�̺���
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
		//��Ǳ��ʱ�Դ�̺���ǵ�����С��
		for (int i = 0;i < count_ess_prime;++i)
		{
			for (int j = 0;j < Nm;++j)
				if (essential_prime[i].cover_whether(minterm[j]) && cover_whether[j] == false)
				{
					cover_whether[j] = true;//�ø��Ǳ�־
					--count_rest;           //δ����������һ
				}
		}

		int maxNum_cover_minterm = 0;       //��¼�Ǳ��ʱ�ԭ�̺�����󸲸�����
		int *num_cover_term = new int[num]; //��¼�Ǳ��ʱ�ԭ�̺������̺�����С������
		for (;count_rest > 0;)
		{
			maxNum_cover_minterm = 0;
			for (int i = 0;i < num;++i)
			{
				num_cover_term[i] = 0;
				for (int j = 0;j < Nm;++j)  //�ų������ʱ�ԭ�̺���ǵ���С��
				{
					if (implicant[i].cover_whether(minterm[j]) && cover_whether[j] == false)
						++num_cover_term[i];//�Ǳ��ʱ�ԭ�̺���ǵ������ۼ�1
				}
				if (num_cover_term[i] > maxNum_cover_minterm)
					maxNum_cover_minterm = num_cover_term[i]; //����
			}
			for (int i = 0;i < num;++i)
			{
				if (num_cover_term[i] == maxNum_cover_minterm)//�ҳ����������ķǱ��ʱ�ԭ�̺���
				{
					essential_prime_implicant[count_prime_imp] = implicant[i];//��¼��������1
					++count_prime_imp;
					for (int j = 0;j < Nm;++j)
					{
						if (implicant[i].cover_whether(minterm[j]) && cover_whether[j] == false)
						{
							cover_whether[j] = true;          //������Ǳ��ʱ�ԭ�̺���ǵ���С��ø��Ǳ�־
							--count_rest;                     //δ���ǵ���С��������һ
						}
					}
					break;
				}
			}
		}
		//ɾ���ظ��ķǱ��ʱ�ԭ�̺���
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
		//����
		qsort(essential_prime, count_ess_prime, sizeof(essential_prime[0]), compare);
		qsort(essential_prime_implicant, count_prime_imp, sizeof(essential_prime_implicant[0]), compare);

		//���
		if (times == 0)
			cout << "�¼��� 2014011050" << endl;

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