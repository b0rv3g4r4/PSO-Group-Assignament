#include <math.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include "pso.h"


void initSettings(pso_settings_t *settings) {

  settings->x_lo = -100;
  settings->x_hi = 100;
  settings->goal = 1e-5;

}


double funcion_media(int *vec, int dim, void *params) {
  int suma = 0;
  for (int i=0; i<dim; i++)
    sum += vec[i];
  return (sum/dim);
}




int main(int argc, char **argv) {

  pso_obj_fun_t obj_fun = funcion_media;
  pso_settings_t settings;
  
  pso_set_default_settings(&settings);
  initSettings(&settings);

  settings.size = 30;
  settings.nhood_strategy = PSO_NHOOD_RING;
  settings.nhood_size = 10;
  settings.w_strategy = PSO_W_LIN_DEC;


  pso_result_t solution;
  solution.gbest = malloc(settings.dim * sizeof(double));
   
  pso_solve(obj_fun, NULL, &solution, &settings);

 
  free(solution.gbest);

  return 0;

}