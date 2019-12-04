#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ELEM 10000
#define MAX_DIM 100000
#define MAX_FILENAME 80

typedef struct{
    long linha;
    long coluna;
    double valor;
}Valor;

/*Prototipos das funcoes*/
void adiciona(Valor v);
void print_matrix();
void print_caracteristicas();
void print_linha(long line);
void print_coluna(long col);
void mergesort(Valor lista[],int min,int max,int i);
void merge(Valor lista[],int min,int middle,int max, int i);
void delete_zeros(); 
void readMatrixfromfile(FILE *ficheiro);
void write_in_file(FILE *ficheiro);
void print_comprimido();    

/*Variaveis Globais*/
Valor Matriz[MAX_ELEM];
double zero=0;
int n_elementos=0;

int main(int argc, char** argv){
    
    FILE *ficheiro;
    char filename[MAX_FILENAME+1];
    filename[0]='\0';

    if(argc > 2){
        fprintf(stderr, "\nNumero invalido de argumentos.\n");
        return 1;
    }    

    else if(argc==2){
        strcpy(filename,argv[1]);
        ficheiro=fopen(filename,"r");
        if (ficheiro!=NULL){
            readMatrixfromfile(ficheiro);
            fclose(ficheiro);
        }
        else{
            fprintf(stderr,"\nErro ao abrir ficheiro\n");
            return 1;
        }
    }
    
    char c;
    char input[MAX_FILENAME+3]; /*command,blank space,filename,\0*/
    
    fgets(input,MAX_FILENAME+2,stdin);    
    c=input[0];
    long linha,col;
    char string[7]; /*reads 'column' in command 'o'*/
    
    char filename_aux[MAX_FILENAME+1];
    Valor value;
    double soma;
    double maior;
    Valor maior_colunas[MAX_DIM];
    int i,j;
    
    while (c!='q'){
        switch(c){
            case 'a': 
                if(sscanf(input,"%c %ld %ld %lf",&c,&value.linha,&value.coluna,&value.valor)==4){
                    adiciona(value);
                }
                else{
                    fprintf(stderr,"\nInvalid Command\n");
                }
                break;
                
            case 'p':
                print_matrix();
                break;
                
            case 'i':
                print_caracteristicas();
                break;
                
            case 'l':
                if(sscanf(input,"%c %ld",&c,&linha)==2){
                    print_linha(linha);
                }
                else{
                    fprintf(stderr,"\nInvalid Command\n");
                }
                break;
                
            case 'c':
                if(sscanf(input,"%c %ld",&c,&col)==2){
                    print_coluna(col);
                }
                else{
                    fprintf(stderr,"\nInvalid Command\n");
                }
                break;
                
            case 'o':
                if(sscanf(input,"%c %s",&c,string)==2){
                    if(strcmp("column",string)==0){
                        mergesort(Matriz,0,n_elementos-1,1);
                    }
                    else{
                        fprintf(stderr,"\nInvalid Command\n");
                    }
                }
                else{
                    mergesort(Matriz,0,n_elementos-1,0);
                }
                break;
                
            case 'z':
                if(sscanf(input,"%c %lf",&c,&zero)==2){                
                    delete_zeros();
                }
                else{
                    fprintf(stderr,"\nInvalid Command\n");
                }                    
                break;
                
            case 's':
                print_comprimido();
                break;
                
            case 'w':
                if (sscanf(input,"%c %s",&c,filename_aux)==2){
                    strcpy(filename,filename_aux);
                }
                else if(filename[0]=='\0'){
                    fprintf(stderr,"\nNo Filename\n");
                    break;
                }                    
                ficheiro=fopen(filename,"w");
                if (ficheiro!=NULL){
                    write_in_file(ficheiro);
                    fclose(ficheiro);
                }
                else{
                    fprintf(stderr,"\nErro ao abrir ficheiro\n");
                }
                break;
            case 'x':
                soma=0;
                for(i=0;i<n_elementos;i++){
                    if(Matriz[i].coluna==Matriz[i].linha){
                        soma+=Matriz[i].valor;
                    }
                }
                printf("%.3f\n",soma);
                break;
            case 'y':
                maior=Matriz[0].valor;
                j=0;
                for (i=0;i<n_elementos;i++){
                    if (Matriz[i].valor>maior){
                        maior=Matriz[i].valor;
                    }
                }
                for(i=0;i<n_elementos;i++){
                    if(Matriz[i].valor==maior){
                        maior_colunas[j]=Matriz[i];
                        j++;
                    }
                }
                mergesort(maior_colunas,0,j-1,1);
                if (j==0){
                    break;
                }
                printf("%ld\n",maior_colunas[0].coluna);
                for(i=1;i<j;i++){
                    if (maior_colunas[i].coluna!=maior_colunas[i-1].coluna){
                        printf("%ld\n",maior_colunas[i].coluna);
                    }
                }
                break;              
        }
        fgets(input,MAX_FILENAME+2,stdin);
        c=input[0];
    }
    return 0;
}            

