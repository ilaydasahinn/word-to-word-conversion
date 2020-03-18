#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 250        //Length of queue.


typedef struct {
    int front,rear;     //Index variables of queue.
    int queue[250];        
    int count;          //Length of queue.
}QUEUE;



int compareWords(char*,char*); //Returns letter difference between words.
int findIndex(char **,int,char*); //Returns index of the word which was given on the matrix.
void labControl(int**,char**,int); //Checks if there is a one letter difference between words.
void initializeQueue( QUEUE *); //Initializes first velues of Queue.
int isEmpty( QUEUE *); //Checks if queue is empty.
int isFull( QUEUE * );//Checks if queue is full.
void BFS(int*, int, int, int*, char**, int**, int); //Breadth First Search.
void enqueue(int , QUEUE * ); //Pushs index of value was given to queue.
int dequeue(QUEUE* ); //Returns first element of queue.

int main()
{
    char** words;    //String array which has words.
    int** adjecencyMatrix;        //Adjecency Matrix for graph.
    int route[50];           //Route from the first word to the second word.Yazdırırken birinci kelimeden ikinci kelimeye giden yol
    char kelime[6],kelime2[6]; //Temporary strings which are used to take input from user.
    int wordCount=0,i,j,menu=0,secim;

    /* Open File*/
    FILE *fi;
    fi = fopen("kelime.txt","r");
    if(fi == NULL){                     //There is no file.
        printf(" kelime.txt could not found.");
        exit(0);
    }
    /* End of the file operations. */


    /* Read matrix which is 'words' and malloc operation.*/
    words = (char**) malloc( sizeof(char*) );     
    if(words == NULL){
        printf("Dynamic memory could not be allocated.");
        exit(0);
    }

    while( fscanf(fi , "%s" , kelime ) != EOF ){        //Reading operation on the loop, and control to see if it reached to end of the file.
            words[wordCount] = (char*) malloc( 6 * sizeof(char) );
            strcpy(words[wordCount],kelime);               //Saves string which was read to matrix (word).
            wordCount++;
            words = (char**) realloc( words, (wordCount+1) * sizeof(char*) );  //Realloc operation to extends matrix.
            if(words == NULL){
                printf("Dynamic memory could not be allocated.");
                exit(0);
            }
    }
    fclose(fi);
    /* Words were read and put to matrix(words). */


    /** Adjecency matris */
    adjecencyMatrix = (int**) calloc(wordCount,sizeof(int*));       //Calloc for adjecency matrix and put 0s.
    for( i = 0 ; i < wordCount ; i++){
            adjecencyMatrix[i] = (int*) calloc(wordCount,sizeof(int*));
            if(adjecencyMatrix[i] == NULL){
                printf("Dynamic memory could not be allocated.");
                exit(0);
            }
    }

    for( i = 0 ; i < wordCount ; i++){
        for( j = i ; j < wordCount ; j++){
            if( i != j ){               //There is no need to compare for example:  [0][0] , [1][1] 
                int fark = compareWords( words[i] , words[j] );
                if( fark == 1 ){            //If the difference is 1 then matrix is marked with 1.
                    adjecencyMatrix[i][j] = 1 ;
                    adjecencyMatrix[j][i] = 1 ;      //Matrix is symetric. 
                }
            }
        }
    }

    while(menu == 0){
       // system("CLS");
        printf("Enter your choise.");
        printf("\n [1]   Check two words.");
        printf("\n [2]   Check Adjecency Matrix.");
        printf("\n [3]   Exit.\n\n");
        printf("[]:");
        scanf("%d",&secim);
        switch(secim){
        case 1:
            system("CLS");
            int* Visited = (int*) calloc (wordCount, sizeof(int));   //Calloc for the array which has nodes which were put to the matrix before.
                           
            printf("\n Enter the first word  : ");
            scanf("%s",kelime);
            int index1 = findIndex(words,wordCount,kelime);
            printf("\n Enter the second word  : ");
            scanf("%s",kelime2);
            int index2 = findIndex(words,wordCount,kelime2);
            BFS(Visited, index1, index2, route, words, adjecencyMatrix, wordCount);
            break;
        case 2:
            system("CLS");
            labControl(adjecencyMatrix,words,wordCount);
            getchar();
            break;
        case 3:
            menu = 1;           //Exit from the menu.
            break;
        }
    }

    /* Free operations */
    for(i = 0; i < wordCount; i++){
        free(words[i]);
        free(adjecencyMatrix[i]);
    }
    free(adjecencyMatrix);
    free(words);


    return 0;
}



