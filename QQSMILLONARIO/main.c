#include "header_tp.h"

int main()
{
    int n_participantes, rondas, tiempo_ronda, nivel_dif;
    char opcion;
    t_lista lista;
    Pregunta preguntas[MAX_PREGUNTAS];

    if(carga_config(&rondas, &tiempo_ronda)==ERROR_CONFIG)
        return ERROR_SALIR;

    menu(&opcion);
    crear_lista(&lista);

    while(opcion=='A')
    {
        carga_participantes(&lista, &n_participantes);
        dificultad(&nivel_dif);
        jugarRondas(n_participantes, tiempo_ronda, rondas, nivel_dif, &lista, preguntas);
        if(calcular_puntaje(&lista, preguntas, rondas, n_participantes)==ERROR_DE_MEMORIA)
        {
            vaciar_lista(&lista);
            return ERROR_SALIR;
        }
        system("cls");
        if(informe_arch_ganador(&lista, preguntas, rondas, n_participantes)== ERROR_DE_MEMORIA)
        {
            vaciar_lista(&lista);
            return ERROR_SALIR;
        }
        menu(&opcion);
        if(opcion=='A')
            vaciar_lista(&lista);
    }

    return 0;
}
