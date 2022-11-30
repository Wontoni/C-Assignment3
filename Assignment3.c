#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief
 *
 */
typedef struct node
{
    // type defines if it is a process or a heap
    char *type;
    // is the start index
    int base;
    // size of the memory block
    int limit;
    struct node *next;
    struct node *prev;
} node;

/**
 * @brief
 *
 * @param head
 * @return node*
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
                break;
            case 2:
                headcpy->limit = atoi(token);
                break;
            default:
                printf("Extra input\n");
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

node *mergeHoles(node *head)
{
    node *curr = head;
    node *newHead = curr;
    node *next;
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
node *compactMemory(node *head)
{
    node *curr = head;
    node *newHead = curr;
    node *next;
    int flag;
    int i = 0;
    while (curr->next != NULL)
    {
        flag = 0;
        if (curr->next->type[0] == 'P')
        {
            if (curr->type[0] == 'H')
            {
                int tempBase = curr->next->base;
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
int menu()
{
    printf("1. load an input file\n2. merge holes\n3. compact memory\n4. print memory view\n5. Exit the program\n\n");
    int option;
    scanf("%d", &option);
    return option;
}

int main(int argc, char **argv)
{
    FILE *inputFile = NULL;
    int option;
    node *head;
    while ((option = menu()) != 5)
    {
        if (option == 1)
        {
            char fileName[100];
            printf("Type the name of the file:\n");
            scanf("%s", &fileName);
            if ((inputFile = fopen(fileName, "r")) != NULL)
            {
                head = createLinkedList(inputFile);
                printf("\nOperation Successful\n\n");
            }
            else
            {
                printf("Error: File could not be opened");
                return 0;
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
                return 0;
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
                return 0;
            }
        }
        else if (option == 4)
        {
            if (inputFile == NULL)
            {
                printf("\nError: No file loaded\n");
                return 0;
            }
            printLinkedList(head);
        }
    }

    return 0;
}
