//带表头
#include <iostream>
using namespace std;
struct SNode
{
	int data;
	SNode *next;
	SNode() :data(0), next(NULL) {}
};
struct LinkList
{
	SNode *head;
};
void mergeList(LinkList &HA, LinkList &HB, LinkList &HC)
{
	//HA和HB为非递减有序线性链表，合并为非递减有序线性链表HC
	SNode *A = HA.head, *B = HB.head, *C = HC.head, *p, *q;
	p = A->next;
	q = B->next;
	if (p == NULL)
		C->next = B->next;
	else if (q == NULL)
		C->next = A->next;
	else
	{
		while (p != NULL&&q != NULL)
		{
			if (p->data < q->data)     //HA链当前结点值小，将其链接到HC链尾
			{
				C->next = p;
				C = p;
				p = p->next;
			}
			else                       //HB链当前结点值小，将其链接到HC链尾
			{
				C->next = q;
				C = q;
				q = q->next;
			}
		}
		if (p != NULL)                 //若HA链还有结点，将它们链接到HC尾
		{
			C->next = p;
			while (p->next != NULL)
				p = p->next;
		}
		if (q != NULL)                 //若HB链还有结点，将它们链接到HC尾
		{
			C->next = q;
			while (q->next != NULL)
				q = q->next;
		}
	}
	HA.head = NULL;                    //原链表置空
	HB.head = NULL;
}
//测试函数
int main()
{
	SNode A[3];
	A[0].next = &A[1];
	A[1].data = 2;
	A[1].next = &A[2];
	A[2].data = 9;
	SNode B[5];
	B[0].next = &B[1];
	B[1].data = 1;
	B[1].next = &B[2];
	B[2].data = 3;
	B[2].next = &B[3];
	B[3].data = 4;
	B[3].next = &B[4];
	B[4].data = 5;
	LinkList AA;
	AA.head = &A[0];
	LinkList BB;
	BB.head = &B[0];
	LinkList C;
	SNode D;
	C.head = &D;
	mergeList(AA, BB, C);
	for (SNode *CC = C.head->next; CC != NULL; CC = CC->next)
	{
		cout << CC->data << "  ";
	}
}