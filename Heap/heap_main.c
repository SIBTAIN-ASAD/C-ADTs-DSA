//Heap Based Program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>


//=========================================
// Customer Class to store customers Data |
//=========================================
struct Customer {
  // Customer Name
  char cus_Name[20];
  // Customer Arival
  int cur_arv;
  // Customer Loyality
  int cus_loy;
  // Customer time of cutting
  int cus_work;
  // Its Styler
  char sty_Name[20];
  // time at which customer leaves
  int completionTime;
};


//=========================================
// PROGRAM DATA SET                       |
//=========================================
//                                       //
// Array of heaps for every Stylist      //
struct Customer ** Heap_Collection;      //
int sty_number;                          //
// explored Customers                    //
struct Customer *explored_customers;     //
int total_customers = 0;                 //
// size of each heap's customers         //
int * size_collection;                   //
// heap time_line                        //
int * timeLine_collecction;              //
// store names of stylists               //
char sty_Names[10][20];                  //
                                         //
//=========================================


//=========================================
// Function used to initilize all dynamic
// memory for the program
static void input_error(const char *message)
{
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}

static void *checked_calloc(size_t count, size_t width)
{
  if (count > SIZE_MAX / width) input_error("Input is too large");
  void *memory = calloc(count ? count : 1, width);
  if (!memory) input_error("Unable to allocate memory");
  return memory;
}

void Allocate_Dynamic_memory(int s_z, int customers)
{
  size_collection = checked_calloc((size_t)s_z, sizeof(int));
  timeLine_collecction = checked_calloc((size_t)s_z, sizeof(int));
  Heap_Collection = checked_calloc((size_t)s_z, sizeof(*Heap_Collection));
  for (int i = 0; i < s_z; i++)
    Heap_Collection[i] = checked_calloc((size_t)customers, sizeof(struct Customer));
  explored_customers = checked_calloc((size_t)customers, sizeof(struct Customer));
}

/* Read a whole token: a width-limited fscanf alone can split a long name. */
static void read_token(FILE *file, char *buffer, size_t capacity)
{
  int ch;
  do { ch = fgetc(file); } while (ch != EOF && isspace((unsigned char)ch));
  if (ch == EOF) input_error("Incomplete input");
  size_t length = 0;
  do {
    if (length + 1 >= capacity) input_error("Input token is too long");
    buffer[length++] = (char)ch;
    ch = fgetc(file);
  } while (ch != EOF && !isspace((unsigned char)ch));
  buffer[length] = '\0';
}

static int read_number(FILE *file)
{
  char token[32];
  read_token(file, token, sizeof(token));
  unsigned int value = 0;
  for (size_t i = 0; token[i]; i++) {
    if (token[i] < '0' || token[i] > '9') input_error("Expected a nonnegative integer");
    unsigned int digit = (unsigned int)(token[i] - '0');
    if (value > ((unsigned int)INT_MAX - digit) / 10) input_error("Integer is too large");
    value = value * 10 + digit;
  }
  return (int)value;
}

//=========================================
// Function used to compare the customers 
// based on 
// 1. loyality, 2. preference 3. Alphbatical
int compareTo(struct Customer cus1, struct Customer cus2)
{
    if(cus1.cus_loy > cus2.cus_loy)  {return 1;}
    else
    {
      if(cus1.cus_loy < cus2.cus_loy) { return -1;}
      else {
              if(strcmp(cus1.sty_Name, cus2.sty_Name) != 0){
                if(strcmp(cus1.sty_Name, "NONE") == 0){return -1;}
                else {return 1; }
              } else{return (strcmp(cus1.sty_Name, cus2.sty_Name));}
          }
      }
}


//=========================================
// Function used to replace the
// customers data
void replace_with(struct Customer *cus1, struct Customer *cus2){
  struct Customer data = *cus2;
  *cus2 = *cus1;
  *cus1 = data;
}


//=========================================
// Function used to place the give to the
// correct position in the heap
void heapify(struct Customer *Heap_Collection, int size, int i){

  if (size != 1){
    int atLength = i;
    int r = 2 * i + 2;
    int l = 2 * i + 1;
  
    if (l < size && compareTo(Heap_Collection[l],Heap_Collection[atLength]) > 0){
      atLength = l;
    }

    if (r < size && compareTo(Heap_Collection[r], Heap_Collection[atLength]) > 0){
      atLength = r; 
    }

    if (atLength != i){
      replace_with(&Heap_Collection[i], &Heap_Collection[atLength]);
      heapify(Heap_Collection, size, atLength);
    }
  }
}

//=========================================
// Function used to sort the explored 
// customer array bases on their
// departure time
static int compareCompletionTime(const void *left, const void *right)
{
    const struct Customer *a = left;
    const struct Customer *b = right;
    return (a->cus_work > b->cus_work) - (a->cus_work < b->cus_work);
}

void quickSort(struct Customer *CustArray, int a, int b)
{
    if (a >= b) return;
    qsort(CustArray + a, (size_t)(b - a + 1), sizeof(*CustArray), compareCompletionTime);
}

//=========================================
// Function used to add the given customer 
// to the respected heap 
void append_in_heap(struct Customer *Heap_Collection, struct Customer newNum, int *size)
{
  // add in empty heap
  if (*size == 0){
    Heap_Collection[0] = newNum;
    *size += 1;
  }else{ // add in last and then heapify
    Heap_Collection[*size] = newNum;
    *size += 1;
    for (int i = *size / 2 - 1; i >= 0; i--)
    {
      heapify(Heap_Collection, *size, i);
    }
  }
}


