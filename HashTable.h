#ifndef __hash__
#define __hash__

#include "definitions.h"

int entriesPerBucket(int);
struct HashTable createHashTable(int, int);
void InsertHashTable(struct HashTable, char*, int, struct ListNode*);
int hash(char*, int);
void InsertEntry(struct HashTableEntry*, char*, int, struct ListNode*);
struct Bucket* allocateBucket(int, int);
int InsertEntryToBucketList(struct Bucket*, struct BucketEntry, int, int, struct ListNode*);
int InsertEntryToBucket(struct Bucket*,struct BucketEntry, int, int, struct ListNode*);
void PrintHashTable(struct HashTable, int, int);
void FreeList(struct ListNode *, struct ListNode *);
void FreeHashTable(struct HashTable * , int);
void FreeBucket(struct Bucket *, struct Bucket *, int);
void FreeBucketEntry(struct BucketEntry *, int);


#endif
