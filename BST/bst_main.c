// COP 3502 //
/*
This program input some data from files, make tree and then store the result in ouput file
This program also make the array of stored values and show them by sorting them on their frequency number
This program also show the sorted and unsorted data on the concole screen
*/

#include <stdio.h>
#include<string.h>
#include <stdlib.h>


//============================================================================
//  Tree_Node, ValueNode Class
//============================================================================
struct tree_Node{ //tree node struct

    struct ValueNode{ //Value struct to hold values
	    char Node_value[20];
	    int fre_of_value;
    };

	struct ValueNode NodeValue; // node of value      
	struct tree_Node* left;		                         
	struct tree_Node* right;

	int depth;                  
};

/*
 function prototypes !!!
// */
void run_some_test_files();
void run_test(const char*, const char* );
void ArrayFunctionality(FILE **);
void QuickSort(struct ValueNode* ,int ,int );
void AddtoFile(struct ValueNode* , int , FILE * );
void openFiles(FILE** , FILE** , const char* , const char*  );
int compareTo(struct ValueNode* , struct ValueNode* );
void FreeHeap(struct tree_Node* );
struct tree_Node* findRecursive(struct tree_Node* , const char* );
struct tree_Node* find(const char* );
void printValueArray(struct ValueNode* , int );
void addValueToArray(struct ValueNode* , int* , struct tree_Node * );
void add(const char* );
struct tree_Node*  addRecursive(struct tree_Node** , struct tree_Node* );

//============================================================================
// Head of Binary Search Tree
//============================================================================
struct tree_Node* root = NULL;	


