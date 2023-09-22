/*
*  GRAFO.CPP - Implementaci�n de la clase GRAFOS
*
*
*  Autor : Antonio Sedeno Noda, Sergio Alonso
*  Fecha : 2013-2020
*/

#include "grafo.h"

unsigned GRAFO::build(char nombrefichero[85])
{
	ifstream file(nombrefichero);
	if (file.is_open())
	{
		destroy();
		ElementoLista temp;
		int p, j, c;
		file >> (unsigned&)n >> (unsigned&)m >> (unsigned&)dirigido;
		LS.resize(n);

		for (unsigned i = 0; i < m; i++)
		{
			file >> (unsigned&)p >> (unsigned&)j >> (int&)c;
			p--;
			j--;
			temp.j = j;
			temp.c = c;
			LS[p].push_back(temp);

			if (!dirigido && p != j)
			{
				temp.j = p;
				LS[j].push_back(temp);
			}
		}

		file.close();

		if (dirigido)
			ListaPredecesores();
		return 0;
	}
	return 1;
}

void GRAFO::destroy()
{
	m = 0;
	n = 0;
	dirigido = 0;
	for (unsigned i = 0; i < n; i++)
	{
		LS[i].clear();
		if (Es_dirigido())
			LP[i].clear();
	}
	LS.clear();
	if (Es_dirigido())
		LP.clear();
}

GRAFO::~GRAFO()
{
	destroy();
}

void GRAFO::actualizar(char nombrefichero[85], int& errorapertura)
{
	errorapertura = build(nombrefichero);
	if (errorapertura)
		cout << "No se ha encontrado el archivo" << endl;
}

void GRAFO::Info_Grafo()
{
	cout << endl;
	if (dirigido == 1)
	{
		cout << "Grafo dirigido ";
	}
	else
	{
		cout << "Grafo no dirigido ";
	};
	cout << " | nodos " << n << " | ";
	if (dirigido == 1)
	{
		cout << " arcos ";
	}
	else
	{
		cout << "aristas ";
	};
	cout << m << " " << endl;
}

unsigned GRAFO::Es_dirigido()
{
	return dirigido;
}

unsigned GRAFO::numero_nodos()
{
	return n;
}

void Mostrar_Lista(vector<LA_nodo> L)
{
	for (unsigned i = 0; i < L.size(); i++)
	{
		cout << "[" << i + 1 << "]: {";
		for (unsigned j = 0; j < L[i].size(); j++)
		{
			if (j != 0)
				cout << ", ";
			cout << L[i][j].j + 1 << " (" << L[i][j].c << ")";
		}
		cout << "}" << endl;
	}
}

void GRAFO::Mostrar_Listas(int l)
{
	if (l == 0 || l == 1)
		Mostrar_Lista(LS);
	else if (l == -1)
		Mostrar_Lista(LP);
}

void GRAFO::dfs(unsigned i, vector<bool>& visitado)
{
	visitado[i] = true;
	cout << i + 1;
	for (unsigned j = 0; j < LS[i].size(); j++)
		if (visitado[LS[i][j].j] == false)
		{
			cout << ", ";
			dfs(LS[i][j].j, visitado);
		}
}

//M�todo de recorrido en amplitud (No funcional sin clase COLA)
void GRAFO::bfs(unsigned i, vector<bool>& visitado)
{
	//Desde la versi�n entregada en las actividades del tema 6, se ha corregido:
	//Un problema seg�n el cual, si cada nodo adyacente al inicial ten�a adyacencias exclusivas, la distancia aumentar�a por error por cada nodo de primera generaci�n examinado
	Queue cola(n);
	unsigned d = 0, lastmember = 0;
	visitado[i] = true;
	cout << i + 1 << "(" << d << ")";
	cola.add_last(i);
	d++;
	while (!cola.empty())
	{
		for (unsigned j = 0; j < LS[cola.get_first()].size(); j++)
			if (!visitado[LS[cola.get_first()][j].j])
			{
				cout << ", " << LS[cola.get_first()][j].j + 1 << "(" << d << ")";
				visitado[LS[cola.get_first()][j].j] = true;
				cola.add_last(LS[cola.get_first()][j].j);
			}
		if (cola.get_first() == lastmember)
		{
			lastmember = cola.get_last();
			d++;
		}
		cola.remove_first();
	}
	cola.~Queue();
}/**/

void GRAFO::ComponentesConexas(bool ori)
{
	vector<bool> visitado(n);
	int num = 0;

	for (unsigned i = 0; i < n; i++)
		visitado[i] = false;

	for (unsigned i = 0; i < n; i++)
	{
		if (!visitado[i])
		{
			cout << "Componente conexa: {";
			ori ? dfs(i, visitado) : bfs(i, visitado);/**/

			cout << "}" << endl;
			num++;
		}
	}
	visitado.clear();
	cout << endl << "Numero de componentes conexos: " << num << endl;
}

