#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "definitions.h"
#include <time.h>
#include "functions.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "SelfBalancedTree.h"
#include "Heap.h"

//https://www.geeksforgeeks.org/avl-tree-set-1-insertion/ -> the implementation of the AVL tree, is based- NO COPY PASTE HERE -on the code, located in this post

struct TreeNode* createTreeNode(char* date, struct ListNode* list){
  struct TreeNode *head=malloc(sizeof(struct TreeNode));
  CreateDate(&(head->date.day), &(head->date.month), &(head->date.year), date);
  head->left=NULL;
  head->right=NULL;
  head->treeList=NULL;
  head->list=list;
  head->height=1;
  return head;
}

struct TreeNode *RotateLeft(struct TreeNode *unbalancedNode){
    struct TreeNode *head = unbalancedNode->right, *child = head->left;
    head->left =unbalancedNode;
    unbalancedNode->right = child;
    unbalancedNode->height = GetMax(GetHeight(unbalancedNode->left), GetHeight(unbalancedNode->right))+1;
    head->height = GetMax(GetHeight(head->left), GetHeight(head->right))+1;
    int NodeBalance=GetBalance(head);
    return head;
}

struct TreeNode *RotateRight(struct TreeNode *unbalancedNode){
    struct TreeNode *head = unbalancedNode->left, *child = head->right;
    head->right=unbalancedNode;
    unbalancedNode->left=child;
    unbalancedNode->height = GetMax(GetHeight(unbalancedNode->left), GetHeight(unbalancedNode->right))+1;
    head->height = GetMax(GetHeight(head->left), GetHeight(head->right))+1;
    int NodeBalance=GetBalance(head);
    return head;
}

void InsertTreeList(struct TreeNode **tlist, char* date, struct ListNode *list){
  struct TreeNode *temp1;
  if(*tlist==NULL) *tlist=createTreeNode(date, list);
  else {
    temp1=*tlist;
    while(temp1->treeList!=NULL){
      temp1=temp1->treeList;
    }
    temp1->treeList=createTreeNode(date, list);
  }
}

struct TreeNode* InsertTreeNode(char* date, struct ListNode *list, struct TreeNode *tree){
  Date datetemp;
  int NodeBalance;
  CreateDate(&datetemp.day, &datetemp.month, &datetemp.year, date);
  if(tree==NULL) return createTreeNode(date, list);
  if(checkEntryDate(datetemp, tree->date)==0) tree->right=InsertTreeNode(date, list, tree->right);
  else if(checkEntryDate(datetemp, tree->date)==1) tree->left=InsertTreeNode(date, list, tree->left);
  else {
    InsertTreeList(&(tree->treeList), date, list);
    //printf("The treenode with date %s was added to the list\n",date);
  }
  tree->height=GetMax(GetHeight(tree->left),GetHeight(tree->right))+1;
  NodeBalance=GetBalance(tree);
  if(NodeBalance>1 && checkEntryDate(datetemp, tree->left->date)==1) return RotateRight(tree);
  if(NodeBalance<-1 && checkEntryDate(datetemp, tree->right->date)==0) return RotateLeft(tree);
  if (NodeBalance> 1 && checkEntryDate(datetemp, tree->left->date)==0){
   tree->left=RotateLeft(tree->left);
   return RotateRight(tree);
  }
  if (NodeBalance<-1 && checkEntryDate(datetemp, tree->right->date)==1){
    tree->right = RotateRight(tree->right);
    return RotateLeft(tree);
  }
  return tree;
}

int checkEntryDate(Date tempDate, Date nodeDate){
  // 0 the date of the node, which will be inserted(the first argument),is greater
  // 1 the date of the node, which will be inserted,is smaller
  // 2 the date of the node, which will be inserted,is equal
  if(tempDate.year>nodeDate.year){
    return 0;
  }
  else if(tempDate.year<nodeDate.year){
    return 1;
  }
  else if(tempDate.year==nodeDate.year){
    if(tempDate.month>nodeDate.month){
      return 0;
    }
    else if(tempDate.month<nodeDate.month){
      return 1;
    }
    else if(tempDate.month==nodeDate.month){
      if(tempDate.day>nodeDate.day){
        return 0;
      }
      else if(tempDate.day<nodeDate.day){
        return 1;
      }
      else if (tempDate.day==nodeDate.day){
        return 2;
      }
    }
  }
}

void PrintTree(struct TreeNode *root){
  if(root!=NULL) {
    printf("--------the date is: %d-%d-%d and the id is: %s and the height is: %d\n", root->date.day, root->date.month,  root->date.year, root->list->recordID, root->height);
    struct TreeNode* tlist=root->treeList;
    while(tlist!=NULL){
        printf("\t traversing the date is: %d-%d-%d and the id is: %s country: %s and the height is: %d\n",tlist->date.day, tlist->date.month,  tlist->date.year, tlist->list->recordID,tlist->list->country, tlist->height);
          tlist=tlist->treeList;
    }
    PrintTree(root->left);
    PrintTree(root->right);
  }
}

void CreateDate(int *day, int *month, int *year, char *date){
  char *word=NULL, *line=NULL;
  int counter=0;
  const char del[2] = "-";
  line=malloc(strlen(date)+1);
  strcpy(line,date);
  word=strtok(line,del);
  while(word!=NULL) {
    switch (counter) {
    case 0:
      *day=atoi(word);
      break;
    case 1:
      *month=atoi(word);
      break;
    case 2:
      *year=atoi(word);
      break;
    default:
      break;
    }
    counter++;
    word= strtok(NULL,del);
  }
  free(line);
}

int GetHeight(struct TreeNode* node){
  if(node==NULL) return 0;
  else return node->height;
}

int GetBalance(struct TreeNode* node){
  if (node==NULL) return 0;
  else return GetHeight(node->left)-GetHeight(node->right);
}