/* Functions.. */


/* Returns letter difference between words. */
int compareWords(char* kelime1, char* kelime2){
    int i,farkliHarf = 0;
    for( i = 0 ; i < 5 ; i++ ){
        if( kelime1[i] != kelime2[i] )  //Compares the words letter by letter.
            farkliHarf++;               //When it finds different letters then counter increases.
    }
    return farkliHarf;              //Returns count of difference letters.
}

/* Returns index of the word which was given on the matrix.*/
int findIndex(char **words,int size,char* kelime){
    int i = 0;
    int konum = -1;

    while ( (i < size) && ( konum == -1) ){  //Continues until end of the words matrix.
        if( strcmp( words[i] , kelime ) == 0 )   //Compares the words
            konum = i; //When it find the word make location i.
        i++; //When it doesn't find i increases.
    }
    return konum;  //Returns location. It means when it finds it returns value of location but otherwise it returns -1.
}

/*Checks if there is a one letter difference between words.*/
void labControl(int** adjecencyMatrix,char** words,int size){
    int index1,index2;
    char kelime1[6],kelime2[6];         //Temporary strings to get inputs from user.

    printf("\nKelime1 giriniz     :");
    scanf("%s",kelime1);
    printf("\nKelime2 giriniz     :");
    scanf("%s",kelime2);

    index1 = findIndex(words,size,kelime1); //Finds indexes of strings which was taken from user on words matrix.
    index2 = findIndex(words,size,kelime2);      

    if( (index1 == -1) || (index2 == -1) ){
        printf("Select word from the file.");     //If returned -1 from findIndex then it means there is no word such that in the file.
        return;
    }
    else{
        if ( adjecencyMatrix[index1][index2] == 1 )          //Checks location of adjacency matrix.
            printf("\n Var.");
        else
            printf("\n Yok.");
        return;
     }
}

/* Functions about the queue. */

/* Initializes first velues of Queue. */
void initializeQueue(QUEUE *que){
    que->front = 0;
    que->rear = 0;
    que->count = 0;
}
/* Checks if queue is empty. */
int isEmpty( QUEUE *que){
    if( que->count == 0 )
        return 1;
    else
        return 0;
}

/* Checks if queue is full. */
int isFull( QUEUE *que ){
    if( que->count == SIZE )
        return 1;
    else
        return 0;
}

/* Breadth First Search Function */
void BFS(int* Visited, int index1, int index2, int* route, char** words, int** adjecencyMatrix, int wordCount) 
{
    int found = 0, i, j, indexTMP;
    QUEUE que;
    initializeQueue(&que);

    enqueue(index1, &que);
    Visited[index1] = -1;
    while((isEmpty(&que) == 0) && (found == 0)) 
    {
        indexTMP = dequeue(&que);
        if(indexTMP == index2)
            found = 1;
        else
        {
            for(j = 0; j< wordCount; j++)
            {
                if((adjecencyMatrix[indexTMP][j] == 1) && (Visited[j] == 0))
                {
                    enqueue(j, &que);
                    Visited[j] = indexTMP;
                }
            }
        }
    }
    if ( found == 1){                             // Starts to write..
                printf("\n\n\n");
                int count = 0;
                i = indexTMP;                               
                while (Visited[i] > 0){
                    route[count] = Visited[i] ;
                    count++;
                    i = Visited[i];                         //Node by node.
                }

                for(i=count-1 ; i>=0 ; i--)
                    printf(" %s ", words[route[i]]);          //Reverse for loop to print words straight.
                printf(" %s ",words[indexTMP]);


                printf("\n\n\n Count of step : %d\n",count);
            }
            else{                                               //Transportation is impossible in this case.
                printf("\n\n\n\nThere is no transition between words.");
            }
            free(Visited);                                  // Makes free.
            getchar();
}

/* Pushs index of value was given to queue. */
void enqueue(int value , QUEUE *que){
    if( isFull( que ) == 0 ){   //If queue is not full.
        que->queue[que->rear] = value;   
        que->rear = (que->rear + 1) % SIZE; //Increases rear 1 more.
        que->count++;       //Increases number of elements of queue 1 more.

    }
    else
        printf("\nThe queue is full.");
}
/* Returns first element of queue. */
int dequeue(QUEUE* que){
    int item = -1;

    if( isEmpty( que ) == 0 ){
        item = que->queue[ que->front ];
        que->front = ( que->front + 1) % SIZE;
        que->count--;
    }else
        printf("\nThe queue is empty.");
    return item;
}