#include "linkedList.h"

#define TRUE  1;
#define FALSE 0;

/**
 * @file linkedList.c
 * @author Josh Cotes
 * @note CSCD340_w16_Lab1
 */

/**
 * @brief The so called "constructor" for the linked list
 *
 * The linked list constructor builds a non circular linked list that
 * contains a dummy head node.  This is meant to be similar to a Java
 * linked list constructor.
 *
 * @return LinkedList * - The linked list pointer that contains a dummy head node but is not circular.
 */
LinkedList *linkedList() {

    LinkedList *new_list = calloc(1, sizeof(LinkedList));
    new_list->head = calloc(1, sizeof(Node));
    new_list->head->data = NULL;
    new_list->head->next = NULL;
    new_list->head->prev = NULL;
    new_list->size = 0;
    return new_list;
}

/**
 * @brief The add last function for the linked list
 *
 * Appends the specified node to the end of this list
 *
 * @param theList - The specified linked list
 * @param nn - The node to be added
 *
 * @warning - The passed in LinkedList * theList is checked - exit(-99) if NULL
 * @warning - The passed in Node * nn is checked - exit(-99) if NULL
 */
void addLast(LinkedList *theList, Node *nn) {

    if (theList == NULL || nn == NULL)
        exit(-99);

    else if (theList->size == 0) {
        theList->size++;
        theList->head->next = nn;
        nn->prev = theList->head;
    }
    else {
        Node *curr = theList->head;
        int node_num = 0;
        while (node_num++ < theList->size)
            curr = curr->next;
        curr->next = nn;
        nn->prev = curr;
        theList->size++;
    }
}

/**
 * @brief The add first function for the linked list
 *
 * Appends the specified node to the beginning of this list
 *
 * @param theList - The specified linked list
 * @param nn - The node to be added
 *
 * @warning - The passed in LinkedList * theList is checked - exit(-99) if NULL
 * @warning - The passed in Node * nn is checked - exit(-99) if NULL
 */
void addFirst(LinkedList *theList, Node *nn) {

    if (theList == NULL || nn == NULL)
        exit(-99);

    else if (theList->size == 0) {
        theList->head->next = nn;
        nn->prev = theList->head;
        theList->size++;
    }
    else {
        nn->next = theList->head->next;
        nn->prev = theList->head;
        theList->head->next->prev = nn;
        theList->head->next = nn;
        theList->size++;
    }
}

/**
 * @brief The remove item function for the linked list
 *
 * Removes the first occurrence of the specified element from this list, if it
 * is present.  If this list does not contain the element, it is unchanged.
 *
 * @param theList - The specified linked list
 * @param nn - The node to be removed
 * @param *removeData - The function pointer for freeing the specific data type
 * @param *compare - The compare function to compare specific data type
 *
 * @warning - The passed in LinkedList * theList is checked - exit(-99) if NULL
 * @warning - The passed in Node * nn is checked - exit(-99) if NULL
 */
int removeItem(LinkedList *theList, Node *nn, void (*removeData)(void *), int (*compare)(const void *, const void *)) {

    int found = FALSE;

    if (theList == NULL || nn == NULL) {
        exit(-99);
    }
    else if (theList->size == 0) {
        return FALSE;
    }
    else {
        Node *curr = theList->head;
        Node *remove;

        do {
            if (compare(curr->next->data, nn->data) == 0) {

                found = TRUE;
                remove = curr->next;
                // remove the node //
                curr->next = curr->next->next;
                if (curr->next != NULL)
                    curr->next->prev = curr;
                theList->size--;
                // clean the removed node //
                removeData(remove->data);
                free(remove);
                remove = NULL;
            }
            curr = curr->next;
        } while (!found && curr->next != NULL && curr != NULL);
    }
    // clean up the passed in node //
    if (nn->data != NULL) {
        removeData(nn->data);
        nn->data = NULL;
    }
    free(nn);
    nn = NULL;
    if (found)
        return TRUE;
    return FALSE;
}

