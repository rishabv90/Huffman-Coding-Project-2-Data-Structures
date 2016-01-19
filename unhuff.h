#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define ENCODE 0 
#define DECODE 1
#define NO 0 
#define YES 1 
#define ASCEND 2 
 
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

typedef struct listnode {
  struct listnode * next;
  TreeNode * tree_ptr;
} ListNode;

///////////////////////////////////////////////////////////////////////THESE FUNCTIONS ARE IN .C FILE
TreeNode * read_header(FILE * infptr);
int bitread(FILE * fptr, unsigned char * bit, unsigned char * wbit, unsigned char * cbyte);
int unhuff(char * infile, char * outfile);
////////////////////////////////////////////////////////////////////////

ListNode * create_ln(TreeNode * tn);
//This will create a list node , similar to huff lnc reeate

//simple malloc and takes care of its struct values.
ListNode * create_ln(TreeNode * tn)
{
  ListNode * nnode;//new node
  nnode = malloc(sizeof(ListNode));
  
  if(nnode == NULL)
    {
      printf("ERROR in node creation\n\n");
      return NULL;
    }
  
  nnode -> next = NULL;
  nnode -> tree_ptr = tn;


  return nnode;
}

/////////////////////////////////////////////////////////////////////
ListNode * insert_ln(ListNode * head, ListNode * nnode, int mode);

//This function will insert the new ln to the tree depending on there modee

ListNode * insert_ln(ListNode * head, ListNode * nnode, int mode)
{

  ////////////error check here 
  if (nnode == NULL)
    {
      printf("NEW NODE IS NULL, PROBLEM IN INSERT\n");
      return NULL;
    }

  if ((nnode -> next) != NULL)
    {
      printf("New node next is nULL\n");
    }
  //getting segfault here

  int w_onode;//weight of old node 
  int w_nnode ;//weight of new node
  
  if(head == NULL)
    return nnode;
  
  //this is an important step
  if (mode == YES)
  {
    nnode -> next = head; //similar to a stack push 
    return nnode;
  }

  //Veryimportant steps
  if (mode == NO)
    {
      head -> next = insert_ln(head -> next, nnode, mode);
      return head;
    }
  w_onode = (head -> tree_ptr) -> weight;
  w_nnode= (nnode -> tree_ptr) -> weight;
  
  //this statement will implement a q push
  if (w_onode > w_nnode)
    {
      nnode -> next = head;
      return nnode;
    }
  head -> next = insert_ln(head -> next, nnode, mode);

  
  return head;
}


/////////////////////////////////////////////////////////////////////
TreeNode * create_tn(int chval, int wval);

//This function tries to create a new tree node and returns the newly made tree node
//simple create tree node function ,, respected values are pointed to their struct values

TreeNode * create_tn(int chval, int wval)
{
  TreeNode * tnode = malloc(sizeof(TreeNode));

  if(tnode == NULL)
    {
      printf("ERROR in tn creation");
      return NULL;
    }
  
  //weight and character vlauessss;
  tnode -> character = chval;
  tnode -> weight = wval;

  //donot set children to any va;
  tnode -> left = NULL;
  tnode -> right = NULL;
  
  return tnode;
}


///////////////////////////////////////////////////////////////////
void destroy_tree(TreeNode * tnode);

//basic function which destroys part of the tree node 
//pretty straight forward

void destroy_tree(TreeNode * tnode)
{
  if(tnode == NULL)
    {
      printf("NULL cannot be destroyed\n");
      return;
    }
  //destroy children too
  destroy_tree(tnode -> left);
  destroy_tree(tnode -> right);
  
  //need free here leak
  free(tnode);
}

//////////////////////////////////////////////////////////////////
TreeNode * merge_tree(TreeNode * tn1, TreeNode * tn2);

//this function merges two baby trees to form a larger tree
//allocates space for it and returs the newly merged tree which has treenode 1 and tree node 2
TreeNode * merge_tree(TreeNode * tn1, TreeNode * tn2)
{
  TreeNode * tnode = malloc(sizeof(TreeNode));
  if(tnode == NULL)
  {
    printf("ERROR in merge_tree\n");
    return NULL;
  }


  tnode -> left = tn1;
  tnode -> right = tn2;
  
  tnode -> character = 0;

  //weight has to be added
  tnode -> weight = (tn1 -> weight) + (tn2 -> weight);

  return tnode;
}

/////////////////////////////////////////////////////////////////////
ListNode * merge_ln(ListNode * head, int lnend);

//this function is similar to the function foud in huff.h
//just merges two nodes to form a tree, also the weight is the sum

ListNode * merge_ln(ListNode * head, int lnend)
{
  TreeNode * tfinal;//this is the merged listnode together to for a tree
  ListNode * two;
  ListNode * three;
  TreeNode * tnode1 ;
  TreeNode * tnode2 ;

  two = head -> next;
  three = two -> next;

  tnode1 = head -> tree_ptr;
  tnode2 = two -> tree_ptr;

  //memory leak here
  free(head);
  free(two);
  
  head = three;

  //important step 
  //this tell which treenodes to merge and in which way depenginf on the vlaue of list nopde ending 
  //, which is passed inside
 
  if (lnend == ENCODE)
    {
      tfinal = merge_tree(tnode1, tnode2);
    }
 
  else
    {
      tfinal = merge_tree(tnode2, tnode1);
    }
  
  ListNode * nnode;

  nnode = create_ln(tfinal);
  
  if (lnend == ENCODE)
    {
      head = insert_ln(head, nnode, ASCEND);
    }

  else
    {
      head = insert_ln(head, nnode, YES);
    }
  return head;
}


