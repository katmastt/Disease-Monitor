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

//https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
void ConvertToUpper(char *word){
  char* convertU=word;
  while (*convertU) {
    *convertU = toupper((unsigned char) *convertU);
    convertU++;
  }
}

int DiseaseStatistics(struct HashTable table, char *start, char *end, char *country, char *disease, int bucketS){
  int cases;
  if(country==NULL && disease==NULL){        //global diseasestats
    for(int i=0; i<table.entriesNum; i++){
      if(table.entry[i].FirstBucket!=NULL){
        int entries=entriesPerBucket(bucketS);
        struct Bucket *b=table.entry[i].FirstBucket;
        while(b!=NULL){
          if(b->next==NULL){
            entries=entries-table.entry[i].remainLastBucket;
          }
          for(int index=0; index<entries; index++){
            cases=GetCases(b->entry[index].treeRoot, start, end, country);
            if(strcmp(start, "1-1-1500")==0 && strcmp(end, "30-12-2030")==0){
              //printf("The total cases of the disease %s are: %d\n", b->entry[index].value, cases);
              printf("%s %d\n",b->entry[index].value, cases);
            }
            else{
              printf("%s %d\n",b->entry[index].value, cases);
              //printf("The cases of the disease %s for the given time period are: %d\n", b->entry[index].value, cases);
            }
          }
          b=b->next;
        }
      }
    }
  }
  else {             //diseaseFrequency
    int index=hash(disease, table.entriesNum);
    int found=0;
    if(table.entry[index].FirstBucket!=NULL){
      int entries=entriesPerBucket(bucketS);
      struct Bucket *b=table.entry[index].FirstBucket;
      while(b!=NULL){
        if(b->next==NULL){
          entries=entries-table.entry[index].remainLastBucket;
        }
        for(int i=0; i<entries; i++){
          if(strcmp(b->entry[i].value, disease)==0){
            found=1;
            cases=GetCases(b->entry[i].treeRoot, start, end, country);
            if(country==NULL){
              printf("%s %d\n",b->entry[i].value, cases);
              //printf("The cases of the disease %s for the given time period are: %d\n", b->entry[i].value, cases);
            }
            else{
              printf("%s %d\n",b->entry[i].value, cases);
              //printf("The cases of the disease %s for the given time period in the country %s are: %d\n", b->entry[i].value, country, cases);
            }
            break;
          }
        }
        b=b->next;
      }
    }
    if(found==0) printf("error\n"); //printf("--ERROR: The is no virus with name %s\n", disease);
  }
  return cases;
}

int GetCases(struct TreeNode* Troot, char* start, char* end, char* country){
  int counter=0;
  Date startD, endD;
  struct TreeNode *root=Troot;
  CreateDate(&(startD.day), &(startD.month), &(startD.year), start);
  CreateDate(&(endD.day), &(endD.month), &(endD.year), end);
  if(root==NULL) return 0;
  counter+=GetCases(root->right, start, end, country);
  counter+=GetCases(root->left, start, end, country);
  if(country==NULL){   //global diseasestats diseasefrequency(without country)
    if(checkEntryDate(startD, root->date)!=0 && checkEntryDate(endD, root->date)!=1){
      counter++;
      struct TreeNode *list=root->treeList;
      while(list!=NULL){
        counter++;
        list=list->treeList;
      }
    }
  }
  else {         //diseasefrequency(with country)
    if(checkEntryDate(startD, root->date)!=0 && checkEntryDate(endD, root->date)!=1){
      struct TreeNode *list1=root->treeList;
      if(strcmp(root->list->country, country)==0){
        counter++;
        while(list1!=NULL && strcmp(list1->list->country, country)==0){
          counter++;
          list1=list1->treeList;
        }
      }
      while(list1!=NULL){
        if(strcmp(list1->list->country, country)==0){
          counter++;
        }
        list1=list1->treeList;
      }
    }
  }
  return counter;
}

