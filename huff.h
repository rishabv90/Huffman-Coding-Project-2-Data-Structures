#include <stdio.h>
#include <stdlib.h>


#define NOO -1

typedef struct freqList
{
  int freq;
  int char_val;
  struct freqList * next;
} Node;

typedef struct treenode{
  int character;
  int weight;
  struct treenode * left;
  struct treenode * right;
} TreeNode;

typedef struct listnode{
  struct listnode * next;
  TreeNode * tree_ptr;
} ListNode;

//FUNCTION declarations
void get_freq(char *Filename, int * frequency);
Node * build_list(int * freq);
ListNode * build_ListNode(Node * freqList);
int height_Tree(TreeNode * treen);
int Bit_write(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curbyte);
void char_bits(FILE * outfptr, int ch, unsigned char * whichbit, unsigned char * curbyte);
void header_tree(TreeNode * tn, unsigned int numChar, char * Filename);
int compression(char * infile, char * outfile, int ** array3d, int * mapping);
int fix_bits(FILE * fptr, unsigned char * wbit, unsigned char * cbyte);
unsigned int get_charnum(unsigned int numbChar, int * freqarray);
Node * List_create(int value, int index);
Node * ascend_sort(Node * head, int value, int index);
void destroy_list(Node * head);
void Tree_print ( TreeNode * tn , int level );
ListNode * create_listnode(TreeNode * tn);
void Helper_header(TreeNode * tn, FILE * outfptr, unsigned char * wbit, unsigned char * cbyte);
void Tree_destroy(TreeNode * node);
ListNode * insert_listnode(ListNode * head, ListNode * tmerged);
TreeNode * merge_tree(TreeNode * tn1, TreeNode * tn2);
int Helper_height(TreeNode * tn, int height);
void Helper_treeleaf(TreeNode * tn, int * num);
int leaf_tree(TreeNode * tn);
void Helper_array3d(TreeNode * tn, int ** codebook, int * row, int col);
void build_array3d(TreeNode * root, int ** codebook);
/////////////////////////////////////////////////////////////////////////////////////


//function definitions

//this function creates an initial list and provides space for the list(malloc)
Node * List_create(int value, int index)
{
  Node * list_node = malloc(sizeof(Node));
  
  if(list_node == NULL)
    {
      printf("ERROR IN LIST_CREATE");
    }
  
  list_node -> next = NULL;
  list_node -> char_val = value;
  list_node -> freq = index;

  return list_node;
}


//Function sorts list in ascending order using insertion sort
Node * ascend_sort(Node * head, int value, int index)
{
  if(head == NULL)//because i got some weird error
    {
      printf("ERROR in ascend sort passing");
    }
   
  Node * cur;//current node

  if (head == NULL || (head -> freq >= index))
    {
      
      cur = List_create(value, index);
      cur -> next = head;
      
      return(cur);
    }

  else
    {
      cur = head;
      while((cur-> next != NULL) && (cur->freq < index))
	{
	  cur = cur->next;
	}

      Node * temp;
      temp = List_create(value, index);
      temp->next = cur->next;

      cur->next = temp;
    }
  return head;
}



//Function deletes list
void destroy_list(Node * head)
{
  if(head != NULL)
    {
      destroy_list(head -> next);//point to the next
    }
  free(head);//take care of memory leaks
}



//Function creates individual list nodes to be added to lists
ListNode * create_listnode(TreeNode * tn)
{
  ListNode * newn = malloc(sizeof(ListNode));//create the new node
  
  if(newn == NULL)
    {
      printf("ERROR IN LISTnode_CREATE\n");
    }

  newn -> tree_ptr = tn;
  newn -> next = NULL;//CREATES INDIVISUAL LISTNODES

 return newn;
}


//Function takes created list nodes and inserts them
ListNode * insert_listnode(ListNode * head, ListNode * tmerged)
{
  if(head == NULL)
    {
      return tmerged;
    }
  int weight1 = (head -> tree_ptr) -> weight;
  int weight2 = (tmerged -> tree_ptr) -> weight;
  if (weight1 > weight2)
    {
      tmerged -> next = head;
      return tmerged;
    }
  head -> next = insert_listnode(head -> next, tmerged);
  return head;
}

//Function prints entire tree
void Tree_print(TreeNode * tn, int level)
{
  if(tn == NULL)
    {
      return;
    }
  TreeNode * lc = tn -> left ; 
  TreeNode * rc = tn -> right ; 

  Tree_print ( lc , level + 1) ;
  Tree_print ( rc , level + 1) ;

  int depth ;

  for ( depth = 0; depth < level ; depth ++)
    {
      printf ("   ") ;
    }
  
  printf (" freq = %d" , tn -> weight);
  
  if ((lc == NULL) && (rc == NULL))
    {
      printf (" value = %d , ’%c’" , tn -> character , tn -> character ) ;
    }
  
  printf ("\n");
}


//Function destroys tree
void Tree_destroy(TreeNode * node)
{
  if(node == NULL)
    return;
  Tree_destroy(node -> left);
  Tree_destroy(node -> right);
  free(node);
}



