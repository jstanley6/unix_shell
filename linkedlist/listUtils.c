/**
 * @file listUtils.c
 * @author Josh Cotes
 * @note CSCD340_w16_Lab1
*/

#include "listUtils.h"
#include "../utils/fileUtil.h"

/**
 * @brief Builds a node that contains a pointer to the specific data type.
 *
 * The buildNode function builds a node for the linked list. The node's void *
 * data will refer to the specific data type. The specific data type is built
 * by calling the appropriate specific data function, which is called via
 * function pointer.
 *
 * @param fin - The FILE * used to read the data from the file
 * @param *buildData - The function pointer to build an object of the specific data type
 * @return Node * - Representing a node for the linked list containing the specific data type.
 *
 * @note - The first parameter FILE * fin is not used in the function.  It is entirely
 * used as a pass through parameter for the function pointer which is passed a FILE * also.
 *
 * @warning - Since FILE *fin is a pass through it is not checked.
 */
Node *buildNode(FILE *fin, void *(*buildData)(FILE *in)) {

    Node *new_node = calloc(1, sizeof(Node));

    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->data = buildData(fin);
    if (new_node->data == NULL) {
        free(new_node);
        return NULL;
    }
    else
        return new_node;
}


/**
 * @brief Builds a node that contains a call to a specific type.
 *
 * The buildNode_Type function builds a node for the linked list. The node's void *
 * data will refer to the specific data type. The specific data type is built
 * by calling the appropriate specific data function, which is called via
 * function pointer.
 *
 * @param *passedIn - The void * for the data type being created
 * @return Node * - Representing a node for the linked list containing the specific data type.
 */
Node *buildNode_Type(void *passedIn) {

    Node *s_newnode;
    s_newnode = calloc(1, sizeof(Node));
    s_newnode->data = passedIn;
    s_newnode->prev = NULL;
    s_newnode->next = NULL;
    return s_newnode;
}

/**
 * @brief Sorts the linked list.
 *
 * The sort function resides here because sorting a linked list
 * is not considered an integral function within the required
 * linked list functions.
 *
 * @param theList - The linked list  * representing the list
 * @param *compare - The specific data type compare function as a function pointer
 *
 * @warning - The passed in LinkedList * theList is checked - exit(-99) if NULL
 * @warning - The theList-> size is checked and if the list contains 0 or 1 element then the function
 * does not attempt to sort the list.
 */
void sort(LinkedList *theList, int (*compare)(const void *, const void *)) {

    if (theList == NULL)
        exit(-99);
    else if (theList->size > 1) {

        Node *runner = theList->head->next->next;
        Node *curr;

        void *temp_data;

        while (runner != NULL) {
            curr = runner;
            while (curr->prev->data != NULL && compare(curr->data, curr->prev->data) < 0) {

                temp_data = curr->prev->data;
                curr->prev->data = curr->data;
                curr->data = temp_data;
                curr = curr->prev;
            }
            runner = runner->next;
        }
    }
}

/**
 * @brief Builds an initial linked list.
 *
 * The build list creates an initial linked list by reading from
 * the file, calling the buildNode function and then adds that node
 * into the list by calling addFirst.
 *
 * @param theList - The linked list  * representing the list
 * @param total - The total items to be placed into the list
 * @param fin - The FILE * used to read the data from the file
 * @param *buildData - The function pointer to call the specific function
 * to build the appropriate data type.
 *
 * @note - The parameter FILE * fin is not used in the function.  It is entirely
 * used as a pass through parameter for the function pointer which is also passed a FILE *.
 *
 * @warning - The passed in LinkedList * theList is checked - exit(-99) if NULL
 * @warning - The passed in int total is checked - exit(-99) if <= 0
 * @warning - Since FILE *fin is a pass through it is not checked.
 */
void buildList(LinkedList *myList, int total, FILE *fin, void *(*buildData)(FILE *in)) {

    if (myList == NULL || total <= 0)
        exit(-99);

    Node *curr = myList->head;

    int i;
    for (i = 0; i < total; i++) {
        curr->next = buildNode(fin, buildData);
        curr->next->prev = curr;
        curr->next->next = NULL;
        curr = curr->next;
        myList->size++;
    }
}

/**
 * builds the list in reverse
 */
void buildHistory(LinkedList *myList, int total, FILE *fin, void *(*buildData)(FILE *in)) {

    if (myList == NULL || total <= 0)
        exit(-99);

    Node *curr = NULL;
    while (curr == NULL)
        curr = buildNode(fin, buildData);

    myList->size++;
    int i;
    for (i = 1; i < total; i++) {
        while (curr->prev == NULL)
            curr->prev = buildNode(fin, buildData);
        curr->prev->next = curr;
        curr->prev->prev = NULL;
        curr = curr->prev;
        myList->size++;
    }
    curr->prev = myList->head;
    myList->head->next = curr;
}

void writeHistory(LinkedList *list) { // builds the list backwards (like in ubuntu)

    if (list == NULL || list->size == 0)
        return;

    else {
        history *s_hist;

        FILE *fp = fopen(MSSHHISTLOC, "w");
        Node node_curr;
        node_curr = *list->head;

        do {
            node_curr = *node_curr.next;
        } while (node_curr.next != NULL);

        do {

            int counter, i;

            s_hist = (history *) node_curr.data;

            for (counter = 0; counter < s_hist->argc; counter++) {
                fputs(*(s_hist->argv + counter), fp);
                fputc(' ', fp);
            }
            fputc('\n', fp);
            node_curr = *node_curr.prev;
        } while (node_curr.prev != NULL);
        fclose(fp);
    }
}