//Algoritmo de Kruskal (simple)
void GRAFO::Kruskal(bool traza)
{
	vector<AristaPesada> Aristas(m);

	for (unsigned i = 0, j = 0; i < n; i++)
		for (unsigned k = 0; k < LS[i].size(); k++)
			if (i < LS[i][k].j)
			{
				Aristas[j].extremo1 = i;
				Aristas[j].extremo2 = LS[i][k].j;
				Aristas[j++].peso = LS[i][k].c;
			}

	unsigned a = 0, head = 0;
	int pesoMST = 0;
	AristaPesada dummy;

	vector<unsigned> Raiz(n);
	for (unsigned i = 0; i < n; i++)
		Raiz[i] = i;

	do
	{
		unsigned Candidata = head;
		for (unsigned i = head + 1; i < m; i++)
			if (Aristas[i].peso < Aristas[Candidata].peso)
				Candidata = i;
		if (Candidata != head)
		{
			dummy = Aristas[Candidata];
			Aristas[Candidata] = Aristas[head];
			Aristas[head] = dummy;
		}
		if (Raiz[Aristas[head].extremo1] != Raiz[Aristas[head].extremo2])
		{
			a++;
			if (traza)
				cout << "Arista numero " << a << " incorporada (" << Aristas[head].extremo1 + 1 << ", " << Aristas[head].extremo2 + 1 << "), con peso " << Aristas[head].peso << endl;
			unsigned kill = Raiz[Aristas[head].extremo2];
			for (unsigned i = 0; i < n; i++)
				if (Raiz[i] == kill)
					Raiz[i] = Raiz[Aristas[head].extremo1];
			pesoMST += Aristas[head].peso;
		}
		head++;

	} while (a < (n - 1) && head < m);

	if (a == (n - 1)) {
		cout << "El peso del arbol generador de minimo coste es " << pesoMST << endl;
	}
	else {
		cout << "El grafo no es conexo, y el bosque generador de minimo coste tiene peso " << pesoMST << endl;
	}
}






















//Funci�n alternativa de Kruskal para dar detalles de componentes separadas en grafos no conexos
void GRAFO::Kruskal2(bool traza)
{
	vector<AristaPesada> Aristas(m);
	AristaPesada dummy;
	vector<bool> visitado(2 * m + 1);
	unsigned vis = 0;

	for (unsigned i = 0; i < 2 * m; i++)
		visitado[i] = false;

	dummy.extremo1 = 0;
	dummy.extremo2 = 0;
	dummy.peso = 0;

	//Algoritmo muy poco �ptimo para extraer la lista de aristas pesadas de manera ordenada en un principio
	for (unsigned i = 0; i < m; i++)
	{
		visitado[2 * m] = false;
		for (unsigned j = 0, v = 0; j < n; j++)
			for (unsigned k = 0; k < LS[j].size(); k++, v++)
				if (LS[j][k].j > j && !visitado[v] && (!visitado[2 * m] || LS[j][k].c < dummy.peso))
				{
					dummy.extremo1 = j;
					dummy.extremo2 = LS[j][k].j;
					dummy.peso = LS[j][k].c;
					vis = v;
					visitado[2 * m] = true;
				}
		visitado[vis] = true;
		Aristas[i] = dummy;
	}

	//Display de las aristas ordenadas
	/*for (unsigned i = 0; i < m; i++)
		cout << "Aristas[" << i << "]= " << Aristas[i].extremo1 << " -> " << Aristas[i].extremo2 << " (" << Aristas[i].peso << ")" << endl;
	/**/

	unsigned a = 0, head = 0;
	int pesoMST = 0;

	vector<unsigned> Raiz(n);
	vector<int> Costes(n);
	for (unsigned i = 0; i < n; i++)
	{
		Raiz[i] = i;
		Costes[i] = 0;
	}
	if (traza)
	{
		cout << "-La Raiz (Componentes Conexas) actual es [" << Raiz[0] + 1;
		for (unsigned i = 1; i < n; i++)
			cout << ", " << Raiz[i] + 1;
		cout << "]." << endl << endl;
	}
	do
	{
		if (traza)
			cout << "-La arista de peso minimo no considerada anteriormente es ({" << Aristas[head].extremo1 + 1 << "} - {" << Aristas[head].extremo2 + 1 << "}, (" << Aristas[head].peso << "))." << endl;
		if (Raiz[Aristas[head].extremo1] != Raiz[Aristas[head].extremo2])
		{
			if (traza)
				cout << "-Los nodos {" << Aristas[head].extremo1 + 1 << "} y {" << Aristas[head].extremo2 + 1 << "} pertenecen a componentes diferentes ([" << Raiz[Aristas[head].extremo1] + 1 << "] y [" << Raiz[Aristas[head].extremo2] + 1 << "], la arista candidata se aprueba." << endl;
			a++;
			Costes[Raiz[Aristas[head].extremo1]] += Costes[Raiz[Aristas[head].extremo2]];
			Costes[Raiz[Aristas[head].extremo2]] = 0;
			Costes[Raiz[Aristas[head].extremo1]] += Aristas[head].peso;
			unsigned kill = Raiz[Aristas[head].extremo2];
			for (unsigned i = 0; i < n; i++)
				if (Raiz[i] == kill)
					Raiz[i] = Raiz[Aristas[head].extremo1];
			pesoMST += Aristas[head].peso;
			if (traza)
			{
				cout << "-El coste total ahora es (" << pesoMST << "). El numero de aristas es (" << a << ")." << endl;
				cout << "-La Raiz actual es [" << Raiz[0] + 1;
				for (unsigned i = 1; i < n; i++)
					cout << ", " << Raiz[i] + 1;
				cout << "]." << endl << endl;
			}
		}
		else if (traza)
			cout << "-Los nodos {" << Aristas[head].extremo1 + 1 << "} y {" << Aristas[head].extremo2 + 1 << "} pertenecen a la misma componente conexa [" << Raiz[Aristas[head].extremo1] + 1 << "], la arista candidata se rechaza." << endl << endl;
		head++;
	} while (a < (n - 1) && head < m);

	if (traza)
		if (a == (n - 1))
			cout << "-Hay " << a << " aristas conectadas (" << n << "-1), todos los nodos estan en la misma compomente: el algoritmo termina con un arbol generador completo." << endl << endl;
		else
			cout << "-Todas las aristas han sido comprobadas, hay " << a << " aristas conectadas de " << n - 1 << " necesarias para completar el arbol generador. El grafo es no conexo. El algoritmo termina con un bosque de arboles generadores." << endl << endl;

	if (a == (n - 1)) {
		cout << "El peso del arbol generador de minimo coste es " << pesoMST << endl;
	}
	else {
		cout << "El grafo no es conexo, el coste del arbol generador por componente es:" << endl;
		for (unsigned i = 0; i < n; i++)
		{
			if (i == Raiz[i])
			{
				cout << "Componente [" << Raiz[i] + 1 << "], comuesta por {" << Raiz[i] + 1;
				for (unsigned j = i + 1; j < n; j++)
					if (Raiz[j] == i)
						cout << ", " << j + 1;
				cout << "}, tiene peso: " << Costes[i] << endl;
			}
		}
		cout << "El coste total es " << pesoMST << endl;
	}
}

