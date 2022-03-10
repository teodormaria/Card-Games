#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cards.h"

#define LINEMAX 40

int main()
{
    FILE* in = fopen("in","r");
    FILE* out = fopen("out", "w+");
    char* line= calloc(LINEMAX, sizeof(char));
    int tasknum;
    fscanf(in,"%s",line);

    if(strcmp(line,"amestecare") == 0){
        tasknum = 0;
    } else if(strcmp(line,"razboi") == 0){
        tasknum = 1;
    } else if(strcmp(line,"rollingstone") == 0){
        tasknum = 2;
    } else if(strcmp(line,"scarabei") == 0){
        tasknum = 3;
    }

    int numInstructions, i;
    TList s = createstack();
    fscanf(in,"%d",&numInstructions);
    for(i = 0; i < numInstructions; i++){

        fscanf(in,"%s",line);

        if(strcmp(line,"inversare") == 0){
            s = invert(s); 
        } else if(strcmp(line, "intercalare") == 0){
            s = intercalare(s);
        }else if(strcmp(line, "mongean") == 0){
            s = mongean(s);
        }  else if(strcmp(line,"npiles") == 0){
            int N, j = 0;
            fscanf(in,"%d",&N);
            int* order = (int*) malloc(N*sizeof(int));
            for(j = 0; j< N;j++){
                fscanf(in,"%d",&(order[j]));
            }
            s = NPiles(s,N,order);
            free(order);
        }
    }

    if(tasknum == 0){
        printStack(s,out);
        freeTList(s);
    } 
    if(tasknum == 1){
        GameStacks p = splitcardsrazboi(s);
        int contor = 0;
        razboiRound(p,out,&contor);
    }
    if(tasknum == 2){
        rollingstone(s,out);
    }
    if(tasknum == 3){
        scarabei(s,out);
    }
    
    free(line);
    fclose(out);
    fclose(in);
    return 0;
}

