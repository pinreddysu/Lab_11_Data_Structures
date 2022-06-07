#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define size 30

typedef struct unique_node {   
  char data[size];
  int rep;
  int doc_num;
  struct unique_node *next;     
} node_t;

typedef struct node{
    int doc_id;
    struct node *next;
}doc_node_t;

typedef struct hash_element {
  char key[30];
  int collision;
  int doc_freq;
  doc_node_t *head;
  doc_node_t *rear;
} hash_element_t;

typedef struct ht{
   hash_element_t *enteries;
}ht_test;




node_t * create_node (char word[], int freq, int doc, node_t *ptr);
void insert (node_t **head, char word[], int num);
int search(node_t *ptr, char word[],node_t **prev, node_t **cur, int doc_id);
ht_test* ht_create(int tab_size);
void hash_table(node_t *head, ht_test *table, int table_size);
int hashcode(char key[], int tabsize);
int linear_probbing(ht_test *table, int conflict, int mode, int tabsize);
void print_hash_table (ht_test *table, int table_size);
doc_node_t * create_node_hash (int doc, doc_node_t *ptr);
void insert_hash (doc_node_t **head, int num);
int linear_probbing_search(ht_test *table, int conflict,int tabsize, char data[]);
void hash_search(ht_test *table, int table_size);

/*
 AUTHOR: Yash Nikumb, Suchith Pinreddy
 FILENAME: problem1.c
 SPECIFICATION: Implementing a hash table where collision resolution is performed with linear probing. Each
word entry in the table should have a head pointer that points to a linked list of document id’s that include the word
 FOR: CS 2413 Data Structures Section 002
*/

int main(void) {
  char data[90];
  char *temp;
  int table_size;
  
  //hash_element_t * make_hash_table (int size);;
  char temp1[90];
  FILE *infile;
  int counter1;
  int test = 0;
  int m;
  char unique_words[1000][30] = {'\0'};
  node_t *head2 = NULL;
  node_t *counter = NULL;
  for(m = 1; m<= 12; m++)
  {
    char name1[50];
    //char all_words[1000][30] = {'\0'};
    //char unique_words[1000][30] = {'\0'};
    sprintf(name1, "document%d.txt", m);
    infile  = fopen(name1, "r");
    int s = 0;
    while(fscanf(infile, "%s", data)==1){
      for(int k = 0; k<strlen(data); k++){
        data[k] = tolower(data[k]);
      }
      for (int i = 0, j; data[i] != '\0'; ++i) {

      // enter the loop if the character is not an alphabet
      // and not the null character
      while (!(data[i] >= 97 && data[i] <= 122) && !(data[i] == '\0')) {
         for (j = i; data[j] != '\0'; ++j) {

            // if jth element of line is not an alphabet,
            // assign the value of (j+1)th element to the jth element
            data[j] = data[j+1];
         }
         data[j] = '\0';
      }
   }
    //printf("Words: %s\n",data);
      if(strlen(data)>=4){
        //test++;
        insert(&head2,data,m);
      }
     }
  }
  counter = head2;
    while (counter != NULL){
        counter1++;
        counter = counter->next;
    }
  //printf("\nTotal words %d\n", counter1);
  //printf("Test %d\n",test);
  //table_size = counter1 * 1.5;
  table_size = 1129;
  ht_test *table50;
  table50 = ht_create(table_size);
  //printf("val %p\n",table50->enteries[0].head);
  hash_table(head2,table50, table_size);
  //print_hash_table (table50, table_size);
  hash_search(table50,table_size);
}