//Algoritmo de Prim
void GRAFO::Prim(bool traza, unsigned start)
{
	vector<NodoPrim> M(n);
	if (traza)
		cout << "-Se inicia el algoritmo. Se inicializa el vector de distancia con todos sus valores a (+INF) y el de predecesores a (0)" << endl << "-Se anade el nodo {" << start + 1 << "} al arbol(conjunto M) y se establece su distancia a(0) y su predecesor(" << start + 1 << "). " << endl << endl;;
	for (unsigned i = 0; i < n; i++)
	{
		M[i].inside = false;
		//2147483647 es el n�mero m�ximo de un int de 4 bytes 
		M[i].distancia = 2147483647;
		M[i].predecesor = 0;
	}
	M[start].inside = true;
	M[start].predecesor = start + 1;
	M[start].distancia = 0;

	unsigned a = 0, last = start;
	int pesoMST = 0, current = 0;

	do {
		if (traza)
		{
			cout << "-Se analizan los adyacentes de {" << last + 1 << "}: ";
			for (unsigned j = 0; j < LS[last].size(); j++)
			{
				cout << "{" << LS[last][j].j + 1 << "}";
				if (j != LS[last].size() - 1)
					if (j == LS[last].size() - 2)
						cout << " y ";
					else
						cout << ", ";
			}
			cout << endl;
		}
		current = start;
		for (unsigned j = 0; j < LS[last].size(); j++)
		{
			if (traza)
				if (M[LS[last][j].j].inside)
					cout << "-El nodo {" << LS[last][j].j + 1 << "} ya pertenece a M, no es mejorable." << endl;
				else if (M[LS[last][j].j].predecesor == 0)
					cout << "-El nodo {" << LS[last][j].j + 1 << "} no visitado establece su distancia a (" << LS[last][j].c << ") y predecesor a (" << last + 1 << "). " << endl;
				else if (LS[last][j].c < M[LS[last][j].j].distancia)
					cout << "-El nodo {" << LS[last][j].j + 1 << "} es mejorable (" << LS[last][j].c << "<" << M[LS[last][j].j].distancia << "): actualiza su distancia a (" << LS[last][j].c << ") y predecesor a (" << last + 1 << "). " << endl;
				else if (LS[last][j].c == M[LS[last][j].j].distancia)
					cout << "-El nodo {" << LS[last][j].j + 1 << "} es indiferente (" << LS[last][j].c << "==" << M[LS[last][j].j].distancia << "): Se mantiene igual. " << endl;
				else
					cout << "-El nodo {" << LS[last][j].j + 1 << "} es no mejorable (" << LS[last][j].c << ">" << M[LS[last][j].j].distancia << "): Se mantiene igual. " << endl;
			if (!M[LS[last][j].j].inside && LS[last][j].c < M[LS[last][j].j].distancia)
			{
				M[LS[last][j].j].distancia = LS[last][j].c;
				M[LS[last][j].j].predecesor = last + 1;
			}
		}
		if (traza)
		{
			cout << "-Distancia ahora (";
			for (unsigned i = 0; i < n; i++)
			{
				if (M[i].distancia == 2147483647)
					cout << "INF";
				else
					cout << M[i].distancia;
				if (i != n - 1)
					cout << ", ";
			}
			cout << "). Predecesores ahora (";
			for (unsigned i = 0; i < n; i++)
			{
				cout << M[i].predecesor;
				if (i != n - 1)
					cout << ", ";
			}
			cout << ")." << endl;
		}
		for (unsigned i = 0; i < n; i++)
			if (!M[i].inside && M[i].predecesor != 0)
			{
				if (current == start)
				{
					a++;
					current = i;
				}
				if (M[current].distancia > M[i].distancia)
					current = i;
			}
		//Bucle para imprimir las listas de predecesor, distancia y si est�n en M
		/*for (unsigned i = 0; i < n; i++)
			cout << "Elemento " << i +1 << ": Predecesor " << M[i].predecesor << "; dentro? " << (M[i].inside?1:0) << "; Distancia " << M[i].distancia << endl;
		/**/
		M[current].inside = true;
		pesoMST += M[current].distancia;
		last = current;
		if (current != start && traza)
		{
			cout << "-La menor adyacencia con un nuevo nodo es ({" << M[current].predecesor << "} - {" << current + 1 << "}, (" << M[current].distancia << ")): Se anade el nodo {" << current + 1 << "} a M. M ahora es {";
			for (unsigned i = 0, t = 0; i < n && t != a + 1; i++)
				if (M[i].inside && t != a + 1)
				{
					cout << i + 1;
					if (i != n - 1)
						cout << ", ";
					t++;
				}
			cout << "}. El numero de aristas es (" << a << ").  " << endl << endl;
		}

	} while (a < (n - 1) && start != current);

	if (a == (n - 1)) {
		if (traza)
			cout << "-El numero de aristas ha alcanzado (" << n << "-1=" << n - 1 << "). M contiene todos los nodos, el algoritmo termina habiendo completado el arbol generador." << endl << endl;
		cout << "El peso del arbol generador de minimo coste es " << pesoMST << endl;
	}
	else {
		if (traza)
			cout << "-El numero de aristas es inferior a (" << n << "-1=" << n - 1 << "). El algoritmo termina habiendo completado el arbol generador del subgrafo conexo que contiene el nodo " << start + 1 << "." << endl << endl;
		cout << "El grafo no es conexo, y el arbol generador de minimo coste de la componente que contiene el nodo " << start + 1 << " tiene peso " << pesoMST << endl;
	}
}

