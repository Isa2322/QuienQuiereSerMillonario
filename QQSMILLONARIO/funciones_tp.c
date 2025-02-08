#include "header_tp.h"

void crear_lista(t_lista *lista)
{
    *lista=NULL;
}

///

void vaciar_lista(t_lista *lista)
{
    t_nodo *aux;

    if(*lista==NULL)
        return;

    while(*lista)
    {
        aux=(*lista)->sig;

        if(aux==(*lista))
        {
            *lista=NULL;
        }
        else
        {
            (*lista)->sig=aux->sig;
        }

        free(aux->info);
        free(aux);
    }
}

///

int poner_sig(t_lista *lista, const void *elem, unsigned tam_elem) //pongo un nuevo nodo a continuacion del que esta apuntado
{
    t_nodo *nue;

    nue=malloc(sizeof(t_nodo));

    if(!nue)
    {
        return ERROR_DE_MEMORIA;
    }

    nue->info=malloc(tam_elem);

    if(!(nue->info))
    {
        free(nue);
        return ERROR_DE_MEMORIA;
    }

    memcpy(nue->info, elem, tam_elem);
    nue->tam_info=tam_elem;

    if((*lista) == NULL)
    {
        nue->sig=nue;
        (*lista)=nue;
        return TODO_OK;
    }

    nue->sig=(*lista)->sig;
    (*lista)->sig=nue;

    return TODO_OK;
}

///

int randomizer(t_lista *lista, int num) //lo que hago aca es avanzar un numero n de nodos y luego declarar ese como el primero
{
    if(*lista == NULL)
        return ERROR_DE_MEMORIA;

    t_nodo *aux;

    int i, j;

    srand(time(NULL));
    j=rand()%num;

    aux=*lista;

    for(i=0; i<j; i++)
    {
        aux=aux->sig;
    }

    *lista=aux;

    return TODO_OK;
}

///

void sig_lista(t_lista *lista)
{
    t_nodo *aux;

    aux=(*lista)->sig;
    (*lista)=aux;
}

///

int carga_config(int* rounds, int* cant_rounds)
{
    FILE *config;
    config=fopen("config.txt","rt");
    if(!config)
    {
        puts("Error al cargar las configuraciones");
        return ERROR_CONFIG;
    }

    fscanf(config,"Rounds: %d\nTiempo por round: %d",rounds, cant_rounds);

    fclose(config);


    if((*rounds)<4 || (*rounds)>7)
    {
        puts("Error al cargar las configuraciones");
        return ERROR_CONFIG;
    }

    return TODO_OK;
}

///

int carga_participantes(t_lista *lista,int* n_participantes)
{
    int i;
    jugador nuevo_j;

    do
    {
        printf("Ingrese la cantidad de participantes: ");
        scanf("%d",n_participantes);
        fflush(stdin);
        system("cls");
    }
    while((*n_participantes)<1); ///como no tengo un limite de participantes no tengo como validar que no sea una letra aun

    system("cls");

    for(i=0; i<(*n_participantes); i++)
    {
        printf("Ingrese el nombre del participante %d: ", i + 1);
        scanf("%s", nuevo_j.nombre);
        fflush(stdin);
        nuevo_j.puntos=0;

        if(i>1)
        {
            randomizer(lista,i+1);
        }

        poner_sig(lista, &nuevo_j, sizeof(jugador));

        system("cls");
    }


    return TODO_OK;
}

///

void menu(char *opcion)
{
    printf("\nSelecione una opcion....\n");
    printf("\n[A]Jugar\n");
    printf("\n[B]Salir\n");

    do
    {
        scanf("%c",opcion);
        fflush(stdin);
        (*opcion)=toupper(*opcion);
    }
    while((*opcion)!='A' && (*opcion)!='B');

    system("cls");

}

///

void dificultad(int *dif)
{
    *(dif)=0;
    do
    {
        printf("Ingrese el nivel de dificultad \n(1)facil\n(2)medio\n(3)dificil: ");
        fflush(stdin);
        scanf("%d",dif);
        fflush(stdin);
        system("cls");
    }
    while((*dif)<1 || (*dif)>3);
}

