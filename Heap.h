#ifndef __heap__
#define __heap__

#include "definitions.h"

HeapNode* CreateHeapNode(char*, int, HeapNode *);
HeapNode* InsertHeap(HeapNode *, char *, int, HeapNode *);
int GetHeightH(HeapNode*);
int MinBranch(HeapNode*);
int LongestBranch(HeapNode*);
void SwapHeapNodes(HeapNode *, HeapNode *);
void PopHeapNode(HeapNode *, char **, int *);
void PrintHeap(HeapNode *);
void Pop(HeapNode**, char **, int *);
void Heapify(HeapNode*);


#endif
