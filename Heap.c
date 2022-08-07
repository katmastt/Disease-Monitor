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

HeapNode* CreateHeapNode(char* value, int number, HeapNode *parent){
  HeapNode *node=malloc(sizeof(HeapNode));
  node->left=NULL;
  node->right=NULL;
  node->parent=parent;
  node->cases=number;
  node->height=1;
  node->value=malloc(strlen(value)+1);
  strcpy(node->value, value);
 // printf("this is the value %s \n", node->value);
  if(node->parent!=NULL){
    if(node->cases>node->parent->cases) SwapHeapNodes(node, node->parent);
  }
  return node;
}

HeapNode* InsertHeap(HeapNode *heap, char *value, int number, HeapNode *parent){
  if(heap==NULL) return CreateHeapNode(value, number, parent);
  int minright=MinBranch(heap->right);
  int minleft=MinBranch(heap->left);
  if(minleft<=minright) heap->left=InsertHeap(heap->left, value, number, heap);
  else if(minleft>minright) heap->right=InsertHeap(heap->right, value, number, heap);
  heap->height=GetMax(GetHeightH(heap->left),GetHeightH(heap->right))+1;
  if(heap->parent!=NULL){
    if(heap->cases>heap->parent->cases) SwapHeapNodes(heap, heap->parent);
  }
  return heap;
}

void PopHeapNode(HeapNode *heap, char **value, int *cases){
  if(heap->height==1){
    *value=malloc(strlen(heap->value)+1);
    strcpy(*value, heap->value);
    *cases=heap->cases;
    if(heap->parent!=NULL){
      if(strcmp(heap->parent->left->value, heap->value)==0) heap->parent->left=NULL;
      else if(strcmp(heap->parent->right->value, heap->value)==0) heap->parent->right=NULL;
    }
   // printf("this is the value %s \n", heap->value);
    free(heap->value);
    free(heap);
    return;
  }
  int minleft=MinBranch(heap->left);
  int maxright=LongestBranch(heap->right);
  int maxleft=LongestBranch(heap->left);
  if(maxright==maxleft) PopHeapNode(heap->right, value, cases);
  else if(maxleft>maxright) PopHeapNode(heap->left, value, cases);
  heap->height=GetMax(GetHeightH(heap->left),GetHeightH(heap->right))+1;
  
}

void SwapHeapNodes(HeapNode * child, HeapNode * parent){
  int tempc=child->cases;
  child->cases=parent->cases;
  parent->cases=tempc;
  char *tempv=malloc(strlen(child->value)+1);
  strcpy(tempv, child->value);
  free(child->value);
  child->value=malloc(strlen(parent->value)+1);
  strcpy(child->value, parent->value);
  free(parent->value);
  parent->value=malloc(strlen(tempv)+1);
  strcpy(parent->value, tempv);
  free(tempv);
}

int GetHeightH(HeapNode* node){
  if(node==NULL) return 0;
  else return node->height;
}

int MinBranch(HeapNode* root){
    if (root == NULL) return 0;
    return 1 + GetMin(MinBranch(root->right), MinBranch(root->left));
}

int LongestBranch(HeapNode* root){
    if (root == NULL) return 0;
    return 1 + GetMax(LongestBranch(root->right), LongestBranch(root->left));
}

void PrintHeap(HeapNode *heap){
  if(heap!= NULL) {
    printf("This is the value: %s\n", heap->value);
    PrintHeap(heap->left);
    PrintHeap(heap->right);
  }
}

void Pop(HeapNode **heap, char **maxval, int *maxcas){
  if(*heap==NULL) return;
  char *vlast=NULL;
  int clast;
  *maxval=malloc(strlen((*heap)->value)+1);
  strcpy(*maxval, (*heap)->value);
  free((*heap)->value);
  *maxcas=(*heap)->cases;
  if((*heap)->left==NULL && (*heap)->right==NULL){
    free(*heap);
    *heap=NULL;
    return;
  }
  PopHeapNode(*heap, &vlast, &clast);
  (*heap)->value=malloc(strlen(vlast)+1);
  strcpy((*heap)->value, vlast);
  (*heap)->cases=clast;
  Heapify(*heap);
}

void Heapify(HeapNode *heap){
  if(heap==NULL) return;
  if(heap->right!=NULL && heap->left!=NULL){
    if(heap->cases<heap->right->cases && heap->cases<heap->left->cases){
      if(heap->left->cases>heap->right->cases) SwapHeapNodes(heap->left, heap);  //the value in the left node, would be greater than the right one.
      else SwapHeapNodes(heap->right, heap);
    }
    else if(heap->cases<heap->left->cases) SwapHeapNodes(heap->left, heap);
    else if(heap->cases<heap->right->cases) SwapHeapNodes(heap->right, heap);
  }
  else if(heap->right==NULL && heap->left!=NULL && heap->cases<heap->left->cases) SwapHeapNodes(heap->left, heap);
  else if(heap->left==NULL && heap->right!=NULL && heap->cases<heap->right->cases) SwapHeapNodes(heap->right, heap);
  Heapify(heap->left);
  Heapify(heap->right);
}

