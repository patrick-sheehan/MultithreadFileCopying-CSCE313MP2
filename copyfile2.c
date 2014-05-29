/*  copyfile2.c
	 	Patrick Sheehan
	 	CSCE 313 MP2
	 	26 Feb 2014
	 	Resources: stackoverflow, computing.llnl.gov,
	 							pubs.opengroup.org
*/
#include <stdio.h>

void myCopy(char*, char*);

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Expected input and output files as parameters\n");
		return 0;
	}
	myCopy(argv[1], argv[2]);
	return 0;
}

void myCopy(char* src, char* dest)
{	// this function taken from mcopyfile1.c (and MP1-simplefile.c)

  // Declare new files and an int to read characters
  FILE* oldfile;
  FILE* newfile;
  int ch;

  // open input file for reading
  oldfile = fopen(src, "r");
  if (oldfile == NULL) printf("Could not open %s\n", src);
  
  // create new output file for writing
  newfile = fopen(dest, "w");
  if (newfile == NULL) printf("Could not create new file %s\n", dest);

  while ((ch = fgetc(oldfile)))
  { // print each character from input file to screen and to the output file
    if (feof(oldfile)) break;
    fputc(ch, newfile);
  }

  // close both files
  fclose(oldfile);
  fclose(newfile);  
//  printf("%s >>copied to>> %s\n\n", src, dest);
};
