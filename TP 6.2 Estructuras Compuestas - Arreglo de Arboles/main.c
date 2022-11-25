#include <stdio.h>
#include <stdlib.h>

#define archivoAnimales "animales.dat"

typedef struct{
    char animal[30];
    int cant;
    int habitat;
    int idEspecie;
    char especie[20];
}registroArchivo;

typedef struct{
    char nombreAnimal[30];
    int cantidad;
    int habitat;
}animal;

typedef struct{
    animal dato;
    struct nodoArbol* izq;
    struct nodoArbol* der;
}nodoArbol;

typedef struct{
    int idEspecie;
    char especie[20];
}especie;

typedef struct{
    especie dato;
    nodoArbol* arbolDeAnimales;
}celdaEspecie;

nodoArbol* inicArbol(){
    return NULL;
}

nodoArbol* CrearNodoArbol(animal dato){
    nodoArbol* nodoNuevo = (nodoArbol*)malloc(sizeof(nodoArbol));
    nodoNuevo->dato = dato;
    nodoNuevo->izq = NULL;
    nodoNuevo->der = NULL;
    return nodoNuevo;
}

nodoArbol* insertar(nodoArbol* arbol, nodoArbol* hijoNuevo){
    if(arbol==NULL){
        arbol = hijoNuevo;
    }else{
        arbol->izq = insertar(arbol->izq,hijoNuevo);
    }
    return arbol;
}

void getHabitatChar(int n, char h[]){
    switch(n){
        case 0:
            strcpy(h,"Selva");
            break;
        case 1:
            strcpy(h,"Savana");
            break;
        case 2:
            strcpy(h,"Bosque");
            break;
        case 3:
            strcpy(h,"Mar");
            break;
        default:
            strcpy(h,"Error");
    }
}

void mostrarAnimal(animal animal){
    char habitat[10];
    getHabitatChar(animal.habitat,habitat);
    printf(" ANIMAL: %s\n",animal.nombreAnimal);
    printf(" CANTIDAD: %i\n",animal.cantidad);
    printf(" HABITAT: %i\n",animal.habitat);
    printf("---------------------------------------------\n");

}

void mostrarNodo(nodoArbol* arbol){
    mostrarAnimal(arbol->dato);
}

void inorder(nodoArbol* arbol){
    if(arbol!=NULL){
        inorder(arbol->izq);
        mostrarNodo(arbol);
        inorder(arbol->der);
    }
}

int sumarCantNodosArbol(nodoArbol* arbol){
    return (arbol)?1+sumarCantNodosArbol(arbol->izq)+sumarCantNodosArbol(arbol->der):0;
}

int BuscarPosicionEspecie(celdaEspecie ada[], especie dato, int validos){
    int rta=-1;
    int i=0;
    while((i<validos)&&(rta==-1)){
        if(strcmpi(ada[i].dato.especie, dato.especie)==0){
            rta=i;
        }
        i++;
    }
    return rta;
}

int agregar(celdaEspecie ada[], especie dato, int validos){
    ada[validos].dato = dato;
    ada[validos].arbolDeAnimales = inicArbol();
    validos++;
    return validos;
}

int alta(celdaEspecie ada[], especie dato, animal animal, int validos){
    nodoArbol* animalNuevo = CrearNodoArbol(animal);
    int pos = BuscarPosicionEspecie(ada,dato,validos);
    if(pos==-1){
        validos=agregar(ada,dato,validos);
        pos=validos-1;
    }
    ada[pos].arbolDeAnimales = insertar(ada[pos].arbolDeAnimales,animalNuevo);
    return validos;
}

int pasarDeArchivoToADA(celdaEspecie ada[], int validos, int dimension){
    FILE* fAnimales = fopen(archivoAnimales,"rb");
    if(fAnimales!=NULL){
        registroArchivo registro;
        especie especie;
        animal animal;
        while((fread(&registro,sizeof(registroArchivo),1,fAnimales)>0)&&(validos<=dimension)){
            especie.idEspecie = registro.idEspecie;
            strcpy(especie.especie,registro.especie);
            animal.cantidad = registro.cant;
            strcpy(animal.nombreAnimal,registro.animal);
            animal.habitat = registro.habitat;
            validos = alta(ada,especie,animal,validos);
        }
        fclose(fAnimales);
    }
    return validos;
}

