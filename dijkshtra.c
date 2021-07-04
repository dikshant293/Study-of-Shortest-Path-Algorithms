// Dijkstra Algorithm for Shortest Path between given 2 nodes
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

typedef struct heap_node //Minheap node structure format
{
    int vertex , distance;
}heap_node;

typedef struct min_heap //Minheap Structure format for Priority queue
{
    int capacity; // Capacity of min heap (number of vertices)
    int *pos; // For storing the position of vertex
    int size; // Number of heap nodes present currently
    heap_node **arr; //array for minheap

}min_heap;

min_heap *create_minheap(int capacity) //Function to create Minheap
{
    min_heap *H = (min_heap *)malloc(sizeof(min_heap)); //Dynamically allocating space for Minheap

    H->capacity = capacity;
    H->pos = (int *)malloc(capacity * sizeof(int));
    H->size = 0;
    H->arr = (heap_node **)malloc(capacity * sizeof(heap_node *));
    return H;
}

heap_node *new_heapnode(int vertex, int distance) //Function to create new minheap node
{
    heap_node *min_node = (heap_node *)malloc(sizeof(heap_node));

    min_node->vertex = vertex;
    min_node->distance = distance;

    return min_node;
}

int find(min_heap *H, int vertex) //Checks if the Vertex is in minHeap or not
{
    return (H->pos[vertex] < H->size);
}

void swap(heap_node **a, heap_node **b) //Function to swap two nodes of Minheap
{
    heap_node *temp = *b;
    *b = *a;
    *a = temp;
}

//This Function updates the position of the node as per their value so as to maintain the property of Minheap
void heapify(min_heap *H, int index)
{
    int min = index;
    int l = 2 * index + 1;
    int r = 2 * index + 2;

    if (l < H->size && H->arr[l]->distance < H->arr[min]->distance)
        min = l;

    if (r < H->size && H->arr[r]->distance < H->arr[min]->distance)
        min = r;

    if (min != index)
    {
        // The nodes to be swapped in min heap
        heap_node *minnode = H->arr[min];
        heap_node *temp = H->arr[index];

        // Swap nodes
        swap(&H->arr[min], &H->arr[index]);

        // Swap positions to the correct position
        H->pos[minnode->vertex] = index;
        H->pos[temp->vertex] = min;

        heapify(H, min);
    }
}

heap_node *remove_min(min_heap *H) // This Function returns the minimum node from the heap
{
    if (H->size==0)
        return NULL;

    // Store the root node
    heap_node *root = H->arr[0];

    // Replace root node with last node
    heap_node *last_node = H->arr[H->size - 1];
    H->arr[0] = last_node;

    // Update position of last node to maintain the property of Minheap
    H->pos[root->vertex] = H->size - 1;
    H->pos[last_node->vertex] = 0;

    // Reduce heap size and call heapify function to heapify root
    H->size--;
    heapify(H, 0);

    return root;
}

void update_key(min_heap *H, int vertex, int distance) //This function decreases the distance between given Vertex and the Source Node
{
    int i = H->pos[vertex];
    H->arr[i]->distance= distance;

    //This while loop works until the heap property is restored (heapified)
    //Stops when element reaches to top or the parent is less than child
    while (i && H->arr[i]->distance < H->arr[(i - 1) / 2]->distance)
    {
        // Swap this node with its parent
        swap(&H->arr[i], &H->arr[(i - 1) / 2]);

        // correcting the position of the vertices
        H->pos[H->arr[(i-1)/2]->vertex] = (i - 1) / 2;
        H->pos[H->arr[i]->vertex] = i;

        // move to parent index
        i = (i - 1) / 2;
    }
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

//This is the Dijkstra Function which calculates the distance of shortest path from source node to every vertex of the graph
void dijkstra(graph *G, int source , int destination)
{
    int num_vertices = G->num_vertices;

    //Distance array to store the shortest path length
    //Prev Array to store the previous node to the destination node
    int distance[num_vertices], prev[num_vertices];

    min_heap *H = create_minheap(num_vertices);

    for (int i = 0; i < num_vertices; i++) //Initializing the Minheap with all the vertices distance
    {
        distance[i] = MAX;
        prev[i] = -1;
        H->arr[i] = new_heapnode(i, distance[i]);
        H->pos[i] = i;
    }

    H->pos[source] = source;
    H->arr[source] = new_heapnode(source, distance[source]);

    distance[source] = 0; //Distance of source from source is 0 so that it is picked first as a greedy step
    update_key(H, source, distance[source]);

    H->size = num_vertices;

    while (H->size)
    {
        heap_node *min_node = remove_min(H); // Minimum Vertex from the Minheap
        int j = min_node->vertex;
        adjlist_node *temp = G->arr[j];

        //Traversing through all the vertices and updating their distance
        while (temp)
        {
            int i = temp->destination;
            if (find(H, i))
                if ( distance[i] > temp->cost + distance[j])
                {
                    distance[i] = distance[j] + temp->cost;
                    prev[i] = j; // saving prev node of i as j
                    update_key(H, i, distance[i]); //Updating the distance value in Minheap
                }
            temp = temp->next;
        }
    }
    //printArr(distance, num_vertices, prev);

    printf("\nPATH:\t");
    printpath(prev , source ,destination , num_vertices);
    if (distance[destination] != MAX) printf("PATH LENGTH: %d\n",distance[destination]);

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

    dijkstra(G, source , destination);

    return 0;
}

