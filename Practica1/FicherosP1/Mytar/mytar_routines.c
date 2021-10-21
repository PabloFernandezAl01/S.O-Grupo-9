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

	int copiedBytes = 0;
	int c;

	//Mientras que los bytes copiados no sea menos que los
	//que se tienen que copiar o no se ha llegado al final del archivo
	while(copiedBytes < nBytes && (c = getc(origin)) != EOF) {
		
		//Escribe char a char
		putc((unsigned char) c, destination);
		copiedBytes++;
	}

	return copiedBytes;
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
	
	int n, size = 0;
	char * buf;

	do{
		n = getc(file);
		size++;
	}while ((n!=(int) '\0') && (n != EOF));

	if(n == EOF)
			return NULL;

	if((buf = (char*) malloc(size)) == NULL) return NULL;

	fseek(file, -size, SEEK_CUR);
	fread(buf, 1, size, file);

	return buf;
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

	stHeaderEntry* header;

	fread(nFiles, sizeof(int), 1, tarFile);

	if((header = (stHeaderEntry*) malloc(sizeof(stHeaderEntry)*(*nFiles))) == NULL){
		perror("Error al alojar memoria para devolver la cabecera");
		fclose(tarFile);
		return NULL;
	}

	for(int i = 0; i < *nFiles; i++){
		header[i].name = loadstr(tarFile);
		fread(&header[i].size, sizeof(header[i].size), 1, tarFile);
	}

	return header;
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

	 FILE* tarfile, *inputFile;
	 stHeaderEntry* header;
	 unsigned int headersize;

	//Si el numero de archivos es 0 o menor
	 if(nFiles <= 0){
		 fprintf(stderr, "%s", use);
		 return (EXIT_FAILURE);
	 }

	//Si no se puede abrir el fichero
	 if((tarfile = fopen(tarName, "w+")) == NULL){
		 fprintf(stderr, "El archivo mytar %s podría no haberse abierto: ", tarName);
		 perror(NULL);
		 return (EXIT_FAILURE);
	 }

	//Reserva el tamaño de la cabecera (con los nombres con tamaño fijo, porque van a ser punteros)
	//Si no puede reservar el tamaño salta una excepción
	 if((header = malloc(sizeof(stHeaderEntry)*nFiles)) == NULL){
		 perror("Error al alojar memoria en la cabecera del archivo mtar");
		 fclose(tarfile);
		 remove(tarName);
	 }

	//De momento el tamaño de la cabecera es el tamaño de un int para indicar el número de archivos que hay en el mtar
	 headersize = sizeof(int);

	//Para cada archivo, reserva en memoria el tamaño de su nombre (string)
	//y aumenta el tamaño de la cabecera con el tamaño del nombre y el tamaño del size en bytes que ocupa el fichero
	 for(int i = 0; i < nFiles; i++){
		 int namesize = strlen(fileNames[i]) + 1;

		 header[i].name = (char*) malloc(namesize);
		 strcpy(header[i].name, fileNames[i]);

		 headersize+= namesize+ sizeof(header->size);
	 }

	//Coloca el cursor en la posicion donde debería a empezar a escribir los ficheros
	 fseek(tarfile, headersize, SEEK_SET);

	 for(int i = 0; i < nFiles; i++){
		 if((inputFile = fopen(fileNames[i], "r")) == NULL){
			 fprintf(stderr, "No es posible abrir el archivo de entrada %s : \n", fileNames[i]);
			 perror(NULL);
			 fclose(tarfile);
			 remove(tarName);
			 for(int j = 0; j < nFiles; j++){
				free(header[j].name);
			 }
			 free(header);
			 return (EXIT_FAILURE);
		 }

		//Los escribe y guarda en el array de stHeaderEntry lo que ocupa en bytes
		 header[i].size = copynFile(inputFile, tarfile, INT_MAX);

		 //Va cerrando cada archivo ya copiado
		 fclose(inputFile); 
	 }

	//Vuelve a apuntar al comienzo del archivo para rellenar la cabecera una vez obtenida la información
	rewind(tarfile);

	fwrite(&nFiles, sizeof(int), 1, tarfile);

	for(int i = 0; i < nFiles; i++){
		fwrite(header[i].name, 1, strlen(header[i].name)+1, tarfile);
		fwrite(&header[i].size, sizeof(header[i].size), 1, tarfile);
	}

	for(int j = 0; j < nFiles; j++) free(header[j].name);
	free(header);

	return (EXIT_SUCCESS);

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
	stHeaderEntry* header = NULL;
	int nArchivos;
	FILE* tarFile = NULL;

	if( (tarFile = fopen(tarName, "r")) != NULL) {
		header = readHeader(tarFile, &nArchivos);

		for(int i = 0; i < nArchivos; i++){
			FILE* newFile = fopen(header[i].name, "w+");
			copynFile(tarFile, newFile, header[i].size);
			fclose(newFile);
		}

		for(int i = 0; i < nArchivos; i++) free(header[i].name);
		free(header);
		fclose(tarFile);

		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
