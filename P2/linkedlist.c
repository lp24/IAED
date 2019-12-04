#include "linkedlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

Node* create_node(int type){
    Node *node=malloc(sizeof(Node));
    if (node==NULL){
        fprintf(stderr,"\nErro ao alocar memoria\n");
        return(NULL);
    }
    switch(type){
        case 1:
            node->data=malloc(sizeof(Task));
            if (node->data==NULL){
                fprintf(stderr,"\nErro ao alocar memoria\n");
                return NULL;
            }
            break;
        case 2:
            break;
        default:
            return NULL;
    }
    node->type=type;
    return node;    
}

Node* add_element(Node *head, int type){
    Node *last=create_node(type);
    if (head==NULL){
        last->prev=last;
        last->next=last;
        return last;
    }
    last->next=head;
    last->prev=head->prev;
    head->prev->next=last;
    head->prev=last;
    return last;
}

Node* find_id(Node *head, long id){
    if (head==NULL){
        return NULL;
    }
    Node *node=head;
    do{
        if(node->data->id==id){
            return node;
        }           
        node=node->next;
    }while(node!=head);
    return NULL;
}

void free_element(Node *element){
    if (element==NULL){
        return;
    }
    switch(element->type){
        case 1:
            free(element->data->descricao);
            free_list(element->data->dependente_de);
            free(element->data);
            break;
        case 2:
            element->data->necessario--;
            break;
        default:
            return;
    }
    free(element);
}   
        
void free_list(Node *head){
    if (head==NULL){
        return;
    }
    head=head->prev;
    Node *node=head;
    Node *temp;    
    do{ /*removes from last to first */
        temp=node;
        node=node->prev;
        free_element(temp);
    }while(node!=head);
}       
    
Node *remove_element(Node *head, Node *element){
    if (element==NULL||head==NULL){
        return NULL;
    }
    
    if(head->next==head){    /*removing from a single element list*/
        head=NULL;
    }
    else{
        if (head==element){ /*removing the head*/
            head=head->next;
        }
        element->next->prev=element->prev;
        element->prev->next=element->next;
    }
    free_element(element);
    return head;
}    
        






