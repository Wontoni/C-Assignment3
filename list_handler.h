/**
 * This is the node struct.
 * Contains definitions regarding type,
 * index base, and memory limit.
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
node *createLinkedList(FILE *fp);
void printLinkedList(node *head);
node *mergeHoles(node *head);
node *compactMemory(node *head);