///

void mezclar_preguntas(Pregunta preguntas[], int num_preguntas)
{
    int i, j;
    Pregunta aux;

    srand(time(0));

    for(i=(num_preguntas-1); i>0; i--)
    {
        j=rand()%(i);

        aux=preguntas[i];
        preguntas[i]=preguntas[j];
        preguntas[j]=aux;
    }
}

///

void jugarRondas(int n_participantes,int tiempo_ronda,int rondas,int nivel_dif, t_lista *lista, Pregunta preguntas[])
{
    int r, num_preguntas=0, j, hay_resp=0, tiempo_respuesta, i;
    char respuesta;
    jugador *act;
    Pregunta *pregunta;
    clock_t start_time;

    act=malloc(sizeof(jugador));

    if(!act)
    {
        return;
    }

    if (!obtener_preguntas(nivel_dif, preguntas, &num_preguntas))
    {
        printf("No se pudieron obtener las preguntas\n");
        return;
    }

    mezclar_preguntas(preguntas, num_preguntas);
/// nota: las fotos de los lotes de prueba corresponden al orden de las preguntas por defecto
/// para comprobar que las fotos de los lotes de prueba sean correctos se puede comentar
/// la funcion mezclar_preguntas para conseguir los mismos resultados que en el lote, ya que es
/// no es absolutamente necesaria, sin embargo, para jugar es mejor dejar la funcion tal como está

    printf("Se jugaran %d rondas de %d segundos cada una\nEl orden de los jugadores sera:\n", rondas, tiempo_ronda);

    for(i=0; i<n_participantes; i++)
    {
        memcpy(act, (*lista)->info, sizeof(jugador));
        printf("%d) %s\n",i+1,act->nombre);
        sig_lista(lista);
    }

    system("pause");
    system("cls");


    for(i=0; i<n_participantes; i++)
    {
        memcpy(act, (*lista)->info, sizeof(jugador));
        printf("Turno de %s.\n Estas listo/a?\n", act->nombre);
        system("pause");
        system("cls");
        fflush(stdin);

        for(r=0; r<rondas; r++)
        {
            hay_resp=0;
            pregunta=&preguntas[r];

            printf("Pregunta: %s\n", pregunta->pregunta);


            for(j=0; j < pregunta->num_opciones; j++)
            {
                printf("%s\n", pregunta->opciones[j]);
            }

            start_time=clock();

            respuesta='\n';
            do
            {
                if(kbhit())
                    respuesta=getchar();

                fflush(stdin);
                if(respuesta<101 && respuesta>96) ///abcd minuscula
                {
                    respuesta=toupper(respuesta); ///Paso a mayuscula
                    hay_resp=1;
                }
                else
                {
                    if(respuesta>64 && respuesta<69)
                        hay_resp=1;
                }

            }
            while(((clock()-start_time)/CLOCKS_PER_SEC)<tiempo_ronda && hay_resp==0);

            tiempo_respuesta=(clock()-start_time)/CLOCKS_PER_SEC;

            if(hay_resp==0)
            {
                printf("\nTiempo agotado. Siguiente pregunta...\n\n");
                respuesta='\n'; // Sin respuesta
                tiempo_respuesta=tiempo_ronda;
            }

            act->respuestas[r]=respuesta;
            act->tiempo[r]=tiempo_respuesta;
        }

        memcpy((*lista)->info, act, sizeof(jugador));
        sig_lista(lista);

        system("pause");
        system("cls");
    }
}

///

