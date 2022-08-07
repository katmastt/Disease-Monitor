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

int entriesPerBucket(int bucketS){
  int remain=bucketS-sizeof(struct Bucket);
  int entriesNum=remain/sizeof(struct BucketEntry);
  return entriesNum;
}

struct HashTable createHashTable(int entriesNum, int remain){
  struct HashTable temp;
  temp.entriesNum=entriesNum;
  temp.entry=malloc(entriesNum*sizeof(struct HashTableEntry));
  for(int i=0; i<entriesNum; i++){
    temp.entry[i].FirstBucket=NULL;
    temp.entry[i].remainLastBucket=remain;
  }
  return temp;
}
// https://stackoverflow.com/questions/20462826/hash-function-for-strings-in-c
int hash(char *key, int entriesNum) {
  unsigned int hash = 0, c;
  for (size_t i = 0; key[i] != '\0'; i++) {
    c = (unsigned char)key[i];
    hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + c;
  }
  return hash % entriesNum;
}

struct BucketEntry CreateBucketEntry(char *key){
  struct BucketEntry entryB;
  entryB.value=malloc(strlen(key)+1);
  strcpy(entryB.value,key);
  entryB.treeRoot=NULL;
  return entryB;
}

struct Bucket* allocateBucket(int entriesNum, int bucketS){
  int bucketEntriesSize=entriesPerBucket(bucketS)*sizeof(struct BucketEntry);
  int allocateSize=bucketEntriesSize+sizeof(struct Bucket);
  void *memory=malloc(allocateSize);
  struct Bucket *bucket=(struct Bucket *) memory;
  bucket->next=NULL;
  bucket->entry=(struct BucketEntry*)(memory+sizeof(struct Bucket));
  return bucket;
}

int InsertEntryToBucket(struct Bucket* bucket, struct BucketEntry entry, int remainLast, int bucketS, struct ListNode* listnode){
  // 0: did not create bucket
  // 1: new bucket was created
  int entriesNum=entriesPerBucket(bucketS);
  if(remainLast!=0){
    int index=entriesNum-remainLast;
    bucket->entry[index]=entry;
    bucket->entry[index].treeRoot=InsertTreeNode(listnode->entryDate, listnode, bucket->entry[index].treeRoot);
    return 0;
  }
  else{
    struct Bucket *new=allocateBucket(entriesNum, bucketS);
    bucket->next=new;
    InsertEntryToBucket(new, entry, entriesNum, bucketS, listnode);
    bucket->entry[0].treeRoot=InsertTreeNode(listnode->entryDate, listnode, bucket->entry[0].treeRoot);
    return 1;
  }
}

int InsertEntryToBucketList(struct Bucket* bucket, struct BucketEntry entry, int remainLast, int bucketS, struct ListNode* listnode){
  int entriesNum=entriesPerBucket(bucketS);
  while(bucket->next!=NULL){
    for(int i=0; i<entriesNum; i++){
      if (strcmp(bucket->entry[i].value, entry.value)==0){
        bucket->entry[i].treeRoot=InsertTreeNode(listnode->entryDate, listnode, bucket->entry[i].treeRoot);
        return 2;
      }
    }
    bucket=bucket->next;
  }
  int index=entriesNum-remainLast;
  for(int i=0; i<index; i++){
    if (strcmp(bucket->entry[i].value, entry.value)==0){
      bucket->entry[i].treeRoot=InsertTreeNode(listnode->entryDate, listnode, bucket->entry[i].treeRoot);
      return 2;
    }
  }
  int newBucket=InsertEntryToBucket(bucket, entry, remainLast, bucketS, listnode);
  return newBucket;
}

