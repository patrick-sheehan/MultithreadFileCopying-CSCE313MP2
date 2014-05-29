/*  mcopyfile1.c
	 	Patrick Sheehan
	 	CSCE 313 MP2
	 	26 Feb 2014
	 	Resources: stackoverflow, computing.llnl.gov,
	 							pubs.opengroup.org
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dirlock = PTHREAD_MUTEX_INITIALIZER;

void myCopy(char* files[])
{
  // Declare new files and an int to read characters
  FILE* myfile;
  FILE* newfile;
  int ch;
  char* original = files[0];
  char* new = files[1];

  // open input file for reading
  myfile = fopen(original, "r");
  if (myfile == NULL) printf("Could not open %s\n", original);
  
  // create new output file for writing
  newfile = fopen(new, "w");
  if (newfile == NULL) printf("Could not create new file %s\n", new);
  

  while ((ch = fgetc(myfile)))
  { // print each character from input file to screen and to the output file
    if (feof(myfile)) break;
    fputc(ch, newfile);
  }

  // close both files
  fclose(myfile);
  fclose(newfile);  
  // printf("%s >>copied to>> %s\n\n", original, new);
	pthread_mutex_unlock(&lock);
};
void copyDirectory(char* files[2])
{	// recursively

	char * fromDir = files[0];
	char * toDir = files[1];
	DIR *  dir = opendir(fromDir);
	DIR *  newDir = opendir(toDir);
	struct dirent * currDirent;
		
	if (dir == NULL)
	{
		printf("Error opening %s\n", fromDir);
		return;
	}
	if (newDir == NULL)
	{
		mkdir(toDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		newDir = opendir(toDir);
	}

	while ((currDirent = readdir(dir)) != NULL)
	{
		if (strcmp(currDirent->d_name, ".") == 0 || strcmp(currDirent->d_name, "..") == 0)
			continue;

		char *next_dir = malloc(strlen(fromDir) + strlen(currDirent->d_name) + 1 + 1);
		sprintf(next_dir, "%s/%s", fromDir, currDirent->d_name);
		
		char *next_new_dir = malloc(strlen(toDir) + strlen(currDirent->d_name) + 1 + 1);
		sprintf(next_new_dir, "%s/%s", toDir, currDirent->d_name);

	
		if (currDirent->d_type == DT_REG)
		{ // regular file
			pthread_mutex_lock(&lock);

			pthread_t thread;
			char* files[2] = {next_dir, next_new_dir};
			pthread_create(&thread, NULL, (void *)myCopy, files);
		}
		else if (currDirent->d_type == DT_DIR)
		{
			char* files[2] = {next_dir, next_new_dir};
			copyDirectory(files);
			pthread_mutex_unlock(&dirlock);
			pthread_mutex_unlock(&lock);
		}

	}
	pthread_mutex_lock(&dirlock);
	pthread_mutex_lock(&lock);

	closedir(dir);
	closedir(newDir);
}
int main(int argc, char* argv[])
{	// copy files from dir1 -> dir2 (2 command line args)
	if (argc != 3)
	{
		printf("Please provide two directory names as arguments\n");
		return 0;
	}
	char* files[2] = {argv[1], argv[2]};
	copyDirectory(files);
}