void removeFirst(LinkedList *list, void (*removeData)(void *)) {

    if (list == NULL)
        exit(-99);
    if (list->size == 0)
        return;

    Node *curr = list->head;
    Node *remove = curr->next;
    // remove the node //
    curr->next = curr->next->next;
    if (curr->next != NULL)
        curr->next->prev = curr;
    list->size--;
    // clean the removed node //
    removeData(remove->data);
    free(remove);
    remove = NULL;

}

/**
 * @brief Empties the list and its contents
 *
 * Removes all of the elements from this list.  The list will be empty after the function completes
 *
 * @param theList - The specified linked list
 * @param *removeData - The function pointer for freeing the specific data type
 * @param *compare - The compare function to compare specific data type
 *
 * @warning - The passed in LinkedList * theList is checked - if NULL nothing happens
 */
void clearList(LinkedList *theList, void (*removeData)(void *)) {

    if (theList != NULL && theList->size > 0) {

        Node *curr = theList->head->next;

        // free the head node //
        if (curr->prev != NULL)
            free(curr->prev);
        curr->prev = NULL;
        // free the rest of the nodes and their data //
        while (curr->next != NULL) {
            curr = curr->next;
            if (curr->prev->data != NULL)
                removeData(curr->prev->data);
            free(curr->prev);
            curr->prev = NULL;
        }
        // free the last node //
        removeData(curr->data);
        free(curr);
    }
        // if the list is empty, free only the head node //
    else if (theList != NULL && theList->size == 0) {
        theList->head->prev = NULL;
        theList->head->data = NULL;
        theList->head->next = NULL;
        free(theList->head);
    }
}

/**
 * @brief Prints the contents of this linked list
 *
 * Prints the contents, if there are any of this linked list.  If the list is NULL
 * or empty, "Empty List" is printed.
 *
 * @param theList - The specified linked list
 * @param *convert - The function pointer for printing the specific data type
 *
 * @warning - The passed in LinkedList * theList is checked - if NULL "Empty List" is printed
 */
void printList(const LinkedList *theList, void (*convertData)(void *)) {

    if (theList == NULL || theList->size == 0)
        return;

    else {
        Node node_curr;
        node_curr = *theList->head;
        do {
            node_curr = *node_curr.next;
            convertData(node_curr.data);
        } while (node_curr.next != NULL);
        printf("\n");
    }
}

void printHistory(const LinkedList *theList, int count, void (*convertData)(void *)) {

    int cmd_num = 1;
    int start;
    if (theList == NULL || theList->size == 0)
        return;
    else {
        Node node_curr;
        node_curr = *theList->head;
        int size = theList->size;
        if (theList->size > count)
            while (size-- > count) {
                node_curr = *node_curr.next;
                cmd_num++;
            }
        do {
            if (theList->size)
            node_curr = *node_curr.next;
            printf("%d  ", cmd_num++);
            convertData(node_curr.data);
        } while (node_curr.next != NULL);
    }
}

void *getItem(const LinkedList *theList, int num) {

    int cmd_num = 0;
    if (theList == NULL || theList->size == 0 || num > theList->size || num < 1)
        return NULL;


    Node *node_curr;
    node_curr = theList->head;

    while (cmd_num++ < num)
        node_curr = node_curr->next;

    return node_curr->data;
}

void emptyList(LinkedList *theList, void (*removeData)(void *)) {

    if (theList != NULL && theList->size > 0) {

        Node *curr = theList->head->next;

        // free the rest of the nodes and their data //
        while (curr->next != NULL) {
            curr = curr->next;
            if (curr->prev->data != NULL)
                removeData(curr->prev->data);
            free(curr->prev);
            curr->prev = NULL;
        }
        // free the last node //
        removeData(curr->data);
        free(curr);
        theList->size = 0;
    }
}