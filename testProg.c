#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#define SECRET_LEN 8192

int ioctlTest(char *msg)
{
  int FD, res;
  uid_t rootID;

  /*Open for writing*/
  if( (FD = open("/dev/Secret", O_WRONLY)) < 0 )
  {
    printf("Failed write open\n");
    return -1;
  }
  printf("Opening for write. FD: %d\n", FD);

  /*Write a message as user*/
  if( (res = write(FD, msg, strlen(msg))) < 0 )
  {
    printf("Failed write\n");
    return -1;
  }
  printf("Writing message. res: %d\n", res);

  rootID = 0;
  
  /*Attempt grant*/
  if( res = ioctl(FD, SSGRANT, &rootID))
  {
    printf("Failed ioctl\n");
  }
  printf("Changing owner to root. res: %d\n", res);

  /*Close the file descriptor*/
  if( close(FD) < 0 )
  {
    printf("Failed write close\n");
    return -1;
  }

  return 0;
}

int writeTwiceTest(char *msg, char *msg2)
{
  int FD, res;
  char *buf;

  /*Buffer overly sized on purpose, to make sure we read the entire message
   *and also ensure that we don't read anything more*/
  if( !(buf = malloc(SECRET_LEN)) )
  {
    printf("Failed malloc");
    return -1;
  }

  /*Open for writing*/
  if( (FD = open("/dev/Secret", O_WRONLY)) < 0 )
  {
    printf("Failed write open\n");
    return -1;
  }
  printf("Opening for write. FD: %d\n", FD);
  
  /*Write the first message*/
  if( (res = write(FD, msg, strlen(msg))) < 0 )
  {
    printf("Failed first write\n");
    return -1;
  }
  printf("Writing first message. res: %d\n", res);

  /*Write the first message*/
  if( (res = write(FD, msg2, strlen(msg2))) < 0 )
  {
    printf("Failed first write\n");
    return -1;
  }
  printf("Writing second message. res: %d\n", res);

  /*Close the file descriptor*/
  if( close(FD) < 0 )
  {
    printf("Failed write close\n");
    return -1;
  }

  /*Open for reading*/
  if( (FD = open("/dev/Secret", O_RDONLY)) < 0 )
  {    
    printf("Failed read open\n");
    return -1;
  }
  printf("Opening for read. FD: %d\n", FD);

  /*Read the message, and output it*/
  if( (res = read(FD, buf, SECRET_LEN)) < 0 )
  {
    printf("Failed read\n");
    return -1;
  }
  printf("Reading message. res: %d\n", res);
  printf("Expected res: %d\n", strlen(msg) + strlen(msg2));
  printf(buf);

  /*Clean up*/
  free(buf);

  if( close(FD) < 0 )
  {
    printf("Failed read close");
    return -1;
  }
}

int basicTest(char *msg)
{
  int FD, res;
  char *buf;

  if( !(buf = malloc(strlen(msg))) )
  {
    printf("Failed malloc");
    return -1;
  }
  
  /*Open for writing*/
  if( (FD = open("/dev/Secret", O_WRONLY)) < 0 )
  {
    printf("Failed write open\n");
    return -1;
  }
  printf("Opening for write. FD: %d\n", FD);
  
  /*Write the message*/
  if( (res = write(FD, msg, strlen(msg))) < 0 )
  {
    printf("Failed write\n");
    return -1;
  }
  printf("Writing message: \n%s \nres: %d\n", msg, res);

  /*Close the file descriptor*/
  if( close(FD) < 0 )
  {
    printf("Failed write close\n");
    return -1;
  }

  /*Open for reading*/
  if( (FD = open("/dev/Secret", O_RDONLY)) < 0 )
  {    
    printf("Failed read open\n");
    return -1;
  }
  printf("Opening for read. FD: %d\n", FD);

  /*Read the message, and output it*/
  if( (res = read(FD, buf, strlen(msg))) < 0 )
  {
    printf("Failed read\n");
    return -1;
  }
  printf("Reading message. res: %d\n", res);
  printf(buf);

  /*Clean up*/
  free(buf);

  if( close(FD) < 0 )
  {
    printf("Failed read close\n");
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  char *msg, *msg2;

  if( !(msg = malloc(SECRET_LEN)) )
  {
    printf("Failed malloc1\n");
    return -1;
  }

  if( !(msg2 = malloc(SECRET_LEN)) )
  {
    printf("Failed malloc2\n");
    return -1;
  }

  /*Create message1*/
  if( sprintf(msg, "This is a test secret.\n") < 0 )
  {
    printf("Failed to create message 1.\n");
    exit(EXIT_FAILURE);
  }

  /*Create message2*/
  if( sprintf(msg2, "A slightly different message with more length\n") < 0 )
  {
    printf("Failed to create message 1.\n");
    exit(EXIT_FAILURE);
  }


  printf("\n***** Basic Test *****\n");
  
  /*Basic write then read.*/
  if( basicTest(msg) < 0 )
  {
    printf("Failed basic test\n");
    exit(EXIT_FAILURE);
  }

  printf("\n***** Write Twice Test *****\n");

  /*Write to a buffer, write it again with same FD, output result*/
  if( writeTwiceTest(msg, msg2) < 0 )
  {  
    printf("Failed writing twice test\n");
    exit(EXIT_FAILURE);
  }
  
  printf("\n***** IOCTL Test *****\n");

  /*Write to the secret and give it to root*/
  if( ioctlTest(msg) < 0 )
  {
    printf("Failed ioctl test\n");
    exit(EXIT_FAILURE);
  }
}
