/**
 * @file    : escape_character.c
 * @author  : bullydd
 * @brief   : Program that converts characters like newline, tab and space
 *            into visible escape sequences like \n and \t as it copies the string
 *            t to s. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ======== FUNCTION PROTOTYPES ======== */

void escape(char *s, char *t);

/* ====== MAIN PROGRAM ENTRY POINT ==== */

int main (int argc, char *argv[]) {

    long index = 0, length = 0;


    if (argc < 2) {
        printf ("Usage\n %s filename.txt\n", argv[0]);
    }

    else if (argc == 2) {
        length = strlen(argv[1]);
    }

    char *filename = (char*)malloc(length + 1);
    if (filename == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return EXIT_FAILURE;
    }
    // copy argv value to filename
    strcpy(filename, argv[1]);

    FILE *fp = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed.\n");
        return -1;
    }

    /* ====== BENCHMARKS ======== */

    clock_t start = clock();

    // Count the number of character in this file
    
    /*
    
        BENCHMARK 1 : Elapsed time is 0.000416 seconds
        Counting the number of char with this while loop 
        and then allocate memory for s after copy each 
        character from fp to s, take 416 microseconds

        while(fgetc(fp) != EOF) {
            index++;
        }
            
    */

    /*
        BENCHMARK 2 : Elapsed time is 0.000310 seconds

        while counting the number of character with fseek
        and then allocate memory for s after copy each 
        character from fp to s, take 310 microseconds.
        106 microseconds of difference.
        
    */
    fseek(fp, 0, SEEK_END);
    index = ftell(fp);

    // Now Allocate the real size for user entry
    char *s = (char*)malloc((index+1) * sizeof(char));
    if (s == NULL) {
        printf("ERROR::Allocation failed\n");
        return -1;
    }

    int i = 0, line = 0;
    rewind(fp); // Place the cursor back to the top of fp

    // Now copy fp into s
    while( (line = fgetc(fp)) != EOF) {
        *(s+i) = line;
        i++;
    }
    s[i] = '\0';

    clock_t end = clock();

    double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Elapsed time is %f seconds\n", elapsedTime);


    char *escaped = (char*)malloc(4 * strlen(s) + 1);
    printf("\n\n****** Before escape sequence ******\n\n%s\n", s);

    escape(escaped, s);
    printf("\n\n****** After escape sequence *****\n\n%s\n", escaped);

    // De-allocate memory
    free(s);
    free(escaped);
    free (filename);
    

    // Close the opened file
    fclose (fp);

    return EXIT_SUCCESS;
}


/* ======= FUNCTION DEFINITIONS ======= */

void escape(char *s, char *t) {

    int i = 0, j = 0;

    while (t[i] != '\0') {

        switch (t[i])
        {
            case ' ':
                s[j++] = '_';
                break;
            case '\t':
                s[j++] = '\\';
                s[j++] = 't';
                break;
            case '\n':
                s[j++] = '\\';
                s[j++] = 'n';
                break;
            default:
                s[j++] = t[i];
                break;
        }
        i++;
    }
    s[j] = '\0';

}   