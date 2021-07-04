// Bellman-Ford Algorithm for Shortest Path between given 2 nodes
#include <stdio.h>
#include <stdlib.h>
#define MAX 10000
#define FILENAME "sample1.txt"

typedef struct adjlist_node //Structure to store node values in Adjacency List
{
    int destination , cost;
    struct adjlist_node *next;
}adjlist_node;

typedef struct graph //Representation of Graph
{
    int num_vertices;
    struct adjlist_node **arr; //Nodes are stored in an Array of "size" = Number of Vertices
}graph;

adjlist_node *new_listnode(int destination, int cost) //Function To create new Node
{
    adjlist_node *new_node = (adjlist_node *)malloc(sizeof(adjlist_node));

    new_node->destination = destination;
    new_node->cost = cost;
    new_node->next = NULL;
    return new_node;
}

graph *create_graph(int num_vertices) //Function to create an empty Graph consisting of "num_vertices" Vertices
{
    graph *G = (graph *)malloc(sizeof(graph));
    G->num_vertices = num_vertices;
    G->arr = (adjlist_node **)malloc(num_vertices * sizeof(adjlist_node*)); //Dynamically allocating space for array of size num_vertices

    for (int i = 0; i < num_vertices; i++)
        G->arr[i] = NULL; //Initializing with NULL
    return G;
}

void add_newedge(graph *G, int src, int dest, int cost) //Function to add a new directed edge in the graph
{
    adjlist_node *new_node = new_listnode(dest, cost);
    new_node->next = G->arr[src];
    G->arr[src] = new_node;
}

//funtion to print the arrays dist[] and prev[] containing information about shortest distance from source and previous vertex in the path respectively
void printArr(int dist[], int n, int prev[])
{
    printf("Vertex   Distance from Source   Previous Node\n");
    for (int i = 0; i < n; i++)
        printf("  %d \t\t %d \t\t     %d\n", i, dist[i], prev[i]);
}

void printpath(int prev[] , int source , int destination , int num_vertices) // Function to print the path from source to destination
{
    int i = 0 , temp[num_vertices];

    temp[i] = destination;

    //BACKTRACKING USING PREV NODES
    while (temp[i] != source)
    {
        if (prev[temp[i]] == -1)
        {
            printf("PATH DOESN'T EXIST\n");
            return;
        }
        temp[i+1] = prev[temp[i]];
        i++;
    }

    //PRINTING IN STRAIGHT ORDER FROM SOURCE TO DESTINATION
    printf("%d (SOURCE) ",temp[i--]);
    for ( ; i >= 0 ; i--)
        printf("--> %d ",temp[i]);
    printf("(DESTINATION)\n");

}

//This is the BellmanFord Function which calculates the distance of shortest path from source node to every vertex of the graph
void bellman_ford(graph* G, int source , int destination)
{
    int num_vertices = G->num_vertices;

	//Distance array to store the shortest path length
    //Prev Array to store the previous node to the destination node
    int distance[num_vertices] , prev[num_vertices];

    //Initializing the vertex distances to MAX
    for (int i = 0; i < num_vertices; i++)
	{
		distance[i] = MAX;
		prev[i] = -1;
	}

	//Distance of source from source is 0 so that it is picked first as a greedy step
    distance[source] = 0;

    //The shortest path of graph that contain V vertices, contain atmost "V-1" edges
	// Doing "V-1" relaxations
    for (int i = 0; i < num_vertices-1; i++)
    {
		// To check whether changes are occurring or not in a particular iteration
		// If there is no change, program can be terminated sooner (as in Bubble Sort)
		int change = 0;

		//"u" : start node
        for (int u = 0 ; u < num_vertices ; u++)
        {
            adjlist_node *temp = G->arr[u];

			//Traversing through all the vertices and updating their distance
			while (temp)
			{
				int v = temp->destination;
				int cost = temp->cost;

					if ( distance[v] > cost + distance[u])
					{
						distance[v] = distance[u] + temp->cost;
						prev[v] = u; // saving prev node of v as u
						change =1;
					}
				temp = temp->next;
			}
        }

		// Terminate the loop earlier if the shortest paths are found before (num_vertices-1)th iteration
		// Also, negative cost cycle cannot exist in this case
		if (change == 0) goto print;
    }

    // Shortest path found (if negative cost cycle doesn't exist)
	// Checking for negative cycle: if the distances change after one more iteration, then there is a negative cost cycle.
    for (int u = 0 ; u < num_vertices ; u++)
    {
        adjlist_node *temp = G->arr[u];

        //Traversing through all the vertices and updating their distance
        while (temp)
        {
            int v = temp->destination;
            int cost = temp->cost;

            if ( distance[v] > cost + distance[u])
            {
                printf("\nThis graph contains negative edge cycle\nSo, the shortest path is not defined\n");
                return;
            }
            temp = temp->next;
        }
    }

	print:

    //printArr(distance, num_vertices, prev);

    printf("\nPATH:\t");
    printpath(prev , source ,destination , num_vertices);
    if (distance[destination] != MAX) printf("PATH LENGTH: %d\n",distance[destination]);

    return;
}

// Function to read the file provided and generate a graph using that
graph *readfile()
{
	FILE *fp ;
	fp = fopen(FILENAME,"r");
	if (fp==NULL) { printf("Input file could not open\n"); exit(1); }//Checking proper opening of file

	// V: #vertices
	// E: #edges
	int V, E;
	fscanf(fp,"%d %d",&V, &E);
	printf(" (Vertex numbers should be less than %d):\n",V);

	graph *G = create_graph(V);

	for (int i = 0 ; i < E ; i++)
	{
		int source, destination, cost;
		fscanf(fp, "%d %d %d", &source, &destination, &cost);
		add_newedge(G,source, destination, cost);
	}
	return G;
}

int main()
{
	int source , destination;

	printf("Enter the source and the destination");

	graph *G = readfile();

	scanf("%d %d" , &source , &destination);

    bellman_ford(G, source , destination);

    return 0;
}

