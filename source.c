#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
  int n; // dimensiunea grafului
  int **matrix; // matricea
} Graph;

// initializarea grafului n x n
Graph *init (int n)
{
	Graph *g = (Graph *) malloc (sizeof(Graph));
	g->n = n;
	g->matrix = (int **) malloc (n * sizeof(int *));
	int i, j;
	for (i = 0; i < n; i++)
		g->matrix[i] = (int *) malloc (n * sizeof(int));
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
      g->matrix[i][j] = INT_MAX; // toate elementele, cu exceptia diagonalei
      // principale, sunt intializate cu distanta infinita
  }
  for (i = 0; i < n; i++)
    g->matrix[i][i] = 0; // diagonala principala
	return g;

}

int *build_array (FILE * input, int k) // construirea vectorului din fisier
{
  int *interests = (int *) malloc (k * sizeof(int));
  int i;
  for (i = 0; i < k; i++)
    fscanf(input, "%d", &(interests[i]));
  return interests;

}

// citirea primei matrici din fisier
void build_matrix_1 (FILE *input, int k, Graph *graph)
{
  int i = 0;
  while (i < k)
  {
    fscanf(input, "%d %d %d", &graph->matrix[i][0], &graph->matrix[i][1],
      &graph->matrix[i][2]);
    i++;
  }
}

// citirea celei de-a doua matrici din fisier
void build_matrix_2 (FILE *input, int k, Graph *graph)
{
  int i = 0;
  while (i < k)
  {
    fscanf(input, "%d %d", &graph->matrix[i][0], &graph->matrix[i][1]);
    i++;
  }
}

void free_matrix (int **matrix, int n) // eliberarea memoriei matricei
{
  int i;
  for (i = 0; i < n; i++)
    free(matrix[i]);
  free(matrix);
}

void free_graph (Graph *graph) // eliberarea memoriei ocupata de graf
{
  free_matrix(graph->matrix, graph->n);
  free(graph);
}

// determinarea fermei pentru task-ul 1
int determine_farm (int *interests, int k)
{
  int i;
  for (i = 0; i < k; i++)
  {
    if (interests[i] == 1)
      return i;
  }
  return 0;
}

// verifica daca exista muchie intre nodurile i si j
int check_edge (Graph *graph, int i, int j)
{
	if ((graph->matrix[i][j] != INT_MAX) && (graph->matrix[i][j] != 0))
		return 1;
	return 0;
}

// algoritmul Floyd_Warshall pentru obtinerea matricei cu distantele minime
void Floyd_Warshall (Graph *g, Graph *distances)
{
	int n = g->n;
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			distances->matrix[i][j] = g->matrix[i][j];
	}
	for (k = 0; k < n; k++)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (distances->matrix[i][j] > (distances->matrix[i][k] + distances->matrix[k][j])
        && (distances->matrix[i][k] != INT_MAX) && (distances->matrix[k][j] != INT_MAX))
					distances->matrix[i][j] = distances->matrix[i][k] + distances->matrix[k][j];
			}
		}
	}
}

// indicele pentru distanta minima, folosit in Dijkstra
int minimum_distance (int *dist, int *visited, int n)
{
    int min = INT_MAX, index, i;
    for (i = 0; i < n; i++)
        if (visited[i] == 0 && dist[i] <= min)
        {
          min = dist[i];
          index = i;
        }
    return index;
}

// afisarea caii dintre doua noduri, neincluzand nodurile de start si de oprire,
// folosind vectorul prev din Dijkstra, afisand ca stiva
// j e capatul, k trebuie ignora, k fiind capatul
void print_path (int *prev, int i, int k, FILE * output)
{
    if (prev[i] == -1)
        return;
    print_path(prev, prev[i], k, output);
 	  if (i != k)
      fprintf(output, "%d ", i);
}