/* NAME: create_node
 PARAMETERS: char word[], int freq, int doc, node_t *ptr
 PURPOSE: The function creates a new node and returns the address
 PRECONDITION: Actual parameters [passed should be an array, 2 int values and node_t values 
 POSTCONDITION: It should create a node and return the address successfully 
*/
node_t * create_node (char word[], int freq, int doc, node_t *ptr) { //end of list comes first
  node_t *newp = malloc(sizeof(node_t)); // newp is intialized to structure
  //newp -> data = malloc(strlen(word)+1);  //char pointer will require separate memory allocation
  strncpy(newp -> data,word, size);
  newp->data[strlen(word)] = '\0';
  //newp->data = word;
  newp->rep = 1;
  newp->doc_num = doc;
  newp->next = ptr;
  //printf("node date %s", newp->data);
  return (newp); //returns address   pointers think of memory and
}
/* NAME: insert
 PARAMETERS: node_t **head, char word[], int num
 PURPOSE: It inserts the unique words which are greater than or equal to 4 and ignores duplicate words rom same excerpt
 PRECONDITION: Actual parameters passed should be an addressn an array and an integer
 POSTCONDITION: It should successfully insert unique words correctly
*/
void insert (node_t **head, char word[], int num) { //pointer to pointer since address is being inputted.
  // dynamically allocate the list node and initialize it to num and NULL

  node_t *cur;
  node_t *prev;
  //search(*head,word,&prev,&cur);
  if (search(*head,word,&prev,&cur,num)){
    cur->rep++;
  }
  else{
    node_t *newp = create_node(word,1,num,NULL);
    if(prev==NULL){
        *head = newp;
    }
    else{
        newp->next=cur;
        prev->next=newp;
    }
  }
}
/* NAME: search 
 PARAMETERS: node_t *head, char word[], node_t **prev, node_t **cur, int doc_id
 PURPOSE: The search program is used the algorithm to search the words and bring the output according to the program
 PRECONDITION: Actual variables should be an address, an array, another 2 address and an integer value accordingly 
 POSTCONDITION: The function should search and return the location of the word
*/
int search (node_t *head, char word[], node_t **prev, node_t **cur, int doc_id) {
  int found = 0;
  *prev = NULL;
  *cur = head;
  while (*cur != NULL && strncmp((*cur)->data,word,strlen(word)) != 0) {
    *prev = *cur;
    *cur = (*cur)->next;
  }
  if (*cur != NULL){
      if((*cur)->doc_num != doc_id){
          found = 0;
      }
      else{
          found = 1;
      }
  }
  return (found);
}
 
/* NAME: ht_create
 PARAMETERS: int tab_size
 PURPOSE: Allocates space for the enteries and null every value in the hash table 
 PRECONDITION: Actual parameter should be an int value 
 POSTCONDITION: It should return Null valur or a hash table correctly
*/
ht_test* ht_create(int tab_size){
    ht_test *table1 = malloc(sizeof(ht_test));
    if(table1==NULL){
        return NULL;
    }
    table1->enteries = calloc(tab_size, sizeof(hash_element_t));
    if(table1->enteries == NULL){
        free(table1);
        return NULL;
    }
    return table1;
}
/* NAME: hash_table
 PARAMETERS: node_t *head, ht_test *table, int table_size
 PURPOSE: It is used to insert the words in the hash table 
 PRECONDITION: Actual variables should be a head node, hash table and an int value
 POSTCONDITION: It should correctly display all the elements in the hash table 
*/
void hash_table(node_t *head, ht_test *table, int table_size){
  node_t *cur;
  cur = head;
  int test;
  int index;
  while(cur!=NULL){
    
    index = hashcode(cur->data, table_size);

    if(strncmp(table->enteries[index].key, cur->data,30)!=0){
      //col++;
      //table->enteries[index].collision +=1;
      index = linear_probbing(table,index,1,table_size);
    }
      strncpy(table->enteries[index].key, cur->data, size);
      insert_hash(&(table->enteries[index].head),cur->doc_num);
      table->enteries[index].key[strlen(cur->data)] = '\0';
    table->enteries[index].rear = table->enteries[index].head;
    cur = cur ->next;
  }
  
} 
 
