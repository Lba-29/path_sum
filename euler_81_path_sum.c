#include <stdio.h>
#include <stdlib.h>

//Cuenta numero de filas del csv donde se almacena la matriz NxN
int numero_filas(char *archivo){
    FILE *f;
    int nodo=1;
    char c;

    f=fopen(archivo,"r");

    if(f==NULL){printf("Error al abrir archivo\n");return 0;}

    //c recorre el fichero y suma 1 por cada ; del .csv
    //nodo=1 y no en 0 porque el último no tiene ;
    while(c!='\n'){
        c=getc(f);
        if(c==';') nodo++;
        //printf("caracter = %c  nodo = %d\n",c,nodo);
    }
    fclose(f);

    return nodo;
}

//Función liberar espacio matriz
void free_matriz_int(int **matriz, int m) {
    int i;

    for (i=0; i<m; i++)
        free(matriz[i]);

    free(matriz);
}

//Funcion reservar memoria lo que ocupa la matriz de red nxn
int** malloc_matriz_int(int m, int n) {
    int i, j, **matriz;

    if ((matriz = (int**) malloc(m*sizeof(int*))) == NULL){
        printf("No fue bien la reserva de memoria");
        return NULL;
        }

    for (i=0; i<m; i++) {
        if ((matriz[i] = (int*) malloc(n*sizeof(int))) == NULL) {
            free_matriz_int(matriz, i);
            return NULL;
        }
    }

    return matriz;
}

//Funcion para coger valores csv y rellenar una matriz que representa la red
//IMPORTANTE: Para que esto funcione, el formato del csv es num1;num2;num3 suponiendo que es matriz 3x3. Debe haber 3 filas, no 4 con la última en blanco
void matriz_red(int **red, int N, char *archivo){
    int i=0,j,k=0,l=0;
    FILE *f;
    char c,dato[10];//Restricción: maximo de 10 cifras el dato del csv

    //printf("%s\n",archivo);
    f=fopen(archivo,"r");

    if(f==NULL){printf("Error al abrir archivo\n");return;}

    while(c!=EOF){
        c=getc(f);
        if((c==';')||(c=='\n')||(c==EOF)){
            //if(i==0){dato[0]='0';i++;}//Caso especial de encontrarte ;; entre medias hay 0
            //La he comentado porque parece que el programa funciona bien sin esto
            i=0;//Inicializo el dato
            red[k][l]=strtol(dato,NULL,10);//Convertir string->integer
            l++; //Avanzas una columna, nuevo dato
            //Vaciar la variable dato
            for(j=0;j<10;j++){dato[j]='\0';}
            if(c=='\n'){k++;l=0;}
        }
        else{dato[i]=c;i++;}
    }
    fclose(f);

    return;
}

//Simplificar triangulo en una fila con valores y rutas priorizando las más cortas
//Este algoritmo esta pulido en prueba.c del mismo fichero que path_sum_81.c
void caminos_base_triangulo(int N, int **down, int **pasos_ruta){
    int i,j,k,l;
    int **pasos_ruta_ant;
    //Crear la matriz pasos_ruta_ant como variable auxiliar, dimensiones de pasos_ruta
    pasos_ruta_ant = malloc_matriz_int(N,N);

    for(i=1;i<N;i++){

            //Volcar pasos_ruta_ant=pasos_ruta. Para no volcar todos los elementos pongo <i, pero vale igual si pones N-1
            //en su lugar
            for(k=0;k<i;k++){
                for(l=0;l<i;l++){
                    pasos_ruta_ant[k][l]=pasos_ruta[k][l];
                }
            }

            for(j=0;j<=i;j++){
                //Elem 1a columna coge camino arriba siempre
                if(j==0){
                    down[i][j]+=down[i-1][j];
                    pasos_ruta[0][i-1]=0;
                }
                //Elem ultima columna coge camino oblicuo siempre
                else if(j==i){
                    down[i][j]+=down[i-1][j-1];
                    for(k=0;k<i;k++){
                        pasos_ruta[j][k]=1;
                    }
                }
                else{
                    if(down[i-1][j]>down[i-1][j-1]){
                        down[i][j]+=down[i-1][j-1];
                        for(k=0;k<i-1;k++){
                            pasos_ruta[j][k]=pasos_ruta_ant[j-1][k];
                        }
                        pasos_ruta[j][i-1]=1;
                    }
                    else{
                        down[i][j]+=down[i-1][j];
                        for(k=0;k<(i-1);k++){
                            pasos_ruta[j][k]=pasos_ruta_ant[j][k];
                        }
                        pasos_ruta[j][i-1]=0;
                    }
                }
            }
    }

    free_matriz_int(pasos_ruta_ant,N);

    return;
}