// de acest algoritm ne folosim pentru a afisa calea dintre 2 noduri, folosind
// vectorul prev
void Dijkstra (int **graph, int src, int client, int n, FILE *output)
{
    int dist[1000];
    int visited[1000];
    int prev[1000];
    int i;
    for (i = 0; i < n; i++)
    {
        prev[src] = -1;
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[src] = 0;
    int count;
    for (count = 0; count < n-1; count++)
    {
        int u = minimum_distance(dist, visited, n);
        visited[u] = 1;
        int v;
        for (v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] != 0 && graph[u][v] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v])
            {
                prev[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    print_path(prev, client, client, output); // end e capatul
}

void task_one (FILE * input, FILE * output) // task-ul 1
{
  int n, m, c, sum = 0, i, farm; // sum e suma distantelor
  // n e numarul de puncte de interests
  // m e numarul de culoare aeriene
  // c e numarul de clienti, fapt valabil la toate taskurile
  Graph *one = NULL, *two = NULL, *graph = NULL, *distances = NULL;
  // one e prima matrice, pe care o procesam in graph
  // two e graful clientilor
  // distances e graful distantelor minime
  fscanf(input, "%d\n", &n); // citim n
  int *interests = build_array(input, n); // interesele
  fscanf(input, "%d\n", &m); // citim m
  one = init(m); // prima matrice din fisier
  build_matrix_1(input, m, one); // adaugam matricea din fisier in graf
  fscanf(input, "%d\n", &c); // citim c
  two = init(c); // a doua matrice din fisier
  build_matrix_2(input, c, two); // adaugam matricea din fisier in graf
  graph = init(n); // graful cu fermele si clientii
  distances = init(n); // graful cu distantele minime ale nodurilor din graph
  farm = determine_farm(interests, n); // ferma
  for (i = 0; i < m; i++) // construirea grafului cu fermele si clientii
    graph->matrix[one->matrix[i][0]][one->matrix[i][1]] = one->matrix[i][2];
  free_graph(one); // eliberam memoria ocupata de graful one
  Floyd_Warshall(graph, distances); // construim graful cu distantele minime
  for (i = 0; i < c; i++) // afisarea drumurilor cu cost minim
  {
    fprintf(output, "%d ", farm);
    Dijkstra(graph->matrix, farm, two->matrix[i][1], n, output);
    fprintf(output, "%d ", two->matrix[i][1]);
    Dijkstra(graph->matrix, two->matrix[i][1], farm, n, output);
    int a = distances->matrix[farm][two->matrix[i][1]];
    int b = distances->matrix[two->matrix[i][1]][farm];
    // a e distanta de la primul client pana la ferma
    // b e distanta de la ferma la al doilea client
    sum = sum + a + b;
  }
  fprintf(output, "%d\n", farm); // destinatia finala
  fprintf(output, "%d\n", sum); // suma costurilor
  free(interests); // eliberam memoria ocupata de vectorul interests
  free_graph(two); // eliberam memoria ocupata de graful two
  free_graph(graph); // eliberam memoria ocupata de graful graph
  free_graph(distances); // eliberam memoria ocupata de graful distances
}

// numarul de ferme de tipul p
int number_of_farms (int *interests, int n, int p)
{
  int k = 0, i;
  for (i = 0; i < n; i++)
  {
    if (interests[i] == p)
      k++;
  }
  return k;
}

int *list_farms (int *interests, int n, int p) // vectorul de ferme de tipul p
{
  int *farms  = (int *) malloc (n * sizeof(int));
  int k = 0, i;
  for (i = 0; i < n; i++)
  {
    if (interests[i] == p)
      farms[k++] = i;
  }
  return farms;
}

// returneaza ferma cea mai apropiata fata de un nod k dat, prin cautarea
// celei mai mici distante dintre nodul k si o ferma
int nearest (int **matrix, int *farms, int n, int k, int p)
{
  int i, minimum = INT_MAX, position = -1;
  for (i = 0; i < p; i++)
  {
    // dam skip la cautare, in caz ca i e fix nodul dat ca parametru
    if (k == farms[i])
      continue;
    else
      if (matrix[k][farms[i]] < minimum) // cautam distanta minima
        minimum = matrix[k][farms[i]];
  }
  for (i = 0; i < n; i++)
  {
    if (k == i)
      continue;
    else
      if (matrix[k][i] == minimum) // daca am gasit ferma cu distanta minima
        position = i;
  }
  return position; // returnam indicele fermei
}

// returneaza ferma care se afla pe drumul minim dintre 2 clienti
// cautand ferma ce
int minimum_farm (int **matrix, int one, int two, int *farms, int no_farms)
{
  int i, minimum_dist = INT_MAX;
  for (i = 0; i < no_farms; i++)
  {
    if (((matrix[one][farms[i]] + matrix[farms[i]][two]) < minimum_dist)
    && (matrix[one][farms[i]] != INT_MAX) && (matrix[farms[i]][two] != INT_MAX))
      minimum_dist = matrix[one][farms[i]] + matrix[farms[i]][two];
    else continue;
  }
  for (i = 0; i < no_farms; i++)
  {
    if ((matrix[one][farms[i]] + matrix[farms[i]][two]) == minimum_dist)
      return farms[i]; // am gasit ferma
  }
  return -1; // nu s-a gasit ferma
}

void task_two (FILE * input, FILE * output)
{
  int n, m, c, sum = 0, i, first_farm; // sum e suma distantelor
  // first_farm e ferma de start
  Graph *one = NULL, *two = NULL, *graph = NULL, *distances = NULL;
  fscanf(input, "%d\n", &n); // citim n
  int *interests = build_array(input, n); // interesele
  fscanf(input, "%d\n", &m); // citim m
  one = init(m); // prima matrice din fisier
  build_matrix_1(input, m, one); // adaugam matricea din fisier in graf
  fscanf(input, "%d\n", &c); // citim c
  two = init(c); // a doua matrice din fisier
  build_matrix_2(input, c, two); // adaugam matricea din fisier in graf
  graph = init(n); // graful cu fermele si clientii
  distances = init(n); // graful cu distantele minime ale nodurilor din graph
  fscanf(input, "%d\n", &first_farm); // ferma de start
  for (i = 0; i < m; i++) // construirea grafului cu fermele si clientii
    graph->matrix[one->matrix[i][0]][one->matrix[i][1]] = one->matrix[i][2];
  free_graph(one); // eliberam memoria ocupata de graful one
  Floyd_Warshall(graph, distances); // construim graful cu distantele minime
  int no_farms = number_of_farms (interests, n, 1); // numarul de ferme
  int *farms = list_farms (interests, n, 1); // vectorul de ferme
  fprintf(output, "%d ", first_farm); // afisam ferma de start
  Dijkstra(graph->matrix, first_farm, two->matrix[0][1], n, output);
  sum = distances->matrix[first_farm][two->matrix[0][1]];
  for (i = 1; i < c; i++)
  {
    fprintf(output, "%d ", two->matrix[i-1][1]);
    int p = minimum_farm (distances->matrix, two->matrix[i-1][1],
      two->matrix[i][1], farms, no_farms);
    Dijkstra(graph->matrix, two->matrix[i-1][1], p, n, output);
    fprintf(output, "%d ", p);
    Dijkstra(graph->matrix, p, two->matrix[i][1], n, output);
    int a = distances->matrix[two->matrix[i-1][1]][p];
    int b = distances->matrix[p][two->matrix[i][1]];
    // a e distanta de la primul client pana la ferma
    // b e distanta de la ferma la al doilea client
    sum = sum + a + b;
  }
  fprintf(output, "%d ", two->matrix[c-1][1]);
  int e = nearest(distances->matrix, farms, n, two->matrix[c-1][1], no_farms);
  // e este ferma cea mai apropiata ultimului client
  Dijkstra(graph->matrix, two->matrix[c-1][1], e, n, output);
  fprintf(output, "%d\n", e);
  sum = sum + distances->matrix[two->matrix[c-1][1]][e];
  fprintf(output, "%d\n", sum);
  free(farms);
  free(interests);
  free_graph(two); // eliberam memoria ocupata de graful two
  free_graph(graph); // eliberam memoria ocupata de graful graph
  free_graph(distances);
}

// prin maxim, putem afla numarul de tipuri de ferme
int maximum_element (int *array, int dimension)
{
  int max = INT_MIN, i;
  for (i = 0; i < dimension; i++)
  {
    if (array[i] > max)
      max = array[i];
  }
  return max;
}

// numarul de ferme de un anumit tip
int apparitions (int *array, int dimension, int k)
{
  int nr = 0, i;
  for (i = 0; i < dimension; i++)
  {
    if (array[i] == k)
      nr++;
  }
  return nr;
}

int *all_farms (int *interests, int n) // vectorul in care sunt toate fermele
{
  int *farms  = (int *) malloc (n * sizeof(int));
  int k = 0, i;
  for (i = 0; i < n; i++)
  {
    if (interests[i] != 0)
      farms[k++] = i;
  }
  return farms;
}

// factorialul unui numar n dat
int factorial (int n)
{
	int fact = 1;
	int i;
	for (i = 2; i <= n; i++)
		fact *= i;
	return fact;
}

// interschimbarea a doua valori date
void swap (int *a, int *b)
{
  int aux;
  aux = *a;
  *a = *b;
  *b = aux;
}

// salvarea unui vector intr-o linie data dintr-o matrice
void save_line_in_matrix (int **matrix, int *array, int n, int line)
{
  int i;
  for(i = 0; i < n; i++)
    matrix[line][i] = array[i];
}

// generarea tuturor permutarilor unui vector dat (array in cazul de fata)
void backtracking (int *line, int **matrix, int *array, int n, int k)
{
  int i;
  if (k == n)
  {
    save_line_in_matrix(matrix, array, n, (*line)++);
    return;
  }

  for (i = k; i < n; i++)
  {
    	  swap (&array[k], &array[i]);
        backtracking (line, matrix, array, n, k + 1);
        swap (&array[k], &array[i]);
  }
}

// tipul de ferma pentru un client dat
int type_farm (int client, int n, int **all_clients)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (client == all_clients[i][1])
      return all_clients[i][0];
  }
  return -1;
}

