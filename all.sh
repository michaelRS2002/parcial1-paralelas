#!/usr/bin/env bash
#
# Este script se encarga de invocar los tres programas que permiten la 
# conversion de un PNG a secuencia de pixeles, se procesan esos pixeles y
# posteriormente se convierte esa secuencia de pixeles a un archivo en formato
# PNG
#
# Autor: John Sanabria - john.sanabria@correounivalle.edu.co
# Fecha: 2024-08-22
#
# Asignar la carpeta como argumento
DIRECTORIO=$1
NUM_THREADS=$2 
echo "Ejecutando con '$NUM_THREADS' hilos."
#Iterar sobre todos los archivos PNG en la carpeta
for INPUT_PNG in "${DIRECTORIO}"/*.png; do
    # Definir un nombre temporal para el archivo binario
    TEMP_FILE="${INPUT_PNG%.*}.bin"
    python3 fromPNG2Bin.py "${INPUT_PNG}"
    ./main "${TEMP_FILE}" "$NUM_THREADS"
    python3 fromBin2PNG.py "${TEMP_FILE}.new"
done