void adiciona(Valor v){
    int i;
    if (v.valor==zero){
        int del=0;
        /* se valor e zero, elimina valor, e faz 'shift left' dos outros membros*/
        for(i=0;i<n_elementos;i++){
            if (v.linha==Matriz[i].linha){
                if (v.coluna==Matriz[i].coluna){
                    Valor temp;
                    temp=Matriz[i];
                    Matriz[i]=Matriz[i+1];
                    Matriz[i+1]=temp;
                    del=1;
                }
            }
        }
        if (del==1){
            n_elementos--;
        }
    }
    else{
        /*se valor nao e zero, ve se o ponto ja esta na matriz. se estiver(changed=1),atualiza. se nao estiver,adiciona */
        int changed=0;
        for(i=0;i<n_elementos;i++){
            if (v.linha==Matriz[i].linha){
                if (v.coluna==Matriz[i].coluna){
                    Matriz[i].valor=v.valor;
                    changed=1;
                    i=n_elementos;
                }
            }
        }
        if (changed==0){
            if(n_elementos>MAX_ELEM){
                fprintf(stderr,"\nNumero maximo de elementos atingido\n");
            }
            else{
                Matriz[n_elementos]=v;
                n_elementos++;
            }            
        }
    }
}
    
void print_matrix(){
    if (n_elementos==0){
        printf("empty matrix\n");
    }
    else{
        int i;
        for (i=0;i<n_elementos;i++){
            printf("[%ld;%ld]=%.3lf\n",Matriz[i].linha,Matriz[i].coluna,Matriz[i].valor);
        }
    }
}
                    
void print_caracteristicas(){
    if (n_elementos==0){
        printf("empty matrix\n");
    }
    else{
        long min_l=MAX_DIM+1, max_l=0; /*menor e maior linhas da matriz */
        long min_c=MAX_DIM+1, max_c=0; /*menor e maior colunas da matriz */
        float size;
        int i;
    
        for(i=0;i<n_elementos;i++){
            if (Matriz[i].linha>max_l) {max_l=Matriz[i].linha;}
            if (Matriz[i].linha<min_l) {min_l=Matriz[i].linha;}
            if (Matriz[i].coluna>max_c) {max_c=Matriz[i].coluna;}
            if (Matriz[i].coluna<min_c) {min_c=Matriz[i].coluna;}
        }
        size=(max_l-min_l+1)*(max_c-min_c+1);
        float densidade=n_elementos/size*100;
        printf("[%ld %ld] [%ld %ld] %d / %.0lf = %.3f%%\n",min_l,min_c,max_l,max_c,n_elementos,size,densidade);
    }
}

void print_linha(long line){
    int i;
    long min_c=MAX_DIM+1,max_c=-1; /*menor e maior colunas*/
    float linha[MAX_DIM]; /*guarda os valores da linha em questao */
    int empty=1;
    
    for (i=0;i<MAX_DIM;i++){
        linha[i]=zero;
    }
    
    /*copia valores da matriz para linha[]*/
    for(i=0;i<n_elementos;i++){
        if (Matriz[i].linha==line){
            linha[Matriz[i].coluna]=Matriz[i].valor;
            empty=0;
        }
        if (Matriz[i].coluna>max_c) {max_c=Matriz[i].coluna;}
        if (Matriz[i].coluna<min_c) {min_c=Matriz[i].coluna;}
        
    }

    if (empty){
        printf("empty line\n");
    }
    else{
        for(i=min_c;i<=max_c;i++){
            printf(" %.3f",linha[i]);
        }
        printf("\n");
    }
}

void print_coluna(long col){
    int i;
    long min_l=MAX_DIM+1,max_l=-1; /*menor e maior linhas*/
    float coluna[MAX_DIM]; /*guarda valores da coluna em questao */
    int empty=1;
    
    for (i=0;i<MAX_DIM;i++){
        coluna[i]=zero;
    }
    
    /*copia os valores para coluna[]*/
    for(i=0;i<n_elementos;i++){
        if (Matriz[i].coluna==col){
            coluna[Matriz[i].linha]=Matriz[i].valor;
            empty=0;
        }       
        if (Matriz[i].linha>max_l) {max_l=Matriz[i].linha;}
        if (Matriz[i].linha<min_l) {min_l=Matriz[i].linha;}
    }
    if (empty){
        printf("empty column\n");
    }
    else{
        for(i=min_l;i<=max_l;i++){
            printf("[%d;%ld]=%.3f\n",i,col,coluna[i]);
        }
    }
}

