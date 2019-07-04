#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "randvar.h"
#include <iostream>

using namespace std;



//Parametros estaticos

#define UNFIT 9999999
#define VELOCITY_MIN -1
#define VELOCITY_MAX 1



typedef struct {
     int dimensions;
     int * position; 
     int * velocity; 
     int * best_position;
     double fitness;
     double best_fitness;
} particle;

typedef struct {
     double min;
     double max;
} bound;



void evaluate(particle  *p,int * asignaturas,int dimensions,double * gbest_fitness,int * gbest_position,int ng,int na,int np); 
double secuencial(int np, int ng, int na,int * asignaturas,int niteraciones,int nparticulas);

double fitness(int vec[],int na){
    double suma = 0;
  for (int i=0; i<na; i++)
    suma += vec[i];
  return (suma/na);
}


int main(int argc, char *argv[]){
     if (argc < 3) {
         cout << "Modo de uso:" << endl;
        cout << "./particulas_openmp <niteraciones> <nparticulas> < <fichero_caso>" <<endl;
        return -1;
     }
     int niteraciones = atoi(argv[1]);
     int nparticulas = atoi(argv[2]);

     //Leemos la matriz de datos:
     srand(time(NULL));
     struct timeval ti, tf;
     double tiempo;
     gettimeofday(&ti, NULL); 

     int ng = 0,np = 0,na = 0;
     cin >> np;
     cin >> ng;
     cin >> na;

     //cout <<  "NP: "<< np << "NG: "<< ng << "NA:" <<  na << endl;     
     int * datos = (int * )malloc(sizeof(int)*np*na);
     memset(datos, 0, sizeof(datos));
     for(int i = 0 ; i< np*na;i++){
          cin >> datos[i];
     }

     double fit = secuencial(np,ng,na,datos,niteraciones,nparticulas);
     printf("Fitness = %f\n",fit);

     gettimeofday(&tf, NULL);
     tiempo= (tf.tv_sec - ti.tv_sec)*1000000 + (tf.tv_usec - ti.tv_usec);
     cout << "En "<<tiempo/1000000 << "segundos" <<endl;
     free(datos);
     return 0;
}


double secuencial(int np, int ng, int na,int * asignaturas,int niteraciones,int nparticulas){
     //cout << "CheckPoint ENTRAMOS SECUENCIAL" << endl;
     double weight_decay = 0.6 / niteraciones;
     struct timeval ti, tf;
     double tiempo;
     double inertial_weight = 1;
     double gbest_fitness;
     int gbest_position[np];
     int domain_min = 1;
     int domain_max = na;
     //cout << "CheckPoint 1.2 : nParticulas =" << nparticulas<< endl;
     //particle enjambre[nparticulas];
     particle * enjambre = new particle[nparticulas];
     //cout << "CheckPoint 1.3" << endl;
     bound dominio[np];
     bound velocidad[np];
     //Inicializamos el dominio
     //cout << "CheckPoint GO DOMAIN" << endl;
     for (int d = 0; d < np; d++) {
          dominio[d].min = domain_min;
          dominio[d].max = domain_max;
          velocidad[d].min = VELOCITY_MIN;
          velocidad[d].max = VELOCITY_MAX;
     }
     //cout << "CheckPoint DOMINIO OK" << endl;
     // Inicializamos el enjambre
     particle *p;
     for (int i = 0; i < nparticulas; i++) {
          p = &enjambre[i];
               //Inicializamos cada particula
               p->position = (int *)malloc(sizeof(int)*np);
               p->velocity = (int *)malloc(sizeof(int)*np);
               p->best_position = (int *)malloc(sizeof(int)*np);
               for (int j = 0; j < np; j++) {   
                    p->position[j] = uniform(dominio[j].min, dominio[j].max);
                    p->velocity[j] = uniform(velocidad[j].min, velocidad[j].max);
               }
               memcpy(p->best_position, p->position, sizeof(p->position));
               p->fitness = UNFIT;
               p->best_fitness = p->fitness; 
     }
     //cout << "CheckPoint Enjambre OK" << endl;
     gbest_fitness = 10.0;
     double phi1,phi2;
     for (int i = 0; i < niteraciones; i++) {
          for (int j = 0; j < nparticulas; j++) {
               //Cogemos la particula
               p = &enjambre[j];
               //Actualizamos su velocidad
                phi1= uniform(0,2);
                phi2 = uniform(0,2);
                for (int z = 0; z < np; z++) {
                    p->velocity[z] = inertial_weight*p->velocity[z] + phi1*(p->best_position[z]-p->position[z]) + phi2*(gbest_position[z]-p->position[z]);         
                }
                for (int x = 0; x < np; x++) {
                    p->position[x] = p->position[x] + p->velocity[x];
                    if ((p->position[x] < dominio[x].min) || (p->position[x] > dominio[x].max)){
                            for (int m = 0; m < np; m++) {   
                                   p->position[m] = uniform(dominio[m].min, dominio[m].max);
                                   p->velocity[m] = uniform(velocidad[m].min, velocidad[m].max);
                              }
                              memcpy(p->best_position, p->position, sizeof(p->position));
                              p->fitness = UNFIT;
                              p->best_fitness = p->fitness; 
                    }
                }
               //cout << "CheckPoint Llegamos a evaluate" << endl;
               //Evaluat
          
               evaluate(p,asignaturas,np,&gbest_fitness,gbest_position,ng,na,np);
                //cout << "CheckPoint Salimos de evaluate" << endl;
              
          }         
          inertial_weight -= weight_decay;
     }
     delete [] enjambre;
     return gbest_fitness;
}



void evaluate(particle  *p,int * asignaturas,int dimensions,double * gbest_fitness,int * gbest_position,int ng,int na,int np) {

           //int * subgrupos = new int[ng][na];
           int **subgrupos = new int*[ng];
           for(int i = 0; i < ng; ++i) {
                    subgrupos[i] = new int[na];
            }
           //cout << "CheckPoint Iniciamos evaluate" << endl;
            //cout <<  "NP: "<< np << "NG: "<< ng << "NA:" <<  na << endl;     

           int alumno = 0;
            for(int j = 0;j<np*na;j+=na){
                for(int z = 0;z<na;z++){
                     //cout << "z= " << z << endl;
                    if(asignaturas[j+z] == 1){
                        //printf("Vamos a sumar 1 al grupo: %d en la asignatura: %d \n",asignacion[alumno]-1,z);
                         subgrupos[p->position[alumno]-1][z] += 1; 
                    }
                }
                alumno++;
            }


          //cout << "CheckPoint Vamos a calcular diff" << endl;
         int * diff = (int *)malloc(sizeof(int)*na);
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
        double myfitness = fitness(diff,na);
        p->fitness = myfitness;

        //Checkeamos el caso de todo 0
        int suma = 0;
        for(int i= 0;i<dimensions;i++){
            suma+=p->position[i];
        }

     if (p->fitness < p->best_fitness) {
          p->best_fitness = p->fitness;
          memcpy(p->best_position, p->position, sizeof(p->position));
     }

     if (p->fitness < *gbest_fitness) {
          *gbest_fitness = p->fitness;
          memcpy(gbest_position, p->position, sizeof(p->position));
     }
    
     for(int i = 0; i < ng; ++i) {
          delete [] subgrupos[i];
     }
     delete [] subgrupos;

}

