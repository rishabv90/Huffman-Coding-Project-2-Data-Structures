
#include "sorting.h"

long * Load_File(char *Filename, int *Size)
{

  FILE *fptr;// file pointer
  long i;
  int totalnum;
  

  fptr = fopen(Filename, "r");//open file 
    if(fptr == NULL)//error check
      {
	*Size = 0;
	printf("\n FPTR IS NULLL \n");
	return NULL;
      }
 
    fscanf(fptr, "%d", &totalnum); //get size of array

    if(totalnum == 0)//error check
      {
	*Size = 0;
	fclose(fptr);
	printf("\n TOTAL NUM IS NULL NIQ\n");
	return NULL;
      }
    
  long *array = malloc(sizeof(long) * totalnum);//creating space for the array 
  
    for (i= 0; i < totalnum; ++i)
      {
	if(feof(fptr))//check if file ends unexpectedly
	  {
	    fprintf(stderr, "\nEMPTY FILE AFTER READING\n");
	    free(array);
	    *Size = i;
	    fclose(fptr);
	    return (NULL);
	  }
	fscanf(fptr, "%ld", &array[i]);
      }
    
    fclose(fptr);
 
    *Size = totalnum;

  return array;
}


int Save_File(char *Filename, long *Array, int Size)
{
  FILE *fptr = NULL;
  int wnum = 0;// Written nums
  int i = 0;

  fptr = fopen(Filename, "w");
  if(fptr == NULL)
    {
      printf("\nSOMETHING WRONG WITH SAVE FPTR\n");
      return -1;
    }

  fprintf(fptr, "%d\n", Size); 

  for(i= 0; i < Size; ++i)
    {
      fprintf(fptr, "%ld\n", Array[i]);
      ++wnum;
    }

  fclose(fptr);
  
  return wnum;
}


void Save_Seq1(char * Filename, int N)
{

  int Size = N;
  int *array = malloc(sizeof(int)* Size);
  array[0] = 1;
  int ptr2 = 0, ptr3 = 0;
  int u2, u3;
  int i = 1;

  while(array[i] < Size)
    {
      if((array[ptr2] * 2) == (array[i-1]))//got rid of -1
	{
	  ptr2 +=1;
	}
      if((array[ptr3]*3) == (array[i-1]))
	{
	  ptr3 += 1;
	}
      
      u2 = array[ptr2] * 2;
      u3 = array[ptr3] * 3;
      
      if(u2 < u3)
	{
	  ptr2 += 1;
	  array[i] = u2;
	}
      else//(u2 > u3)
	{
	  ptr3 += 1;
	  array[i] = u3;
	}
       i++;      
    }

 //saving into a file 
 FILE *fptr = NULL;
 int wnum = 0;// Written nums
 i = 0;

 fptr = fopen(Filename, "w");

 if(fptr == NULL)
   {
     printf("\nSOMETHING WRONG WITH SAVE FPTR\n");
     //return -1;
   }
  
  fprintf(fptr, "%d\n", Size);
  
  for(i= 0; i < Size; ++i)
    {
      fprintf(fptr, "%d\n", array[i]);
      ++wnum;
    }

  fclose(fptr);
  free(array);
}



 void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
  //this is the sequence gerator
  //generates sequence in the form of 2^p & 3^q

  int *array = malloc(sizeof(int)* Size);
  array[0] = 1;
   
  int ptr2 = 0, ptr3 = 0;
  int u2, u3;
  
  int i = 0;
  
  for(i = 1;i < Size; i++)
    {
      if((array[ptr2] * 2) == (array[i-1]))
	{
	  ptr2 +=1;
	}

      if((array[ptr3]*3) == (array[i-1]))
	{
	  ptr3 += 1;
	}
      
      u2 = array[ptr2] * 2;
      u3 = array[ptr3] * 3;
      
      if(u2 < u3)
	{
	  ptr2 += 1;
	  array[i] = u2;
	}
      else
	{
	  ptr3 += 1;
	  array[i] = u3;
	}
      
      if(array[i]<Size)
	{break;}
      
    }
  
  //sequnce generated till here
  //shell sorting from now onwards

  int counter = 0;
  counter = i;
  int comp =0;
  int move = 0;
  int k =0;//gap
  int j = 0;   
  int a = 0; 
  int b = 0;
  int temp;
 
  for(a = (counter-1) ; a >= 0 ; a--) //to get the array values from top to first
    {
      k = array[a];
      for(j = k; j < Size; j++)
	{
	  temp = Array[j];
	  move = move +1;
	  b = j;
	  
	  while((b >= k) && (Array[b - k] > temp))
	    {
	      Array[b]= Array[b-k];
	      b -= k;
	      comp = comp + 1;
	      move = move + 1;
	    }
	  
	  Array[b] = temp;
	  move = move + 1;
	  
	}    
    }
  
  *N_Comp = comp;
  *N_Move = move;
  free (array);  

}


void Save_Seq2(char*Filename, int N)
{
  int Size = 0;
  Size = N;
  long *array = malloc(sizeof(long)* Size);
  int i = 0;
  array[0] = Size/1.3;	      
  for (i = 1; i < (Size-1); i++)
    {
       array[i] = array[i-1]/1.3;
      
      if(array[i] == 9 || array[i]==10)
	{
	  array[i] = 11;
	}
      
      if(array[i] == array[i - 1])
	{
	  array[i] = array[i]/(1.3);
	}
      if(array[i] == 1)
	{
	  break;
	}           
    }

  //saving into a file
   FILE *fptr = NULL;
   i = 0;
   fptr = fopen(Filename, "w");
   if(fptr == NULL)
     {
       printf("\nSOMETHING WRONG WITH SAVE FPTR\n");
       //return -1;
     }
   
   for(i= 0; i < Size; ++i)
     {
       fprintf(fptr, "%ld\n", array[i]);
       if(array[i]==0)
	 {
	   break;
	 }
     }
   
   fclose(fptr);
   free(array);
}
 
 void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
  //long *array = malloc(sizeof(long)* Size);
 
  int value = 1;//this is useless..testing purpose
  int size_1 = 0
  size_1 = Size;
  int i = 0;
  int move = 0;
  int comp = 0;

  long temp  = 0;

  while(size_1 > 1)
    {
      comp += 1;
      if   (size_1 > 1)
	{
	  comp = 1;
	  size_1 = (int) (size_1/1.3);//this decreases the size fo the input and hence increases the speed #optimization

	}
      value = 0;//testing purposses
      
      for(i =0; (i + size_1) < Size; i++)
	{
	  comp += 1;
	  if(Array[i] > Array[i + size_1])
		{
		  //need temp variablee
		  temp = Array[i];
		  Array[i] = Array[i + size_1];
		  //error here

		  Array[i+size_1] = temp;
		  move += 3;
		  value = 1;
		  comp += 1; 
		}
	}
    }

  *N_Comp = comp;
  *N_Move = move;

}



