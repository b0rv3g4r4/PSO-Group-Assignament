#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main (int argc, char **argv) {

    srand(time(NULL));

    int np,ng,na;
    printf("Introduce el numero de personas: ");
    int err = fscanf(stdin, "%d", &np);
    printf("Introduce el numero de grupos: ");
    err = fscanf(stdin, "%d", &ng);
    printf("Introduce el numero de asignaturas: ");
    err = fscanf(stdin, "%d", &na);
    
    // Abrimos el fichero
    FILE *fichero = fopen("caso.txt","w");
    // Escribimos los datos dentro
        fprintf(fichero, "%d %d %d\n", np, ng, na);
        for (int i = 0; i < np; i++) {
            for (int j = 0; j < na-1; j++) {
                fprintf(fichero, "%d ", rand() % 2);
            }
            fprintf(fichero, "%d\n", rand() % 2);
        }
        // Cerramos el fichero
    fclose(fichero);
}
