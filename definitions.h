#ifndef __definitions__
#define __definitions__

#define Default_File "patientRecordsFile.txt"
#define Default_EntriesD 40
#define Default_EntriesC 40
#define Default_bucketS 70


struct ListNode{
  char *recordID;
  char *patientFirstName;
  char *patientLastName;
  char *diseaseID;
  char *country;
  char *entryDate;
  char *exitDate;
  struct ListNode *next;
};

struct BucketEntry{
  char *value;
  struct TreeNode *treeRoot;
};

struct Bucket{
  struct Bucket *next;
  struct BucketEntry *entry;
};

struct HashTableEntry{
  struct Bucket *FirstBucket;
  int remainLastBucket;
};

struct HashTable{
  struct HashTableEntry *entry;
  int entriesNum;
};

typedef struct date{
  int day;
  int month;
  int year;
} Date;

struct TreeNode{
  Date date;
  struct ListNode *list;
  struct TreeNode *left;
  struct TreeNode *right;
  struct TreeNode *treeList;
  int height;
};

typedef struct heapNode{
  char *value;
  int cases;
  int height;
  struct heapNode *parent;
  struct heapNode *left;
  struct heapNode *right;
} HeapNode;


#endif
