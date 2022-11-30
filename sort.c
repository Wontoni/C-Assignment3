#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sort.h"
#include "list_handler.h"

/**
 * File:    sort.c
 *
 * Author:  Sumit Khanduri
 * Date:    Fall 2022
 * Partner: Winston Nguyen
 *          Amarra Hong
 *          Eugene Jeong
 * Course:  COMP 2510 - Procedural Programming
 *
 * Summary of File:
 *      This file contains code to sort a linked list using the
 *      mergesort algorithm.
 *
 */

/**
 * The findMid function will look for the middle node in the
 * linked list.
 *
 * @param head Head of the linked list to check through
 *
 * @return the middle node of the linked list
 *
 */
node *findMid(node *head)
{
    node *slow = head;
    node *fast = head->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

/**
 * The merge function will check the left and right nodes
 * values and order them.
 *
 * @param left left node to merge
 * @param right right node to merge
 *
 * @return the nodes sorted into one linked list.
 *
 */
node *merge(node *left, node *right)
{
    if (left == NULL)
        return right;
    if (right == NULL)
        return left;

    node *ans = (node *)malloc(sizeof(node));
    node *temp = ans;
    ans->next = (node *)malloc(sizeof(node));

    while (left != NULL && right != NULL)
    {
        if (left->base < right->base)
        {
            temp->next = left;
            left->prev = temp;
            temp = temp->next;
            left = left->next;
        }
        else
        {
            temp->next = right;
            right->prev = temp;
            temp = temp->next;
            right = right->next;
        }
    }

    while (left != NULL)
    {
        temp->next = (node *)malloc(sizeof(node));
        temp->next = left;
        left->prev = temp;
        temp = temp->next;
        left = left->next;
    }
    while (right != NULL)
    {
        temp->next = (node *)malloc(sizeof(node));
        temp->next = right;
        right->prev = temp;
        temp = temp->next;
        right = right->next;
    }
    ans = ans->next;
    ans->prev = NULL;
    return ans;
}

/**
 * The mergeSort function will sort a linked list using
 * the mergesort algorithm, by finding the middle node,
 * and sorting the left and right through recursive call
 * chains.
 *
 * @param head:   Head of the linked list to sort
 *
 * @return the sorted linked list's head
 *
 */
node *mergeSort(node *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    node *mid = findMid(head);
    node *left = head;
    node *right = mid->next;
    mid->next = NULL;

    left = mergeSort(left);
    right = mergeSort(right);

    node *result = merge(left, right);

    return result;
}