// se cauta cea mai apropiata ferma de un client, indiferent de tipul acesteia
int nearest_farm (int **matrix, int **all_farms, int n, int *apparitions, int no_farms)
{
  int i, j, minimum = INT_MAX, position = -1;
  for (i = 0; i < no_farms; i++)
  {
    for (j = 0; j < apparitions[i]; j++)
      if (matrix[n][all_farms[i][j]] < minimum)
        minimum = matrix[n][all_farms[i][j]];
  }
  for (i = 0; i < no_farms; i++)
  {
    for (j = 0; j < apparitions[i]; j++)
      if (matrix[n][all_farms[i][j]] == minimum)
        position = all_farms[i][j];
  }
  return position;
}

// ce tip de ferma e o ferma data
int what_type_farm (int **all_farms, int n, int *apparitions, int m)
{
  int i, j;
  for (i = 0; i < m; i++)
  {
    for (j = 0; j < apparitions[i]; j++)
    {
      if (n == all_farms[i][j])
      {
        int m = i + 1;
        return m;
      }
    }
  }
  return 0;
}

// suma totala de cost minim pe tot drumul
int total_sum (int *clients, int n, int **all_clients, int **all_farms,
    int types_of_farms, int first_farm, int **distances, int *no_of_apparitions)
{
    int sum = 0; // suma de cost minim
  // calculam suma de la ferma de start pana la primul client si vedem daca
  // ferma de start are acelasi tip cu primul client
  if (type_farm(clients[0], n, all_clients) == what_type_farm(all_farms,
    first_farm, no_of_apparitions, types_of_farms))
      sum = distances[first_farm][clients[0]];
  else
  {
    int p = type_farm(clients[0], n, all_clients) - 1;
    int k = minimum_farm(distances, first_farm, clients[0], all_farms[p],
      no_of_apparitions[p]);
    sum = distances[first_farm][k] + distances[k][clients[0]];
  }
  int i;
  // adunam la suma drumurile dintre doi clienti succesivi
  for (i = 1; i < n; i++)
  {
    int k = type_farm(clients[i], n, all_clients) - 1; // tipul fermei
    // suma
    int a = distances[clients[i-1]][minimum_farm (distances, clients[i-1],
    clients[i], all_farms[k], no_of_apparitions[k])];
    int b = distances[minimum_farm (distances, clients[i-1], clients[i],
      all_farms[k], no_of_apparitions[k])][clients[i]];;
    sum = sum + a + b;
  }
  // adunam la suma distanta dintre ultimul client si cea mai apropiata ferma de el
  int e = nearest_farm (distances, all_farms, clients[n-1], no_of_apparitions,
    types_of_farms);
  sum = sum + distances[clients[n-1]][e];
  return sum;
}

