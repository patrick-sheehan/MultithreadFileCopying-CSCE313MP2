/*  copyfile1.c
	 	Patrick Sheehan
	 	CSCE 313 MP2
	 	26 Feb 2014
	 	Resources: stackoverflow
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
	 	
void myCopy(char* original, char* new)
{
  // Declare new files and an int to read characters
  FILE* myfile;
  FILE* newfile;
  int ch;

  // open input file for reading
  myfile = fopen(original, "r");
  if (myfile == NULL) 
  {
	printf("Could not open %s\n", original);
	return;
  }
  
  // create new output file for writing
  newfile = fopen(new, "w");
  if (newfile == NULL)
  {
	printf("Could not create new file %s\n", new);
	return;
  }

  while ((ch = fgetc(myfile)))
  { // print each character from input file to screen and to the output file
    if (feof(myfile)) break;
	
    fputc(ch, newfile);
  }

  // close both files
  fclose(myfile);
  fclose(newfile);  
};
void copyDirectory(char* fromDir, char* toDir)
{	// recursively

	DIR * dir = opendir(fromDir);
	DIR * newDir = opendir(toDir);

	char* original;
	char* new;

	struct dirent* currDirent;
		
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

		if (currDirent->d_type == DT_DIR)
		{	// currDirent is a subdirectory
			copyDirectory(next_dir, next_new_dir);
		}
		else
		{
			myCopy(next_dir, next_new_dir);
//			printf("%s copied to:\n", next_dir);
//			printf("\t%s\n", next_new_dir);
		}	
	}
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
	copyDirectory(argv[1], argv[2]);
}
