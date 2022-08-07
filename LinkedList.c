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


int InsertList(struct ListNode** node, char* line){
  //return 0 -> the element was not inserted
  //return 1 -> the element was inserted
  struct ListNode *temp=NULL, *temp1=NULL;
  int counter=0;
  char *word, *exitD=NULL, *entryD=NULL;
  const char del[2] = " ";
  temp=malloc(sizeof(struct ListNode));
  temp->recordID=NULL;
  temp->patientFirstName=NULL;
  temp->patientLastName=NULL;
  temp->diseaseID=NULL;
  temp->country=NULL;
  temp->entryDate=NULL;
  temp->next=NULL;
  temp->exitDate=NULL;
  word=strtok(line,del);
  while(word!=NULL) {
    ConvertToUpper(word);
    switch (counter) {
      case 0:
        temp->recordID=malloc(strlen(word)+1);
        strcpy(temp->recordID,word);
        break;
      case 1:
        temp->patientFirstName=malloc(strlen(word)+1);
        strcpy(temp->patientFirstName,word);
        break;
      case 2:
        temp->patientLastName=malloc(strlen(word)+1);
        strcpy(temp->patientLastName,word);
        break;
      case 3:
        temp->diseaseID=malloc(strlen(word)+1);
        strcpy(temp->diseaseID,word);
        break;
      case 4:
        temp->country=malloc(strlen(word)+1);
        strcpy(temp->country,word);
        break;
      case 5:
        RemoveSpaces(word);
        temp->entryDate=malloc(strlen(word)+1);
        strcpy(temp->entryDate,word);
        entryD=malloc(strlen(word)+1);
        strcpy(entryD,word);
        break;
      case 6:
        RemoveSpaces(word);
        temp->exitDate=malloc(strlen(word)+1);
        strcpy(temp->exitDate,word);
        exitD=malloc(strlen(word)+1);
        strcpy(exitD,word);
        break;
      default:
        break;
    }
    counter++;
    word= strtok(NULL,del);
  }
  if(temp->recordID==NULL || temp->patientFirstName==NULL || temp->patientLastName==NULL || temp->diseaseID==NULL || temp->country==NULL || temp->entryDate==NULL){
    //printf("--ERROR: The entry wasn't complete, so it will be rejected!\n");
    return 0;
  }
  int valid=checkDate(entryD,exitD);
  if(valid==0){
    //printf("--ERROR: Invalid dates, the entry will be rejected!\n");
    free(temp->recordID);
    free(temp->patientFirstName);
    free(temp->patientLastName);
    free(temp->diseaseID);
    free(temp->country);
    free(temp->entryDate);
    free(temp->exitDate);
    free(temp);
    free(exitD);
    free(entryD);
    return 0;
  }
  else if (valid==2){
    free(temp->exitDate);
    temp->exitDate=malloc(strlen("-")+1);
    strcpy(temp->exitDate, "-");
    //printf("aftre\n");
  }
  if(*node==NULL) *node=temp;
  else{
    temp1=*node;
    while(temp1->next!=NULL) {
      if(strcmp(temp1->recordID,temp->recordID)==0){
        //printf("--ERROR: Same recordID - %s - the entry will be rejected!\n",temp->recordID);
        free(temp->recordID);
        free(temp->patientFirstName);
        free(temp->patientLastName);
        free(temp->diseaseID);
        free(temp->country);
        free(temp->entryDate);
        free(temp->exitDate);
        free(temp);
        free(exitD);
        free(entryD);
        return 0;
      }
      else temp1=temp1->next;
    }
    if(strcmp(temp1->recordID,temp->recordID)==0){
      //printf("--ERROR: Same recordID - %s - the entry will be rejected!\n",temp->recordID);
      free(temp->recordID);
      free(temp->patientFirstName);
      free(temp->patientLastName);
      free(temp->diseaseID);
      free(temp->country);
      free(temp->entryDate);
      free(temp->exitDate);
      free(temp);
      free(exitD);
      free(entryD);
      return 0;
    }
    temp1->next=temp;
  }
  free(exitD);
  free(entryD);
  return 1;
}

int checkDate(char* entryD, char* exitD){
  //0->greater, 2->no exit date
  int y1,y2,m1,m2,d1,d2,counter=0;
  if(entryD==NULL){
    entryD=malloc(strlen("1-1-1500")+1);
    strcpy(entryD,"1-1-1500");
  }
  if(exitD==NULL) {
    return 2;
  }
  if(strlen(exitD)<8){
    return 2;
  }
  else{
    char *date1, *date2;
    const char del[2] = "-";
    date1=strtok(entryD,del);
    while(date1!=NULL) {
      switch (counter) {
        case 0:
          d1=atoi(date1);
          break;
        case 1:
          m1=atoi(date1);
          break;
        case 2:
          y1=atoi(date1);
          break;
        default:
          break;
      }
      counter++;
      date1= strtok(NULL,del);
    }
    counter=0;
    date2=strtok(exitD,del);
    while(date2!=NULL) {
      switch (counter) {
        case 0:
          d2=atoi(date2);
          break;
        case 1:
          m2=atoi(date2);
          break;
        case 2:
          y2=atoi(date2);
          break;
        default:
          break;
      }
      counter++;
      date2= strtok(NULL,del);
    }
    if(y1>y2){
      return 0;
    }
    else if(y1==y2){
      if(m1>m2){
        return 0;
      }
      else if(m1==m2) {
        if(d1>d2){
           return 0;
        }
        else{
          return 1;
        }
      }
    }
  }
  return 1;
}

void PrintList(struct ListNode *list){
  printf("\n------------------------- Start of the list -------------------------\n\n");
  struct ListNode *p=list;
  while(p!=NULL){
    printf("%s  %s  %s  %s  %s  %s  %s \n",p->recordID,p->patientFirstName,p->patientLastName, p->diseaseID ,p->country,p->entryDate, p->exitDate);
    p=p->next;
  }
  printf("\n------------------------- End of the list -------------------------\n");
}

void ListSearchID(struct ListNode *list, char* id){
  struct ListNode *p=list;
  while(p!=NULL){
    if(strcmp(p->recordID, id)==0){
      printf("%s  %s  %s  %s  %s  %s  %s \n",p->recordID,p->patientFirstName,p->patientLastName, p->diseaseID ,p->country,p->entryDate, p->exitDate);
      break;
    }
    p=p->next;
  }
}

void FreeList(struct ListNode *list, struct ListNode *next){
  struct ListNode *temp=next;
  if(temp->next==NULL) {
    free(temp->recordID);
    free(temp->patientFirstName);
    free(temp->patientLastName);
    free(temp->diseaseID);
    free(temp->country);
    free(temp->entryDate);
    free(temp->exitDate);
    free(temp);
    list->next=NULL;
    return;
  }
  if(temp!=NULL) FreeList(temp, temp->next);
}