int compareTo(struct ValueNode* NodeA, struct ValueNode* NodeValue){ //function used to compare two values nodes based on their frequency

    if(NodeA->fre_of_value == NodeValue->fre_of_value)
    {
        if((strcmp(NodeA->Node_value, NodeValue->Node_value)) <= 0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    if(NodeA->fre_of_value >= NodeValue->fre_of_value){
        return -1;
    }
    else{
        return 1;
    }
}



void addValueToArray(struct ValueNode* my_arr, int* array_size, struct tree_Node * Node){ // to populate array using tree nodes

    if (Node != NULL){ 
        addValueToArray(my_arr, array_size, Node->left);
        my_arr[(*array_size)++] = Node->NodeValue;
        addValueToArray(my_arr, array_size, Node->right);
    }
}

void printValueArray(struct ValueNode* arr, int array_size){ // to display array on console

    for(int i = 0; i < array_size; i++)
    {
        printf("%s %d", arr[i].Node_value, arr[i].fre_of_value);
        printf("\n");
    }
}


void AddtoFile(struct ValueNode* arr, int array_size, FILE * f2){ // to save array in file

    for(int i = 0; i < array_size; i++)
    {
        fprintf(f2,"%s %d\n", arr[i].Node_value, arr[i].fre_of_value);
    }
}   




struct tree_Node* findRecursive(struct tree_Node* Node, const char* value){ // helper function to find value recursively

    if (Node == NULL){
      return NULL;
    }
    else{
        if(strcmp(Node->NodeValue.Node_value, value) == 0){ // checking for value of same
            return Node;
        }
        else{
            if(strcmp(Node->NodeValue.Node_value, value) < 0){ // checking samll
                return findRecursive(Node->right, value);
            }
            else{
                return findRecursive(Node->left, value); //else
            }
        }
    }
}


struct tree_Node* find(const char* value){ //to find the given string in the tree
    return findRecursive(root, value);
}

void FreeHeap(struct tree_Node* Node){ // to remove all nodes of tree from heap
    if(Node != NULL){
        FreeHeap(Node->left);
        FreeHeap(Node->right);
        free(Node);
    }
}

void openFiles(FILE** f1, FILE** f2, const char* input_file, const char* output_file ){ //to open and validation of files

    *f1 = fopen(input_file,"r");
    *f2 = fopen(output_file,"w");

    if (*f1 == NULL){
        printf("*** ERROR! Input File ***");
        exit(0);
    }
    if (*f2 == NULL){
        printf("*** ERROR! Output File ***");
        exit(0);
    }


}
void QuickSort(struct ValueNode* my_array,int start,int end){ // sorting algorithm to sort array

   int i, j, pivot;

   struct ValueNode NodeA; 

   if( start < end){

      pivot = start;
      i = start;
      j = end;

      while( i < j){

         while((compareTo(&my_array[j], &my_array[pivot])) == 1)
            j--;

         while((compareTo(&my_array[i], &my_array[pivot])) == -1 && i < end) 
            i++;

         if( i < j){

            NodeA = my_array[i];
            my_array[i] = my_array[j];
            my_array[j] = NodeA;

        }
    }

      NodeA = my_array[pivot];
      my_array[pivot] = my_array[j];
      my_array[j] = NodeA;

      QuickSort(my_array,start,j-1);
      QuickSort(my_array,j+1,end);
   }
}

struct tree_Node*  addRecursive(struct tree_Node** NodeA, struct tree_Node* NodeValue){ // helper function to add value recursively
	if(*NodeA == NULL){
        *NodeA = NodeValue;
		return *NodeA;
	}
    else{

		if(strcmp(NodeValue->NodeValue.Node_value, (*NodeA)->NodeValue.Node_value) == 0){ //check for same
			(*NodeA)->NodeValue.fre_of_value++;
		}
		else{

			if(strcmp(NodeValue->NodeValue.Node_value, (*NodeA)->NodeValue.Node_value) > 0){ // for greater
                NodeValue->depth = (*NodeA)->depth + 1;
				(*NodeA)->right = addRecursive(&((*NodeA)->right), NodeValue);
			}
			else{ //else conditon
                NodeValue->depth = (*NodeA)->depth + 1;
				(*NodeA)->left = addRecursive(&(*NodeA)->left, NodeValue);
			}
		}
	    return *NodeA;
	}
		

}


void add(const char* value){ //to add the given string in the tree

	struct tree_Node *NodeValue;

    NodeValue = (struct tree_Node*) malloc(sizeof(struct tree_Node)); //creating tree node

    int i;
	for (i = 0; i < 20 && value[i] != '\0'; i++){ //storing data in the node

		NodeValue->NodeValue.Node_value[i] = value[i];
	}

    NodeValue->NodeValue.Node_value[i] = '\0'; //termnating condition

    NodeValue->right = NULL;
    NodeValue->left = NULL;
    NodeValue->depth = 0;
    NodeValue->NodeValue.fre_of_value = 1;
	addRecursive(&root, NodeValue); //calling add recursive function
}


void ArrayFunctionality(FILE **f2){ //driver function to perform array actions

    struct ValueNode arr[100]; // array to structs
    int val = 0;
    int* array_size = &val;

    addValueToArray(arr, array_size, root); // adding values to array
    printf("\n---------------------\nArray befor sorting\n\n");

    printValueArray(arr, val); //printing array before sorting

    QuickSort(arr,0, val-1);

    printf("\n---------------------\nArray After sorting\n\n");

    printValueArray(arr, val); //printing array after sorting
    AddtoFile(arr, val, *f2);
}


void run_test(const char* input_file, const char* output_file){ // driver of the program (get file names and run them)

    printf("\n************ EXECUING TEST CASE ***************\n");
    FILE *f1;
    FILE *f2;
    openFiles(&f1,&f2, input_file, output_file); //opening files
    int commands, action; //input command
    char * value; 
    value = (char*) malloc(20 * sizeof(char));  //char array to input string
    struct tree_Node* NodeA; //  node to store finded address
    fscanf(f1,"%d", &commands); 
    for(int i = 0; i < commands; i++)
    {
        fscanf(f1,"%d", &action);
        fscanf(f1,"%s", value);

        switch(action){
            case 1:
                add(value);
                break;
            case 2: 
                NodeA = find(value);
                if(NodeA == NULL){
                    printf("%d %d", -1,-1);
                    printf("\n");
                    fprintf(f2 ,"%d %d", -1,-1);
                    fprintf(f2,"\n");
                }
                else{
                    fprintf(f2 ,"%d %d\n", NodeA->NodeValue.fre_of_value,NodeA->depth);
                    printf("%d %d\n", NodeA->NodeValue.fre_of_value,NodeA->depth);
                }
                break;
            default: 
                printf("Invalid Command");
                printf("\n");
                break;
        }
    }
    ArrayFunctionality(&f2);
    FreeHeap(root);
    root = NULL;
    free(value);
    fclose(f1);
    fclose(f2);
}


void run_some_test_files(){ //write the files to test them in this functions
    run_test("in1.txt", "out1.txt");
}

int main() { //main
	
    run_some_test_files();
	return 0;
}