void MostrarCamino(unsigned s, unsigned i, vector<unsigned> pred)
{

	if (i != s)
	{
		MostrarCamino(s, pred[i], pred);
		cout << " -> " << i + 1;
	}
	else
	{
		cout << s + 1;
	}
}

void GRAFO::Dijkstra()
{
	vector<bool> PermanentementeEtiquetado;
	vector<int> d;
	vector<unsigned> pred;
	bool all;
	unsigned s, candidato;

	//Inicialmente no hay ningun nodo permanentemente etiquetado 
	PermanentementeEtiquetado.resize(n, false);
	//Inicialmente todas las etiquetas distancias son infinito 
	d.resize(n, maxint);
	//Inicialmente el pred es null 
	pred.resize(n, UERROR);

	//Solicitamos al usuario nodo origen 
	cout << endl;
	cout << "Caminos minimos: Dijkstra" << endl;
	cout << "Nodo de partida? [1-" << n << "]: ";
	cin >> (unsigned&)s;
	if (s<1 || s>n)
	{
		cout << "El nodo de inicio esta fuera del rango" << endl << endl;
		return;
	}
	//La etiqueta distancia del nodo origen es 0, y es su propio pred 
	d[--s] = 0;
	pred[s] = s;
	candidato = s;
	if (LS[s].size() == 0)
	{
		all = false;
		PermanentementeEtiquetado.resize(n, true);
	}
	do
	{
		all = false;
		//Buscamos un nodo candidato a ser permanentemente etiquedado: aquel de entre los no permanentemente etiquetados con menor etiqueta distancia no infinita. 
		for (unsigned i = 0; i < n; i++)
			if (!PermanentementeEtiquetado[i])
			{
				if (PermanentementeEtiquetado[candidato])
					candidato = i;
				else
					all = true; // Detecta si el candidato no es el �ltimo en salir
				if (d[i] < d[candidato])
					candidato = i;
			}
		//Si existe ese candidato, lo etiquetamos permanentemente y usamos los arcos de la lista de sucesores para buscar atajos. 
		PermanentementeEtiquetado[candidato] = true;
		for (unsigned i = 0; i < LS[candidato].size(); i++)
		{
			//Se comprueba que el candidato tenga predecesor, ya que si le sumas al valor m�s alto de int, se pasan a negativo, es decir, se acepta su "mejora" y evitan que se detecte como inf.
			if (/*pred[candidato] != UERROR &&/**/ !PermanentementeEtiquetado[LS[candidato][i].j] && (d[candidato] + LS[candidato][i].c) < d[LS[candidato][i].j])
			{
				pred[LS[candidato][i].j] = candidato;
				d[LS[candidato][i].j] = d[candidato] + LS[candidato][i].c;
			}
		}
		//Esto lo hacemos mientras haya candidatos
	} while (all);

	cout << "Soluciones:" << endl;
	//En esta parte del c�digo, mostramos los caminos m�nimos, si los hay 
	for (unsigned i = 0; i < n; i++)
		if (i != s)
			if (pred[i] == UERROR)
				cout << "No hay camino desde " << s + 1 << " al nodo " << i + 1 << endl;
			else
			{
				cout << "El camino desde " << s + 1 << " al nodo " << i + 1 << " es: ";
				MostrarCamino(s, i, pred);
				cout << " de longitud " << d[i] << endl;
			}
}