void InsertEntry(struct HashTableEntry *entryH, char *key, int bucketS, struct ListNode* listnode){
  struct BucketEntry entryB=CreateBucketEntry(key);
  if(entryH->FirstBucket==NULL){
    entryH->FirstBucket=allocateBucket(entryH->remainLastBucket, bucketS);
    InsertEntryToBucketList(entryH->FirstBucket, entryB, entryH->remainLastBucket, bucketS, listnode);
    entryH->remainLastBucket--;
    //printf("\t--First Bucket was created and the number of remaining entries is %d\n\n", entryH->remainLastBucket);
    return;
  }
  int newBucket=InsertEntryToBucketList(entryH->FirstBucket, entryB, entryH->remainLastBucket, bucketS, listnode);
  if(newBucket==1){
    entryH->remainLastBucket=entriesPerBucket(bucketS)-1;
    //printf("\t--A New bucket was created and the number of remaining entries is %d\n\n", entryH->remainLastBucket);
  }
  else if(newBucket==0){
    entryH->remainLastBucket--;
    //printf("\t--New bucket was NOT created and the number of remaining entries is %d\n\n", entryH->remainLastBucket);
  }
  else{
    //printf("The element wasn't inserted, because it exists already\n");
  }
}

void InsertHashTable(struct HashTable table, char *key, int bucketS, struct ListNode* listnode){
  int index=hash(key, table.entriesNum);
  InsertEntry(&(table.entry[index]), key, bucketS, listnode);
}


void PrintHashTable(struct HashTable table, int entriesB, int mode){
  printf("\n------------------------- Start of the HashTable -------------------------\n\n");
  for(int index=0; index<table.entriesNum; index++){
    struct Bucket *b=table.entry[index].FirstBucket;
    while(b!=NULL){
      int ent=entriesB;
      if (!b->next){
        // Subtract only in the last bucket.
        // All the other buckets are full.
        ent-=table.entry[index].remainLastBucket;
      }
      for(int i=0; i<ent; i++){
        if(mode==0) printf("HashTableEntry %d: country %s \n", index, b->entry[i].value);
        else printf("HashTableEntry %d: disease %s \n", index, b->entry[i].value);
      }
      b=b->next;
    }
  }
  printf("\n------------------------- End of the HashTable ---------------------------\n");
}

void FreeHashTable(struct HashTable * table, int entriesB){
  for(int index=0; index<table->entriesNum; index++){
    printf("inside\n");
    struct Bucket *b=table->entry[index].FirstBucket;
    int i=0;
    int ent=entriesB;
    while(b!=NULL){
      ent=entriesB;
      if (!b->next) ent-=table->entry[index].remainLastBucket;
      i++;
      printf("this is the i:%d\n", i);
      b=b->next;
    }
    printf("in there\n");
    struct Bucket *b3=table->entry[index].FirstBucket, *bprev=NULL;
    if(i==1){
      printf("inside i=1\n");
      struct Bucket *b1=table->entry[index].FirstBucket;
      FreeBucket(b1, bprev, ent);
      //bprev->next=NULL;
    }
    else if(i>1){
      for(int k=0; k<i-1; k++){
        bprev=b3;
        b3=b3->next;
        printf("in there2\n");
      }
      FreeBucketEntry(b3->entry, ent);
      bprev->next=NULL;
      struct Bucket *b2=table->entry[index].FirstBucket;
      ent=entriesB;
      if(i-1>1){
        while(b2!=NULL){
          FreeBucket(b2, b2->next, ent);
          b2=b2->next;
        }
      }
    }
    printf("after else\n");
    //FreeBucket(bprev, b->next, table.entry[index].remainLastBucket);
  }
}

void FreeBucket(struct Bucket *bucket, struct Bucket *next, int num){
  //printf("inside2\n");
  struct Bucket *temp=next;
  if(temp==NULL){
    FreeBucketEntry(bucket->entry, num);
    free(bucket);
    printf("inside FreeBucket\n");
    //bucket->next=NULL;
    return;
  }
  else if(temp!=NULL){
    printf("inside FreeBucket2\n");
    FreeBucket(temp, temp->next, num);
  }
}

void FreeBucketEntry(struct BucketEntry *entry, int num){
  printf("this is the num :%d\n", num);
  for(int i=0; i<num; i++){
    free(entry[i].value);
    printf("inside FreeBucketEntry\n");
    //FreeTree(entry->treeRoot);
  }
}
