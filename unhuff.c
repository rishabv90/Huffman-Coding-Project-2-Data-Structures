#include "unhuff.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FALSE -1


int main(int argc, char **argv)
{
  if (argc != 2)
    {
      printf("Need an argument for input file\n");
      return -1;
    }
  
  int length = 0; //this is the length of argv[1]
  char * fileread ; //the file we will read
  char * output;
  
  //for the input file 
  fileread = argv[1];
  
  //take length of the name of the inputfile
  length = strlen(argv[1]);
  
  
  
  // fileread = fopen(argv[1], "r");
  if (fileread == NULL)
    {
      printf("ERROR OPENING FILE READ1\n");
      return -1;
    }
  
  output = (char *)malloc(length + 5 * sizeof(char));
  strcpy(output,fileread);
  output = strcat(output, ".unhuff");
  unhuff(fileread,output);
 
  free(output);
  
  return 0; 
  
}


//THIS FUNCTION DOES EXACTLY THE OPPOSITE OF BIR_WRITE IN HUFF.C
//helper function for read_header;
int bitread(FILE * fptr, unsigned char * bit, unsigned char * wbit, unsigned char * cbyte)
{
  
  unsigned char holder; 
  int final = 1;
   
  //this is check for where exactly are we on the 8 bits
   if((*wbit) == 0)
    {
      final = fread(cbyte, sizeof(unsigned char), 1, fptr);
    }
  
   //error check
   if(final != 1)
     {
       printf("Hello from bitread\n");
       return -1;
     }
 
   //this process was obtained from the sorce sourced in the report.

  holder = (*cbyte) >> (7 - (*wbit));//same we bitshift crom the current byte;
 
  //this is important , as holder needs to 1 for this case
  holder = holder & 0X01;//hex for 1 , we will go once at a time
  //this takes acre of reading them at 8 bits at a time
  *wbit = ((*wbit) + 1) % 8;

  //holder is a temproray variable 
  *bit = holder;


  return 0;
}








//This function is resposible for reading the header from the compressed file.
TreeNode * read_header(FILE * infptr)
{
  //some declarations
  int count;//will count number of bits
  ListNode * head = NULL;
  unsigned char val;
  unsigned char wbit = 0;
  unsigned char cbyte = 0;
  unsigned char bit1 = 0;
  int final = 0;
  ListNode * nnode;
  TreeNode * tnode;
  TreeNode * root;
  
  while(final == 0)
    {
      //this will make the call to get the function header
      bitread(infptr, &bit1, &wbit, &cbyte);
    
      if(bit1 == 1)
	{
	  val = 0;
	  
	  for(count = 0; count < 7; count++)
	    {
	      //left shight here 
	      val <<= 1; //bitshift here for the value
	      
	      bitread(infptr, &bit1, &wbit, &cbyte);
	     
	      val |= bit1; 
	    }
	  
	  //we will now try to create teh tree base on the header information
	  tnode = create_tn(val, 0);
	  

	  
	  nnode= create_ln(tnode);
	  
	  head = insert_ln(head, nnode, YES);
	  
	}
      else
	{
	  //simple checks for NULL
	  if(head == NULL)
	    {
	      printf("Problem in head ---readheader\n\n");
	    }
	     
	  if((head -> next) == NULL)
	    {
	      final = 1;
	    }
	  else
	    {
	       head = merge_ln(head, DECODE);
	    }
	  
	}
    }

  //this is the new header to the tree
  root = head -> tree_ptr;
 
 //memory error here
  free(head);

  return(root);
  
}

//final; function which is resposible to get teh string sequence are teh header is extracted from the input file
int unhuff(char * infile, char * outfile)
{
  unsigned char wbit = 0;
  unsigned char bit1 = 0;
  unsigned char cbyte = 0;
  FILE * infptr;
  FILE * outfptr;
  TreeNode * root;
  TreeNode * tnode;
  unsigned int charnum = 0;//number of characters in the inputfiule
  unsigned char nline; //this is for new line character//this is important
  
  infptr = fopen(infile, "r");
  //simple error check
  if(infptr == NULL)
    {
      printf("CANNOT wrtite in file in unhuff\n");
      return FALSE;
    }
  
  root = read_header(infptr);
  //we will read the character one at a time
  fread(&charnum, sizeof(unsigned int), 1, infptr);  

   if(charnum < 0)
     {
       printf("ERROR in unhuff\n");
       return FALSE;
     }
   
   fread(&nline, sizeof(unsigned char), 1, infptr);//we wanna read till we get \n

   //THIS caused problems as earlier i didnot have this
   if(nline == '\n')
     {
       printf("NEWLINE FOUND in UNHUFF \n");
     }
   
   //Print in new file here in this function
   outfptr = fopen(outfile, "w");
   //simple error check
   if(outfptr == NULL)
     {
       printf("OUTfptr is NULL, chekc UNHUFF\n");
     }
   
    //still need to go throught the characterssss
   while(charnum != 0)
     {
       
       //shift the values to yree node
       tnode = root;
	
       while(tnode->left != NULL)
	 {
	   bitread(infptr, &bit1, &wbit, &cbyte);
	   // readBit(infptr, &bit1, &wbit, &cbyte);
	   if(bit1 == 0)
	     {
	       
	       tnode = tnode->left;
	     }
	   else
	     {
	       tnode = tnode->right;
	      }	   
	 }
	
       //this will print character by character
       fprintf(outfptr, "%c", tnode->character);
       charnum--;
     }
   //check for memory leaks here
   destroy_tree(root);
   
   //close files
   fclose(infptr);
   fclose(outfptr);
   
   return(0);
}

