#ifndef __func__
#define __func__

#include "definitions.h"

void ConvertToUpper(char *);
void RemoveSpaces(char *);
int DiseaseStatistics(struct HashTable, char *, char *, char *, char *, int);
int GetCases(struct TreeNode*, char*, char*, char*);
void CurrentPatients(struct HashTable, char*, int);
int NoExitDate(struct TreeNode *);
void Topk(struct HashTable, char *, int, char *, char *, int, int);
int Cases (char *, char *, char *, struct TreeNode *, int);
int InsertExitDate(struct ListNode**, char*, char*);
int GetMin(int, int);
int GetMax(int, int);

#endif
