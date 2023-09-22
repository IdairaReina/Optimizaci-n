/*
*  PG1.CPP - Actividad I de Optimiza!cion.
*
*
*               Autores : Antonio Sedeno Noda, Sergio Alonso.
*               Fecha : Marzo-2020
*/

#include <string.h>
#include "grafo.h"

void clrscr() //No funciona en todos los sistemas: opcional
{
	system("cls");
}


void menu(unsigned dirigido, char& opcion)
//Expresion del menu de opciones segun sea un grafo dirigido o no dirigido
{
	if (dirigido)
		cout << endl << "Opciones para grafo dirigido:" << endl << endl
		<< "c. Cargar grafo desde fichero" << endl
		<< "i. Informacion basica" << endl
		<< "s. Lista de sucesores" << endl
		<< "p. Lista de predecesores" << endl
		<< "d. Camino minimo: Dijkstra" << endl
		<< "D. Camino minimo: Dijkstra (detallado)" << endl
		<< "b. Camino minimo: Bellman-Ford-Moore" << endl
		<< "f. Matriz de caminos minimos: Floyd-Warshall" << endl
		<< "T. Toggle Mostrar tabla (en Floyd-Warshall)" << endl
		<< "t. Toggle Mostrar traza" << endl
		<< "q. Finalizar programa" << endl << endl
		<< ">> ";
	else
		cout << endl << "Opciones para grafo no dirigido:" << endl << endl
		<< "c. Cargar grafo desde fichero" << endl
		<< "i. Informacion basica" << endl
		<< "a. Lista de adjacentes" << endl
		<< "o. Componentes Conexas (dfs)" << endl
		<< "l. Componentes Conexas (bfs) [requiere clase Queue]" << endl
		<< "k. Mostrar el arbol generador de minimo coste (Kruskal)" << endl
		<< "j. Algoritmo de Kruskal para grafos no conexos (version detallada)" << endl
		<< "p. Mostrar el arbol generador de minimo coste (Prim)" << endl
		<< "t. Toggle Mostrar traza" << endl
		<< "q. Finalizar programa" << endl << endl
		<< ">> ";
	cin >> opcion;
}


// El principal es simplemente un gestor de menu, diferenciando acciones para grafo dirigido y para no dirigido
int main(int argc, char* argv[])
{
	int error_apertura;
	char nombrefichero[85], opcion;
	bool traza = true, tabla = false;

	clrscr();
	//Cargamos por defecto el fichero que se pasa como argumento del ejecutable
	if (argc > 1)
	{
		cout << "Cargando datos desde el fichero dado como argumento" << endl;
		strcpy(nombrefichero, argv[1]);
	}
	else
	{
		cout << "Introduce el nombre completo del fichero de datos" << endl;
		cin >> nombrefichero;
		clrscr();
	}
	GRAFO G(nombrefichero, error_apertura);
	if (error_apertura == 1)
	{
		cout << "Error en la apertura del fichero: revisa nombre y formato" << endl;
	}
	else
	{
		do
		{
			menu(G.Es_dirigido(), opcion);
			switch (opcion)
			{
				//Situar aquí el código de gestión de las opciones, según sea el grafo dirigido o no dirigido
			case 'c':
				clrscr();
				cout << "Introduce el nombre completo del fichero de datos" << endl;
				cin >> nombrefichero;
				G.actualizar(nombrefichero, error_apertura);
				clrscr();
				break;
			case 'i':
				clrscr();
				G.Info_Grafo();
				break;
			case 's':
				clrscr();
				if (G.Es_dirigido())
					G.Mostrar_Listas(1);
				else
					cout << "Comando invalido" << endl;
				break;
			case 'p':
				clrscr();
				if (G.Es_dirigido())
					G.Mostrar_Listas(-1);
				else
					G.Prim(traza);
				break;
			case 'd':
				clrscr();
				if (G.Es_dirigido())
				{
					G.Dijkstra();
				}
				else
					cout << "Comando invalido" << endl;
				break;
			case 'D':
				clrscr();
				if (G.Es_dirigido())
				{
					cout << "Introduce el nodo inicial [1-" << G.numero_nodos() << "]: ";
					unsigned ch;
					cin >> ch;
					if (ch > G.numero_nodos() || ch < 1)
					{
						cout << "Indice fuera del rango" << endl << endl;
						break;
					}
					G.Dijkstra2(ch - 1, ch - 1, traza);
				}
				else
					cout << "Comando invalido" << endl;
				break;
			case 'b':
				clrscr();
				if (G.Es_dirigido())
				{
					cout << "Introduce el nodo inicial [1-" << G.numero_nodos() << "]: ";
					unsigned ch;
					cin >> ch;
					if (ch > G.numero_nodos() || ch < 1)
					{
						cout << "Indice fuera del rango" << endl << endl;
						break;
					}
					G.Bellman_Ford_Moore(ch - 1, traza);
				}
				else
					cout << "Comando invalido" << endl;
				break;
			case 'f':
				clrscr();
				if (G.Es_dirigido())
				{
					G.Floyd_Marshall(traza, tabla);
				}
				else
					cout << "Comando invalido" << endl;
				break;
			case 'a':
				clrscr();
				if (!G.Es_dirigido())
					G.Mostrar_Listas(0);
				else
					cout << "Comando invalido" << endl;
				break;
			case 'o':
				clrscr();
				if (!G.Es_dirigido())
					G.ComponentesConexas();
				else
					cout << "Comando invalido" << endl;
				break;
			case 'l':
				clrscr();
				if (!G.Es_dirigido())
					G.ComponentesConexas(false);
				else
					cout << "Comando invalido" << endl;
				break;
			case 'k':
				clrscr();
				if (!G.Es_dirigido())
					G.Kruskal(traza);
				else
					cout << "Comando invalido" << endl;
				break;
			case 'j':
				clrscr();
				if (!G.Es_dirigido())
					G.Kruskal2(traza);
				else
					cout << "Comando invalido" << endl;
				break;
			case 'T':
				clrscr();
				if (G.Es_dirigido())
				{
					tabla = !tabla;
					cout << "Mostrar tabla (Floyd-Marshall): ";
					if (tabla)
						cout << "ON" << endl;
					else
						cout << "OFF" << endl;
				}
				else
					cout << "Comando invalido" << endl;

				break;
			case 't':
				clrscr();
				traza = !traza;
				cout << "Mostrar traza: ";
				if (traza)
					cout << "ON" << endl;
				else
					cout << "OFF" << endl;
				break;
			case 'q':
				clrscr();
				cout << "Saliendo del programa..." << endl << endl;
				break;
			default:
				clrscr();
				cout << "Comando no reconocido" << endl;
				break;
			}
		} while (opcion != 'q');
	};
	cout << "Fin del programa";
	return(0);
};
