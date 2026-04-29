#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define Q_LIMIT     1000 /* limite de la cola extendido */
#define BUSY        1   /* servidor ocupado */
#define IDLE        0   /* servidor libre */
#define MAX_SERVERS 10 /* limite maximo de servidores q acepta el programa */

/* Modos de operacion del simulador */
#define MODE_MANUAL    1
#define MODE_MM1       2
#define MODE_EMPIRICAL 3

void  initialize();
void  timing();
void  arrive();
void  depart();
void  report();
void  updateTimeAvgStats();  
float genNextInterarrive();
float genNextDepart();
    
/* VARIABLES DE ESTADO Y RELOJ */
float simTime;                  /* el reloj de la simulacion */
int   numInQ;                   /* numero en cola: Q(t) */
float timeLastEvent;            /* tiempo del ultimo evento para calcular areas */
int   nextEventType;            /* 1 = arribo, 2 = partida*/
float timeArrival[Q_LIMIT + 1]; /* array para guardar a q hora llego cada cliente a la cola */

/* VARIABLES MULTI-SERVIDOR */
int numServers; /* cantidad de servidores activos */
int nextServer; /* indica q servidor termino su servicio */
int serverStatus[MAX_SERVERS + 1]; /* array de estados: 1 ocupado, 0 libre */
float timeNextEvent[MAX_SERVERS + 2]; /* FEL. indice 1: arribo, indices 2 al N+1: partidas */
float timeArrivalOfCustomerInService[MAX_SERVERS + 1]; /* a q hora llego el cliente de cada servidor */

/* CONTADORES ESTADISTICOS */
int   numCustsDelayed;          /* contador de clientes q ya terminaron su espera en cola: n */
int   numCustsCompleted;        /* contador de clientes q ya se fueron del sistema */
float totalOfDelays;            /* sumatoria de demoras en cola */
float totalSystemTime;          /* sumatoria de demoras totales en el sistema (cola + servicio) */
float areaNumInQ;               /* integral de Q(t) */
float areaServerStatus;         /* integral de B(t) */

/* CONFIGURACION DEL MODELO (Seteada en main) */
int   simMode;                  /* el modo elegido para generar variables */
float maxSimTime;               /* condicion de corte por reloj */
int   maxCusts;                 /* condicion de corte por cantidad de clientes */

/* Variables para MODE_MANUAL */
float manualInterarrivals[100];
float manualServices[100];
int   manualArrIdx = 0;
int   manualSrvIdx = 0;

/* Variables para MODE_MM1 */
float meanInterarrival;
float meanService;

/* Variables para MODE_EMPIRICAL */
float empArrVals[20], empArrProbs[20];
int   empArrCount;
float empSrvVals[20], empSrvProbs[20];
int   empSrvCount;