int calcular_puntaje(t_lista *lista, Pregunta preguntas[], int rondas, int n_participantes)
{
    int i, j, min, empate;
    jugador *act;
    char resp_part, resp_cor;

    act=malloc(sizeof(jugador));
    if(!act)
    {
        puts("No se pudo calcular los puntajes");
        return ERROR_DE_MEMORIA;
    }

    for(i=0; i<rondas; i++)
    {
        min=-1;
        empate=0;
        for(j=0; j<n_participantes; j++)
        {
            memcpy(act, (*lista)->info, sizeof(jugador));

            resp_part=act->respuestas[i];
            resp_cor=preguntas[i].respcorrecta;


            if(toupper(resp_part) == toupper(resp_cor))
            {
                if((act)->tiempo[i] == min)
                {
                    empate=1;
                }

                if(min==-1)
                {
                    min=act->tiempo[i];
                }

                if((act)->tiempo[i] < min)
                {
                    min=act->tiempo[i];
                    empate=0;
                }

            }

            sig_lista(lista);
        }

        for(j=0; j<n_participantes; j++)
        {
            memcpy(act, (*lista)->info, sizeof(jugador));

            resp_part=act->respuestas[i];
            resp_cor=preguntas[i].respcorrecta;

            if(toupper(resp_part) == toupper(resp_cor))
            {
                if((act)->tiempo[i] == min)
                {
                    if(empate==1)
                    {
                        act->puntos+=2;
                        act->puntos_por_pregunta[i]=2;
                    }
                    else
                    {
                        act->puntos+=3;
                        act->puntos_por_pregunta[i]=3;
                    }
                }
                else
                {
                    act->puntos+=1;
                    act->puntos_por_pregunta[i]=1;
                }
            }
            else
            {
                if(resp_part != '\n')
                {
                    act->puntos-=2;
                    act->puntos_por_pregunta[i]= -2;
                }
                else
                    act->puntos_por_pregunta[i]=0;
            }
            memcpy((*lista)->info, act, sizeof(jugador));
            sig_lista(lista);
        }
    }

    free(act);
    return TODO_OK;

}

///

int informe_arch_ganador(t_lista *lista, Pregunta preguntas[], int rondas, int n_participantes)
{
    int i, j, empate=0, max;
    char str[35];
    jugador *act;
    FILE *pf;
    time_t rawtime;
    struct tm *timeinfo;


    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(str, 35, "informe-juego_%Y-%m-%d-%H-%M.txt", timeinfo);

    pf=fopen(str,"wt");

    if(!pf)
    {
        puts("Error al crear un informe");
        return ERROR_INFORME;
    }


    for(i=0; i<rondas; i++)
    {
        fprintf(pf,"Pregunta: %s\nRespuesta correcta: %c\n\n",preguntas[i].pregunta, toupper(preguntas[i].respcorrecta));

        for(j=0; j<n_participantes; j++)
        {
            act=(*lista)->info;
            fprintf(pf,"Participante: %s\n\tRespuesta: %c\n\tPuntos: %d\n\tTiempo: %d\n\n"
                    ,act->nombre, act->respuestas[i], act->puntos_por_pregunta[i], act->tiempo[i]);
            sig_lista(lista);
        }

        fprintf(pf,"\n");
    }

    fprintf(pf,"\n\n\nPuntaje Total\n");

    for(i=0; i<n_participantes; i++)
    {
        act=(*lista)->info;
        fprintf(pf,"%s: %d Puntos\n",act->nombre, act->puntos);
        sig_lista(lista);
    }

    for(i=0; i<n_participantes; i++)
    {
        act=(*lista)->info;

        if(act->puntos == max && i!=0)
        {
            empate=1;
        }

        if(i == 0 || act->puntos > max)
        {
            max=act->puntos;
            empate=0;
        }

        sig_lista(lista);
    }

    fprintf(pf,"\n\n");

    if(max<=0)
    {
        printf("no hay ganadores\n\n");
        fprintf(pf,"no hay ganadores");
        fclose(pf);
        system("pause");
        system("cls");

        return TODO_OK;
    }
    if(empate == 0)
    {
        for(i=0; i<n_participantes; i++)
        {
            act=(*lista)->info;

            if(act->puntos == max)
            {
                printf("El/la ganador/a es: %s con %d puntos\n\n",act->nombre, max);
                fprintf(pf,"El/la ganador/a es: %s con %d puntos\n\n",act->nombre, max);
            }

            sig_lista(lista);
        }
    }
    else
    {
        printf("Los ganadores son: \n");
        fprintf(pf,"Los ganadores son: \n");

        for(i=0; i<n_participantes; i++)
        {
            act=(*lista)->info;

            if(act->puntos == max)
            {
                printf("%s con %d puntos\n\n",act->nombre, max);
                fprintf(pf,"%s con %d puntos\n\n",act->nombre, max);
            }

            sig_lista(lista);
        }
    }

    fclose(pf);

    system("pause");
    system("cls");

    return TODO_OK;
}
///

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char **readBuffer = (char **)userp;
    *readBuffer = realloc(*readBuffer, strlen(*readBuffer) + realsize + 1);   // Redimensionar el buffer para agregar los nuevos datos
    if (*readBuffer == NULL)
    {
        printf("Error al reasignar memoria\n");
        return 0;
    }
    strncat(*readBuffer, contents, realsize);// Copiar los nuevos datos al buffer
    return realsize;
}

