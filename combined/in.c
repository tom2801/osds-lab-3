/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include "curl.h"
#include "test.h"
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void processFileContents (char * filePath) 
{
	char * buffer = 0;
	long length;
	FILE * f = fopen ("tmp.txt", "rb");

	if (f)
	{
	  fseek (f, 0, SEEK_END);
	  length = ftell (f);
	  fseek (f, 0, SEEK_SET);
	  buffer = malloc (length);
	  if (buffer)
	  {
	    fread (buffer, 1, length, f);
	  }
	  fclose (f);
	}

	  if (buffer)
	  {	printf("in fisier \n");
	  	if ( strstr(buffer,"detected") != NULL ){
	  		remove (filePath);
	  	}
	  }
	}


int recursionWatch(char * root)
{
  int length, i = 0;
  int fd;
  int wd;
  int wd2;
  char buffer[EVENT_BUF_LEN];
  //char * root = "/tmp/";
  /*creating the INOTIFY instance*/
  fd = inotify_init();

  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
  wd = inotify_add_watch( fd, "/tmp", IN_CREATE | IN_DELETE );

  /*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/
do{
	i = 0;
  length = read( fd, buffer, EVENT_BUF_LEN );

  /*checking for error*/
  if ( length < 0 ) {
    perror( "read" );
  }

  /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
  while ( i < length ) {     struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     
  char path [1000];
  strcat(path, root);
  if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) { // add recursion step
          printf( "New directory %s created.\n", event->name );
          char newRoot [1000];
          printf("path = %s \n",path);
          strcat(newRoot,path);
          strcat(newRoot,event->name);
          strcat(newRoot,"/");
          recursionWatch(newRoot);
        }
        else {
          printf( "New file %s created.\n", event->name );
          
          
          strcat(path, event->name);
          
          printf ("path %s \n", path);
          
          unsigned char* hash; 
          hash = call_test(path);
          
          char processedHash [1000];
          
          for(int i = 0; i < 32; i++)
          {
          	sprintf(&processedHash[i*2], "%02x" ,(unsigned int) hash[i]);
          }
          
          printf( "hash = %s \n", processedHash);
          
          call_curl(processedHash);
          
          processFileContents (path); 

          //todo de introdus interpretare si delete la fisier
        }
      }
      else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "Directory %s deleted.\n", event->name );
        }
        else {
          printf( "File %s deleted.\n", event->name );
        }
      }
    }
    i += EVENT_SIZE + event->len;
  }
}while(1);
  /*removing the “/tmp” directory from the watch list.*/
   inotify_rm_watch( fd, wd );

  /*closing the INOTIFY instance*/
   close( fd );

}

void main (int argc , char* argv[])
{
   recursionWatch(argv[1]);
   
   // scrap the idea, fac un file structure mapper si un 
}

