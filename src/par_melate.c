#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

#define MAX_ROWS 3959
#define MAX_COLS 9
#define RANK 56


// Calcula frecuencia
int count_ones(int* mascara, int n){
	int suma = 0;
	for ( int i = 0 ; i < n ; i++ ) {
		suma += mascara[i];
	}
	return suma;
}
// Visualizar vector
void plista(int* lista, int n)
{
	for ( int i = 0 ; i < n ; i++ ) {
		printf("%d,", lista[i]);
	}
	printf("\n");
}
// Read CSV
void leer_csv(char *filename, int *numeros, int n_args){
	FILE *file;
	char line[256];
	int nproducto, concurso, R[7];
	float bolsa;
	char fecha[11];
	int i, j,k, fila_actual = -1;
	int filas_validas[MAX_ROWS];
	int counter = 0, frecuencia[7] = {0};
	int total[7][MAX_ROWS] = {0}; // Replica de historico.csv
	int mascara[7][MAX_ROWS] = {0};
	int single[RANK] = {0}; // Frecuencia de aparición números únicos
	int pair[RANK][RANK] = {0}; // Frecuencia pares
	int adicionales[RANK][RANK] = {0}; // Frecuencia pares

	// Abrir el archivo CSV por primera vez
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error: No se pudo abrir el archivo %s\n", filename);
		exit(1);
	}
	// Primera lectura línea por línea
	while( fgets( line, sizeof(line), file ) )
	{
		sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%s", &nproducto, &concurso, &R[0], &R[1], &R[2], &R[3], &R[4], &R[5], &R[6], &bolsa, fecha);
		// Búsqueda del primer número
		if (fila_actual > -1)
		{
			for ( j = 0 ; j < 7 ; j++ ) {
				total[j][fila_actual] = R[j];
				single[R[j] - 1] += 1;
			}
		}
		fila_actual++;
	}
	fclose(file);
	printf("Frecuencia números únicos\n");
	for ( i = 0 ; i < RANK ; i++ ) {
		printf("%2d,%3d\n", i+1, single[i]);
	}
	/*
	 * Cálculo natural con natural
	 */
	// Primer número
	
	for ( i = 0 ; i <= RANK ; i++ ) {
		printf("%2d,",i);
	}
	printf("\n");
	for ( i = 1 ; i <= RANK ; i++ ) {
		printf("%2d,",i);
		// Segundo número
		for ( j = 1 ; j <= RANK ; j++ ) {
			// Iteramos sobre el historico
			for ( fila_actual = 0 ; fila_actual < MAX_ROWS ; fila_actual++ ) {
				int min,max;
				if (i < j){
					min = i;
					max = j;
				} else {
					min = j;
					max = i;
				}
				bool primerQ = false;
				bool segundoQ = false;
				int pos = 0;
				for ( k = 0 ; k < 6 ; k++ ) {
					if ( total[k][fila_actual] == min )
					{
						primerQ = true;
						pos = k;
						break;
					}
				}
				for ( k = pos + 1 ; k < 6 ; k++ ) {
					if ( total[k][fila_actual] == max )
						segundoQ = true;
				}
				if ( primerQ && segundoQ )
					pair[i][j] += 1;
			}
			printf("%2d,",pair[i][j]);
		}
		printf("\n");
	}
	printf("-----------------------\n");
	printf("Adicionales\n");
	printf("-----------------------\n");
	/*
	 * Cálculo natural con adicional 
	 */
	for ( i = 0 ; i <= RANK ; i++ ) {
		printf("%2d,",i);
	}
	printf("\n");
	for ( i = 1 ; i <= RANK ; i++ ) {
		printf("%2d,",i);
		// Segundo número
		for ( j = 1 ; j <= RANK ; j++ ) {
			// Iteramos sobre el historico
			for ( fila_actual = 0 ; fila_actual < MAX_ROWS ; fila_actual++ ) {
				bool primerQ = false;
				bool segundoQ = false;
				for ( k = 0 ; k < 6 ; k++ ) {
					if ( total[k][fila_actual] == i )
						primerQ = true;
				}
				if ( total[6][fila_actual] == j )
					segundoQ = true;
				if ( primerQ && segundoQ )
					adicionales[i][j] += 1;
			}
			printf("%2d,",adicionales[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	struct timeval stop, start;
	int numeros[7] = {0};
	int i;
	if ( argc > 2 )
	{
		printf("Error: El programa no acepta argumentos.\n");
		return 1;
	}
	// Leer argumentos
	numeros[0] = 1;
	numeros[1] = 2;
	// Leer CSV
	gettimeofday(&start, NULL);
	leer_csv("historico.csv", numeros, argc - 1);
	gettimeofday(&stop, NULL);
	printf("Time: %lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
	return 0;
}
