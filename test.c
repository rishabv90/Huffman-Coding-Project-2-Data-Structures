#include <stdio.h>
//#include <math.h>


#include <stdlib.h>

void Save_Seq2(char* Filename,int Size);
void Seq2(int Size);


void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move);
int main ()
{
  
  
  Save_Seq2("seq2.txt", 100);
  



  
  

  return 0;
}


void Save_Seq2(char*Filename, int N)
{
  int Size = N;
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


  //saving into a file stuff


   FILE *fptr = NULL;
   //int wnum = 0;// Written nums
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
      fprintf(fptr, "%ld\n", array[i]);
      if(array[i]==0)
	{
	  break;
	}
    }
  
  fclose(fptr);
  free(array);
}
