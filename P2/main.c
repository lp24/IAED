#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "linkedlist.h"

#define MAX_DESCRICAO 8000
#define MAX_LE 100
    
/*global variables*/

int caminho_critico=0; /*0 if there is no defined path (after adding or removing task), 1 if there is (after command path)*/
long duracao_projeto=0; /* 0 if there is no defined path */

/*auxiliar functions*/

/*turns command into integer for switch case*/
int case_command(char* token){
    if (!strncmp(token,"add",strlen("add"))){
        return 1;
    }
    else if (!strncmp(token,"duration",strlen("duration"))){
        return 2;
    }
    else  if (!strncmp(token,"depend",strlen("depend"))){
        return 3;
    }
    else  if (!strncmp(token,"remove",strlen("remove"))){
        return 4;
    }
    else  if (!strncmp(token,"path",strlen("path"))){
        return 5;
    }
    else  if (!strncmp(token,"exit",strlen("exit"))){
        return 6;
    }
    else if(!strncmp(token,"first",strlen("first"))){
        return 7;
    }
    else if(!strncmp(token,"second",strlen("second"))){
        return 8;
    }
    return -1;
}

char* parse_string(char *string, char* s){
    char* token;
    token=strtok(string,s);
    return token;
}

long parse_long(char *string){ /* returns -1 if empty string, -2 if string is not an integer*/
    char* s=" ";
    char* token;
    long value;
    token=parse_string(string,s);
    if(token==NULL){
        return -1;
    }
    char *next;

    value=strtol(token,&next,10);
    if(next==token){
        return -2;
    }
    return value;
}

/*creates a new node with a new task and adds it to the project. returns the node*/
Node* nova_tarefa(Node *project,long id,long duracao,char *descricao, Node *dependente_de){
    Node *node=add_element(project,1);
    node->data->id=id;
    node->data->duracao=duracao;
    node->data->descricao=descricao;
    node->data->early=-1;
    node->data->late=-1;
    node->data->dependente_de=dependente_de;
    node->data->necessario=0;
    return node;
}

/*creates a new node poiting to the task 'tarefa' and adds it to the dependencies list 'dependente_de'*/
Node* novo_depende(Node *dependente_de,Task *tarefa){
    Node *node=add_element(dependente_de,2);
    tarefa->necessario++;
    node->data=tarefa;
    return node;
}

/*prints a task in the commands 'duracao' and 'path' format */
void print_task(Task *tarefa){
    if(caminho_critico){
        if (tarefa->early!=tarefa->late){ /*if not critical */
            printf("%ld \"%s\" %ld [%ld %ld]",tarefa->id,tarefa->descricao,tarefa->duracao,tarefa->early,tarefa->late);
        }
        else{
            printf("%ld \"%s\" %ld [%ld CRITICAL]",tarefa->id,tarefa->descricao,tarefa->duracao,tarefa->early);
        }
    }
    else{
        printf("%ld \"%s\" %ld",tarefa->id,tarefa->descricao,tarefa->duracao);
    }
    Node *depende=tarefa->dependente_de;
    if (depende==NULL){
        printf("\n");
        return;
    }        
    Node *node=depende;
    do{
        printf(" %ld",node->data->id);
        node=node->next;
    }while(node!=depende);
    printf("\n");
}

/*prints each task with duration bigger than n */
void print_duration(Node *head,long n){
    if (head==NULL){
        return;
    }
    Node *node=head;
    do{
        if((node->data->duracao)>=n){
            print_task(node->data);
        }
        node=node->next;
    }while(node!=head);
}

/* resets a path after it is no longer valid */
void remove_critico(Node *head){
    duracao_projeto=0;
    if(head==NULL){
       return;
    }
    Node *node=head;
    do{
        node->data->early=-1;
        node->data->late=-1;
        node=node->next;
    }while(node!=head);
}

 /* calculates (if necessary) and returns the early start of task 'tarefa' */    
long early(Task *tarefa){
    if(tarefa==NULL){
        return -1;
    }
    if(tarefa->early!=-1){ /*early start was already calculated*/
        return tarefa->early;
    }
    long max=0;
    Node *depende=tarefa->dependente_de;
    if(depende==NULL){ /*doesn't depend on another task*/
        tarefa->early=0;
        return 0;
    }
    Node *dnode=depende;
    do{ /*checks max value of early start+duration each of the task's dependencies*/
        if (max<(dnode->data->duracao+early(dnode->data))){
            max=(dnode->data->duracao+early(dnode->data));
        }
        dnode=dnode->next;
    }while(dnode!=depende);
    
    tarefa->early=max;
    return max;        
}

