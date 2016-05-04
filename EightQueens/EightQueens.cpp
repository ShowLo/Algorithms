#include <iostream>
using namespace std;

int Site[8];                                         //放置某一行的皇后所处的列数

//判断在row+1行放置第row+1个皇后时是否会发生冲突
bool Valid(int row)
{
	for (int i = 0;i < row;++i)
	{
		if (Site[i] == Site[row])                    //与第row+1行的皇后处于同一列     
			return false;
		if (abs(Site[i] - Site[row]) == (row - i))   //与第row+1行的皇后处于同一对角线
			return false;
	}
	return true;
}

//递归放置第row+1个皇后
int Queen(int row, int QueensNum, int count)
{
	if (row == QueensNum)                            //递归结束条件，说明第QueensNum行已经成功放置了
	{
		++count;
		return count;
	}
	for (int i = 1;i <= QueensNum;++i)               //对第row+1行的每一列都作一次放置皇后操作
	{
		Site[row] = i;                               //标志第row+1行的第i列放置了皇后
		if (Valid(row))
			count = Queen(row + 1, QueensNum, count);//第row+1行放置成功的话就递归调用，接下来在下面一行放置
	}
	return count;
}

//主程序
int main()
{
	int QueensNum;
	int count=0;
	cout << "请输入皇后数：";
	cin >> QueensNum;
	int solutionNum = Queen(0, QueensNum, count);    //从第1行开始放置皇后
	cout << "方法数为：" << solutionNum << endl;
	return 0;
}