int main () {
    /* Semilla para los numeros random (usa la hora del sistema) */
    srand(time(NULL));

    /* CONFIGURACION PARA EL EJERCICIO 2 */
    // simMode = MODE_MANUAL;
    // maxSimTime = 99999.0;    /* tiempo infinito, no cortamos por reloj */
    // maxCusts = 8;     /* la tabla tiene exactamente 8 clientes */
    // numServers = 1;

    /* Interarribos: 0,5 - 0,8 - 1 - 0,3 - 1,1 - 1 - 1,3 - 0,3 */
    // manualInterarrivals[0] = 0.5;
    // manualInterarrivals[1] = 0.8;
    // manualInterarrivals[2] = 1.0;
    // manualInterarrivals[3] = 0.3;
    // manualInterarrivals[4] = 1.1;
    // manualInterarrivals[5] = 1.0;
    // manualInterarrivals[6] = 1.3;
    // manualInterarrivals[7] = 0.3;

    /* Servicios: 2,7 - 2 - 4 - 3,1 - 1 - 2,1 - 4,3 - 1,4 */
    // manualServices[0] = 2.7;
    // manualServices[1] = 2.0;
    // manualServices[2] = 4.0;
    // manualServices[3] = 3.1;
    // manualServices[4] = 1.0;
    // manualServices[5] = 2.1;
    // manualServices[6] = 4.3;
    // manualServices[7] = 1.4;

    /* CONFIGURACION PARA EL EJERCICIO 4 (LAVADERO DE AUTOS) */
    simMode = MODE_EMPIRICAL;
     maxSimTime = 300.0;    /* 5 horas abiertas = 300 minutos */
     maxCusts = 999999;     /* infinito, cortamos por tiempo, no por clientes */
     numServers = 1;    //  /* Cargar tabla empirica de Interarribos para ejercicio 4*/
     empArrCount = 8;
     empArrVals[0] = 5; empArrProbs[0] = 0.05;
     empArrVals[1] = 10; empArrProbs[1] = 0.05;
     empArrVals[2] = 15; empArrProbs[2] = 0.10;
     empArrVals[3] = 20; empArrProbs[3] = 0.10;
     empArrVals[4] = 25; empArrProbs[4] = 0.30;
     empArrVals[5] = 30; empArrProbs[5] = 0.20;
     empArrVals[6] = 35; empArrProbs[6] = 0.15;
     empArrVals[7] = 40; empArrProbs[7] = 0.05;
     /* Cargar tabla empirica de Servicios para ejercicio 4*/
     empSrvCount = 4;
     empSrvVals[0] = 10; empSrvProbs[0] = 0.15;
     empSrvVals[1] = 20; empSrvProbs[1] = 0.25;
     empSrvVals[2] = 30; empSrvProbs[2] = 0.40;
     empSrvVals[3] = 40; empSrvProbs[3] = 0.20;
    /* CONFIGURACION PARA EL EJERCICIO 5 */
    // simMode = MODE_MM1; 
    // numServers = 1;
    // maxSimTime = 99999.0;  /* tiempo infinito, no corto por reloj */
    // maxCusts = 10000000; // simulacion de 10.000 cliente
    // lambda = 3
    // meanInterarrival = 1.0 / 3;
    // mu = 4
    // meanService = 1.0 / 4.0;

    initialize();

    /* corta si se alcanza el maximo de clientes O el maximo de tiempo */
    while (numCustsDelayed < maxCusts && simTime < maxSimTime) {
        timing();               /* mirar la FEL y avanzar el reloj al proximo evento */
        
        /* evita procesar eventos que ocurren despues de cerrar el local */
        if (simTime > maxSimTime) break;

        updateTimeAvgStats();   /* calcular el area de los graficos (base x altura) */

        /* ejecutar logica segun el tipo de evento */
        switch (nextEventType) {
            case 1: arrive(); break;
            case 2: depart(); break;
        }
    }

    report(); /* imprimir los resultados estadisticos */
    return 0; 
}

/* inicializa la fila 0 de la tabla de rastreo */
void initialize() {
    simTime = 0.0;
    numInQ = 0;             
    timeLastEvent = 0.0;    
    numCustsDelayed = 0;
    numCustsCompleted = 0;
    totalOfDelays = 0.0;
    totalSystemTime = 0.0;
    areaNumInQ = 0.0;
    areaServerStatus = 0.0;
    
    // inicializacion de servidores como libres con partidas en infinito
    for (int i = 1; i <= numServers; i++) {
        serverStatus[i] = IDLE;    
        timeNextEvent[i + 1] = 1.0e+30; 
    }

    // primer arribo
    timeNextEvent[1] = simTime + genNextInterarrive();  
}

/* revisa el array timeNextEvent y se fija q evento ocurre primero en el tiempo */
void timing() {
    float minTime = 1.0e+29;
    nextEventType = 0;
    nextServer = 0;

    /* Busca el evento mas cercano (El arribo [1] o cualquier partida [2 al N+1]) */
    for (int i = 1; i <= numServers + 1; i++) {
        if (timeNextEvent[i] < minTime) {
            minTime = timeNextEvent[i];
            if (i == 1) {
                nextEventType = 1; /* Es un arribo */
            } else {
                nextEventType = 2; /* Es una partida */
                nextServer = i - 1; /* Identificar que maquina fue (1, 2, 3...) */
            }
        }
    }
    simTime = minTime;
}

/* sumar el area del rectangulo q acaba de terminar */
void updateTimeAvgStats() {
    float timeSinceLastEvent = simTime - timeLastEvent;
    timeLastEvent = simTime;

    areaNumInQ += (numInQ * timeSinceLastEvent);
    
    /* Sumar cuantos servidores estaban ocupados simultaneamente */
    int busyCount = 0;
    for (int i = 1; i <= numServers; i++) {
        if (serverStatus[i] == BUSY) busyCount++;
    }
    areaServerStatus += (busyCount * timeSinceLastEvent);
} 

