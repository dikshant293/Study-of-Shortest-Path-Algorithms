#include <stdio.h>
#include <stdlib.h>
#define MAX 10000
#define FILENAME "sample1.txt"

//function to create an adjacency matrix and initialization
int **create_adjmat(int n)
{
	//dynamic allocation
    int **s = (int **) malloc(n * sizeof(int *));
    for (int i=0;i<n;i++)
        s[i] = (int *) malloc(n * sizeof(int));

	//initialization
	for (int i=0 ; i<n ; i++)
        for (int j=0 ; j<n ; j++)
        {
            s[i][j] = MAX;
            if (i==j) s[i][j] = 0; // minimum path to start and end on same vertex is 0
        }

    return s;
}

// function to create a "next" matrix and its initialization
int **create_next(int n , int **adjmat) // "next" matrix is used for printing the path
{
	//dynamic allocation
    int **s = (int **) malloc(n * sizeof(int *));
    for (int i=0;i<n;i++)
        s[i] = (int *) malloc(n * sizeof(int));

	//initialization
	for (int i=0 ; i<n ; i++)
        for (int j=0 ; j<n ; j++)
        {
            s[i][j] = j;
            if (adjmat[i][j]== MAX) s[i][j] = -1; // there is no edge between i and j
        }

    return s;
}

//funtion to read the edges and fill the adjacency matrix
void readadjmat(int **adjmat , int E , FILE *fp)
{
    int a,b , cost;
    for (int i =0 ; i<E ; i++)
    {
        fscanf (fp , "%d %d %d", &a,&b,&cost);

        //If entered directed edge already exists, take the minimum
        if (adjmat[a][b] > cost)
			adjmat[a][b]=cost;
    }
}

//function to print the path from specified source to destination
void printpath(int source, int destination , int **Next)
{
    // If path doesn't exist from source to destination
    if (Next[source][destination] == -1)
    {
        printf("PATH DOESN'T EXIST\n");
        return ;
    }

    // When path exists from source to destination
    printf("\nPATH :\t%d (SOURCE) ",source);
    while (source != destination) {
        source = Next[source][destination];
        printf("--> %d ",source);
    }
    printf("(DESTINATION)\n");

    return ;
}

//funtion to print the matrix with size n * n
void printmatrix(int **matrix , int n)
{
    printf("Matrix:\n");
    for (int i=0 ; i<n; i++)
    {
        for (int j=0 ; j<n ; j++)
            printf("%d\t", matrix[i][j]);
        printf("\n\n");
    }
}

//This is the Floyd-Warshall Function which calculates the distance of shortest path from every source node to every vertex of the graph
//Prints the path and path length(if exists) from source to destination
void floyd_warshall(int source , int destination)
{
    // Read the file provided and generate a graph using that
    FILE *fp ;
	fp = fopen(FILENAME,"r");
	if (fp==NULL) { printf("Input file could not open\n"); exit(1); }//Checking proper opening of file

	// V: #vertices
	// E: #edges
	int V, E;
	fscanf(fp,"%d %d",&V, &E);

	int **distance; // initially it represents the Adjacency matrix
    distance = create_adjmat(V);
    readadjmat(distance,E , fp);

    int **next; // to get the shortest path from source to the destination
    next = create_next(V,distance);

    //printmatrix(distance,V);

    for (int i = 0; i < V ; i++) // Intermediate vertex
        for (int u = 0; u < V; u++) // Start vertex
            for (int v = 0; v < V; v++) // End vertex
            {
                // Pass if edge doesn't exist
                if (distance[u][i] == MAX || distance[i][v] == MAX) continue;

                //Update if shorter path exists via "i"
                if (distance[u][i] + distance[i][v] < distance[u][v])
                {
                    distance[u][v] = distance[u][i]+ distance[i][v];
                    next[u][v] = next[u][i];
                }
            }
    // Shortest path found (if negative cost cycle doesn't exist)
	// Checking for negative cycle: if the distances change after one more iteration, then there is a negative cost cycle.
    for (int i = 0; i < V ; i++) // Intermediate vertex
        for (int u = 0; u < V; u++) // Start vertex
            for (int v = 0; v < V; v++) // End vertex
            {
                // Pass if edge doesn't exist
                if (distance[u][i] == MAX || distance[i][v] == MAX) continue;

                //Update if shorter path exists via "i"
                if (distance[u][i] + distance[i][v] < distance[u][v])
                {
                    printf("\nThis graph contains negative edge cycle\nSo, the shortest path is not defined\n");
                    return;
                }
            }

    printpath(source,destination,next);

    if (distance[source][destination] != MAX) printf("PATH LENGTH: %d\n",distance[source][destination]);
}

int getV()// Function to extract the number of vertices (V) from the file
{
    // Read the file provided
    FILE *fp ;
	fp = fopen(FILENAME,"r");
	if (fp==NULL) { printf("Input file could not open\n"); exit(1); }//Checking proper opening of file

	//Extracting V from the file
	int V ;
	fscanf(fp,"%d",&V);
    fclose(fp);
    return V;
}


int main()
{
    int source , destination;

	printf("Enter the source and the destination ");

	printf("(Vertex numbers should be less than %d):\n",getV());

	scanf("%d %d" , &source , &destination);

    floyd_warshall(source , destination);

    return 0;
}
