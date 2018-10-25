#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int a[] = {1,1,1,0,1,0,1,1,1,0,1,0,0,1,1,1,1,0,1,1,1,1,0,1,0,1,1,0,1,1,0,1,1,1,1,1};
    int ng = 3;
    int na = 4;
    int np = 9;
    int asignacion[] = {1,2,3};

int * calculateDiff(int subgrupos[ng][na]){
    int * diff = malloc(sizeof(int)*na);
    for(int i=0;i<na;i++) diff[i] = 0;
    for(int i=0;i<na;i++){
        int mayorDiff = 0;
        int anterior = subgrupos[0][i];
        for(int j = 0;j<ng;j++){
          int diferencia = abs(subgrupos[j][i] - anterior);
          if(diferencia >= mayorDiff){
            mayorDiff = diferencia;  
          }
          anterior = subgrupos[j][i];
        }
        diff[i] = mayorDiff; 
    }

    return diff; 
}

int main(){


    int subgrupos[ng][na];
    for(int i=0;i<ng;i++){
        for(int j = 0;j<na;j++){
            subgrupos[i][j] = 0;
        }
    }

        
            //Traerme las asignaturas de ese alumno
            int alumno = 0;
            for(int j = 0;j<np*na;j+=na){
                for(int z = 0;z<na;z++){
                    if(a[j+z] == 1){
                        //printf("Vamos a sumar 1 al grupo: %d en la asignatura: %d \n",asignacion[alumno]-1,z);
                        subgrupos[asignacion[alumno]-1][z] += 1; 
                    }
                }
                alumno++;
            }

    int * mydiff = calculateDiff(subgrupos);
    free(mydiff);
    printArray(mydiff,na);
    
    
    
   

}