//Obtención camino mas corto en un triangulo partiendo de la base hasta la punta
void camino_menor_triangulo(int N,int **M,int **caminos_posibles,int *ruta){
    int i,j;

    //Aplicar algoritmo
    for(i=N-1;i>0;i--){
        for(j=0;j<i;j++){
            if(M[i][j]<=M[i][j+1]){//Esta es la condicion para coger el camino MENOS VALOR
                M[i-1][j]+=M[i][j];
                caminos_posibles[i-1][j]=j;
            }
            else{
                M[i-1][j]+=M[i][j+1];
                caminos_posibles[i-1][j]=j+1;
            }
        }
    }

    //Recorremos la ruta y la guardamos en el array
    ruta[0]=caminos_posibles[0][0];

    for(i=1;i<N-1;i++){
        j=ruta[i-1];
        ruta[i]=caminos_posibles[i][j];
    }

    return;
}



int main(){

    char nombre_txt[8]="red.csv";//"red_prueba.csv"; //Fichero con la info del triangulo o la red
    int i,j;
    int N=0;//Numero de filas del triangulo
    int **M; //Esta es la matriz que contiene los triangulos o la red, como quieras verlo
    int **up, **down; //La matriz se dividirá en 2 triángulos que se tratan por separado
    int **caminos_up,**caminos_down; //Matriz con todos las posibles rutas que pueden ser la de mayor suma en cada triangulo
    //Cada elemento de esta matriz indica la columna j a la que tienes que moverte en la siguiente fila i
    int **ruta;
    int*ruta_up,*ruta_down;
    //ruta, ruta_up y ruta_down son arrays que guardan las ruta más optimas de los caminos posibles registrados

    //****************************************************
    //*******RESERVA MEMORIA + ASIGNACIÓN VARIABLES*******
    //****************************************************

    //Tamaño de la red para dimensionar con la funcion numero_filas + Mensaje de error y acaba programa si N=0
    if((N=numero_filas(nombre_txt))==0){
        printf("Archivo vacio\n");
        return 0;
    }

    //Reservo espacio memoria y defino matriz y vector en funcion numero de filas y lo leo
    if ((M = malloc_matriz_int(N,N)) == NULL) {
        printf("malloc_matriz_int() ha fallado, M.");
        return 1;
    }
    //Triangulo superior de la matriz M
    if ((up = malloc_matriz_int(N,N)) == NULL) {
        printf("malloc_matriz_int() ha fallado, up.");
        return 1;
    }
    //Triangulo inferior de la matriz M
    if ((down = malloc_matriz_int(N,N)) == NULL) {
        printf("malloc_matriz_int() ha fallado, down.");
        return 1;
    }
    if ((caminos_up= malloc_matriz_int(N-1,N-1)) == NULL) {
        printf("malloc_matriz_int() ha fallado, caminos_posibles.");
        return 1;
    }
    //posibles caminos partiendo de cuspide de up, desembocando en cada uno de los N elementos de la base de up
    //0 significa que no cambia de columna, mientras que 1 si cambia
    if ((caminos_down= malloc_matriz_int(N,N-1)) == NULL) {
        printf("malloc_matriz_int() ha fallado, caminos_posibles.");
        return 1;
    }
    //en ruta se almacenan indices de la matriz M de los elementos que recorre ruta optima
    if ((ruta= malloc_matriz_int(2*N-1,2)) == NULL) {
        printf("malloc_matriz_int() ha fallado, caminos_posibles.");
        return 1;
    }
    ruta_up = (int*)malloc((N-1)*sizeof(int)); //array para almacenar ruta en up
    ruta_down = (int*)malloc((N-1)*sizeof(int));//array para almacenar ruta en down

    matriz_red(M,N,nombre_txt);


    //GENERACIÓN TRIANGULOS
    for(i=0;i<N;i++){
        for(j=0;j<=i;j++){
            up[i][j]=M[i-j][j];
            down[i][j]=M[N-1-j][N-1-i+j];
        }
    }


    //**************************************
    //*******APLICACIÓN DEL ALGORITMO*******
    //CALCULO RUTAS MÁS CORTAS AMBOS TRIANGULOS
    //**************************************

    //Resolver uno de los triangulos de arriba hacia abajo
    caminos_base_triangulo(N,down,caminos_down);

    //Cambiar ultima fila de up por la ultima fila de down modificado
    for(i=0;i<N;i++){
        up[N-1][i]=down[N-1][i];
    }

    //Resolver el otro triangulo de abajo hacia arriba y obtener ruta optima
    camino_menor_triangulo(N,up,caminos_up,ruta_up);



    //**************************************
    //****TRADUCIR RESULTADO A LA MATRIZ****
    //**************************************

    //El ultimo elemento de la ruta de up escoge la ruta del triangulo en down
    for(i=0;i<N-1;i++){
        ruta_down[i]=caminos_down[ruta_up[N-2]][i];
    }

    //Se rellenan los elementos extremos del array ruta, las puntas de la matriz
    ruta[0][0]=0;ruta[0][1]=0;
    ruta[2*N-2][0]=N-1;ruta[2*N-2][1]=N-1;

    //Rellenar los primeros N elementos de ruta con los de ruta_up
    if(ruta_up[0]==0){
        ruta[1][0]=1;
        ruta[1][1]=0;
    }
    else{
        ruta[1][0]=0;
        ruta[1][1]=1;
    }
    for(i=2;i<N;i++){
        if(ruta_up[i-1]==ruta_up[i-2]){
            ruta[i][0]=ruta[i-1][0]+1;
            ruta[i][1]=ruta[i-1][1];
        }
        else{
            ruta[i][0]=ruta[i-1][0];
            ruta[i][1]=ruta[i-1][1]+1;
        }
    }


    //Rellenar los siguientes N-1 elementos de ruta
    for(i=2*N-3;i>=N;i--){
        if(ruta_down[2*N-3-i]==0){
            ruta[i][0]=ruta[i+1][0];
            ruta[i][1]=ruta[i+1][1]-1;
        }
        else{
            ruta[i][0]=ruta[i+1][0]-1;
            ruta[i][1]=ruta[i+1][1];
        }

    }

    //Resultado
    printf("Ruta completa (fil - col - valor) :\n");
    int suma=0;
    for(i=0;i<2*N-1;i++){
        printf("%d\t%d\t%d\n",ruta[i][0],ruta[i][1],M[ruta[i][0]][ruta[i][1]]);
        suma+=M[ruta[i][0]][ruta[i][1]];
    }
    printf("Suma = %d\n",suma);


    //Liberar matrices
    free_matriz_int(M,N);
    free_matriz_int(up,N);
    free_matriz_int(down,N);
    free_matriz_int(caminos_up,N-1);
    free_matriz_int(caminos_down,N-1);
    free(ruta);
    free(ruta_up);
    free(ruta_down);


    return 0;
}
