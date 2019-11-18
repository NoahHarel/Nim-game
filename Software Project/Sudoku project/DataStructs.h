#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* A structure to represent a stack */
struct StackNode {
    int* data;
    struct StackNode* next;
} StackNode ;

/*struct StackNode* newNode(int data)
{
    struct StackNode* stackNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}*/
struct StackNode* newNode(int* data);

int isEmpty(struct StackNode* root);

void push(struct StackNode** root, int* data);

int* pop(struct StackNode** root) ;

int* peek(struct StackNode* root) ;

int backtrack_stack(struct Board board);

#endif


