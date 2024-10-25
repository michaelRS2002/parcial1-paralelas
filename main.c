#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
// Funciones placeholder para la carga y guardado de imágenes
void cargarImagen(int *imagen, int width, int height);
void guardarImagen(int *imagen, int width, int height);

// Función para aplicar un filtro simple
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height);

// Función para calcular la suma de los píxeles (como una estadística)
int calcularSumaPixeles(int *imagen, int width, int height);

char *filename;

int main(int argc, char* argv[]) {
    int width = 1024, height = 1024;
    int *imagen = (int *)malloc(width * height * sizeof(int));
    int *imagenProcesada = (int *)malloc(width * height * sizeof(int));
    if (argc != 3) {
      fprintf(stderr, "Uso: %s <nombre_archivo> <numero_hilos>\n", argv[0]);
      exit(1);
    }

    filename = argv[1];
    int num_threads = atoi(argv[2]);

    omp_set_num_threads(num_threads);
    // Cargar la imagen (no paralelizable)
    cargarImagen(imagen, width, height);

    // Aplicar filtro (paralelizable)
    aplicarFiltro(imagen, imagenProcesada, width, height);

    // Calcular suma de píxeles (parte paralelizable)
    int sumaPixeles = calcularSumaPixeles(imagenProcesada, width, height);

    printf("Suma de píxeles: %d\n", sumaPixeles);

    // Guardar la imagen (no paralelizable)
    guardarImagen(imagenProcesada, width, height);

    free(imagen);
    free(imagenProcesada);
    return 0;
}

// Carga una imagen desde un archivo binario
void cargarImagen(int *imagen, int width, int height) {
    FILE *archivo = fopen(filename, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para cargar la imagen");
        return;
    }

    size_t elementosLeidos = fread(imagen, sizeof(int), width * height, archivo);
    if (elementosLeidos != width * height) {
        perror("Error al leer la imagen desde el archivo");
    }

    fclose(archivo);
}

// Guarda una imagen en un archivo binario
void guardarImagen(int *imagen, int width, int height) {
    char *output_filename;

    output_filename = (char*)malloc(sizeof(char)*(strlen(filename) + 4));
    sprintf(output_filename,"%s.new",filename);
    FILE *archivo = fopen(output_filename, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para guardar la imagen");
        return;
    }

    size_t elementosEscritos = fwrite(imagen, sizeof(int), width * height, archivo);
    if (elementosEscritos != width * height) {
        perror("Error al escribir la imagen en el archivo");
    }

    fclose(archivo);
}

void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height) {
    // Recorre cada píxel de la imagen (excepto los bordes)
    # pragma omp parallel for
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Promedia los valores de los píxeles vecinos
            int sum = 0;
            sum += imagen[(y - 1) * width + (x - 1)];  // Superior Izquierda
            sum += imagen[(y - 1) * width + x];        // Superior
            sum += imagen[(y - 1) * width + (x + 1)];  // Superior Derecha
            sum += imagen[y * width + (x - 1)];        // Izquierda
            sum += imagen[y * width + x];              // Centro
            sum += imagen[y * width + (x + 1)];        // Derecha
            sum += imagen[(y + 1) * width + (x - 1)];  // Inferior Izquierda
            sum += imagen[(y + 1) * width + x];        // Inferior
            sum += imagen[(y + 1) * width + (x + 1)];  // Inferior Derecha

            imagenProcesada[y * width + x] = sum / 9;  // Asigna el promedio al píxel procesado
        }
    }
}


//Paralelizar
int calcularSumaPixeles(int *imagen, int width, int height) {
    int suma = 0;

    #pragma omp parallel
    {
        int suma_local = 0;
        #pragma omp for
        for (int i = 0; i < width * height; i++) {
        suma_local += imagen[i];
        }

        #pragma omp atomic
        suma += suma_local;
    }
    return suma;
}