/*calculates(if necessary) late start of task 'tarefa' and a temporary late start value to every task it depends of */
void late(Task *tarefa){
    if(tarefa==NULL){
        return;
    }
    if(!(tarefa->necessario)){ /*there is no task depending on 'tarefa'*/
        tarefa->late=duracao_projeto-tarefa->duracao;
    }
    
    Node *depende=tarefa->dependente_de;
    if (depende==NULL){ /*doesn't depend on another task */
        return;
    }
    Node *node=depende;
    
    do{ /* sets a temporary late start value to each of the tasks dependencies*/
        if(node->data->late==-1){
            node->data->late=tarefa->late-node->data->duracao;
        }
        else if(node->data->late>tarefa->late-node->data->duracao){
            node->data->late=tarefa->late-node->data->duracao;
        }
        node=node->next;
    }while(node!=depende);
    return;
}

/*Calculates early and late starts of each task*/
void calculate_path(Node *project){
    remove_critico(project);
    if (project==NULL){
        return;
    }
    long duracao_node=0;
    Node *node=project;
    do{ /*calculates early start of each task and project duration*/
        duracao_node=(early(node->data)+node->data->duracao);
        if (duracao_node>duracao_projeto){
            duracao_projeto=duracao_node;
        }
        node=node->next;
    }while (node!=project);
    
    node=project->prev;
    do{ /*calculates late start of each task, starting at the end and going backwards */
        late(node->data);
        node=node->prev;
    }while(node!=project);
    caminho_critico=1;
}

