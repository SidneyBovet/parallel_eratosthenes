#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

void print_time(const struct timeval* start, const struct timeval* stop)
{
  const float time = stop->tv_sec-start->tv_sec +
	(stop->tv_usec-start->tv_usec)/(1000000.0f);
  printf("%f\n",time);
}

int main(int argc, char** argv)
{
  // ### arguments checking ###
  errno = 0;
  if(argc != 4)
  {
    printf("parameters: <max msg size> <begin val> <step>\n");
    return 0;
  }
  char* p;
  const size_t msg_size = strtoll(argv[1], &p, 10);
  if(errno != 0 || *p != '\0')
  {
    printf("Cannot convert %s to a number.\n",argv[1]);
    return 1;
  }
  const size_t BEGIN = strtoll(argv[2], &p, 10);
  if(errno != 0 || *p != '\0')
  {
    printf("Cannot convert %s to a number.\n",argv[1]);
    return 1;
  }
  const size_t STEP = strtoll(argv[3], &p, 10);
  if(errno != 0 || *p != '\0')
  {
    printf("Cannot convert %s to a number.\n",argv[1]);
    return 1;
  }

  MPI_Init(NULL,NULL);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if(rank == 0)
  { // Master
    char* buffer = calloc(msg_size,sizeof(char));

    printf("bcast test...\n");
    struct timeval stop, start;
    for(int i = BEGIN; i <= msg_size; i += STEP)
    {
      gettimeofday(&start, NULL);
      MPI_Bcast(buffer,i,MPI_CHAR,0,MPI_COMM_WORLD);
      gettimeofday(&stop, NULL);
      print_time(&start,&stop);
    }
    printf("\nsimple send test (far)...\n");
    const int far_slave = world_size - 1;
    for(int i = BEGIN; i <= msg_size; i += STEP)
    {
      gettimeofday(&start, NULL);
//printf("master sending to %d...\n",world_size-1);
      MPI_Send(buffer,i,MPI_CHAR,far_slave,0,MPI_COMM_WORLD);
      MPI_Recv(buffer,i,MPI_CHAR,MPI_ANY_SOURCE,
              MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      gettimeofday(&stop, NULL);
      print_time(&start,&stop);
    }
    printf("\nsimple send test (neighbor)...\n");
    for(int i = BEGIN; i <= msg_size; i += STEP)
    {
      gettimeofday(&start, NULL);
//printf("master sending to %d...\n",world_size-1);
      MPI_Send(buffer,i,MPI_CHAR,1,0,MPI_COMM_WORLD);
      MPI_Recv(buffer,i,MPI_CHAR,MPI_ANY_SOURCE,
              MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      gettimeofday(&stop, NULL);
      print_time(&start,&stop);
    }
    free(buffer);
  }
  else
  { // Slave
    char* buffer = calloc(msg_size,sizeof(char));
    for(int i = BEGIN; i <= msg_size; i += STEP)
    {
      MPI_Bcast(buffer,i,MPI_CHAR,0,MPI_COMM_WORLD);
    }

//printf("slave rank = %d, world size = %d\n",rank,world_size);
    if(rank == world_size-1)
    {
      for(int i = BEGIN; i <= msg_size; i += STEP)
      {
//printf("node %d waiting for msg...\n",rank);
        MPI_Recv(buffer,i,MPI_CHAR,MPI_ANY_SOURCE,
              MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(buffer,i,MPI_CHAR,0,0,MPI_COMM_WORLD);
      }
    }
    if(rank == 1)
    {
      for(int i = BEGIN; i <= msg_size; i += STEP)
      {
//printf("node %d waiting for msg...\n",rank);
        MPI_Recv(buffer,i,MPI_CHAR,MPI_ANY_SOURCE,
              MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(buffer,i,MPI_CHAR,0,0,MPI_COMM_WORLD);
      }
    }
    free(buffer);
  }

  MPI_Finalize();
  return 0;
}
