#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
* Returns whether the index-th bit is true
*/
int is_true(char* array, size_t index)
{
  size_t byte_index = floor(((float)index)/8.0f);
  short int bit_index = 7 - (index % 8);
  char byte = array[byte_index];
  return (byte & (1 << bit_index)) >> bit_index;
}

/*
* Sieves the index-th bit by setting if to 0
*/
void sieve(char* array, size_t index)
{
  size_t byte_index = floor(((float)index)/8.0f);
  short int bit_index = 7 - (index % 8);

  array[byte_index] &= ~(1 << bit_index);
}

int main(int argc, char** argv)
{
  // ### arguments checking ###
  errno = 0;
  if(argc != 2)
  {
    printf("usage: seq <maximum number>\n");
    return 1;
  }
  char* p;
  const size_t ARRAY_SIZE = ceil((float)strtoll(argv[1], &p, 10)/8.0f);
  if(errno != 0 || *p != '\0')
  {
    printf("Cannot convert %s to a number.\n",argv[1]);
    return 1;
  }

  FILE* file = fopen("primes.txt","w");
  if(file==NULL)
  {
    printf("Error opening file 'primes.txt'\n");
    return 1;
  }

  printf("### Sequential Eratosthenes Sieve\n");
  printf("### Sieving up to %u, output in 'primes.txt'.\n",ARRAY_SIZE*8-1);

  char* byte_array = calloc(ARRAY_SIZE,sizeof(char));

  // ### set up the array ###
  for(size_t i = 0; i < ARRAY_SIZE; ++i)
  {
    byte_array[i] = ~'\0';
  }
  sieve(byte_array,0);
  sieve(byte_array,1);

  // ### sequential Erathostenes Sieve ###
  for(size_t i = 2; i < sqrt(ARRAY_SIZE*8); ++i)
  {
    if(is_true(byte_array,i))
    {
      //printf("%d is prime\n",i);
      for(size_t j = i+1; j < ARRAY_SIZE*8; ++j)
      {
	//printf("Sieving %d\n",i*i+j*i);
        if(j%i == 0)
        {
          sieve(byte_array,j);
        }
      }
    }
  }

  // ### printing the whole output ###
  for(size_t i = 2; i < ARRAY_SIZE*8; ++i)
  {
    if(is_true(byte_array,i))
    {
      fprintf(file,"%d\n",i);
    }
  }

  printf("done!\n\n### December 2014 - EPFL - Sidney Bovet\n\n");

  fclose(file);
  free(byte_array);
  return 0;
}
