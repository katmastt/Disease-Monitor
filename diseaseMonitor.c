
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "definitions.h"
#include "functions.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "SelfBalancedTree.h"
#include "Heap.h"

int main(int argc, const char * argv[]) {
  int option=0, diseaseE=0, countryE=0, bucketS=0, entriesB=0;
  char *patientRecordsF = NULL;
  FILE *patientfp=NULL;
  struct ListNode *patientRecords=NULL;
  struct HashTable dHashTable, cHashTable;
/* ksekiname na elegxoume orismata kai na ginetai i antikatastasi an kapoio den dothei */
  for(int i=0; i<argc; i++){
    if(strcmp(argv[i], "-b")==0){
      if (argv[i+1]!=NULL){
        bucketS=atoi(argv[i+1]);
        if(bucketS<=0){
          bucketS=Default_bucketS;
          //printf("--ERROR: The size of the bucket isn't valid and will be replaced by the default size %d\n",bucketS);
        }
      }
      else{
        //printf("--ERROR: No bucket size was inserted. The bucket size will be replaced by the default value\n");
        bucketS=Default_bucketS;
      }
      //printf("The bucket size is %d\n",bucketS);
    }
    else if(strcmp(argv[i], "-p")==0){
      if (argv[i+1]!=NULL){
        patientRecordsF=malloc(strlen(argv[i+1])+1);
        strcpy(patientRecordsF,argv[i+1]);
        //printf("The patientRecordsFile title is %s\n",patientRecordsF);
      }
      else {
        patientRecordsF=malloc(strlen(Default_File)+1);
        strcpy(patientRecordsF,Default_File);
        //printf("--ERROR: No file name was inserted and it will be replaced by the default name\n");
      }
    }
    else if(strcmp(argv[i], "-h1")==0){
      if (argv[i+1]!=NULL){
        diseaseE=atoi(argv[i+1]);
        if(diseaseE<=0){
          //printf("--ERROR:The number of entries of the disease hashtable is not valid and will be replaced by the default value\n");
          diseaseE=Default_EntriesD;
        }
      }
      else{
        //printf("--ERROR: No value for the entries of disease hashtable was inserted. The value of entries will be replaced by the default value\n");
        diseaseE=Default_EntriesD;
      }
      //printf("The entries of the disease hashtable are %d\n",diseaseE);
    }
    else if(strcmp(argv[i], "-h2")==0){
      if (argv[i+1]!=NULL){
        countryE=atoi(argv[i+1]);
        if(countryE<=0){
          //printf("--ERROR: The number of entries of the countries hashtable isn't valid and will be replaced by the default value\n");
          countryE=Default_EntriesC;
        }
      }
      else{
        //printf("--ERROR: No value for the entries of country hashtable was inserted. The value of entries will be replaced by the default value\n");
        countryE=Default_EntriesC;
      }
      //printf("The entries of the country hashtable are %d\n",countryE);
    }
  }
  if(patientRecordsF==NULL){
    //printf("--ERROR: No title was inserted. The title will be replaced by the default title\n");
    patientRecordsF=malloc(strlen(Default_File)+1);
    strcpy(patientRecordsF,Default_File);
    //printf("The patientRecordsFile title is %s\n",patientRecordsF);
  }
  if(bucketS==0){
    //printf("--ERROR: No bucket size was inserted. The bucket size will be replaced by the default value\n");
    bucketS=Default_bucketS;
    //printf("The bucket size is %d\n",bucketS);
  }
  if(bucketS<sizeof(struct Bucket)+sizeof(struct BucketEntry)){
    //printf("--ERROR: the bucket can't hold the information needed and will be replaced by the default value\n");
    bucketS=Default_bucketS;
  }
  if(countryE==0){
    //printf("--ERROR: No value for the entries of country hashtable was inserted. The value of entries will be replaced by the default value\n");
    countryE=Default_EntriesC;
    //printf("The entries of the country hashtable are %d\n",countryE);
  }
  if(diseaseE==0){
    //printf("--ERROR: No value for the entries of disease hashtable was inserted. The value of entries will be replaced by the default value\n");
    diseaseE=Default_EntriesD;
    //printf("The entries of the disease hashtable are %d\n",diseaseE);
  }
/* telos me ta orismata, oi parametroi exoun pleon times */
  
  entriesB=entriesPerBucket(bucketS);
  
/* arxizei i anagnosi apo to arxeio */
  dHashTable=createHashTable(diseaseE,entriesB);
  cHashTable=createHashTable(countryE,entriesB);
  patientfp=fopen(patientRecordsF,"r");
  if(patientfp==NULL){
   // printf("The name of the file is not valid and it will be replaced by the default file: %s\n",Default_File);
    free(patientRecordsF);
    patientRecordsF=NULL;
    patientRecordsF=malloc(strlen(Default_File)+1);
    strcpy(patientRecordsF,Default_File);
    patientfp=fopen(patientRecordsF,"r");
  }
  if(patientfp!=NULL){
    //printf("Successful opening of the file: %s\n",patientRecordsF);
    char *entry=NULL, *temp=NULL, *temph=NULL;
    size_t size;
    /* ksekina i eisagogi stin lista kai stis alles domes */
    while((getline(&entry, &size, patientfp)>0)) {
      if(strcmp(entry,"\n")!=0){   //petame tis kenes grammes
        temp=malloc(strlen(entry)+1);
        strcpy(temp,entry);
        temph=malloc(strlen(entry)+1);
        strcpy(temph,entry);
        int inserted=InsertList(&patientRecords, temp);
        if(inserted==1){
          int counter=0;
          char *disease=NULL, *country=NULL, *word=NULL;
          const char del[2] = " ";
          word=strtok(temph,del);
          while(word!=NULL) {
            ConvertToUpper(word);
            switch (counter) {
              case 3:
                disease=malloc(strlen(word)+1);
                strcpy(disease, word);
                break;
              case 4:
                country=malloc(strlen(word)+1);
                strcpy(country, word);
                break;
              default:
                break;
            }
            counter++;
            word= strtok(NULL,del);
          }
          struct ListNode *p=patientRecords;
          while(p->next!=NULL){
            p=p->next;
          }
          InsertHashTable(dHashTable, disease, bucketS, p);
          InsertHashTable(cHashTable, country, bucketS, p);
          free(disease);
          disease=NULL;
          free(country);
          country=NULL;
        }
        free(temp);
        temp=NULL;
      }
    }
  }
  fclose(patientfp);
 // printf("The data insertion from the file %s has been completed\n", patientRecordsF);
  /* telos i anagnosi, to arxeio kleinei*/
  //printf("All the required structs have been created!\nIf you want to see the list's context please write '/PrintList'\nIf you want to see the HasTables' context please write '/PrintHashTables'\n");
  char *entry=NULL, *temp=NULL, *temph;
  size_t size;
  do{
    getline(&entry, &size, stdin);
    temp=malloc(strlen(entry)+1);
    strcpy(temp, entry);
    temph=malloc(strlen(entry)+1);
    strcpy(temph,entry);
    if(strncasecmp(temp, "/insertPatientRecord ",20)==0){
      temp+=21;
      int inserted=InsertList(&patientRecords, temp);
      if(inserted==1){
        int counter=0;
        char *disease=NULL, *country=NULL, *word=NULL;
        const char del[2] = " ";
        word=strtok(temph,del);
        while(word!=NULL) {
          ConvertToUpper(word);
          switch (counter) {
            case 4:
              disease=malloc(strlen(word)+1);
              strcpy(disease, word);
              break;
            case 5:
              country=malloc(strlen(word)+1);
              strcpy(country, word);
              break;
            default:
              break;
          }
          counter++;
          word= strtok(NULL,del);
        }
        struct ListNode *p=patientRecords;
        while(p->next!=NULL){
          p=p->next;
        }
        InsertHashTable(dHashTable, disease, bucketS, p);
        InsertHashTable(cHashTable, country, bucketS, p);
        free(disease);
        disease=NULL;
        free(country);
        country=NULL;
        printf("Record added\n");
        //printf("The entry was added succesfully. If you want to see the updated list enter '/PrintList'!\n");
      }
    }
    else if(strncasecmp(temp, "/recordPatientExit ",18)==0){
      temp+=18;
      const char del[2] = " ";
      char *word=NULL, *recordID=NULL, *exitDate=NULL, *entryDate=NULL, *exD=NULL;
      int counter=0;
      word=strtok(temph,del);
      while(word!=NULL) {
        ConvertToUpper(word);
        switch (counter) {
          case 1:
            recordID=malloc(strlen(word)+1);
            strcpy(recordID, word);
            break;
          case 2:
            RemoveSpaces(word);
            exitDate=malloc(strlen(word)+1);
            strcpy(exitDate, word);
            exD=malloc(strlen(exitDate)+1);
            strcpy(exD, exitDate);
            break;
          default:
            break;
        }
        counter++;
        word= strtok(NULL,del);
      }
      if(recordID==NULL || exitDate==NULL){
        //printf("--ERROR: Too few arguments!\n");
        printf("error\n");
        if(exD!=NULL) free(exD);
      }
      if(checkDate(entryDate, exD)==2){
        printf("error\n");
        //printf("--ERROR: Invalid date!\n");
        if(exD!=NULL) free(exD);
      }
      else {
        int found=InsertExitDate(&patientRecords, recordID, exitDate);
        if(found==1){
          printf("Record updated\n");
          //printf("The exit date of the entry with recordID %s was modified successfully. The updated entry is: \n", recordID);
         // ListSearchID(patientRecords, recordID);
        }
        else if(found==0) printf("Not found\n"); //printf("--ERROR: There is no entry with recordID %s\n", recordID);
        else printf("error\n"); //printf("--ERROR: The exit date should be greater (or at least equal) than the entry date\n");
      }
    }
    else if(strncasecmp(temp, "/globalDiseaseStats",19)==0){
      char *start=NULL, *end=NULL, *word=NULL, *temps=NULL, *tempe=NULL, *str;
      const char del[2]=" ";
      int counter=0, valid=0;
      word=strtok(temph,del);
      while(word!=NULL) {
        ConvertToUpper(word);
        switch (counter) {
          case 1:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              start=malloc(strlen(word)+1);
              strcpy(start, word);
              temps=malloc(strlen(word)+1);
              strcpy(temps, word);
            }
            break;
          case 2:
            if((str = strchr(word, '\n')) != NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              tempe=malloc(strlen(word)+1);
              strcpy(tempe, word);
              end=malloc(strlen(word)+1);
              strcpy(end, word);
            }
            break;
          default:
            break;
        }
        counter++;
        word= strtok(NULL,del);
      }
      if(start!=NULL && end==NULL){
        printf("error\n");
        //printf("--ERROR: Both start and end dates are required!\n");
      }
      else if(start==NULL && end==NULL){
        start=malloc(strlen("1-1-1500")+1);
        strcpy(start,"1-1-1500") ;
        end=malloc(strlen("30-12-2030")+1);
        strcpy(end, "30-12-2030");
      }
      else if(checkDate(temps, tempe)==0){
        printf("error\n");
        //printf("--ERROR: invalid dates\n");
        valid=1;
      }
      
      if(start!=NULL && end!=NULL && valid==0){
        char *c=NULL, *d=NULL;
        int k= DiseaseStatistics(dHashTable, start, end, c, d, bucketS);
      }
      if(start!=NULL) free(start);
      if(end!=NULL) free(end);
    }
    else if(strncasecmp(temp, "/diseaseFrequency",17)==0){
      ConvertToUpper(temp);
      char *start=NULL, *end=NULL, *word=NULL, *virusN=NULL, *country=NULL, *tempe=NULL, *temps=NULL, *str;
      const char del[2]=" ";
      int counter=0, valid=0;
      word=strtok(temph,del);
      while(word!=NULL) {
        ConvertToUpper(word);
        switch (counter) {
          case 1:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              virusN=malloc(strlen(word)+1);
              strcpy(virusN, word);
            }
            break;
          case 2:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              start=malloc(strlen(word)+1);
              strcpy(start, word);
              temps=malloc(strlen(word)+1);
              strcpy(temps, word);
            }
            break;
          case 3:
            if((str = strchr(word, '\n')) != NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              tempe=malloc(strlen(word)+1);
              strcpy(tempe, word);
              end=malloc(strlen(word)+1);
              strcpy(end, word);
            }
            break;
          case 4:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              country=malloc(strlen(word)+1);
              strcpy(country, word);
            }
            break;
          default:
            break;
        }
        counter++;
        word= strtok(NULL,del);
      }
      
      if(virusN==NULL || start==NULL || end==NULL){
        printf("error\n");
        //printf("--ERROR: Too few arguments!\n");
      }
      else{
        int valid=checkDate(temps, tempe);
        if(valid==0) printf("error\n"); //printf("--ERROR: Invalid dates\n");
        else{
          int k= DiseaseStatistics(dHashTable, start, end, country, virusN, bucketS);
        }
      }
    }
    else if(strncasecmp(temp, "/numCurrentPatients",19)==0){
      char *word=NULL, *temps=NULL, *tempe=NULL, *str, *disease=NULL;
      const char del[2]=" ";
      int counter=0, valid=0;
      word=strtok(temph,del);
      while(word!=NULL) {
        ConvertToUpper(word);
        switch (counter) {
          case 1:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              disease=malloc(strlen(word)+1);
              strcpy(disease, word);
            }
            break;
          default:
            break;
        }
        counter++;
        word= strtok(NULL,del);
      }
      CurrentPatients(dHashTable, disease, bucketS);
    }
    else if(strncasecmp(temp, "/topk-Diseases ",14)==0){
      ConvertToUpper(temp);
      char *start=NULL, *end=NULL, *word=NULL, *number=NULL, *country=NULL, *tempe=NULL, *temps=NULL, *str;
      const char del[2]=" ";
      int counter=0, valid=0, k=0;
      word=strtok(temph,del);
      while(word!=NULL) {
        ConvertToUpper(word);
        switch (counter) {
          case 1:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              number=malloc(strlen(word)+1);
              strcpy(number, word);
              k=atoi(number);
            }
            break;
          case 2:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              country=malloc(strlen(word)+1);
              strcpy(country, word);
            }
            break;
          case 3:
            if((str = strchr(word, '\n')) != NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              temps=malloc(strlen(word)+1);
              strcpy(temps, word);
              start=malloc(strlen(word)+1);
              strcpy(start, word);
            }
            break;
          case 4:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              tempe=malloc(strlen(word)+1);
              strcpy(tempe, word);
              end=malloc(strlen(word)+1);
              strcpy(end, word);
            }
            break;
          default:
            break;
        }
        counter++;
        word= strtok(NULL,del);
      }
      
      if( k==0 || country==NULL){
        printf("error\n");
        //printf("--ERROR: Too few arguments!\n");
      }
      else if(start!=NULL && end==NULL){
        printf("error\n");
        //printf("--ERROR: Both start and end dates are required!\n");
      }
      else if(start!=NULL && end!=NULL){
        int valid=checkDate(temps, tempe);
        if(valid==0) printf("error\n"); //printf("--ERROR: Invalid dates\n");
        else Topk(dHashTable, country, k, start, end, bucketS, 1);
      }
      else Topk(dHashTable, country, k, start, end, bucketS, 1);
    }
    else if(strncasecmp(temp, "/topk-Countries ",16)==0){
      ConvertToUpper(temp);
      char *start=NULL, *end=NULL, *word=NULL, *number=NULL, *disease=NULL, *tempe=NULL, *temps=NULL, *str;
      const char del[2]=" ";
      int counter=0, valid=0, k=0;
      word=strtok(temph,del);
      while(word!=NULL) {
        ConvertToUpper(word);
        switch (counter) {
          case 1:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              number=malloc(strlen(word)+1);
              strcpy(number, word);
              k=atoi(number);
            }
            break;
          case 2:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              disease=malloc(strlen(word)+1);
              strcpy(disease, word);
            }
            break;
          case 3:
            if((str = strchr(word, '\n')) != NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              temps=malloc(strlen(word)+1);
              strcpy(temps, word);
              start=malloc(strlen(word)+1);
              strcpy(start, word);
            }
            break;
          case 4:
            if((str=strchr(word, '\n'))!= NULL) *str = '\0';
            RemoveSpaces(word);
            if(strlen(word)>0){
              tempe=malloc(strlen(word)+1);
              strcpy(tempe, word);
              end=malloc(strlen(word)+1);
              strcpy(end, word);
            }
            break;
          default:
            break;
        }
        counter++;
        word= strtok(NULL,del);
      }
      if( k==0 || disease==NULL){
        printf("error\n");
        //printf("--ERROR: Too few arguments\n");
      }
      else if(start!=NULL && end==NULL){
        printf("error\n");
        //printf("--ERROR: Both start and end dates are required!\n");
      }
      else if(start!=NULL && end!=NULL){
        int valid=checkDate(temps, tempe);
        if(valid==0) printf("error\n"); //printf("--ERROR: Invalid dates\n");
        else{
          Topk(cHashTable, disease, k, start, end, bucketS, 0);
        }
      }
      else Topk(cHashTable, disease, k, start, end, bucketS, 0);
    }
    else if(strncasecmp(temp, "/PrintList ",10)==0){
      PrintList(patientRecords);
    }
    else if(strncasecmp(temp, "/PrintHashTables ",16)==0){
      PrintHashTable(cHashTable, entriesB, 0);
      PrintHashTable(dHashTable, entriesB, 1);
    }
    else if(strcasecmp(entry,"/exit\n")!=0){
      printf("error\n");
      //printf("--ERROR: Invalid input\n");
    }
  }while(strcasecmp(entry,"/exit\n")!=0 );
  printf("exiting\n");
  FreeList(patientRecords, patientRecords->next);
 // FreeHashTable(&cHashTable, entriesB);
 // FreeHashTable(&dHashTable, entriesB); 
 return 0;
}

