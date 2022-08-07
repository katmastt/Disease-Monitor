#ifndef __tree__
#define __tree__

#include "definitions.h"

struct TreeNode* createTreeNode(char*, struct ListNode*);
struct TreeNode* InsertTreeNode(char*, struct ListNode*, struct TreeNode*);
int checkEntryDate(Date, Date);
void CreateDate(int*, int*, int*, char*);
int GetHeight(struct TreeNode*);
int GetBalance(struct TreeNode*);
void InsertTreeList(struct TreeNode**, char*, struct ListNode*);
void PrintTree(struct TreeNode *);
struct TreeNode *RotateRight(struct TreeNode *);
struct TreeNode *RotateLeft(struct TreeNode *);

#endif