//M�todo para saber si un nodo forma parte de un circuito (devuelve true si s pertenece a un circuito y false si no hay circuitos o s no pertenece a uno
bool IsThereACycle(unsigned s, vector<unsigned> pred)
{
	unsigned n = pred[s] - 1;
	unsigned siz = pred.size() - 1;
	while (n != pred[n] - 1 && siz!=0)
	{
		if (pred[n] - 1 == s)
		{
			return true;
		}
		n = pred[n] - 1;
		siz--;
	}
	return false;
}

//M�todo para todos los nodos pertenecientes al circuito de un nodo (Solo funciona si hay circuitos con ese nodo)
vector<bool> GetCycle(unsigned s, vector<unsigned> pred)
{
	vector<bool> a(pred.size(), false);
	a[s]=IsThereACycle(s, pred);
	if (a[s])
	{
		unsigned n = pred[s] - 1;
		do
		{
			a[n] = true;
			n = pred[n] - 1;
		} while (n != s);
	}
	return a;
}

void ShowCycle(unsigned s, vector<unsigned> pred)
{
	cout << "[" << s+1;
	unsigned t = pred[s] - 1;
	while (t !=s)
	{
		cout << ", " << t + 1;
		t = pred[t] - 1;
	}
	cout << "]";
}

//Versi�n de mostrar camino donde la lista de predecesores usa �ndices de 1 a n.
void MostrarCamino2(unsigned s, unsigned i, vector<unsigned> pred)
{
	if (IsThereACycle(i, pred))
	{
		cout << s + 1 << " -> ";
		ShowCycle(i, pred);
	}
	else if (i != s)
	{
		MostrarCamino2(s, pred[i] - 1, pred);
		cout << " -> " << i + 1;
	}
	else
	{
		cout << s + 1;
	}
}

//Versi�n personal de Dijkstra
void GRAFO::Dijkstra2(unsigned start, unsigned end, bool traza)
{
	//Inicializaci�n
	vector<bool> A(n, true);
	//2147483647 es el n�mero m�ximo de un int de 4 bytes 
	vector<int> d(n, 2147483647);
	vector<unsigned> pred(n, 0);

	if (traza)
		cout << "-Empieza el algoritmo desde el nodo {" << start + 1 << "}, se inicializa el vector de distancias a +INF y el de predecesores a 0. El almacen contiene todos los nodos." << endl << "El nodo inicial {" << start + 1 << "} cambia su distancia a 0 y su predecesor a " << start + 1 << endl << endl;
	pred[start] = start + 1;
	d[start] = 0;

	unsigned remaining = n;
	unsigned out = start;

	while (remaining > 0 && (end == start || !A[end]))
	{
		// Se localiza el nodo con menor distancia
		for (unsigned i = 0; i < n; i++)
			if (!A[out] || (A[i] && d[i] < d[out]))
				out = i;
		remaining--;
		if (traza)
		{
			cout << "Iteracion " << n - remaining << ":" << endl << " -El nodo {" << out + 1 << "} sale del almacen (";
			for (unsigned i = 0; i < n; i++)
				if (A[i] && i != out)
				{
					if (!A[out])
						cout << ", ";
					else
						A[out] = false;
					cout << "{" << i << "}";
				}
			cout << "), sus etiquetas son permanentes (d:" << d[out] << ", pred:" << pred[out] + 1 << ")." << endl << " -Se actualizan los sucesores de {" << out + 1 << "}." << endl;
		}
		A[out] = false;
		for (unsigned i = 0; i < LS[out].size(); i++)
		{
			if (traza)
			{
				cout << " -El nodo {" << LS[out][i].j + 1 << "} ";
				if (A[LS[out][i].j])
				{
					cout << "esta en el almacen y tiene d(";
					if (pred[out] == 0)
						cout << "INF) = (INF). Permanece igual." << endl;
					else if (pred[LS[out][i].j] == 0)
						cout << "INF) > (" << d[out] << " + " << LS[out][i].c << "). Sus etiquetas se actualizan." << endl;
					else if ((d[out] + LS[out][i].c) < d[LS[out][i].j])
						cout << d[LS[out][i].j] << ") > (" << d[out] << " + " << LS[out][i].c << "). Sus etiquetas se actualizan." << endl;
					else if ((d[out] + LS[out][i].c) == d[LS[out][i].j])
						cout << d[LS[out][i].j] << ") = (" << d[out] << " + " << LS[out][i].c << "). Permanece igual." << endl;
					else
						cout << d[LS[out][i].j] << ") < (" << d[out] << " + " << LS[out][i].c << "). Permanece igual." << endl;
				}
				else
					cout << "no esta en el almacen. Se omite." << endl;
			}
			if (pred[out] != 0 && A[LS[out][i].j] && (d[out] + LS[out][i].c) < d[LS[out][i].j])
			{
				pred[LS[out][i].j] = out + 1;
				d[LS[out][i].j] = d[out] + LS[out][i].c;
			}
		}
		if (traza)
		{
			cout << " -El vector de distancias es ahora (";
			if (pred[0] == 0)
				cout << "INF";
			else if (!A[0])
				cout << "[" << d[0] << "]";
			else
				cout << d[0];
			for (unsigned i = 1; i < n; i++)
				if (pred[i] == 0)
					cout << ", INF";
				else if (!A[i])
					cout << ", [" << d[i] << "]";
				else
					cout << ", " << d[i];
			cout << ") y el de predecesores (";
			if (!A[0])
				cout << "[" << pred[0] << "]";
			else
				cout << pred[0];
			for (unsigned i = 1; i < n; i++)
				if (!A[i])
					cout << ", [" << pred[i] << "]";
				else
					cout << ", " << pred[i];
			cout << ")." << endl << endl;
		}
	}
	if (end != start)
		cout << "La distancia entre {" << start + 1 << "} y {" << end + 1 << "} es " << d[end] << endl;
	else
		for (unsigned i = 0; i < n; i++)
			if (i != start)
				if (pred[i] == 0)
					cout << "No hay camino desde " << start + 1 << " al nodo " << i + 1 << endl;
				else
				{
					cout << "El camino desde " << start + 1 << " al nodo " << i + 1 << " es: ";
					MostrarCamino2(start, i, pred);
					cout << " de longitud " << d[i] << endl;
				}
}