///
/**implementar la api**/
int obtener_preguntas(int niveldif, Pregunta preguntas[], int *num_preguntas)
{
    int count = 0, level;
    CURL *curl;
    CURLcode res;
    char *readBuffer = malloc(1);
    if (readBuffer == NULL)
    {
        printf("Error al asignar memoria\n");
        return 0;
    }
    readBuffer[0] = '\0';  // Inicializar el buffer como cadena vacía
    curl_global_init(CURL_GLOBAL_DEFAULT);// Inicializar el manejo de curl
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, URL);// Establecer la URL de la solicitud GET
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);// Desactivar la verificación del certificado SSL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);// Establecer la función de retorno de llamada para manejar la respuesta
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);// Realizar la solicitud HTTP GET
        if (res != CURLE_OK)// Verificar si la solicitud fue exitosa
        {
            fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
            free(readBuffer);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 0;
        }
        cJSON *root = cJSON_Parse(readBuffer);// Parsear la respuesta JSON
        if (root == NULL)
        {
            fprintf(stderr, "Error al parsear el JSON: %s\n", cJSON_GetErrorPtr());
            free(readBuffer);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 0;
        }
        cJSON *question;// Asumiendo que root es un array de objetos JSON
        cJSON_ArrayForEach(question, root)
        {
            cJSON *nivel = cJSON_GetObjectItem(question, "nivel");
            if (nivel != NULL)
            {
                level = cJSON_GetNumberValue(nivel);
                if (level == niveldif && count < MAX_PREGUNTAS)// Filtrar las preguntas por nivel de dificultad deseado
                {
                    cJSON *text = cJSON_GetObjectItem(question, "pregunta");
                    if (text != NULL)
                    {
                        strcpy(preguntas[count].pregunta, cJSON_GetStringValue(text));
                        preguntas[count].num_opciones = MAX_OPCIONES; // Obtener opciones dinámicamente
                        strcpy(preguntas[count].opciones[0], cJSON_GetStringValue(cJSON_GetObjectItem(question, "opcion_1")));
                        strcpy(preguntas[count].opciones[1], cJSON_GetStringValue(cJSON_GetObjectItem(question, "opcion_2")));
                        strcpy(preguntas[count].opciones[2], cJSON_GetStringValue(cJSON_GetObjectItem(question, "opcion_3")));
                        strcpy(preguntas[count].opciones[3], cJSON_GetStringValue(cJSON_GetObjectItem(question, "opcion_4")));
                        preguntas[count].respcorrecta = *(cJSON_GetStringValue(cJSON_GetObjectItem(question, "resp_correcta")));

                        count++;
                    }
                }
            }
        }

        *num_preguntas = count;
        cJSON_Delete(root);// Liberar memoria
        free(readBuffer);
    }
    else
    {
        free(readBuffer);
        curl_global_cleanup();
        return 0;
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 1;
}
