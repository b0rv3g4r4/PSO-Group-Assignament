#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "randvar.h"



#define UNFIT 999999
#define VELOCITY_MIN -1
#define VELOCITY_MAX 1



#define NUM_PARTICLES 10
#define MAX_ITERATIONS 10
#define DIMENSIONS 9

#define DOMAIN_MIN 1
#define DOMAIN_MAX 4

//Parametros del problema:
    int ng = 3;
    int na = 4;
    int np = 9;
    int a[] = {1,1,1,0,
               1,0,1,1,
               1,0,1,0,
               0,1,1,1,
               1,0,1,1,
               1,1,0,1,
               0,1,1,0,
               1,1,0,1,
               1,1,1,1};

//---------




int max_iterations = MAX_ITERATIONS;
int num_particles = NUM_PARTICLES;
double inertial_weight;
double gbest_fitness;
int gbest_position[DIMENSIONS];




typedef struct {
     int position[DIMENSIONS]; 
     int velocity[DIMENSIONS]; 
     int best_position[DIMENSIONS];
     double fitness;
     double best_fitness;
} particle;

typedef struct {
     double min;
     double max;
} bound;

particle swarm[NUM_PARTICLES];
bound domain[DIMENSIONS];
bound velocity[DIMENSIONS];


/* particle pouplation, swarm size, domain min, domain max */
void define_domain(void);
void initialize_particle(particle *);
void initialize_swarm(particle[], int);
void update_velocity(particle *);
void update_position(particle *);
void evaluate(particle *,int * asignaturas);   //send function pointer 4 eval function

void print_position(int *);
void print_status(void);
double fitness(int vec[na]);
void printArray(int arr[],int size) 
{ 
    int i =0; 
 
    printf("\nElements are : "); 
 
    for(i=0; i<size; i++) 
    { 
        printf("\n\tarr[%d] : %d",i,arr[i]); 
    } 
    printf("\n"); 
}

double fitness(int vec[na]){
    double suma = 0;
  for (int i=0; i<na; i++)
    suma += vec[i];
  return (suma/na);
}


void define_domain(void){
    int i;
    for (i = 0; i < DIMENSIONS; i++) {
        domain[i].min = DOMAIN_MIN;
        domain[i].max = DOMAIN_MAX;
        velocity[i].min = VELOCITY_MIN;
        velocity[i].max = VELOCITY_MAX;
    }
}


void initialize_swarm(particle swarm[], int num_particles){   
     int i;
     particle *p;
     for (i = 0; i < num_particles; i++) {
          p = &swarm[i];
          initialize_particle(p);
     }
}

void initialize_particle(particle *p){
     int i;
     for (i = 0; i < DIMENSIONS; i++) {   
          p->position[i] = uniform(domain[i].min, domain[i].max);
          p->velocity[i] = uniform(velocity[i].min, velocity[i].max);
     }
     memcpy(p->best_position, p->position, sizeof(p->position));
     p->fitness = UNFIT;
     p->best_fitness = p->fitness; 
}

int main()
{
     srand(time(NULL));

     int i, j;
     particle *p;
     double weight_decay = 0.6 / max_iterations;

     /* here, an intertial weight term is added to teh velocity, and decays as the iterations progress.  It hits a minimum value
        of 0.4, thus going from 1 to 0.4 over the course of the simulation
     */

     inertial_weight = 1;

     define_domain();
     initialize_swarm(swarm, num_particles);
        gbest_fitness = 10.0;
     /* Main Loop */
     for ( i = 0; i < max_iterations; i++) {
          for (j = 0; j < num_particles; j++) {
               p = &swarm[j];
               update_velocity(p);
               update_position(p);
               evaluate(p,a);
          }         
          inertial_weight -= weight_decay;
     }

     print_status();
     printf("Done..\n");

     return 0;
}



void update_velocity(particle  *p) {
     /* PSO specific velocity update equation */
     double phi1 = uniform(0,2);
     double phi2 = uniform(0,2);
    
     int i;
     /* need to be careful with phi vars...  I randomize once. I should randomize more (put rand in loop)
        also, should phi 1 and phi2 be equal?  Check pso thesis  for kalman pso
     */
     for (i = 0; i < DIMENSIONS; i++) {
          p->velocity[i] = inertial_weight*p->velocity[i] + phi1*(p->best_position[i]-p->position[i]) + phi2*(gbest_position[i]-p->position[i]);         
     }
}

void update_position(particle  *p) {
     /* Updates particle position vector:  x(i+1) = x(i) + v(i+1)   */
     int i;
     for (i = 0; i < DIMENSIONS; i++) {
          p->position[i] = p->position[i] + p->velocity[i];
          if ((p->position[i] < domain[i].min) || (p->position[i] > domain[i].max)){
            initialize_particle(p);
          }
               
     }
}



/*
    FUNCIONES AUXILIARES DE CALCULO
*/
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


void evaluate(particle  *p,int * asignaturas) {

           int subgrupos[ng][na];
           memset(subgrupos, 0, sizeof(subgrupos[0][0]) * ng * na);
           int alumno = 0;
            for(int j = 0;j<np*na;j+=na){
                for(int z = 0;z<na;z++){
                    if(asignaturas[j+z] == 1){
                        //printf("Vamos a sumar 1 al grupo: %d en la asignatura: %d \n",asignacion[alumno]-1,z);
                        subgrupos[p->position[alumno]-1][z] += 1; 
                    }
                }
                alumno++;
            }
        int * diff = calculateDiff(subgrupos);  //Obtenemos la array de diferencia
        double myfitness = fitness(diff);
        p->fitness = myfitness;

        //Checkeamos el caso de todo 0
        int suma = 0;
        for(int i= 0;i<DIMENSIONS;i++){
            suma+=p->position[i];
        }

   

     if (p->fitness < p->best_fitness) {
          p->best_fitness = p->fitness;
          memcpy(p->best_position, p->position, sizeof(p->position));
     }

     if (p->fitness < gbest_fitness) {
          gbest_fitness = p->fitness;
          memcpy(gbest_position, p->position, sizeof(p->position));
     }
    


}

void print_position(int *p){
    int j;
    for (j = 0; j < DIMENSIONS; j++) {
        printf("Dim %i: %d ", j+1, p[j]);
    }
    printf("\n");
 
}


void print_status(){
    int i;
     particle *p;
     for (i = 0; i < num_particles; i++) {
          p = &swarm[i];
          printf("Particle %i has fitness %4.4f\n", i+1, p->fitness); 
          print_position(p->position);
     }
     printf("Global best fitness is %4.12f at position \n", gbest_fitness);
 print_position(gbest_position);
}