/* llega alguien, se programa cuando llega el proximo y se evalua el estado */
void arrive() {
    timeNextEvent[1] = simTime + genNextInterarrive();

    /* Buscar si hay algun servidor libre */
    int idleServer = 0;
    for (int i = 1; i <= numServers; i++) {
        if (serverStatus[i] == IDLE) {
            idleServer = i;
            break;
        }
    }

    if (idleServer == 0) {
        /* Todos ocupados -> va a la cola */
        numInQ++;
        if (numInQ > Q_LIMIT) { printf("Error: cola llena\n"); exit(2); }
        timeArrival[numInQ] = simTime;
    } else {
        /* Hay un servidor libre -> Pasa directo al idleServer */
        numCustsDelayed++;
        serverStatus[idleServer] = BUSY; 
        timeArrivalOfCustomerInService[idleServer] = simTime; 
        timeNextEvent[idleServer + 1] = simTime + genNextDepart();
    }
}

/* alguien termina su tramite y abandona el sistema */
void depart() {
    float timeInSystem = simTime - timeArrivalOfCustomerInService[nextServer];
    totalSystemTime += timeInSystem;
    numCustsCompleted++;

    if (numInQ == 0) {
        /* El servidor especifico que termino queda libre */
        serverStatus[nextServer] = IDLE;
        timeNextEvent[nextServer + 1] = 1.0e+30; 
    } else {
        numInQ--; 
        float delay = simTime - timeArrival[1];
        totalOfDelays += delay; 
        numCustsDelayed++; 

        /* El nuevo cliente entra al servidor que se acaba de desocupar */
        timeArrivalOfCustomerInService[nextServer] = timeArrival[1]; 
        timeNextEvent[nextServer + 1] = simTime + genNextDepart();

        for (int i = 1; i <= numInQ; i++) {
            timeArrival[i] = timeArrival[i + 1];
        }
    }
}

void report() {
    printf("\n--- Resultados (Modo: %d | Servidores: %d) ---\n", simMode, numServers);
    printf("Tiempo simulado: %.2f min | Clientes: %d\n", simTime, numCustsCompleted);
    printf("1. Demora promedio en cola: %.4f min\n", totalOfDelays / numCustsDelayed);
    printf("2. Demora en el sistema: %.4f min\n", totalSystemTime / numCustsCompleted);
    printf("3. Longitud promedio de cola: %.4f\n", areaNumInQ / simTime);
    printf("4. Numero promedio en sistema: %.4f\n", (areaNumInQ + areaServerStatus) / simTime);
    
    /* La utilizacion se promedia entre la cantidad de maquinas (c) */
    printf("5. Utilizacion PROMEDIO de servidores: %.2f %%\n", (areaServerStatus / (simTime * numServers)) * 100);
}

/* === GENERADORES DE VARIABLES ALEATORIAS === */

/* Genera un numero random uniforme entre 0.0 y 0.9999 */
float getRandom() {
    return (float)rand() / (float)RAND_MAX;
}

float genNextInterarrive() {
    if (simMode == MODE_MANUAL) {
        return manualInterarrivals[manualArrIdx++];
    } 
    else if (simMode == MODE_MM1) {
        return -meanInterarrival * log(1.0 - getRandom());
    } 
    else if (simMode == MODE_EMPIRICAL) {
        float r = getRandom();
        float cumulative = 0.0;
        for (int i = 0; i < empArrCount; i++) {
            cumulative += empArrProbs[i];
            if (r <= cumulative) return empArrVals[i];
        }
    }
    return 0;
}

float genNextDepart() {
    if (simMode == MODE_MANUAL) {
        return manualServices[manualSrvIdx++];
    } 
    else if (simMode == MODE_MM1) {
        return -meanService * log(1.0 - getRandom());
    } 
    else if (simMode == MODE_EMPIRICAL) {
        float r = getRandom();
        float cumulative = 0.0;
        for (int i = 0; i < empSrvCount; i++) {
            cumulative += empSrvProbs[i];
            if (r <= cumulative) return empSrvVals[i];
        }
    }
    return 0;
}