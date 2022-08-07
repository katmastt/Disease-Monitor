# Disease-Monitor

First exercise on Systems' Programming course - Departement of Informatics and Telecommunications, National & Kapodistrian Uneversity of Athens.
This is the C implementation of a disease monitor that can record, process and respond to queries for various disease cases. It consist of different kind of data structures, such as: Hashtables, Max heap, Linked lists and AVL BSStrees.

# RUN

**./diseaseMonitor -p patientRecordsFile –h1 diseaseHashtableNumOfEntries –h2 countryHashtableNumOfEntries –b bucketSize**

- patientRecordsFile: the file that contains the patients record. Each line is an entry and consists of the patients ID, name, surname, disease, country of admission and the date that the patient was admitted and released from the medical institute. For example a record could be the following: **125 Jon Dupont H1N1 USA 12-02-2016 15-02-2016**.
- diseaseHashtableNumOfEntries: the size of hashtable used for locating patient's information.
- countryHashtableNumOfEntries: the size of hashtable used for locating patient's information per country.
- bucketSize: the size in Bytes of the hashtables' buckets.

# IMPLEMENTATION

At the beginning, the check of the arguments takes place. In case some or all of them are faulty - if  diseaseHashtableNumOfEntries, countryHashtableNumOfEntries or bucketSize are not integer numbers or the patientRecordsFile does not exits, then the faulty ones are replaced with the default values from definitions.h. Then each line of the patientRecordsFile is red and the construction of the different data structure begins, so the data can be stored in memory. While each record is processed, the program also checks whether it is valid - if the record ID already exists or if the exit date precedes the entry date of the patient's admission then the entry is rejected. You can have a better understanding of all the different structures used on the following figure:  

<img width="950" alt="Screenshot 2022-08-07 at 8 58 47 PM" src="https://user-images.githubusercontent.com/110672874/183304660-76359cae-fe4c-49b9-9d01-f28af844dd21.png">

Once the data processing is over the user can execute different queries to monitor or update the system:

- **/globalDiseaseStats [date1 date2]**: date1 and date2 are optional. If date1 exists, then date2 should also exits or an error message appears. The program prints the total cases of each disease at the specific time. If data1 and data2 do not exist, then it prints the total cases of each disease.
- **/diseaseFrequency virusName [country] date1 date2**: country is optional. If country is specified, then the program prints for the specific virusName the total cases admitted during [date1 date2] at the specific country. Otherwise it prints the total virusName cases for all countries during [date1 date2] .
- **/topk-Diseases k country [date1 date2]**: date1 and date2 are optional. If date1 exists, then date2 should also exits or an error message appears. For the specified country, the program prints the diseases that constitute for the top k cases during [date1 date2] - if date1 and date2 exist.
- **/topk-Countries k disease [date1 date2]**: for the specific disease the program prints the countries that contains the top k cases of the disease.
- **/insertPatientRecord recordID patientFirstName patientLastName diseaseID entryDate [exitDate]**: exitDate is optional. The program inserts the specific entry to the system - updates all the necessary structures.
- **/recordPatientExit recordID exitDate**: the program adds the specific exitDate to the entry with ID recordID - updates all the necessary structures.
- **/numCurrentPatients [disease]**: disease is optional. If disease is specified, then the program prints the total number of patients, who are still treated with the specific disease. In different case it prints the total number of parients who are still in hospitals.
- **/exit**: the program terminates.

Different txt input files are provided - each contains different number of records. The report of the project, where the interconnection between the data structures are fully explained, is also uploaded on the repository in Greek.