/*main*/
int main(){
    Node *project=NULL;
    do{
        
        /* reads input to string 'temp', MAX_LE chars at a time, then concatenates into the string 'input' */
        char temp[MAX_LE];
        char *input=calloc(MAX_LE,sizeof(char));
        if(input==NULL){
            fprintf(stderr,"\nErro ao alocar memoria\n");
            return -1;
        }            
        int input_len=0;
        int temp_len=0;
        do{  /*le todo o input para 'input' em pedacos de tamanho 'MAX_LE' */
            temp[0]='\0';
            if(fgets(temp,MAX_LE,stdin)==NULL){
                fprintf(stderr,"\nErro a ler input\n");
                return -1;
            }
            temp_len=strlen(temp);
            input_len+=temp_len;
            input=realloc(input,(input_len+1)*sizeof(char));
            if (input==NULL){
                fprintf(stderr,"\nErro ao realocar memoria\n");
                return -1;
            }
            strcat(input,temp);
        }while(temp_len==MAX_LE-1&&temp[temp_len-1]!='\n');
        
        char *token=NULL;        
        long id;
        char *descricao=NULL;
        long duracao; 
        Node *find;
        Node *dependente_de;
        Node *node;
        
        /*primeiro 'token' e o comando*/
        token=parse_string(input," ");

        if (!token){
            fprintf(stderr, "\nNo input\n");
            return -1;
        }
        int command=case_command(token);
        int ilegal_arg; /*used when necessary to break the switch when inside a while loop */      
        
        int bigger_len;
        int can_remove;
        int tam_descricao;
        switch (command){   
            
            case 1: /*add*/
                dependente_de=NULL;

                /*id*/
                id=parse_long(NULL);
                if(id<=0){
                    printf("illegal arguments\n");
                    break;
                }                            
                find=find_id(project,id);
                if (find!=NULL){
                    printf("id already exists\n");
                    break;
                }
            
                /*descricao*/
                token=parse_string(NULL, "\"");            
                int len=strlen(token);
                if(!token||len>MAX_DESCRICAO){ 
                    printf("illegal arguments\n");
                    break;
                }
                /*necessary to copy the description to free the rest of the input */
                descricao=malloc((len+1)*sizeof(char));
                if(descricao==NULL){
                    fprintf(stderr,"\nErro ao alocar memoria\n");
                    return -1;
                }                
                strcpy(descricao,token);
            
                /*duracao*/
                duracao=parse_long(NULL);
                if (duracao<=0){
                    printf("illegal arguments\n");
                    free(descricao);
                    break;
                }                       

                /*dependentes, creates list of dependencies*/
                long d_id;
                ilegal_arg=0;
                d_id=parse_long(NULL);
                while(d_id!=-1){ /*while there are more dependencies*/               
                    if (d_id<=0){
                        fprintf(stderr, "illegal arguments\n");
                        ilegal_arg=1;
                        break;
                    }                  
                    find=find_id(project,d_id);
                    if (find==NULL){
                        printf("no such task\n");
                        free_list(dependente_de);
                        free(descricao);
                        ilegal_arg=1;
                        break;
                    }          
                    node=novo_depende(dependente_de,find->data);
                    if(dependente_de==NULL){
                        dependente_de=node;
                    }
                    d_id=parse_long(NULL);
                }
                if(ilegal_arg){
                    break;
                }
                
                caminho_critico=0;                
                node=nova_tarefa(project,id,duracao,descricao,dependente_de);
                if (project==NULL){
                    project=node;
                }
                break;
        
            case 2: /*duration*/
                duracao=parse_long(NULL);                
                if(duracao==-1){ /*string vazia, duracao e 0*/               
                    duracao=0;
                }
                else if (duracao<=0){
                    printf("illegal arguments\n");
                    break;
                }
                else {
                    token=parse_string(NULL," ");
                    if (token){
                        printf("illegal arguments\n");
                        break;
                    }
                }
                print_duration(project,duracao);
                break;
            
        
            case 3: /*depend*/
                id=parse_long(NULL);
                if(id<=0){
                    printf("illegal arguments\n");
                    break;
                }
                token=parse_string(NULL," ");
                if (token){
                    printf("illegal arguments\n");
                    break;
                }       
            
                find=find_id(project,id);
                if(find==NULL){
                    printf("no such task\n");
                    break;
                }
                
                int necessario=find->data->necessario;
                if(!necessario){
                    printf("%ld: no dependencies\n",id);
                    break;
                }
                
                printf("%ld:",id);
                while(necessario){ /*checks every task after 'find' to see if they have 'id' in their list of dependencies until it finds all dependencies  */
                    find=find->next;
                    node=find_id(find->data->dependente_de,id);
                    if(node!=NULL){
                        printf(" %ld",find->data->id); 
                        necessario--;
                    }
                }
                printf("\n");
                break;
        
            case 4: /*remove*/
                id=parse_long(NULL);
                if(id<=0){
                    printf("illegal arguments\n");
                    break;
                }
                token=parse_string(NULL," ");
                if (token){
                    printf("illegal arguments\n");
                    break;
                }

                find=find_id(project,id);
                if (find==NULL){     
                    printf("no such task\n");
                    break;
                }
                
                if (find->data->necessario){
                    printf("task with dependencies\n");
                    break;
                }
                project=remove_element(project,find);
                caminho_critico=0;
                break;
    
            case 5: /*path */
                token=parse_string(NULL," ");
                if (token){
                    printf("illegal arguments\n");
                    break;
                }
                if(!caminho_critico){
                    calculate_path(project);
                }
                
                node=project;
                do{ /*print each 'critical' task*/
                    if (node->data->early==node->data->late){
                        print_task(node->data);
                    }
                    node=node->next;
                }while (node!=project);
                printf("project duration = %ld\n",duracao_projeto);
                break;
       
            case 6: /*exit*/
                token=parse_string(NULL," ");
                if (token){
                    printf("illegal arguments\n");
                    break;
                }
                
                free_list(project);
                free(input);                
                return 0;
                
            case 7: /*first*/
                if (project==NULL){
                    break;
                }
                bigger_len=0;
                node=project;
                do{
                    tam_descricao=strlen(node->data->descricao);
                    if (tam_descricao>bigger_len){
                        bigger_len=tam_descricao;
                        id=node->data->id;
                    }
                    node=node->next;
                }while (node!=project);
                printf("%ld\n",id);
                break;
                
            case 8: /*second*/
                if (project==NULL){
                    break;
                }
                can_remove=0;
                node=project;
                do{
                    if(node->data->necessario!=0){
                        can_remove++;
                    }
                    node=node->next;
                }while (node!=project);
                printf("%d\n",can_remove);
                break;
                
            default:
                printf("illegal arguments\n");
                break;
        }
        free(input);        
    }while(1);           
    return -1;
}
