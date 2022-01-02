//Heap Based Program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
void Allocate_Dynamic_memory(int s_z)
{
  s_z++;
  size_collection = (int*)calloc(s_z , sizeof(int));
  timeLine_collecction = (int*)calloc(s_z , sizeof(int));
  
  Heap_Collection = malloc(s_z * sizeof(struct Customer*));
  for (int i = 0; i < s_z; i++)
  {
    Heap_Collection[i] = malloc(100 * sizeof(struct Customer)); 
  }
  explored_customers = malloc(100 * sizeof(struct Customer));
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
void quickSort(struct Customer *CustArray, int a, int b){
    int i, j, cen;
    struct Customer currentWord;
    if (a < b)
    {
        cen = a;
        i = a;
        j = b;
        while (i < j)
        {
            while (CustArray[j].cus_work > CustArray[cen].cus_work)
                j--;
            while (CustArray[i].cus_work < CustArray[cen].cus_work && i < b)
                i++;
            if (i < j)
            {
                currentWord = CustArray[i];
                CustArray[i] = CustArray[j];
                CustArray[j] = currentWord;
            }
        }
        currentWord = CustArray[cen];
        CustArray[cen] = CustArray[j];
        CustArray[j] = currentWord;
        quickSort(CustArray, a, j - 1);
        quickSort(CustArray, j + 1, b);
    }
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
    timeLine_collecction[i] = Heap_Collection[i][0].cur_arv;
    // leaving and recording in time way
    for (int j = 0; j < size; j++)
    {
        current_cus = Heap_Collection[i][j];
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
      printf("Corrupt infile");
      exit(0);
  }

  //variables to input data from file
  int number_of_customers = 0;
  int number_of_stylists = 0;
  char str[20];
 
  // reading number_of_customers and number_of_stylists from the filre
  fscanf(infile, "%d %d", &number_of_customers, &number_of_stylists);
  sty_number = number_of_stylists;

  // calling allocation function
  Allocate_Dynamic_memory(sty_number);

  // reading stylists names
  for (int i = 0; i < number_of_stylists; i++){
      fscanf(infile, "%s", sty_Names[i]);
  }

  // reading all customers data
  struct Customer current;
  for (int i = 0; i < number_of_customers; i++)
  {
      fscanf(infile, "%d %s %s %d %d", &current.cur_arv, &current.cus_Name, &current.sty_Name, &current.cus_loy, &current.cus_work);      
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
    free(Heap_Collection[i]);
  }
  free(Heap_Collection);
  free(explored_customers);
}

//=========================================
// Function used to run the functionality
// of the code
void RunCode()
{
    populate_heaps_file("in.txt");
    make_customers_exit();
    quickSort(explored_customers, 0, total_customers-1);
    showOutput("out.txt");
    freeMemory();
}


//=========================================
// Main Function
int main()
{
  RunCode();
  return 0;
}


//=========================================