void GRAFO::Bellman_Ford_Moore(unsigned start, bool traza)
{
	vector<unsigned> pred(n, 0);
	vector<int> d(n, 2147483647);
	Queue cola(n);

	if (traza)
		cout << " -Empieza el algoritmo desde el nodo {" << start + 1 << "}, se inicializa el vector de distancias a +INF y el de predecesores a 0. La cola esta vacia." << endl << " -El nodo inicial {" << start + 1 << "} cambia su distancia a 0 y su predecesor a " << start + 1 << ". Entra en la cola." << endl << endl;
	d[start] = 0;
	pred[start] = start + 1;
	cola.add_last(start);
	unsigned next;
	unsigned iter = 1;

	while (!cola.empty())
	{
		next = cola.extract_first();
		if (traza)
			cout << "Iteracion " << iter++ << ":" << endl << " -Sale de la cola el nodo {" << next + 1 << "}, con distancia " << d[next] << " y predecesor " << pred[next] + 1 << ". Se analizan sus sucesores:" << endl;
		for (unsigned i = 0; i < LS[next].size(); i++)
		{
			if (traza)
			{
				cout << " -El nodo {" << LS[next][i].j + 1 << "} tiene distancia ";
				if (pred[next] == 0)
					cout << "INF = INF. Se omite." << endl;
				if (pred[LS[next][i].j] == 0)
					cout << "INF > (" << d[next] << " + " << LS[next][i].c << "). No ha estado en la cola, se anade al final y se actualizan sus etiquetas (d:" << d[next] + LS[next][i].c << ", pred:" << next + 1 << ")." << endl;
				else if (!cola.IsInQueue(LS[next][i].j) && d[LS[next][i].j] > d[next] + LS[next][i].c)
					cout << d[LS[next][i].j] << " > (" << d[next] << " + " << LS[next][i].c << "). No esta en la cola, se reintroduce al inicio y se actualizan sus etiquetas (d:" << d[next] + LS[next][i].c << ", pred:" << next + 1 << ")." << endl;
				else if (d[LS[next][i].j] > d[next] + LS[next][i].c)
					cout << d[LS[next][i].j] << " > (" << d[next] << " + " << LS[next][i].c << "). Esta en la cola, solo se actualizan sus etiquetas (d:" << d[next] + LS[next][i].c << ", pred:" << next + 1 << ")." << endl;
				else if (d[LS[next][i].j] < d[next] + LS[next][i].c)
					cout << d[LS[next][i].j] << " < (" << d[next] << " + " << LS[next][i].c << "). Se ignora" << endl;
				else
					cout << d[LS[next][i].j] << " = (" << d[next] << " + " << LS[next][i].c << "). Se ignora" << endl;
			}
			if (d[LS[next][i].j] > d[next] + LS[next][i].c)
			{
				if (pred[LS[next][i].j] == 0)
					cola.add_last(LS[next][i].j);
				else if (!cola.IsInQueue(LS[next][i].j))
					cola.add_first(LS[next][i].j);
				d[LS[next][i].j] = d[next] + LS[next][i].c;
				pred[LS[next][i].j] = next + 1;
			}
		}
		if (traza)
		{
			cout << " -El vector de distancias es ahora (" << d[0];
			for (unsigned i = 1; i < n; i++)
				if (pred[i] == 0)
					cout << ", INF";
				else
					cout << ", " << d[i];
			cout << ") y el de predecesores (" << pred[0];
			for (unsigned i = 1; i < n; i++)
				cout << ", " << pred[i];
			if (cola.empty())
				cout << "). La cola esta vacia, el algoritmo termina." << endl << endl;
			else
			{
				cout << "). Quedan " << cola.get_size() << " elementos en la cola. El siguiente elemento es " << cola.get_first() << endl << endl;
			}
		}
	}
	for (unsigned i = 0; i < n; i++)
		if (i != start)
			if (pred[i] == 0)
				cout << "No hay camino desde " << start + 1 << " al nodo " << i + 1 << endl;
			else
			{
				cout << "El camino desde " << start + 1 << " al nodo " << i + 1 << " es: ";
				MostrarCamino2(start, i, pred);
				cout << " de longitud " << d[i] << endl;
			}
}

