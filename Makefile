all: particulas_secuencial particulas_openmp particulas_mpi particulas_openmpmpi generador

particulas_secuencial: secuencial.cpp randvar.c 
	g++ -w  -o particulas_secuencial secuencial.cpp randvar.c

particulas_openmp: openmp.cpp randvar.c 
	g++-9 -w  -o particulas_openmp openmp.cpp randvar.c -fopenmp

particulas_mpi: mpi.cpp randvar.c 
	mpic++ -w  -o particulas_mpi mpi.cpp randvar.c  

particulas_openmpmpi: openmp_mpi.cpp randvar.c 
	mpic++ -w -o particulas_openmpmpi openmp_mpi.cpp randvar.c  -fopenmp

generador: generador.c 
	gcc -w -o generador generador.c

clean: 
	rm -f particulas_secuencial particulas_openmp particulas_mpi particulas_openmpmpi generador


