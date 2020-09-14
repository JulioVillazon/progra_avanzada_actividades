//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez

#include <stdio.h>
#include <stdlib.h>

#define N 10
#define M 3

typedef struct
{
  char * titulo;
  int paginas;
} Libro;


typedef void (*p_type)(void *);
typedef void (*t_recorre)(void *, size_t, size_t, p_type);

void recorre(t_recorre, void *, size_t, size_t, p_type);

void forward(void * vector, size_t count, size_t size, p_type print);
void reverse(void *vector, size_t count, size_t size, p_type print);
void bidirectional(void *vector, size_t count, size_t size, p_type print);

void *begin(void *);
void *end(void *, size_t, size_t);
void *next(void *, void*, size_t, size_t);
void *prev(void *, void*, size_t, size_t);

void printAllInt(int *, int);
void printAllFloat(float *, int);
void printAllStruc(Libro *, int);
void printInt(void *);
void printFloat(void *);
void printStruc(void *);


int main (int arc, const char * argv[]){

  int * vector = (int *) malloc(N * sizeof(int));

  int * aux = vector;
  int * last = N + vector;

  for(; aux < last; ++aux){
    *aux = rand() % 100;
  }

  float *vectorf = (float *) malloc(N * sizeof(float));

  float * auxf = vectorf;
  float * lastf = N + vectorf;

  for(; auxf < lastf; ++auxf){
    *auxf = rand() % 100;
  }

  Libro * lib;
  Libro * libAux;
  Libro * libFinal;

  lib = (Libro *) malloc(M * sizeof(Libro));
  libAux = lib;
  libFinal = lib + M;

  for (; libAux < libFinal; ++libAux)
  {
    libAux -> paginas = rand() % 100;
    libAux -> titulo = (char *) malloc(sizeof(char) * 20);
    libAux -> titulo = "Harry Potter";
  }
  
  printf("\nLista de libros: \n");
  printAllStruc(lib, M);

  printf("Bidirectional: \n");
  recorre(&bidirectional, lib, M, sizeof(*lib), &printStruc);

  printf("\nLista de enteros: \n");
  printAllInt(vector, N);  

  printf("Forward: \n");
  recorre(&forward, vector, N, sizeof(*vector), &printInt);

  printf("\nLista de flotantes: \n");
  printAllFloat(vectorf, N); 

  printf("Reverse: \n");
  recorre(&reverse, vectorf, N, sizeof(*vectorf), &printFloat);

  

  free(vector);
  free(vectorf);
  for (; libAux < (lib + M); ++libAux)
  {
    free(libAux -> titulo);
  }
  free(lib);

  return 0;
}

void recorre(t_recorre algorithm, void * vector, size_t count, size_t size, p_type print){
  (*algorithm)(vector, count, size, print);
}

void forward(void *vector, size_t count, size_t size, p_type print)
{      
  void * current = begin(vector);  
  
  do 
  {
    (*print)(current);
    current = next(vector, current, count, size);
  }while(current != NULL);

  printf("\n");
}

void reverse(void *vector, size_t count, size_t size, p_type print)
{
  void * current = end(vector, count, size);  
  
  do 
  {
    (*print)(current);
    current = prev(vector, current, count, size);
  }while(current != NULL);

  printf("\n");
}

void bidirectional(void *vector, size_t count, size_t size, p_type print)
{
  forward(vector, count, size, print);
  reverse(vector, count, size, print);
}

void *begin(void *vector){
  return vector;
}
void *end(void *vector, size_t count, size_t size){
  return vector + ((count * size) - size);
}
void *next(void *vector, void *current, size_t count, size_t size){
  void * next = current + size;
  void * final = end(vector, count, size);
  if(next > final) {
    return NULL;
  }
  return next;
}
void *prev(void *vector, void *current, size_t count, size_t size){
  void * prev = current - size;
  void * start = begin(vector);
  if(prev < start) {
    return NULL;
  }
  return prev;
}

void printInt(void * current) 
{
  printf("%3d ", *(int *)current);
}

void printFloat(void * current) 
{
  printf("%3f ", *(float *)current);
}

void printStruc(void * current) 
{
  Libro * aux = (Libro *)(current);
  printf("Titulo: %s, ", aux -> titulo);
  printf("Pagina: %d | ", aux -> paginas);
}

void printAllInt(int *vector, int count)
{
  int *aux = vector;
  int *last = vector + count;

  for (; aux < last; ++aux)
  {
    printf("%3d ", *aux);
  }

  printf("\n\n");
}

void printAllFloat(float *vector, int count)
{
  float *aux = vector;
  float *last = vector + count;

  for (; aux < last; ++aux)
  {
    printf("%3f ", *aux);
  }

  printf("\n\n");
}

void printAllStruc(Libro *vector, int count)
{
  Libro *aux = vector;
  Libro *last = vector + count;

  for (; aux < last; ++aux)
  {    
    printf("Titulo: %s, ", aux -> titulo);
    printf("Pagina: %d | ", aux -> paginas);
  }

  printf("\n\n");
}
