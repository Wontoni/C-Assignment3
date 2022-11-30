#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sort.h"
#include "list_handler.h"

/**
 * File:    list_handler.c
 *
 * Author:  Sumit Khanduri
 * Date:    Fall 2022
 * Partner: Winston Nguyen
 *          Amarra Hong
 *          Eugene Jeong
 * Course:  COMP 2510 - Procedural Programming
 *
 * Summary of File:
 *      This file contains code to create a create and parse
 *      through a linked list to print or move holes in memory
 *      together.
 *
 */

/**
 * The createLinkedList function will parse through a given file
 * and format to create a linked list and sorts it.
 *
 * @param fp File to be parsed through
 *
 * @return head node of the produced linked list
 *
 */
node *createLinkedList(FILE *fp)
{
    node *head = (node *)malloc(sizeof(node));
    head->next = NULL;
    node *headcpy = head;
    char *token;
    while (!feof(fp))
    {
        char line[100];
        fgets(line, 100, fp);
        token = strtok(line, " \n");
        headcpy->next = (node *)malloc(sizeof(node));
        headcpy->next->prev = headcpy;
        headcpy = headcpy->next;
        int counter = 0;
        int len;
        while (token != NULL)
        {
            switch (counter)
            {
                case 0:
                    len = strlen(token);
                    headcpy->type = (char *)malloc(len * sizeof(char));
                    strcpy(headcpy->type, token);

                    break;
                case 1:
                    headcpy->base = atoi(token);
                    if(headcpy->base < 0) {
                    printf("Error: The base was less than 0\n");
                    exit(0);
                    }
                    break;
                case 2:
                    headcpy->limit = atoi(token);
                    break;
                default:
                    printf("Error: There was an extra input\n");
                    exit(-1);
                    break;
            }
            token = strtok(NULL, " \n");
            counter++;
        }
        headcpy->next = NULL;
    }

    head = head->next;
    head = mergeSort(head);
    return head;
}

/**
 * The printLinkedList function will start at a linked list's
 * head and print every node into the console.
 *
 * @param head:   Head node of the linked list
 *
 * @return Nothing -- Note: Prints the linked list in the console
 *
 */
void printLinkedList(node *head)
{
    node *temp = head;
    int i = 1;
    while (temp != NULL)
    {
        printf("Node %d:", i);
        if (temp->type[0] == 'H')
        {
            printf("%s (Hole)", temp->type);
        }
        printf("%s, ", temp->type);
        printf("base = %d, ", temp->base);
        printf("limit = %d\n", temp->limit);
        temp = temp->next;
        i++;
    }
    printf("\n");
}

/**
 * The mergeHoles function will check if two nodes adjacent to
 * each other are both holes, and will merge them into one hole
 * if they are.
 *
 *
 * @param head Head of the linked list to check through
 *
 * @return head node of the linked list wit holes merged
 *
 */
node *mergeHoles(node *head)
{
    node *curr = head;
    node *newHead = curr;
    int flag;
    while (curr->next != NULL)
    {
        flag = 0;
        if (curr->next->type[0] == 'H')
        {
            if (curr->type[0] == 'H')
            {
                if (curr->base + curr->limit == curr->next->base)
                {
                    // assign values
                    curr->next->base = curr->base;
                    curr->next->limit += curr->limit;

                    // assign the prev node's to the curr node's next
                    curr->prev->next = curr->next;

                    // move the pointer forward
                    curr = curr->next;

                    //
                    curr->prev = curr->prev->prev;

                    flag = 1;
                }
            }
        }
        if (flag == 0)
            curr = curr->next;
    }
    return newHead;
}

/**
 * The compactMemory function will move all holes in memory at the end, and
 * move allocated blocks of memory together in the linked list.
 *
 *
 * @param head Head of the linked list to check through
 *
 * @return head node of the linked list with memory compacted
 *
 */
node *compactMemory(node *head)
{
    node *curr = head;
    node *newHead = curr;
    int flag;
    while (curr->next != NULL)
    {
        flag = 0;
        if (curr->next->type[0] == 'P')
        {
            if (curr->type[0] == 'H')
            {
                curr->next->base = curr->base;
                curr->base = curr->base + curr->next->limit;

                node *currNext = curr->next;

                // link the curr's prev node to the curr's next cutting
                // off the curr from between
                curr->prev->next = currNext;

                // link curr's next to the curr->next->next
                curr->next = currNext->next;
                // move curr up
                currNext->next = curr;

                currNext->prev = curr->prev;
                curr->prev = currNext;

                // check if we have reached the last or not
                if (curr->next != NULL)
                    curr->next->prev = curr;

                flag = 1;
                newHead = mergeHoles(newHead);
                curr = newHead;
            }
        }
        if (flag == 0)
            curr = curr->next;
    }
    return newHead;
}