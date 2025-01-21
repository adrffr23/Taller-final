#include <stdio.h>
#include <string.h>
#include "funciones.h"

struct Zona zonas[5];
float limitesContaminantes[4] = {400.0, 40.0, 20.0, 25.0};
const char* nombresContaminantes[4] = {"CO2", "NO2", "SO2", "PM2.5"};

int esNumeroPositivo(float num) {
    return num >= 0;
}

void ingresarNombreZona(char* nombre) {
    fgets(nombre, 100, stdin);
    size_t len = strlen(nombre);
    if (len > 0 && nombre[len - 1] == '\n') {
        nombre[len - 1] = '\0';
    }
}



void ingresarDatos() {
    FILE *file = fopen("datos.bin", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    for (int i = 0; i < 5; i++) {
        printf("Ingrese el nombre de la zona %d: ", i + 1);
        while (getchar() != '\n');  // Limpiar el buffer antes de usar fgets
        ingresarNombreZona(zonas[i].nombre);

        printf("Ingrese los niveles de contaminantes (CO2, NO2, SO2, PM2.5):\n");
        for (int j = 0; j < 4; j++) {
            do {
                printf("%s: ", nombresContaminantes[j]);
                scanf("%f", &zonas[i].contaminantes[j]);
                if (!esNumeroPositivo(zonas[i].contaminantes[j])) {
                    printf("Error, ingrese un numero positivo para continuar.\n");
                }
            } while (!esNumeroPositivo(zonas[i].contaminantes[j]));
        }

        printf("Ingrese los datos climaticos (temperatura, velocidad del viento, humedad):\n");
        do {
            printf("Temperatura: ");
            scanf("%f", &zonas[i].temperatura);
            if (!esNumeroPositivo(zonas[i].temperatura)) {
                printf("Error, ingrese un numero positivo para continuar.\n");
            }
        } while (!esNumeroPositivo(zonas[i].temperatura));

        do {
            printf("Velocidad del viento: ");
            scanf("%f", &zonas[i].velocidadViento);
            if (!esNumeroPositivo(zonas[i].velocidadViento)) {
                printf("Error, ingrese un numero positivo para continuar.\n");
            }
        } while (!esNumeroPositivo(zonas[i].velocidadViento));

        do {
            printf("Humedad: ");
            scanf("%f", &zonas[i].humedad);
            if (!esNumeroPositivo(zonas[i].humedad)) {
                printf("Error, ingrese un numero positivo para continuar.\n");
            }
        } while (!esNumeroPositivo(zonas[i].humedad));

        zonas[i].dia = i+1;
    }

    fwrite(zonas, sizeof(struct Zona), 5, file);
    fclose(file);
    printf("Datos guardados correctamente.\n");
}
void leerdatosGuardados(struct Zona *zonasGuardadas, int *Regristro) {
    FILE *file = fopen("datos.bin", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    *Regristro=0;
    while (fread(&zonasGuardadas[*Regristro], sizeof(struct Zona), 1, file))
    {
        (*Regristro)++;   
    }
    ;
    fclose(file);
}
void monitoreoContaminacion() {
    struct Zona zonasguardadas[20];
    int regnum=0;
    leerdatosGuardados(zonasguardadas,&regnum);

    printf("\n************************** MONITOREO DE CONTAMINACION **************************\n");
    for (int i = 0; i < regnum; i++) {
        printf("\nZona: %s\n", zonasguardadas[i].nombre);
        for (int j = 0; j < 4; j++) {
            printf("Nivel de %s: %.2f\n", nombresContaminantes[j], zonasguardadas[i].contaminantes[j]);
            if (zonasguardadas[i].contaminantes[j] > limitesContaminantes[j]) {
                printf("Advertencia! Nivel de %s excede el limite\n Se debe paralizar la quema de materia prima\nreducir el tiempo de horas de trabajo en industrias\n", nombresContaminantes[j]);
            }
        }
    }
}
//guardar las preddiciones en un archivo binario
void save (struct Zona *zonasGuardadas, int *Regristro){
    FILE *file = fopen("predicciones.bin", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    fwrite(zonasGuardadas, sizeof(struct Zona), *Regristro, file);
    fclose(file);
    printf("Datos de prediccion guardados correctamente.\n");
}
void predecirContaminantes() {
    struct Zona zonasguardadas[20];
    int regnum=0;
    float valores[4];
    leerdatosGuardados(zonasguardadas,&regnum);
    float coefTemp = 0.05;  // Ejemplo: 5% de ajuste por cada grado de temperatura
    float coefViento = 0.03;  // Ejemplo: 3% de ajuste por cada unidad de velocidad del viento
    float coefHumedad = 0.02;
    int pesos[5]={1,2,3,4,5};
    float sumaPonderadaTemp = 0, sumaPonderadaViento = 0, sumaPonderadaHumedad = 0;
    int sumaPesosClima = 0;
    for (int i = 0; i < regnum; i++) {
        for(int k=0;k<5;k++){
            if (i-k >= 0)
            {
                sumaPonderadaTemp+=zonasguardadas[i-k].temperatura*pesos[k];
                sumaPonderadaViento+=zonasguardadas[i-k].velocidadViento*pesos[k];
                sumaPonderadaHumedad+=zonasguardadas[i-k].humedad*pesos[k];
                sumaPesosClima+=pesos[k];
            }
        }
    }
    float promedioTemp = sumaPonderadaTemp/sumaPesosClima;
    float promedioViento = sumaPonderadaViento/sumaPesosClima;
    float promedioHumedad = sumaPonderadaHumedad/sumaPesosClima;

    printf("Prediccion de temperatura para el dia siguiente: %.2f\n", promedioTemp);
    printf("Prediccion de velocidad del viento para el dia siguiente: %.2f\n", promedioViento);
    printf("Prediccion de humedad para el dia siguiente: %.2f\n", promedioHumedad);
    save(zonasguardadas,&regnum);
    printf("\n************************** PREDICCIONES DE NIVELES **************************\n");
    for (int j = 0; j < 4; j++) {
        float sumaprom=0;
        int count =0;
        for (int i = 0; i < regnum; i++) {
            for(int k=0;k<5;k++){
                if (i-k >= 0)
                {
                    sumaprom+=zonasguardadas[i-k].contaminantes[j]*pesos[k];
                    count+=pesos[k];
                }
                
                
            }
            
        }
        float promedio = sumaprom/count;
        float ajusteTemp = coefTemp * (promedioTemp - 20);  
        float ajusteViento = coefViento * promedioViento;  
        float ajusteHumedad = coefHumedad * promedioHumedad;
        float prediccion = promedio * (1 + ajusteTemp + ajusteViento + ajusteHumedad);
        valores[j]=prediccion;
            ///printf("Prediccion de %s para el dia siguiente: %.2f\n", nombresContaminantes[j], promedio);
            //if (promedio > limitesContaminantes[j]) {
            //    printf("Alerta! El nivel de %s supera el limite.\n", nombresContaminantes[j]);
            //    printf("Medidas recomendadas:\n");
            //    printf("- Reducción del trafico vehicular\n");
            //    printf("- Cierre temporal de industrias\n");
            //    printf("- Suspension de actividades al aire libre\n");
            //}
    }
    if(valores[0]>limitesContaminantes[0]){
            printf("Alerta! El nivel de %s supera el limite y es de:%.2f.\n", nombresContaminantes[0],valores[0]);
            printf("Medidas recomendadas:\n");
            printf("- Reducción del trafico vehicular\n");
        }else{
            printf("El nivel de %s esta dentro del limite y es de %.2f.\n", nombresContaminantes[0], valores[0]);
        }
    if(valores[1]>limitesContaminantes[1]){
            printf("Alerta! El nivel de %s supera el limite y es de:%.2f.\n", nombresContaminantes[1],valores[1]);
            printf("Medidas recomendadas:\n");
            printf("- Cierre temporal de industrias\n");
        }else{
            printf("El nivel de %s esta dentro del limite y es de %.2f.\n", nombresContaminantes[1], valores[1]);
        }
    if(valores[2]>limitesContaminantes[2]){
            printf("Alerta! El nivel de %s supera el limite y es de:%.2f.\n", nombresContaminantes[2],valores[2]);
            printf("Medidas recomendadas:\n");
            printf("- Suspension de actividades al aire libre\n");
        }else{
            printf("El nivel de %s esta dentro del limite y es de %.2f.\n", nombresContaminantes[2], valores[2]);
        }
    if(valores[3]>limitesContaminantes[3]){
            printf("Alerta! El nivel de %s supera el limite y es de:%.2f.\n", nombresContaminantes[3],valores[3]);
            printf("Medidas recomendadas:\n");
            printf("-uso de mascarilla\n");
        }else{
            printf("El nivel de %s esta dentro del limite y es de %.2f.\n", nombresContaminantes[3], valores[3]);
        }
    
}

void calcularPromediosHistoricos() {
    struct Zona zonasguardadas[20];
    int regnum=0;
    leerdatosGuardados(zonasguardadas,&regnum);

    printf("\n************************** PROMEDIOS HISTÓRICOS **************************\n");
    for (int j = 0; j < 4; j++) {
        float promedio = 0;
        for (int i = 0; i < 5; i++) {
            promedio += zonasguardadas[i].contaminantes[j];
        }
        promedio /= regnum;
        printf("Promedio historico de %s: %.2f\n", nombresContaminantes[j], promedio);

        if (promedio > limitesContaminantes[j]) {
            printf("\nAlerta! El promedio historico de %s supera el limite.\n", nombresContaminantes[j]);
            printf("Medidas recomendadas:\n");
            printf("- Reducción del trafico vehicular\n");
            printf("- Cierre temporal de industrias\n");
            printf("- Suspension de actividades al aire libre\n");
        }
    }
}

void mostrarDatosGuardados() {
    struct Zona zonasguardadas[100];
    int regnum=0;
    leerdatosGuardados(zonasguardadas,&regnum);

    printf("\n************************** DATOS GUARDADOS **************************\n");
    for (int i = 0; i < regnum; i++) {
        printf("\nZona: %s\n", zonasguardadas[i].nombre);
        for (int j = 0; j < 4; j++) {
            printf("Nivel de %s: %.2f\n", nombresContaminantes[j], zonasguardadas[i].contaminantes[j]);
        }
        printf("Temperatura: %.2f\n", zonasguardadas[i].temperatura);
        printf("Velocidad del viento: %.2f\n", zonasguardadas[i].velocidadViento);
        printf("Humedad: %.2f\n", zonasguardadas[i].humedad);
    }
}


void menu() {
    int opcion;
    do {
        printf("\n************************** MENÚ PRINCIPAL **************************\n");
        printf("1. Ingresar datos del dia actual\n");
        printf("2. Monitoreo de contaminacion actual\n");
        printf("3. Prediccion de niveles para el dia siguiente\n");
        printf("4. Calcular promedios historicos\n");
        printf("5. Mostrar datos guardados\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ingresarDatos();
                break;
            case 2:
                monitoreoContaminacion();
                break;
            case 3:
                predecirContaminantes();
                break;
            case 4:
                calcularPromediosHistoricos();
                break;
            case 5:
                mostrarDatosGuardados();
                break;
            case 6:
                printf("Gracias por usar el programa!\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
}