//Function creates tree
TreeNode * create_TreeNode(int char_val, int weight_val)
{
  TreeNode * tnode ;
  tnode = malloc(sizeof(TreeNode));
  
  if(tnode == NULL)
    {
      printf("ERROR in TREENode\n");
    }

  //since we are just creating the the first tree node , left and right children are NULL for now.
  tnode -> weight = weight_val;
  tnode -> character = char_val;
  tnode -> right = NULL;
  tnode -> left = NULL;
  
  return tnode;
}

//Function takes two tree nodes and merges them, creating a new parent node with the value being the sum of the frequencies of both
TreeNode * merge_tree(TreeNode * tn1, TreeNode * tn2)
{
  TreeNode * treenode ;
  treenode = malloc(sizeof(TreeNode));

  if(treenode == NULL)
    {
      printf("ERROR in Tree_merge\n");
      //return NULL;
    }

  treenode -> character = 0;
  treenode -> right = tn2;
  treenode -> left = tn1; 
  treenode -> weight = (tn1 -> weight) + (tn2 -> weight);

  return treenode;
}


//this function will get the header which is required for the compression part.
void Helper_header(TreeNode * tn, FILE * outfptr, unsigned char * wbit, unsigned char * cbyte)
{
  TreeNode * leftc;//for left child 				
  TreeNode * rightc ;//for right child
  
  if (tn == NULL)
    {
      printf("TREE node is NULL at header helper\n");
      return;
    }
  
  leftc = tn -> left;
  rightc = tn -> right;

  //at leaf here
  if((leftc == NULL) && (rightc==NULL))
    {
      
      //this writes the header in binary in the output files
      Bit_write(outfptr, 1, wbit, cbyte);
      
      char_bits(outfptr, tn -> character, wbit, cbyte);
      
      return;
    }

  Helper_header(leftc, outfptr, wbit, cbyte);//recursive calls here for left side
  Helper_header(rightc, outfptr, wbit, cbyte);//for right side
  Bit_write(outfptr, 0, wbit, cbyte);
}


//function helps in creating and calculating tree height
int Helper_height(TreeNode * treen, int height)
{
  int hleft = 0;//left height
  int hright = 0;//right height

  if (treen == 0)
    {
    return height;
    }

  //recursively call the left and right side of the tree node
  hleft = Helper_height(treen->left, height + 1);
  hright = Helper_height(treen->right, height + 1);
  
  if (hleft < hright)
    {
    return hright;
    }
  
  if (hleft > hright)
    {
      return hleft;
    }
  
  return hleft;
}


//this is recursive function which counts the number of leaves in the given binary tree
void Helper_treeleaf(TreeNode * tn, int * num)
{
  if (tn == 0)
    {
      printf("TREE NODE IS 0 for leaf helper\n");
    return;
    }
  
  TreeNode * cleft;//for left child
  TreeNode * cright;//for right child
 
  cright = tn -> right; 
  cleft = tn -> left;

  //at the tree leafs
  if((cleft == NULL) && (cright == NULL))
    {
      (*num)++;
      return;
    }
  
  //go right and leaft
  Helper_treeleaf(cleft, num);
  Helper_treeleaf(cright, num);
  
}

//returns the number of tree leafs present in the BT
int leaf_tree(TreeNode * tn)
{
  int num = 0;
  
  if(tn == 0)
    {
      printf("The treenode is zero\n");
    }
  
  Helper_treeleaf(tn, &num);

  return num;
}


//Function helps to build code book from tree
void Helper_array3d(TreeNode * tn, int ** array, int * row, int col)
{
  if (tn == NULL)
    {
    return;
    }
  
  TreeNode * leftc ;  //for left child
  TreeNode * rightc;  //for right child
  int i = 0;
  int j = 0;
  
  rightc = tn -> right;
  leftc  = tn -> left;
  
  if((leftc == NULL) && (rightc == NULL))//when only one node is present
    {
      array[*row][0] = tn -> character;
      (*row)++;
      return;
    }
  
  if(leftc != NULL)
    {
      int numRow = leaf_tree(leftc);//IMPORTANT STEP
      
      for(i = * row; i < (*row) + numRow; i++)
	{
	  array[i][col] = 0;
	}
      
      Helper_array3d(leftc, array, row, col + 1);
    }

  if(rightc != NULL)
    {
      int numRow = leaf_tree(rightc);//IMPORTANT STEP
     
      for(j = *row; j < (*row) + numRow; j++)
	{
	  array[j][col] = 1;
	}
      
      Helper_array3d(rightc, array, row, col + 1);
    }
}



//Function builds codebook from tree
void build_array3d(TreeNode * root, int ** codebook)
{
  int row = 0;
  Helper_array3d(root, codebook, &row, 1);
}


void print_array3d(int ** array, int numRow);
//this function will print the biary sequence of the tree for each characters
void print_array3d(int ** array, int numRow)
{
  int i = 0;
  int j = 1;
  
  for(i = 0; i < numRow; i++)
    {
      printf("The character %c : ", array[i][0]);
      j = 1;//need go 1 for j again
     
      while (array[i][j] != -1)
	{
	  printf(" %d", array[i][j]);//bits from tree head
	  j++;
	}
      printf("\n");
    }
}



//prints one byte of information
void printByte(unsigned char onebyte)
{
  unsigned char coverma = 0x80;
  
  //while masking has to be greater than 1
  while(coverma > 0)
    {
      printf("%d", (onebyte & coverma) == coverma);
      
      coverma >>= 1;//important step
    }

  //for viewing purpose
  printf("\n");
}