//=========================================
// Function used to get where the given 
// customer should be insert based on
// preference , then least avaliable 
// seats
int get_respected_heap_index(struct Customer data){
  
    // valculating minimum heap
    int min = size_collection[0];
    int min_index = 0;
    for (int i = 0; i < sty_number; i++){
        if(size_collection[i] < min) {
            min = size_collection[i];
            min_index = i;
        }
    }

    // comparing with preferance
    for (int i = 0; i < sty_number; i++){
        if(strcmp(data.sty_Name, sty_Names[i]) == 0){ return i; }
    }

    return min_index;    
}

//=========================================
// Function used to remove all the 
// customers in cronological order
void make_customers_exit(){
    struct Customer current_cus;

    // swap as heap switch the currently working
    //  customer with new one as of due to 
    // higher loyality
    for(int i = 0; i < sty_number; i++)
    {
      if(size_collection[i] == 2 && Heap_Collection[i][0].cur_arv > Heap_Collection[i][1].cur_arv){
        replace_with(&Heap_Collection[i][0], &Heap_Collection[i][size_collection[i]-1]);
      }


    int size = size_collection[i];
    if (size == 0) continue;
    timeLine_collecction[i] = Heap_Collection[i][0].cur_arv;
    // leaving and recording in time way
    for (int j = 0; j < size; j++)
    {
        current_cus = Heap_Collection[i][j];
        if (current_cus.cus_loy > INT_MAX - current_cus.cus_work / 10 ||
            current_cus.cus_work > INT_MAX - timeLine_collecction[i])
          input_error("Customer result exceeds integer range");
        current_cus.cus_loy = current_cus.cus_loy + current_cus.cus_work/10;
        current_cus.cus_work = current_cus.cus_work + timeLine_collecction[i];
        timeLine_collecction[i] = current_cus.cus_work;
        explored_customers[total_customers++] = current_cus;
    }
    }
}

//=========================================
// Function used to populate all data sets
// by using input file given to it
void populate_heaps_file(const char* file)
{
  // making file variable
  FILE* infile;
  infile = fopen(file, "r");

  // file open validation
  if (!infile){
      input_error("Unable to open input file");
  }

  //variables to input data from file
  int number_of_customers = 0;
  int number_of_stylists = 0;
 
  // reading number_of_customers and number_of_stylists from the filre
  number_of_customers = read_number(infile);
  number_of_stylists = read_number(infile);
  if (number_of_stylists < 1 || number_of_stylists > 10)
    input_error("Expected between 1 and 10 stylists");
  sty_number = number_of_stylists;

  // calling allocation function
  Allocate_Dynamic_memory(sty_number, number_of_customers);

  // reading stylists names
  for (int i = 0; i < number_of_stylists; i++){
      read_token(infile, sty_Names[i], sizeof(sty_Names[i]));
  }

  // reading all customers data
  struct Customer current = {0};
  for (int i = 0; i < number_of_customers; i++)
  {
      current.cur_arv = read_number(infile);
      read_token(infile, current.cus_Name, sizeof(current.cus_Name));
      read_token(infile, current.sty_Name, sizeof(current.sty_Name));
      current.cus_loy = read_number(infile);
      current.cus_work = read_number(infile);
      int cur_index = get_respected_heap_index(current);
      int put = 0;
      while(1)
      {
        current.sty_Name[put] = sty_Names[cur_index][put];
        if (sty_Names[cur_index][put] == '\0')
        {
          break;
        }
        put++;
      }

      // inserting customer to the heap
      append_in_heap(Heap_Collection[cur_index], current, &size_collection[cur_index]);
  }

  //closing file
  fclose(infile);
}

//=========================================
// Function used to print output using
//  explored set 
void showOutput(const char * outfile)
{
  FILE * file2;
  file2 = fopen(outfile, "w");
  if (!file2) input_error("Unable to open output file");
  for (int i = 0; i < total_customers; i++)
  {
      printf("%s %d %d %s\n", explored_customers[i].cus_Name, explored_customers[i].cus_work, explored_customers[i].cus_loy, explored_customers[i].sty_Name);    
      fprintf(file2, "%s %d %d %s\n", explored_customers[i].cus_Name, explored_customers[i].cus_work, explored_customers[i].cus_loy, explored_customers[i].sty_Name);    
  }
  fclose(file2);
}

//=========================================
// Function used to free up all the
// dynamic memory from heap
void freeMemory()
{
  free(size_collection);
  free(timeLine_collecction);
  for (int i = 0; i < sty_number; i++)
  {
    if (Heap_Collection) free(Heap_Collection[i]);
  }
  free(Heap_Collection);
  free(explored_customers);
}

//=========================================
// Function used to run the functionality
// of the code
void RunCode()
{
    if (atexit(freeMemory) != 0) input_error("Unable to register memory cleanup");
    populate_heaps_file("in.txt");
    make_customers_exit();
    quickSort(explored_customers, 0, total_customers-1);
    showOutput("out.txt");
}


//=========================================
// Main Function
int main()
{
  RunCode();
  return 0;
}


//=========================================