// elementul minim al unui vector
int minimum_element (int *array, int n)
{
  int i, min = INT_MAX;
  for (i = 0; i < n; i++)
  {
    if (array[i] <= min)
      min = array[i];
  }
  return min;
}

// se cauta indicele unui numar (element) intr-un vector
int find_position (int *array, int n, int element)
{
  int position = -1, i;
  for (i = 0; i < n; i++)
  {
    if (array[i] == element)
      {
        position = i;
        break;
      }
  }
  return position;
}

void task_three (FILE * input, FILE * output)
{
  int n, m, c, i, first_farm; // first_farm e ferma de start
  Graph *one = NULL, *two = NULL, *graph = NULL, *distances = NULL;
  fscanf(input, "%d\n", &n); // citim n
  int *interests = build_array(input, n); // interesele
  int types_of_farms = maximum_element(interests, n); // numarul de tipuri de ferme
  int *no_of_apparitions = (int *) malloc (types_of_farms * sizeof(int));
  for (i = 0; i < types_of_farms; i++)
    no_of_apparitions[i] = apparitions(interests, n, i + 1);
  // matrice cu ferme, unde pe linia i se afla ferme de tipul i + 1
  int **matrix = (int **) malloc (types_of_farms * sizeof(int *));
  for (i = 0; i < types_of_farms; i++)
    matrix[i] = list_farms(interests, n, i + 1);
  fscanf(input, "%d\n", &m); // citim m
  one = init(m); // prima matrice din fisier
  build_matrix_1(input, m, one); // adaugam matricea din fisier in graf
  fscanf(input, "%d\n", &c); // citim c
  two = init(c); // a doua matrice din fisier
  build_matrix_2(input, c, two); // adaugam matricea din fisier in graf
  graph = init(n); // graful cu fermele si clientii
  distances = init(n); // graful cu distantele minime ale nodurilor din graph
  fscanf(input, "%d\n", &first_farm); // ferma de start
  for (i = 0; i < m; i++) // construirea grafului cu fermele si clientii
    graph->matrix[one->matrix[i][0]][one->matrix[i][1]] = one->matrix[i][2];
  free_graph(one); // eliberam memoria ocupata de graful one
  Floyd_Warshall(graph, distances); // construim graful cu distantele minime
  int *first_clients = (int *) malloc (c * sizeof(int)); // vectorul de clienti
  for (i = 0; i < c; i++)
    first_clients[i] = two->matrix[i][1];
  int fact = factorial(c); // numarul de permutari
  // matricea de permutari
  int **all_permutations = (int **) malloc (fact * sizeof(int *));
  for (i = 0; i < fact; i++)
    all_permutations[i] = (int *) malloc (c * sizeof(int));
  int nr = 0; // indicele de linie cand se completeaza matricea de permutari
  // salvarea permutarilor in matricea all_permutations
  backtracking(&nr, all_permutations, first_clients, c, 0);
  // vector de sume de cost minim pentru fiecare linie din matricea de permutari
  int *sums = (int *) calloc (fact, sizeof(int));
  for (i = 0; i < fact; i++)
    sums[i] = total_sum (all_permutations[i], c, two->matrix, matrix, types_of_farms,
      first_farm, distances->matrix, no_of_apparitions);
  int minimum_sum = minimum_element(sums, fact); // suma minima
  // pozitia unde se afla suma minima in vectorul de sume
  int position = find_position(sums, fact, minimum_sum);
  // vectorul de clienti ce va contine permutarea optima minima
  int *clients = (int *) malloc (c * sizeof(int));
  for (i = 0; i < c; i++)
    clients[i] = all_permutations[position][i];
  // se verifica daca ferma de start are acelasi tip cu primul client
  if (type_farm(clients[0], c, two->matrix) ==
    what_type_farm(matrix, first_farm, no_of_apparitions, types_of_farms))
  {
    fprintf(output, "%d ", first_farm); // afisam ferma de start
    Dijkstra(graph->matrix, first_farm, clients[0], n, output);
  }
  else
  {
    int type = type_farm(clients[0], c, two->matrix) - 1;
    int k = minimum_farm(distances->matrix, first_farm, clients[0],
      matrix[type], no_of_apparitions[type]);
    fprintf(output, "%d ", first_farm);
    Dijkstra(graph->matrix, first_farm, k, n, output);
    fprintf(output, "%d ", k);
    Dijkstra(graph->matrix, k, clients[0], n, output);
  }
  for (i = 1; i < c; i++)
  {
    fprintf(output, "%d ", clients[i-1]);
    int k = type_farm(clients[i], c, two->matrix) - 1;
    int p = minimum_farm (distances->matrix, clients[i-1], clients[i],
      matrix[k], no_of_apparitions[k]);
    Dijkstra(graph->matrix, two->matrix[i-1][1], p, n, output);
    fprintf(output, "%d ", p);
    Dijkstra(graph->matrix, p, clients[i], n, output);
  }
  fprintf(output, "%d ", clients[c-1]);
  int e = nearest_farm (distances->matrix, matrix, clients[c-1],
    no_of_apparitions, types_of_farms);
  Dijkstra(graph->matrix, clients[c-1], e, n, output);
  fprintf(output, "%d\n", e);
  fprintf(output, "%d\n", minimum_sum);
  free(interests);
  free(clients);
  free(first_clients);
  free(sums);
  free(no_of_apparitions);
  free_graph(two); // eliberam memoria ocupata de graful two
  free_graph(graph); // eliberam memoria ocupata de graful graph
  free_graph(distances);
  free_matrix(matrix, types_of_farms);
  free_matrix(all_permutations, fact);
}


int main (int argc, char **argv)
{
    FILE * input = fopen(argv[1], "r");
    FILE * output = fopen(argv[2], "w+");
    int id; // numarul taskului
    fscanf(input, "%d", &id);
    switch (id) {
      case 1:
        task_one (input, output);
      break;
      case 2:
        task_two (input, output);
      break;
      case 3:
        task_three (input, output);
      break;
    }
    fclose(input);
    fclose(output);
    return 0;
}