void CurrentPatients(struct HashTable table, char* disease, int bucketS){
  int counter=0;
  if(disease==NULL){
    for(int i=0; i<table.entriesNum; i++){
      if(table.entry[i].FirstBucket!=NULL){
        int entries=entriesPerBucket(bucketS);
        struct Bucket *b=table.entry[i].FirstBucket;
        while(b!=NULL){
          if(b->next==NULL){
            entries=entries-table.entry[i].remainLastBucket;
          }
          for(int index=0; index<entries; index++){
            int current=NoExitDate(b->entry[index].treeRoot);
            //printf("The patients still in hospital, diagnosed with disease %s are %d\n" , b->entry[index].value, current);
            printf("%s %d\n", b->entry[index].value, current);
          }
          b=b->next;
        }
      }
    }
  }
  else {
    int index=hash(disease, table.entriesNum);
    int found=0;
    if(table.entry[index].FirstBucket!=NULL){
      int entries=entriesPerBucket(bucketS);
      struct Bucket *b=table.entry[index].FirstBucket;
      while(b!=NULL){
        if(b->next==NULL){
          entries=entries-table.entry[index].remainLastBucket;
        }
        for(int i=0; i<entries; i++){
          if(strcmp(b->entry[i].value, disease)==0){
            found=1;
            int current;
            current=NoExitDate(b->entry[i].treeRoot);
            printf("%s %d\n", disease, current);
            //printf("The patients still in hospital, diagnosed with disease %s are %d\n", disease, current);
            break;
          }
        }
        b=b->next;
      }
    }
    if(found==0) printf("%s %d\n", disease, 0); //printf("--ERROR: There is no virus with name %s!\n", disease);
  }
}

int NoExitDate(struct TreeNode *Troot){
  int counter=0;
  struct TreeNode *root=Troot;
  if(root==NULL) return 0;
  counter+=NoExitDate(root->right);
  counter+=NoExitDate(root->left);
  if(strcmp(root->list->exitDate,"-")==0) counter++;
  struct TreeNode *sameDate=root->treeList;
  while(sameDate!=NULL){
    if(strcmp(sameDate->list->exitDate,"-")==0) counter++;
    sameDate=sameDate->treeList;
  }
  return counter;
}

void Topk(struct HashTable table, char * value, int k, char * start, char * end, int bucketS, int mode){
  int counter=0;
  int cases=0;
  HeapNode *heap=NULL;
  if(start==NULL && end==NULL){      //whole time
    start=malloc(strlen("1-1-1500")+1);
    strcpy(start,"1-1-1500") ;
    end=malloc(strlen("30-12-2030")+1);
    strcpy(end, "30-12-2030");
  }
  for(int index=0; index<table.entriesNum; index++){
    if(table.entry[index].FirstBucket!=NULL){
      int entries=entriesPerBucket(bucketS);
      struct Bucket *b=table.entry[index].FirstBucket;
      while(b!=NULL){
        if(b->next==NULL){
          entries=entries-table.entry[index].remainLastBucket;
        }
        for(int i=0; i<entries; i++){
          cases=Cases(value, start, end, b->entry[i].treeRoot, mode);
          counter+=cases;
          heap=InsertHeap(heap, b->entry[i].value, cases, heap);
        }
        b=b->next;
      }
    }
  }
  //if(mode==0 && strcmp(start, "1-1-1500")==0) printf("%s %d\n",value, k); //printf("If there are countries with the same number of cases for the given disease, the function will print all of them.\nFor the disease %s the top %d countries are:\n", value, k);
  //else if(mode==0 && strcmp(start, "1-1-1500")!=0) printf("%s %d\n",value, k); //printf("If there are countries with the same number of cases for the given disease, the function will print all of them.\nFor the disease %s the top %d countries in the given time period are:\n", value, k);
 // else if(mode==1 && strcmp(start, "1-1-1500")==0) printf("%s %d\n",value, k); //printf("If there are diseases with the same number of cases for the given country, the function will print all of them.\nFor the country %s the top %d diseases are:\n", value, k);
  //else if(mode==1 && strcmp(start, "1-1-1500")!=0) printf("%s %d\n",value, k); //printf("If there are diseases with the same number of cases for the given country, the function will print all of them.\nFor the country %s the top %d diseases in the given time period are:\n", value, k);
  char* maxv;
  int maxc;
  int count=k;
  int previous_max=0;
  while(count!=0){
    if(heap!=NULL){
      Pop(&heap, &maxv, &maxc);
      if(maxc!=previous_max) count--;
      previous_max=maxc;
      printf("%s %d\n",maxv, maxc);
      //printf("\t- %s with %d cases\n", maxv, maxc);
     // free(maxv);
    }
    else break;
  }
  if(count==0 && heap!=NULL){
    Pop(&heap, &maxv, &maxc);
    if(maxc==previous_max && heap==NULL) printf("%s %d\n",maxv, maxc); //printf("\t- %s with %d cases\n", maxv, maxc);
    while(maxc==previous_max && heap!=NULL){
      printf("%s %d\n",maxv, maxc);
      //printf("\t- %s with %d cases\n", maxv, maxc);
      Pop(&heap, &maxv, &maxc);
      // free(maxv);
    }
  }
  //if(count!=0 && mode==0) printf("There are no other recorded countries\n");
  //if(count!=0 && mode==1) printf("There are no other recorded diseases\n");
  while(heap!=NULL) {
    Pop(&heap, &maxv, &maxc);
   // free(maxv);
  }
}

