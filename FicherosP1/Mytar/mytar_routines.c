#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes)
{
	// Complete the function
	origin = fopen(origin, "r");
	destination = fopen(destination, "w");

	if(origin != NULL && destination != NULL){

	for(int i = 0; i < nBytes; ++i){
		int c = fgetc (origin);

		if(c != EOF){
			fputc ( c, destination );
		}
	}
	fclose(origin); fclose(destination);

	return EXIT_SUCCESS;
	}

	return -1;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	// Complete the function
	
	int cont = 0;
	if (file != NULL){
		while(getc(file) != EOF) cont++;

		fseek(file, 0, SEEK_END);
		cont = ftell(file);
	}
	char* ptr = malloc(cont);

	rewind(file);

	if(fread(ptr, cont, 1, file) >= 1) {
		fwrite(ptr, cont, 1, stdout);
		return ptr;
	}

	return NULL;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles)
{
	// Complete the function

	// stHeaderEntry* array=NULL;
	// int nr_files=0;
	// //... Leemos el numero de ficheros (N) del tarFile y lo volcamos en nr_files ...
	// /* Reservamos memoria para el array */
	// array=malloc(sizeof(stHeaderEntry)*nr_files);
	// //... Leemos la metainformacion del tarFile y la volcamos en el array ...
	// /* Devolvemos los valores le´ıdos a la funci´on invocadora */
	// (*nFiles)=nr_files;
	// return (array);

	return NULL;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
	// Complete the function

	FILE* dest = fopen(tarName, "w+");

	int c,ret;
	if(dest != NULL){
		for(int i = 0; i < nFiles; ++i){
			FILE* file = fopen(fileNames[i], "r");

			while ( (c = fgetc ( file )) != EOF ) {
        		fputc ( c, dest );
    		}
			fclose(file);
		}
		fclose(dest);

		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{
	// Complete the function
	return EXIT_FAILURE;
}
