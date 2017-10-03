#include<stdio.h>
#include<stdlib.h>

int *heap_int(int i, int *result)
{
  //int *result = malloc(sizeof(int));
  *result = i;
  return result;
}

int *fib(int *n)
{
  if (*n == 0)
    {
      int h1;
      return heap_int(0, &h1);
    }
  else
    {
      if (*n == 1)
	{
          int h2;
	  return heap_int(1, &h2);
	}
      else
	{
          int h3;
          int h4;
          int h5;
	  return heap_int(*fib(heap_int(*n-1, &h3)) + *fib(heap_int(*n-2, &h4)), &h5);
	}
    }
}

// OBSERVERA!
// Detta program läcker minne som ett såll -- det frigör inte några allokerade pekare
int main(int argc, char **argv)
{
  if (argc < 2)
    {
      puts("Usage: ./fib-rec 5");
    }
  else
    {
      int h6;
      int *n = heap_int(atoi(*(argv+1)), &h6);
      printf("fib(%d) = %d\n", *n, *fib(n));
    }
  return 0;
}
