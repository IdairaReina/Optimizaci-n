/*
 *  GRAFO.h - definicion de la clase GRAFO
 *
 *  Autor : Antonio Sedeno Noda, Sergio Alonso
 *  Curso 2019-2020
 */

#ifndef _GRAPH_H
#define _GRAPH_H


#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;
const unsigned UERROR = 65000;
const int maxint = 1000000;

// definicion de un elemento de la lista de adyacencia

typedef struct
{
	unsigned j; // nodo
	int      c; // atributo para expresar el peso, la longitud, el coste, la inversion, etc..., esto es, lo que me implica que ese arco o arista forme parte de la solucion
} ElementoLista;

// definicion del tipo de una lista de adyacencia de un nodo

typedef vector<ElementoLista> LA_nodo;

/* El algoritmo de Kruskal no usa la adyacencia pesada para su ejecución, sino que precisa de una lista ordenada de aristas: la creamos*/
typedef struct {
	unsigned extremo1, extremo2;
	int peso;
} AristaPesada;

//El algoritmo de Prim requiere dos listas y un conjunto (lista de bool) Cada NodoPrim describe las caracteristicas en las listas de cada nodo
typedef struct {
	bool inside;
	unsigned predecesor;
	int distancia;
} NodoPrim;


class GRAFO
{
	unsigned            n;         /* numero de NODOS */
	unsigned            m;         /* numero de ARCOS */
	unsigned   			dirigido;  /* 0 si el grafo es no dirigido y 1 si es dirigido */
	vector<LA_nodo>     LS;        /* Lista de adyacencia de los sucesores: es una matriz dode cada fila posiblemente es posiblemente de distinto tamaño*/
	vector<LA_nodo>     LP;        /* Lista de adyacencia de los predecesores: es una matriz dode cada fila posiblemente es posiblemente de distinto tamaño*/

	unsigned build(char nombrefichero[85]);
	void destroy();

public:
	GRAFO(char nombrefichero[85], int& errorapertura);
	void actualizar(char nombrefichero[85], int& errorapertura);
	unsigned Es_dirigido();   /*devuelve 0 si el grafo es no dirigido y 1 si es dirigido*/
	unsigned numero_nodos();
	void Info_Grafo();
	void Mostrar_Listas(int l);
	void ListaPredecesores();
	void ComponentesConexas(bool ori = true); //
	void bfs(unsigned i, vector<bool>& visitado); //bfs requiere la clase adicional COLA
	void dfs(unsigned i, vector<bool>& visitado);
	void Dijkstra();
	void Dijkstra2(unsigned start, unsigned end, bool traza = true);
	void Bellman_Ford_Moore(unsigned start, bool traza = true);
	void Floyd_Marshall(bool traza = true, bool tabla=true);
	void Kruskal(bool traza=true);
	void Kruskal2(bool traza=false);
	void Prim(bool traza = false, unsigned start = 0);
	~GRAFO();
};

//Clase cola para bfs y Bellman-Ford-Moore
class Queue
{
public:
	Queue(unsigned size = 0);
	~Queue();
	void resize(unsigned size);

	unsigned get_first();
	unsigned get_last();
	unsigned extract_first();
	void add_last(unsigned);
	void add_first(unsigned);
	void remove_first();

	bool empty();
	bool IsInQueue(unsigned);
	bool full();

	unsigned get_max_size();
	unsigned get_size();

private:
	void Crawl_back();
	void Crawl_foward_1();

	vector<unsigned> data;
	unsigned first;
	unsigned last;
	unsigned max_size;
};

#endif