/* NAME: hashcode
 PARAMETERS: char key[], int tabsize
 PURPOSE: It generates index position 
 PRECONDITION: actual parameters should be an char array and the int value 
 POSTCONDITION: It should generate index position of element 
*/
int hashcode(char key[], int tabsize){
  char temp[size];
  unsigned long long int pos = 0;
  strncpy(temp, key, 30);
  temp[strlen(key)] = '\0';
   for(int i = 0; i < strlen(temp); i++){
       pos = pos * 97 + temp[i];
   }
  pos = pos%tabsize;
  return pos;
}
/* NAME: linear_probbing
 PARAMETERS: ht_test *table,int conflict, int mode, int tabsize
 PURPOSE: If there is a collision it then it provides the new index position in the array so that the word can be inserted   
 PRECONDITION: It should be a hash table and 3 int value 
 POSTCONDITION: It should correctly display the index position when collison accurs 
*/
int linear_probbing(ht_test *table,int conflict, int mode, int tabsize){
  char empty[30] = {'\0'};
  int temp = conflict;
  int empty_index;
  if(mode){
    while(strncmp(table->enteries[conflict].key,empty,30) != 0)    {
      table->enteries[conflict].collision=1;
      conflict++;
      
      if(conflict>=tabsize){
        conflict = 0;
      }
      if(temp==conflict){
        //printf("ARRay full");
        break;
      }
    }
    if(strncmp(table->enteries[conflict].key, empty,30)==0){
      empty_index = conflict;
    }
    }
 
  return empty_index;
} 

/* NAME: linear_probbing_search
 PARAMETERS: ht_test *table, int conflict,int tabsize, char data[]
 PURPOSE: It rints the linear_probbing function 
 PRECONDITION: The actual parameter should be a hash table and two int values and a char array 
 POSTCONDITION: It should print the information correctly 
*/
int linear_probbing_search(ht_test *table, int conflict,int tabsize, char data[]){
  int temp = conflict;
  int flag =1;
  while(strncmp(table->enteries[conflict].key,data, size) !=0){
    conflict++;
    if(conflict>=tabsize){
      conflict = 0;
    }
    if(temp==conflict){
      //printf("ARRay full");
      return tabsize+2;
      break;
    }
    }
  if((strncmp(table->enteries[conflict].key, data, size)==0)&&flag){
    return conflict;
  }
  return 0;
} 
/* NAME: print_hash_table
 PARAMETERS: ht_test *table, int table_size
 PURPOSE: It prints hash table 
 PRECONDITION: the actual parameter should be 2 int values and char array value 
 POSTCONDITION: Prints hash table correctly 
*/
void print_hash_table (ht_test *table, int table_size) {
  int i;
  int counter1 =0;
  int counter = 0;
  int test = 0;
  doc_node_t *cur;
  doc_node_t *cur1;
  char empty[30] = {'\0'};
  printf("\nHash Table:\n");
  for (i = 0; i < table_size; i++) {
    cur = table->enteries[i].head;
    cur1 = table->enteries[i].head;
    if (strncmp(table->enteries[i].key, empty,30) != 0) {
       test++;
      printf("  %4d: %s %d\t",i,table->enteries[i].key,table->enteries[i].collision);
      while(cur1!=NULL){
        counter++;
        cur1 = cur1->next;
      }
      printf("\tDf = %d\t",counter);
      counter = 0;
      while(cur!=NULL){
        printf("\t ->doc %d\t",cur->doc_id);
        cur = cur->next;
      }
      printf("\n");
    }
  }
  for(int j = 0; j < table_size; j++){
    if(strncmp(table->enteries[j].key, empty,30)!=0){
      counter1 = table->enteries[j].collision + counter1;
      
    }
  }
  //printf("Collisions %d\n",counter1);
  //printf("TEST %d\n",test);
} 
/* NAME: create_node_hash
 PARAMETERS: int doc, doc_node_t *ptr
 PURPOSE: it creates a node hash table
 PRECONDITION: Actual parameters should be an integer and a node 
 POSTCONDITION: It should correctly create new pointer node
*/
doc_node_t * create_node_hash (int doc, doc_node_t *ptr) { //end of list comes first
  doc_node_t *newp = (doc_node_t *)malloc(sizeof(doc_node_t)); // newp is intialized to structure
  //newp -> data = malloc(strlen(word)+1);  //char pointer will require separate memory allocation
  newp->doc_id = doc;
  newp->next = ptr;
  //printf("node date %s", newp->data);
  return (newp); //returns address   pointers think of memory and
}
/* NAME: insert_hash 
 PARAMETERS: doc_node_t **head, int num
 PURPOSE: It inserts a Hash table using create node function
 PRECONDITION: It should be the node and an integer 
 POSTCONDITION: It should correctly insert the hash table 
*/
void insert_hash (doc_node_t **head, int num) {
  // dynamically allocate the list node and initialize it to num and NULL
  doc_node_t *newp = create_node_hash(num, NULL);
  doc_node_t *cur;
  if (*head == NULL) {            // check for an empty list
    *head = newp;                 // set head to newp
  }
  else {
    cur = *head;
    while (cur->next != NULL)     // traverse to the last node in the list
      cur = cur->next;
    cur->next = newp;             // set the last node's next pointer to newp
  }
} 
/* NAME: hash_search
 PARAMETERS: ht_test *table, int table_size
 PURPOSE: It checks the words which are in the query.txt with the words in the hash table. It prints the info. 
 PRECONDITION: It should have a hash table and an integer as an actual variable
 POSTCONDITION: It should properly print the information in the program.
*/
void hash_search(ht_test *table, int table_size){
  char data[size];
  int data_arra[13] = {0};
  int index = 0;
   FILE *infile  = fopen("query.txt", "r");
    int s = 0;
    while(fscanf(infile, "%s", data)==1){
      for(int k = 0; k<strlen(data); k++){
        data[k] = tolower(data[k]);
      }
      for (int i = 0, j; data[i] != '\0'; ++i) {
      while (!(data[i] >= 97 && data[i] <= 122) && !(data[i] == '\0')) {
         for (j = i; data[j] != '\0'; ++j) {
            data[j] = data[j+1];
         }
         data[j] = '\0';
      }
   }
  index = hashcode(data, table_size);
  if(strncmp(table->enteries[index].key, data,30)==0){
    while(table->enteries[index].rear!=NULL){
      data_arra[table->enteries[index].rear->doc_id] +=1;
      table->enteries[index].rear = table->enteries[index].rear->next;
    }
  }
  else{
    index = linear_probbing_search(table,index,table_size,data);
    if(index == table_size+2){
      /*for(int p =1; p<=12; p++){
        data_arra[p] = 0;
      }*/
    }
    else{
    while(table->enteries[index].rear!=NULL){
      data_arra[table->enteries[index].rear->doc_id] +=1;
      table->enteries[index].rear = table->enteries[index].rear->next;
  }
    }
}
  }
  fclose(infile);
  FILE *infile1  = fopen("query.txt", "r");
     char chunk[128];
 
     while(fgets(chunk, sizeof(chunk), infile1) != NULL) {
         printf("Processing Query: %s\n", chunk);
         //fputs("|*\n", stdout);  // marker string used to show where the content of the chunk array has ended
 }
  int counter = 0;
  for(int k = 1; k<=12; k++){
    if(data_arra[k]!=0){
      counter++;
      }
    
  }
  if(counter ==1){
      printf("%d Document\n", counter);
    }
    else{
      printf("%d Documents\n", counter);
    }
  for(int k = 1; k<=12; k++){
    if(data_arra[k]!=0){
      if(data_arra[k]>1){
        printf("\tDocument\t\t%d\t-\t\t%d\tterms\n",k,data_arra[k]);
      }
      else{
        printf("\tDocument\t\t%d\t-\t\t%d\tterm\n",k,data_arra[k]);
      }
      
  }
    }
  }
  
