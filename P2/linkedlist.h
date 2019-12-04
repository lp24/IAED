#include <stdio.h>

typedef struct{
    long id;
    char *descricao;
    long duracao;
    long early;
    long late;
    struct Node *dependente_de; /*list of tasks that it depends on */
    int necessario; /*number of tasks that require this one, which is also the number of pointers pointing to this task */
}Task;

/*node of circular doubly linked list*/
typedef struct Node{
    Task *data; 
    int type; /* type1 is the list of tasks, type2 is the list of dependencies (points to the task they depend on in the list of taks)*/
    struct Node *next;
    struct Node *prev;
}Node;
 
Node *add_element(Node *next_node, int type); /*creates, then adds a node before 'next node'. Can add to empty list. Returns the node added */
Node *find_id(Node *node, long id); /*returns the node with the id 'id'*/
Node *remove_element(Node *head, Node *node); /*removes the node 'node', returns the head of the list */
void free_list(Node *node);
