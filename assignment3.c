#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sort.h"
#include "list_handler.h"

/**
 * File:    assignment3.c
 *
 * Author:  Sumit Khanduri
 * Date:    Fall 2022
 * Partner: Winston Nguyen
 *          Amarra Hong
 *          Eugene Jeong
 * Course:  COMP 2510 - Procedural Programming
 *
 * Summary of File:
 *      This file contains code to drive the program, and displays a menu in the console
 *      to prompt the user to input an option.
 *
 */

/**
 *
 * int menu()
 *
 * Summary of the findMid function:
 *      The menu function will print a menu in the console
 *      and prompt the user to input an option.
 *
 * Return Value : option selected by the user
 *
 */
int menu()
{
    printf("\n1. load an input file\n2. merge holes\n3. compact memory\n4. print memory view\n5. Exit the program\n\n");
    int option;
    scanf("%d", &option);
    return option;
}

/**
 *
 * int main(int argc, char **argv)
 *
 * Summary of the main function:
 *      The main function will drive the program, and check if input
 *      values are valid, and call other files/functions.
 *
 * Parameters   : argc:   Number of arguments
 *                **argv: Provided arguments
 *
 *
 * Return Value : exit value
 *
 */
int main(int argc, char **argv)
{
    FILE *inputFile = NULL;
    int option;
    node *head;
    int fileFlag = 0;
    while ((option = menu()) != 5)
    {
        if (option == 1)
        {
            if (fileFlag == 1)
            {
                printf("File was already loaded");
                return -1;
            }
            char fileName[100];
            printf("Type the name of the file:\n");
            scanf("%s", &fileName);
            if ((inputFile = fopen(fileName, "r")) != NULL)
            {
                head = createLinkedList(inputFile);
                printf("\nOperation Successful\n\n");
                fileFlag = 1;
            }
            else
            {
                printf("Error: File could not be opened");
                return -1;
            }
        }
        else if (option == 2)
        {
            if (inputFile != NULL)
            {
                head = mergeHoles(head);
                printf("\nOperation Successful\n\n");
            }
            else
            {
                printf("\nError: No file loaded\n");
                return -1;
            }
        }
        else if (option == 3)
        {
            if (inputFile != NULL)
            {
                head = compactMemory(head);
                printf("\nOperation Successful\n\n");
            }
            else
            {
                printf("\nError: No file loaded\n");
                return -1;
            }
        }
        else if (option == 4)
        {
            if (inputFile == NULL)
            {
                printf("\nError: No file loaded\n");
                return -1;
            }
            printLinkedList(head);
        }
        else if (option == 5)
        {
            return 0;
        }
        else
        {
            printf("\nError: Invalid option\n");
            return -1;
        }
    }

    fclose(inputFile);
    return 0;
}