void GRAFO::Floyd_Marshall(bool traza, bool tabla)
{
	vector<vector<int>> d(n);
	vector<vector<unsigned>> pred(n);
	vector<bool> neg(n, false);
	for (unsigned i = 0; i < n; i++)
	{
		d[i].resize(n);
		pred[i].resize(n);
		for (unsigned j = 0; j < n; j++)
		{
			if (i == j)
			{
				d[i][j] = 0;
				pred[i][j] = i + 1;
			}
			else
			{
				d[i][j] = 2147483647;
				pred[i][j] = 0;
			}
		}
		for (unsigned j = 0; j < LS[i].size(); j++)
		{
			d[i][LS[i][j].j] = LS[i][j].c;
			pred[i][LS[i][j].j] = i + 1;
		}
	}

	if (traza)
		cout << " -Empieza el algoritmo, se inicializa la matriz de distancias a +INF y la de predecesores a 0. Las celdas con indices de fila y columna coincidentes establecen su distancia a 0 y su predecesor a ese indice. Se anaden los sucesores inmediatos a cada nodo." << endl << endl;

	for (unsigned i = 0; i < n; i++)
	{
		if (tabla)
		{
			cout << "Tabla  " << i << " (d | P)"<< endl;

			for (unsigned j = 0; j < n; j++)
			{
				for (unsigned k = 0; k < n; k++)
					if (pred[j][k] == 0 && (k == i || j == i))
						cout << "\t[INF]";
					else if (pred[j][k] == 0)
						cout << "\tINF";
					else if ((k == i || j == i))
						cout << "\t[" << d[j][k] << "]";
					else
						cout << "\t" << d[j][k];
				cout << "\t|";
				for (unsigned k = 0; k < n; k++)
					if ((k == i || j == i))
						cout << "\t[" << pred[j][k] << "]";
					else
						cout << "\t" << pred[j][k];
				cout << endl;
			}
			cout << endl;
		}
		if (traza)
			cout << endl << " Iteracion " << i + 1 << ":" << endl; //<< endl;
		for (unsigned j = 0; j < n; j++)
		{
			if (traza && i != j)
			{
				if (pred[j][i] == 0)
					cout << endl << "  [" << j + 1 << "] -Se salta la fila " << j + 1 << ", ya que no se pueden mejorar los caminos de la fila desde uno con coste INF." << endl << endl;
				else
					cout << "  [" << j + 1 << "] -Se analizan los caminos mejorables de la fila a partir del camino de {" << j + 1 << "} a {" << i + 1 << "} (" << d[j][i] << ")." << endl;
			}
			if (i != j && pred[j][i] != 0)
				for (unsigned k = 0; k < n; k++)
				{
					if (traza && i != k)
					{
						cout << "\t -Analisis del camino de {" << k + 1 << "} a {" << j + 1 << "}. Propuesta d: ";
						if (pred[i][k] == 0)
							cout << "INF, pred: " << pred[i][k] << endl << "\tSe salta la celda (" << j + 1 << ", " << k + 1 << ") ya que no se puede mejorar un camino a partir de un coste INF." << endl;
						else if (pred[j][k] == 0)
							cout << d[j][i] + d[i][k] << ", pred: " << pred[i][k] << endl << "\tSu coste original es INF > " << d[j][i] + d[i][k] << ", la propuesta se acepta." << endl;
						else if (d[j][k] > d[j][i] + d[i][k])
						{
							cout << d[j][i] + d[i][k] << ", pred: " << pred[i][k] << endl << "\tSu coste original es " << d[j][k] << " > " << d[j][i] + d[i][k] << ", la propuesta se acepta." << endl;
							if (j == k)
								cout << "\t (Se forma un circuito negativo, las consecuencias se calcularan despues del algoritmo)" << endl;
						}
						else if (d[j][k] < d[j][i] + d[i][k])
							cout << d[j][i] + d[i][k] << ", pred: " << pred[i][k] << endl << "\tSu coste original es " << d[j][k] << " < " << d[j][i] + d[i][k] << ", la propuesta se rechaza." << endl;
						else
							cout << d[j][i] + d[i][k] << ", pred: " << pred[i][k] << endl << "\tSu coste original es " << d[j][k] << " = " << d[j][i] + d[i][k] << ", la propuesta se rechaza." << endl;
					}
					if (i != k && (pred[i][k] != 0) && d[j][k] > d[j][i] + d[i][k])
					{
						if (j == k)
							neg[j] = true;
						d[j][k] = d[j][i] + d[i][k];
						pred[j][k] = pred[i][k];
					}
				}
		}
	}
	//Bucle de detecci�n y actualizaci�n para circuitos negativos
	for (unsigned i = 0; i < n; i++)
		if (neg[i])
		{
			vector<bool> temp(n, false);
			temp =GetCycle(i, pred[i]);
			if (traza)
			{
				cout << endl << "Detectado circuito negativo con los nodos: {" << i + 1 << "}";
				for (unsigned j = 0; j < n; j++)
					if (temp[j] && j != i)
						cout << ", {" << j + 1 << "}";
				cout << endl;
			}

			// Bucle para establecer los costes del nodo de origen perteneciente al circuito.
			for (unsigned j = 0; j < n; j++)
			{
				if (temp[j])
					neg[j] = false; // Ya que los circuitos act�an como un nodo en s� mismo, se desmarcan los nodos pertenecientes al circuito ya detectado
				if (pred[i][j] != 0)
					d[i][j] = -2147483648; //minimo valor para int de 4 bits.
			}

			// Bucle para establecer el efecto del circuito
			for(unsigned j=0;j< n;j++)
				if(j!=i && pred[j][i] !=0) // Nodos con acceso o pertenecientes al circuito copian solo las mejoras que este implementa y el circuito en s�
					for (unsigned k = 0; k < n; k++)
					{
						if (temp[k])
							pred[j][k] = pred[i][k];
						if (pred[i][k]!=0)
							d[j][k] = d[i][k];
					}
		}

	if (tabla)
	{
		cout << "Tabla final:" << " (d | P)" << endl;

		for (unsigned j = 0; j < n; j++)
		{
			for (unsigned k = 0; k < n; k++)
				if (pred[j][k] == 0)
					cout << "\tINF";
				else if (d[j][k] == -2147483648)
					cout << "\t-inf";
				else
					cout << "\t" << d[j][k];
			cout << "\t|";
			for (unsigned k = 0; k < n; k++)
				cout << "\t" << pred[j][k];
			cout << endl;
		}
		cout << endl;
	}
	for (unsigned i = 0; i < n; i++)
	{
		cout << endl << "Caminos desde el nodo " << i + 1 << ":" << endl;
		for (unsigned j = 0; j < n; j++)
			if (j != i)
				if (pred[i][j] == 0)
					cout << "No hay camino desde " << i + 1 << " al nodo " << j + 1 << endl;
				else
				{
					cout << "El camino desde " << i + 1 << " al nodo " << j + 1 << " es: ";
					MostrarCamino2(i, j, pred[i]);
					if (d[i][j]==-2147483648)
						cout << "\t de longitud -inf. (El camino puede no estar completo a causa de circuito)" << endl;
					else
					cout << "\t de longitud " << d[i][j] << endl;
				}
	}
}