int Cases (char * value, char * start, char * end, struct TreeNode * Troot, int mode){
  Date startD, endD;
  int counter=0;
  struct TreeNode *root=Troot;
  CreateDate(&(startD.day), &(startD.month), &(startD.year), start);
  CreateDate(&(endD.day), &(endD.month), &(endD.year), end);
  if(root==NULL) return 0;
  counter+=Cases(value, start, end, root->right, mode);
  counter+=Cases(value, start, end, root->left, mode);
  if(checkEntryDate(startD, root->date)!=0 && checkEntryDate(endD, root->date)!=1 && mode==0){
    if(strcmp(root->list->diseaseID, value)==0){
      counter++;
      struct TreeNode *list=root->treeList;
      while(list!=NULL){
        if(strcmp(list->list->diseaseID, value)==0) counter++;
        list=list->treeList;
      }
    }
    else{
      struct TreeNode *list=root->treeList;
      while(list!=NULL){
        if(strcmp(list->list->diseaseID, value)==0) counter++;
        list=list->treeList;
      }
    }
  }
  else if(checkEntryDate(startD, root->date)!=0 && checkEntryDate(endD, root->date)!=1 && mode==1){
    if(strcmp(root->list->country, value)==0){
      counter++;
      struct TreeNode *list=root->treeList;
      while(list!=NULL){
        if(strcmp(list->list->country, value)==0) counter++;
        list=list->treeList;
      }
    }
    else {
      struct TreeNode *list=root->treeList;
      while(list!=NULL){
        if(strcmp(list->list->country, value)==0) counter++;
        list=list->treeList;
      }
    }
  }
  return counter;
}

int InsertExitDate(struct ListNode** list, char* recordID, char* date){
  struct ListNode* temp=*list;
  char *entry=NULL, *exit=NULL;
  while(temp!=NULL){
    if(strcmp(temp->recordID, recordID)==0){
      entry=malloc(strlen(temp->entryDate)+1);
      exit=malloc(strlen(date)+1);
      strcpy(entry, temp->entryDate);
      strcpy(exit, date);
      if(checkDate(entry, exit)!=0){ //valid
        free(temp->exitDate);
        temp->exitDate=malloc(strlen(date)+1);
        strcpy(temp->exitDate, date);
        if(entry!=NULL) free(entry);
        if(exit!=NULL) free(exit);
        return 1; //ok
      }
      else {   //invalid
        if(entry!=NULL) free(entry);
        if(exit!=NULL) free(exit);
        return 2;   //invalid
      }
      
    }
    temp=temp->next;
  }
  if(entry!=NULL) free(entry);
  if(exit!=NULL) free(exit);
  return 0;  //invalid recordID
}

void RemoveSpaces(char * stringB){
  int index=0;
  for(int i=0; stringB[i]!=NULL; i++){
    if(stringB[i]!=' ' && stringB[i]!='\t' && stringB[i]!='\n') {
      stringB[index]=stringB[i];
      index++;
    }
  }
  stringB[index]='\0';
}

int GetMax(int n1, int n2){
  if(n1>n2) return n1;
  else return n2;
}

int GetMin(int n1, int n2){
  if(n1<n2) return n1;
  else return n2;
}
