// Linked list implementation in C

#include <stdio.h>
#include <stdlib.h>

// Creating a node
struct node 
{
    int num;                        //Data of the node
    struct node *nextptr;           //Address of the next node
} *stnode; //stnode is the checker if the node contains a value or not

struct node *pole1, *pole2, *pole3;

// TODO: Pass the POLE AND No of Disk
// This will 
void createNodeList(int n)
{
    struct node *fnNode, *tmp;
    int num, i;
    stnode = (struct node *)malloc(sizeof(struct node));

    if(stnode == NULL) //check whether the fnnode is NULL and if so no memory allocation
    {
        printf(" Memory can not be allocated.");
    }
    else
    {
        // reads data for the node through keyboard
        stnode->num = n;      
        stnode->nextptr = NULL; // links the address field to NULL
        tmp = stnode;

        // Creating n nodes and adding to linked list
        for(i=n-1; i>=1; i--)
        {
            fnNode = (struct node *)malloc(sizeof(struct node));
            if(fnNode == NULL)
            {
                printf(" Memory can not be allocated.");
                break;
            }
            else
            { 
                fnNode->num = i;      // links the num field of fnNode with num
                fnNode->nextptr = NULL; // links the address field of fnNode with NULL
 
                tmp->nextptr = fnNode; // links previous node i.e. tmp to the fnNode
                tmp = tmp->nextptr; 
            }
        }
    }
}


void displayList()
{
    struct node *tmp;
    if(stnode == NULL)
    {
        // Since stnode will be POLE1, POLE2, POLE3 we can already
        // distinguish the len then we will just loop this according to
        // the no of disk
        printf(" List is empty.");
    }
    else
    {
        tmp = stnode;
        while(tmp != NULL)
        {
            printf("%d=", tmp->num);       // prints the data of current node
            tmp = tmp->nextptr;                     // advances the position of current node
        }

        printf("\n");
    }
} 

int main() {
    int n = 3;

    //initialize the Disks
    createNodeList(n);
    displayList();
}