void GRAFO::ListaPredecesores()
{
	ElementoLista temp;
	LP.clear();
	LP.resize(n);

	for (unsigned i = 0; i < n; i++)
		for (unsigned j = 0; j < LS[i].size(); j++)
		{
			temp.j = i;
			temp.c = LS[i][j].c;
			LP[LS[i][j].j].push_back(temp);
		}
}

GRAFO::GRAFO(char nombrefichero[85], int& errorapertura)
{
	m = 0;
	n = 0;
	dirigido = 0;
	errorapertura = build(nombrefichero);
	if (errorapertura)
		cout << "No se ha encontrado el archivo" << endl;
}

//////////////////////////////////////////////////////////////////////////
///////////////////////////Clase Queue////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Queue::Queue(unsigned size)
{
	resize(size);
}

Queue::~Queue()
{
	max_size = 0;
	first = 0;
	last = 0;
	data.clear();
}

void Queue::resize(unsigned size)
{
	max_size = size;
	first = 0;
	last = 0;
	data.resize(max_size);
}

void Queue::Crawl_back()
{
	if (first != 0)
	{
		for (unsigned i = 0; i < last - first; i++)
			data[i] = data[i + first];
		last -= first;
		first = 0;
	}
}

void Queue::Crawl_foward_1()
{
	if (last != max_size)
	{
		for (unsigned i = first + 1; i < last + 1; i++)
			data[i] = data[i - 1];
		last++;
		first++;
	}
}

unsigned Queue::get_size()
{
	return last - first;
}

unsigned Queue::get_max_size()
{
	return max_size;
}

unsigned Queue::extract_first()
{
	unsigned r = get_first();
	remove_first();
	return r;
}

unsigned Queue::get_first()
{
	if (empty())
		return 0;
	else
		return data.at(first);
}

unsigned Queue::get_last()
{
	if (empty())
		return 0;
	else
		return data.at(last - 1);
}

void Queue::add_last(unsigned n)
{
	if (!full())
	{
		if (last == max_size)
			Crawl_back();
		data[last++] = n;
	}
}

void Queue::add_first(unsigned n)
{
	if (!full())
	{
		if (first == 0)
			Crawl_foward_1();
		data[--first] = n;
	}
}

void Queue::remove_first()
{
	if (!empty())
		first = first + 1;
}

bool Queue::full()
{
	return first == 0 && last == max_size;
}

bool Queue::empty()
{
	return first == last;
}

bool Queue::IsInQueue(unsigned n)
{
	for (unsigned i = first; i < last; i++)
		if (data[i] == n)
			return true;
	return false;
}