void mostrarADA(celdaEspecie ada[], int validos){
    printf(" ARREGLO DE ESPECIES\n");
    int i=0;
    while(i<validos){
        printf(" ID ESPECIE: %i\n", ada[i].dato.idEspecie);
        printf(" ARBOL DE ANIMALES: %6d\n", sumarCantNodosArbol(ada[i].arbolDeAnimales));
        printf(" CANTIDAD DE ANIMALES: %6d\n", sumarCantNodosArbol(ada[i].arbolDeAnimales));
        printf("--------------------------------------------------------------\n");
        inorder(ada[i].arbolDeAnimales);
        i++;
    }
}


void mostrarElAnimal(registroArchivo animal){
    printf(" ANIMAL: %s\n", animal.animal);
    printf(" CANTIDAD: %i\n", animal.cant);
    printf(" HABITAT: %i\n", animal.habitat);
    printf(" ID ESPECIE: %i\n", animal.idEspecie);
    printf(" ESPECIE: %s\n", animal.especie);
    printf(" -------------------------------------\n");
}

void mostrarArchivoAnimales(){
    FILE* fAnimales = fopen(archivoAnimales,"rb");
    if(fAnimales!=NULL){
        registroArchivo animal;
        while(fread(&animal,sizeof(registroArchivo),1,fAnimales)>0){
            mostrarElAnimal(animal);
        }
    }
}

void guardarArbol(FILE* file, nodoArbol* arbol){
    if(arbol!=NULL){
        guardarArbol(file,arbol->izq);
        fwrite(&arbol->dato,sizeof(animal),1,file);
        guardarArbol(file,arbol->der);
    }
}

void pasarDeADAToArchivosAnimales(celdaEspecie ada[], int validos){
    char especie[20];
    for(int i=0;i<validos;i++){
        especie[0]='\0';
        strcat(especie,ada[i].dato.especie);
        strcat(especie,".dat");

        FILE* fEspecie = fopen(especie,"wb");
        if(fEspecie!=NULL){
            guardarArbol(fEspecie,ada[i].arbolDeAnimales);
            fclose(fEspecie);
        }
    }
}

void mostrarUnAnimal(char nombreArchivo[]){
    animal animal;
    FILE* fRegistro = fopen(nombreArchivo,"rb");
    if(fRegistro!=NULL){
        while(fread(&animal,sizeof(animal),1,fRegistro)>0){
            mostrarAnimal(animal);
        }
    }
}


int main()
{
    srand(time(NULL));
    celdaEspecie ada[4];
    int validos = 0;
    int dim = 4;
    printf(" ====================================================\n");
    printf(" ARCHIVO ANIMALES\n");
    mostrarArchivoAnimales();

    printf(" ====================================================\n");
    printf(" DE ARCHIVO A ADA\n");
    validos = pasarDeArchivoToADA(ada,validos,dim);

    printf(" ====================================================\n");
    printf(" ADA\n");
    mostrarADA(ada,validos);

    printf(" ====================================================\n");
    printf(" ADA A ARCHIVO POR ESPECIE\n");
    pasarDeADAToArchivosAnimales(ada,validos);

    printf(" ====================================================\n");
    printf(" MAMIFEROS\n");
    mostrarUnAnimal("Mamiferos.dat");

    printf(" ====================================================\n");
    printf(" AVES\n");
    mostrarUnAnimal("Aves.dat");

    printf(" ====================================================\n");
    printf(" REPTILES\n");
    mostrarUnAnimal("Reptiles.dat");

    printf(" ====================================================\n");
    printf(" PECES\n");
    mostrarUnAnimal("Peces.dat");

    return 0;
}
