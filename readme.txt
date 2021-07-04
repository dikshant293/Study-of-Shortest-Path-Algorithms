1. Each algorithm code file contains 
	#define FILENAME "sample.txt"
   for changing the filename just change this name (if required)

2. The input file contains the information in the following format
	
	V
	E
	start1 	end1   cost1
	start2 	end2   cost2
	  .	  .	 .
	  .	  .	 .
	  .	  .	 .
	startE 	endE   costE

    where V denotes the number of nodes/vertices
	  E denotes the number of edges
	  next "E" lines contain edge information: 
		start_i denotes start node of the edge i
		end_i denotes end node of the edge i
		cost_i denotes the cost of the edge i

3. The code asks the source and destination of the path required from the user and provides the max limit for entry
   write two whole numbers with some space between them 
   1st number denotes the source node
   2nd number denotes the destination node

4. The code outputs the Shortest Path and its path length (if exists)
   