void delete_zeros(){
    int i;
    int num_espacos=0; /*numero de elementos encontrados com o novo valor zero*/
 
    for (i=0;i<n_elementos;i++){
        if(Matriz[i].valor==zero){
            num_espacos++;
        }
        /*se tiver espacoes, faz 'shift left'*/
        else if(num_espacos>0){
            Matriz[i-num_espacos]=Matriz[i];
            Matriz[i].valor=zero;
        }
    }
    n_elementos=n_elementos-num_espacos;
}            
    
void print_comprimido(){
    long min_l=MAX_DIM+1, max_l=0; /* menor e maior linha da matriz, respetivamente*/
    long min_all_c=MAX_DIM+1, max_all_c=0; /*menor e maior coluna da matriz, respetivamente */
    int i,k,j;
    long size;
    long ind_menor=-1, ind_maior=-1; /*indice da linha com menor densidade (e maior coluna), e maior densidade (e menor coluna), respetivamente*/
    int dens_linhas[MAX_DIM]; /*numero de elementos em cada linha*/
    int maior=0,menor=n_elementos; /* maior e menor numero de elementos numa linha*/
    
    if (n_elementos==0){printf("empty matrix\n");}
         
    else{
        for(i=0;i<MAX_DIM;i++){
            dens_linhas[i]=0;
        }
        /*descobre numero de elementos em cada linha, poe em dens_linhas */
        for(i=0;i<n_elementos;i++){
            if (Matriz[i].linha>max_l) {max_l=Matriz[i].linha;}
            if (Matriz[i].linha<min_l) {min_l=Matriz[i].linha;}
            if (Matriz[i].coluna>max_all_c) {max_all_c=Matriz[i].coluna;}
            if (Matriz[i].coluna<min_all_c) {min_all_c=Matriz[i].coluna;}
            dens_linhas[Matriz[i].linha]++;
        }
        
        /*descobre primeiro e ultimo indices, para auxiliar a escolha das linhas a comprimir */
        for (i=min_l;i<=max_l;i++){
            
            if (dens_linhas[i]<menor){
                menor=dens_linhas[i];
                ind_menor=i;
            }
            else if (dens_linhas[i]==menor){
                if (i>ind_menor){
                    ind_menor=i;
                }
            }                
            if (dens_linhas[i]>maior){
                maior=dens_linhas[i];
                ind_maior=i;                    
            }
            else if (dens_linhas[i]==maior){
                if (i<ind_maior){
                    ind_maior=i;
                }
            }
        }
        
        int n_linhas=max_l-min_l+1;
        size=n_linhas*(max_all_c-min_all_c+1);
        
        if (size<n_elementos*2){printf("dense matrix\n");}
        else{
            long ultima_linha;
            double values[MAX_DIM*2];
            long index[MAX_DIM*2];
            int offset[MAX_DIM];
            float linha[MAX_DIM];
            int maior_off=0;

            /*inicializa listas a zero */
            for(i=0;i<MAX_DIM*2;i++){
                values[i]=zero;
                index[i]=0;
            }
            
            /*comprime cada linha*/
            for(i=0;i<n_linhas;i++){
                for(k=min_all_c;k<=max_all_c;k++){
                    linha[k]=zero;
                }
                long min_c=MAX_DIM+1;
                long max_c=0;
                /*encontra a linha (codigo print linha)*/
                for(k=0;k<n_elementos;k++){
                    if (Matriz[k].linha==ind_maior){
                        linha[Matriz[k].coluna]=Matriz[k].valor;
                        if (Matriz[k].coluna>max_c) {max_c=Matriz[k].coluna;}
                        if (Matriz[k].coluna<min_c) {min_c=Matriz[k].coluna;}
                    }
                }
                
                int off=0;
                /*verifica se consegue inserir todos os valores na lista de valores,pra cada offset (encontra offset)*/

                for(j=min_c;j<=max_c;j++){
                    if(linha[j]!=zero && values[j-min_all_c+off]!=zero){
                        j=min_c-1;
                        off++;
                    }
                }
                
                /*atualiza listas*/
                for(j=min_all_c;j<=max_c;j++){
                    if(linha[j]!=zero){
                        values[j-min_all_c+off]=linha[j];
                        index[j-min_all_c+off]=ind_maior;
                    }
                }
                
                offset[ind_maior-min_l]=off;

                /*encontra maior offset*/
                if(maior_off<off){
                    maior_off=off;
                    
                }
                
                /*descobre proxima linha*/
                ultima_linha=ind_maior;
                ind_maior=ind_menor;                
                for(j=min_l;j<=max_l;j++){
                    if (dens_linhas[j]>dens_linhas[ind_maior]||(dens_linhas[j]==dens_linhas[ind_maior]&&j<ind_maior)){ /*j e melhor escolha que ind_maior*/
                        
                        if (dens_linhas[j]<dens_linhas[ultima_linha]||(dens_linhas[j]==dens_linhas[ultima_linha]&&j>ultima_linha)){ /*ainda nao escolheu j*/
                            ind_maior=j;
                        }
                    }
                }
            }
            
            int tam_necessario=max_all_c-min_all_c+maior_off; /*padding*/  
            printf("value =");
            for(i=0;i<=tam_necessario;i++){
                printf(" %.3lf",values[i]);
            }
            printf("\nindex =");
            for(i=0;i<=tam_necessario;i++){
                printf(" %ld",index[i]);
            }
            printf("\noffset =");
            for(i=0;i<n_linhas;i++){
                printf(" %d",offset[i]);
            }
            printf("\n");
        }
    }
}