/*
*****Resources:
1) https://www.programiz.com/c-programming/examples/remove-characters-string  
2) https://benhoyt.com/writings/hash-table-in-c/ 


***** Team members: 
1) Suchith Pinreddy:-
a) node_t * create_node (char word[], int freq, int doc, node_t *ptr); = Implemeted 
b) void insert (node_t **head, char word[], int num);=implemeted 
c) int search(node_t *ptr, char word[],node_t **prev, node_t **cur, int doc_id); = modified
e) ht_test* ht_create(int tab_size);= imlplemented 
f) void hash_table(node_t *head, ht_test *table, int table_size); = Implemeted 
g) int hashcode(char key[], int tabsize); = Modified 
h) int linear_probbing(ht_test *table, int conflict, int mode, int tabsize); = Modified 
i) void print_hash_table (ht_test *table, int table_size);= modified 
j) doc_node_t * create_node_hash (int doc, doc_node_t *ptr); = implemeted
k) void insert_hash (doc_node_t **head, int num); = modified 
l) int linear_probbing_search(ht_test *table, int conflict,int tabsize, char data[]); = Modified
m) void hash_search(ht_test *table, int table_size); =Implemeted  

2) Yash Nikumb:- 
a) node_t * create_node (char word[], int freq, int doc, node_t *ptr); = mpdified
b) void insert (node_t **head, char word[], int num);= Modified 
c) int search(node_t *ptr, char word[],node_t **prev, node_t **cur, int doc_id); = Implemeted 
e) ht_test* ht_create(int tab_size);= modified 
f) void hash_table(node_t *head, ht_test *table, int table_size); = modified 
g) int hashcode(char key[], int tabsize); = implemeted 
h) int linear_probbing(ht_test *table, int conflict, int mode, int tabsize); = implemented 
i) void print_hash_table (ht_test *table, int table_size); = implemented
j) doc_node_t * create_node_hash (int doc, doc_node_t *ptr); = modified 
k) void insert_hash (doc_node_t **head, int num); = implemeted  
l) int linear_probbing_search(ht_test *table, int conflict,int tabsize, char data[]); = implemeted 
m) void hash_search(ht_test *table, int table_size); = modified  

******Test Cases and Status
1. 1 document excerpt file and hash table – passed
2. 2 to 12 document excerpt files and hash table – passed
3. searching for words in the hash table – passed
4. searching for words not in the hash table – passed
5. searching for words in and not in the hash table – passed

********Hashing Analysis
1. Give at least two hash table sizes that are a power of 2, such as 1024, and how many collisions occurred after hashing the words from the 12 excerpt document files. 
Ans. With 1024 table size, There are 399 collisions were occured. With 2048 table size, there are 133 collisions 
2. Give at least two hash table sizes that are a prime number, such as 1031, and how many collisions occurred after hashing the words from the 12 excerpt document files. 
Ans. With 1031 table size, There are 335 collisions were occured. With 1291 table size, there are 220 collisions 
3. What size for the hash table did you choose and why? 
Ans. We chose 1129 for the hash table becuase we want the size 1.5- 2 times the unique word size and goes well with division hash function because of the uniformity in the keys.
4. What hash function did you choose and why? 
Ans. We used the division method because it gives more uniformity in putting keys.
5. What is the largest primary cluster that occurred in the hash table you chose? 
Ans. The largest primary cluster is 10. 
6. What is the overall order of the program (itemize each significant part and show the total rather than give one order)? Use m for the number of excerpt document files, n for the approximate number of words in each file, and q for the number of queries. 
Ans.Time complexity of every bit: 

void insert (node_t **head, char word[], int num); = O(n) linear
int search(node_t *ptr, char word[],node_t **prev, node_t **cur, int doc_id); = O(n) linear
void hash_table(node_t *head, ht_test *table, int table_size); =
O(n) linear
int hashcode(char key[], int tabsize); = O(n) linear
int linear_probbing(ht_test *table, int conflict, int mode, int tabsize); = O(n) linear
void insert_hash (doc_node_t **head, int num); = O(n) linear
int linear_probbing_search(ht_test *table, int conflict,int tabsize, char data[]); = O(n) linear
void hash_search(ht_test *table, int table_size); = O(q^3) since it is for query.txt


7. What is the big O of the overall storage requirements for the program (itemize each significant storage piece used for the data and show the total rather than give just one order)? Use m for the number of excerpt document files, n for the approximate number of words in each file, and q for the number of queries. 
Ans. In this program m is a constant value which is 12. Here the constant doesn't count in the over all storage . Therefore, the overall storage is O(n) and it never depends on the no. of queries.   

Overall storage complexity 
void insert (node_t **head, char word[], int num); = O(n) linear
int search(node_t *ptr, char word[],node_t **prev, node_t **cur, int doc_id); = O(n) linear
void hash_table(node_t *head, ht_test *table, int table_size); =
O(n) linear
int hashcode(char key[], int tabsize); = O(n) linear
int linear_probbing(ht_test *table, int conflict, int mode, int tabsize); = O(n) linear
void insert_hash (doc_node_t **head, int num); = O(n) linear
int linear_probbing_search(ht_test *table, int conflict,int tabsize, char data[]); = O(n) linear
void hash_search(ht_test *table, int table_size); = O(q) linear 
*/