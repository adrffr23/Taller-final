#ifndef FUNCIONES_H
#define FUNCIONES_H

struct Zona {
    char nombre[50];
    float contaminantes[4];  
    float temperatura;
    float velocidadViento;
    float humedad;
    int dia;
};


void ingresarDatos();
void monitoreoContaminacion();
void predecirContaminantes();
void calcularPromediosHistoricos();
void ingresarNombreZona(char* nombre);
void mostrarDatosGuardados();
void leerdatosGuardados(struct Zona *zonasGuardadas, int *Regristro);
void menu();

#endif 
