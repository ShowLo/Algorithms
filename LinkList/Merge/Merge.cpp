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
	if (HA.head == NULL)                   //若HA链空，则HB链为合并后的链
		HC.head = HB.head;
	else if (HB.head == NULL)              //若HB链空，则HA链为合并后的链
		HC.head = HA.head;
	else
	{
		SNode *A = HA.head,*B = HB.head,*p,*q;
		if (A->data > B->data)            
		{
			HC.head = B;                   //以HB链头结点为HC链头结点
			p = A;
			q = B->next;
			while (p != NULL&&q != NULL)
			{
				if (p->data < q->data)     //HA链当前结点值小，将其链接到HC链尾
				{
					B->next = p;
					B = p;
					p = p->next;
					B->next = q;
				}
				else                       //HB链当前结点值小，将其链接到HC链尾
				{
					B = q;
					q = q->next;
				}
			}
			if (p != NULL)                 //若HA链还有结点，将它们链接到HC尾
			{
				B->next = p;
				while (p->next != NULL)
					p = p->next;
			}
		}
		else
		{
			HC.head = A;                   //以HA链头结点为HC链头结点
			p = A->next;
			q = B;
			while (p != NULL&&q != NULL)
			{
				if (q->data < p->data)     //HB链当前结点值小，将其链接到HC链尾
				{
					A->next = q;
					A = q;
					q = q->next;
					A->next = p;
				}
				else                       //HA链当前结点值小，将其链接到HC链尾
				{
					A = p;
					p = p->next;
				}
			}
			if (q != NULL)                 //若HB链还有结点，将它们链接到HC尾
			{
				A->next = q;
				while (q->next != NULL)
					q = q->next;
			}
		}
	}
	HA.head = NULL;                        //原链表置空
	HB.head = NULL;
}
int main()
{
	SNode A[3];
	A[0].data = 2;
	A[0].next = &A[1];
	A[1].data = 2;
	A[1].next = &A[2];
	A[2].data = 9;
	SNode B[5];
	B[0].data = 0;
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
	mergeList(AA, BB, C);
	for (SNode *CC = C.head; CC != NULL; CC = CC->next)
	{
		cout << CC->data << "  ";
	}
}