void write_in_file(FILE *ficheiro){
    int i;
    for(i=0;i<n_elementos;i++){
        fprintf(ficheiro,"(%ld,%ld,%lf)\n",Matriz[i].linha,Matriz[i].coluna,Matriz[i].valor);
    }
}

void readMatrixfromfile(FILE*ficheiro){
    n_elementos=0;
    long linha,coluna;
    double v;
    char string[100];
    while(fgets(string,99,ficheiro)){
        if(sscanf(string,"(%ld,%ld,%lf)",&linha,&coluna,&v)==3){;
            Matriz[n_elementos].linha=linha;
            Matriz[n_elementos].coluna=coluna;
            Matriz[n_elementos].valor=v;
            n_elementos++;
        }
        else{
            fprintf(stderr,"\n Error reading from file\n");
            exit(0);
        }
    }
}
    
void mergesort(Valor list[],int min,int max, int i){
    int middle=(max+min)/2;
    if (max<=min){return;}
    mergesort(list,min,middle,i);
    mergesort(list,middle+1,max,i);
    merge(list,min,middle,max,i);    
}

void merge(Valor list[],int min,int middle,int max,int i){
    int j,r=0,l=0; /*indices para lista 'principal', para a lista com elementos da direita e esquerda, respetivamente*/  
    long valor_left,valor_right; /*valores a comparar (depende de comando 'o')*/
    
    /*cria listas auxilĩares*/
    Valor left[middle-min+1];
    Valor right[max-middle];
    for(j=0;j<(middle-min+1);j++){
        left[j]=list[j+min];
    }    
    for(j=0;j<(max-middle);j++){
        right[j]=list[j+middle+1];
    }
    /*ordena*/
    for(j=min;j<=max;j++){ 
        if (l<middle-min+1&&r<max-middle){
            /*dependendo da especificao do comando 'o', pode ser ordenado por linha ou coluna*/
            if (i==0){
                valor_left=left[l].linha;
                valor_right=right[r].linha;
            }   
            else{
                valor_left=left[l].coluna;
                valor_right=right[r].coluna;
            }
        
            /*compara*/
            if (valor_left<valor_right){
                list[j]=left[l];
                l++;
            }
            else if(valor_left>valor_right){
                list[j]=right[r];
                r++;
            }
        
            /*se linhas ou colunas iguais, compara colunas ou linhas respetivamente*/
            else{            
                if (i==1){
                    valor_left=left[l].linha;
                    valor_right=right[r].linha;
                }
                else{
                    valor_left=left[l].coluna;
                    valor_right=right[r].coluna;
                }
                
                /*compara denovo*/
                if (valor_left<valor_right){
                    list[j]=left[l];
                    l++;
                }
                else{
                    list[j]=right[r];
                    r++;
                } 
            }
        }
        else if(l==middle-min+1){ /*lista da esquerda ja nao tem membros */
            list[j]=right[r];
            r++;
        }
        else{ /*lista da direita ja nao tem membros*/
            list[j]=left[l];
            l++;
        }          
    }
}
        
        
        
        
                    
                    
                        
                        
                        
                        
                    
 
                    
                    
                    
    
