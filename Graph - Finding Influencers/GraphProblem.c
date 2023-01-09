#include<stdio.h>
#include<stdlib.h>
#include <string.h> 

typedef struct node{
	int vertex;
	int in_degree;
	int deleted;
	char name[20];
	char surname[20];
	struct node* next;
	int connection;
	int isInfluencer;
}NODE;

typedef struct adjList {
    struct node* list;   // head of the adjacency list
}ADJLIST;

typedef struct graph{
	int numOfVertices;
	struct adjList* adjLists;
}GRAPH;

NODE* createNode(int v); // Create a node
GRAPH* createGraph(int vertices); // Create a graph
void addEdge(GRAPH* graph, int s, int d); // Add edge
void readFromFile(FILE *file, GRAPH *graph);  // Read data from file and place it in Adjacency List
void printGraph(GRAPH* graph); //Print the graph
void calculateInDegree(GRAPH* graph);
void deleteNodes(GRAPH *graph,int M,int i);
GRAPH* updateFollowed(GRAPH *graph,int v);
void findConnections(GRAPH *graph,int i,int *visited);
void findInfluencers(GRAPH *graph,int X,int Y);
int findRows(FILE *file);

int main(){
	
	int slct,i,j,rows;
	int M,X,Y;
	FILE *file;
	GRAPH *graph = NULL;
	printf("\n\n\t\t\t\t ****** WELCOME ******\n\n");
	printf("\t\t\t\t BEYDA GULER  19011010\n\n");
	printf("\t\t\t\t **********************\n");
	printf("\t\t\t\t 1 - Normal Mode\n\t\t\t\t 2 - Deatiled Mode\n\t\t\t\t 3 - EXIT \n");
	printf("\t\t\t\t **********************\n");
	printf("\t\t\t\t Choose the mode -> ");
	scanf("%d",&slct);
	printf("\n\t\t\t   Please enter the value of M --> ");
	scanf("%d",&M);
	printf("\n\t\t\t   Please enter the value of X --> ");
	scanf("%d",&X);
	printf("\n\t\t\t   Please enter the value of Y --> ");
	scanf("%d",&Y);
	system("cls");
		
	if((file = fopen("socialNET.txt","r")) == NULL){
				printf("Couldn't find file...\n");
				exit(EXIT_SUCCESS);
	}
	rows=findRows(file)/2;

	file = fopen("socialNET.txt","r");
	switch(slct){
		case 1:
			
			printf("\n\t\t\t   ****NORMAL MODE****\n\n");
			printf("\t\t\t HERE IS THE INFLUENCERS!\n\n");
			graph = createGraph(rows);
			readFromFile(file,graph);
			calculateInDegree(graph);
			
			for(i=1;i<=graph->numOfVertices;i++){
				deleteNodes(graph,M,i);
			}
			int *visited = (int*)malloc((graph->numOfVertices+1)*sizeof(int));
			for(i=1;i<=graph->numOfVertices;i++){
				for(j=1;j<=graph->numOfVertices;j++)
					visited[j] = 0;
				findConnections(graph ,i ,visited);
			}
			findInfluencers(graph,X,Y);
			for(i=1;i<=graph->numOfVertices;i++){
				if(graph->adjLists[i].list->isInfluencer ==1){
					printf("%d  %s %s \t",graph->adjLists[i].list->vertex,graph->adjLists[i].list->name,graph->adjLists[i].list->surname);
					printf("he/she has %d connections and %d in-degree value.\n",graph->adjLists[i].list->connection,graph->adjLists[i].list->in_degree);
				}	
			}
			
			break;
			
		case 2:
			printf("\n\t\t\t  ****DETAILED MODE*****\n\n");
			printf("\t\t   HERE IS THE DETAILS YOU OVERTHINKER!\n\n");
			graph = createGraph(rows);   
			readFromFile(file,graph);
			calculateInDegree(graph);
			printf("\t\t  THIS IS HOW IT SEEM AT THE BEGINNING...\n\n");
			printGraph(graph);
			for(i=1;i<=graph->numOfVertices;i++){
				deleteNodes(graph,M,i);
			}
			printf("\n\n\t\tNODES THAT ARE NOT DELETED AFTER DELETION(M=%d)\n\n",M);
			for(i=1;i<=graph->numOfVertices;i++){
				if(graph->adjLists[i].list->deleted ==0)
					printf("\t\tVertex:%d\tIn-degree %d\t%s %s\n",graph->adjLists[i].list->vertex,graph->adjLists[i].list->in_degree,graph->adjLists[i].list->name,graph->adjLists[i].list->surname);
			}
			
			visited = (int*)malloc((graph->numOfVertices+1)*sizeof(int));
			for(i=1;i<=graph->numOfVertices;i++){
				for(j=1;j<=graph->numOfVertices;j++)
					visited[j] = 0;
				findConnections(graph ,i ,visited);
			}
			findInfluencers(graph,X,Y);
			printf("\n\t\t\t    INFLUENCER INFORMATIONS \n\n");
			for(i=1;i<=graph->numOfVertices;i++){
				if(graph->adjLists[i].list->isInfluencer ==1){
					printf("%s %s",graph->adjLists[i].list->name,graph->adjLists[i].list->surname);
					printf(" -->\t has %d total (directed/undirected) connections and %d in-degree value.\n",graph->adjLists[i].list->connection,graph->adjLists[i].list->in_degree);
				}	
			}
			break;
		case 3:
			printf("Exited\n");
			break;
		}
	
	printf("\nBYE:)\n");
	
	return 0;
}

// Create a node
NODE* createNode(int v){
	NODE* newNode = malloc(sizeof(NODE));
	newNode->vertex = v;
	newNode->in_degree=0;
	newNode->deleted = 0;
	newNode->next = NULL;
	newNode->connection = 0;
	return newNode;
}

int findRows(FILE *file){
	char ch;
	int i = 0;
	while(!feof(file)){
		ch=getc(file);
		if(ch=='\n')
			i++;
	}
	fclose(file);
	return i+1;
}

// Create a graph
GRAPH* createGraph(int vertices) {
	int i;
	GRAPH* graph = (GRAPH*)malloc(sizeof(GRAPH));
	graph->numOfVertices = vertices;
	graph->adjLists = (ADJLIST*)malloc(vertices*sizeof(ADJLIST));
	for (i = 1; i <= vertices; i++)
		graph->adjLists[i].list = NULL;
		
	return graph;
}

// Add edge
void addEdge(GRAPH* graph, int s, int d) {
	
	NODE *srcNode,*destNode,*tmp;
	if(graph->adjLists[s].list == NULL){
		srcNode = createNode(s);
		graph->adjLists[s].list=srcNode;
	}
	destNode = createNode(d);
	tmp = graph->adjLists[s].list;
	while(tmp->next!=NULL)
		tmp = tmp->next;
		
	tmp->next=destNode;
}

// Read data from file and place the values in Adjacency List
void readFromFile(FILE *file, GRAPH *graph){
	
	char vertexNum,endOfLine;
	char tmp[40];
	char *pt;
	int v,v_dest;
	int d=0;
	while(!feof(file)){
		while(1){
			vertexNum = fgetc(file);
			if(vertexNum == ',')
				break;
			d= d*10;
			v = (int)vertexNum - 48;
			d = d+v;
		}
		v=d;
		d=0;
		graph->adjLists[v].list=createNode(v);
		graph->adjLists[v].list->vertex=v;
		fscanf(file,"%[^,],%[^\n]",&(graph->adjLists[v].list->name),&(graph->adjLists[v].list->surname));
		endOfLine = fgetc(file);
		fscanf(file,"%[^\n]",tmp);
    	pt = strtok (tmp,",");
    	while (pt != NULL) {
       		v_dest = atoi(pt);
       		addEdge(graph,v,v_dest);
        	pt = strtok (NULL, ",");
    	}
		endOfLine=fgetc(file);
	}
}


void calculateInDegree(GRAPH* graph){
	
	int size = graph->numOfVertices,v;
	int vertex;
	NODE* list;
	for(v=1;v<=size;v++){
		list = graph->adjLists[v].list;
		if(list->deleted != 1){
			list = list->next;
			while(list){
				vertex = list->vertex;
				graph->adjLists[vertex].list->in_degree++;
				list=list->next;
			}
		}
	}
}


//Print the graph
void printGraph(GRAPH* graph){
	NODE *list;
	int size = graph->numOfVertices,v;
	for(v=1;v<=size;v++){
		list = graph->adjLists[v].list;
		if(list->deleted == 0){
			printf("\t\t%d  %s  %s\t",list->vertex,list->name,list->surname);
			list = list->next;
			while (list) {
				if(list->deleted == 0)
	            	printf("-> %d ", list->vertex);
	            list = list->next;
	    	}
	    	printf("-> null\n\n");
		}
	}
	printf("\t\t\t\tIN-DEGREE VALUES\n\n");
	for(v=1;v<=size;v++){
		if(graph->adjLists[v].list->deleted != 1)
			printf("\t\t   Vertex Number : %d\t In-degree : %d\n",graph->adjLists[v].list->vertex,graph->adjLists[v].list->in_degree);
	}
}

void deleteNodes(GRAPH *graph,int M,int i){
	
	NODE *list=graph->adjLists[i].list;
	if(list->deleted == 1)
		return;
	else{
		if(list->in_degree < M){
			list->deleted = 1;
			graph = updateFollowed(graph,list->vertex);
			list=list->next;
			while(list){
				graph->adjLists[list->vertex].list->in_degree--;
				deleteNodes(graph , M , list->vertex);
				list = list->next;
			}
		}
	}
}

GRAPH* updateFollowed(GRAPH *graph,int v){
	int i;
	NODE* tmp;
	for(i=1;i<=graph->numOfVertices;i++){
		tmp = graph->adjLists[i].list;
		if(tmp->deleted != 1){
			tmp = tmp->next;
			while(tmp){
				if(tmp->vertex == v){
					tmp->deleted = 1;
				}
				tmp = tmp->next;
			}
		}
	}
	return graph;
}

void findConnections(GRAPH *graph,int i,int *visited){
	
	visited[i] = 1;  // i'th node has been visited.
	NODE* tmp = graph->adjLists[i].list;
	tmp=tmp->next;
	while(tmp){
		if(visited[tmp->vertex] == 0 && tmp->deleted == 0){  // if it hasn't been visited.
			graph->adjLists[tmp->vertex].list->connection++;
			visited[tmp->vertex] = 1;
			findConnections(graph,tmp->vertex,visited);
		}
		tmp = tmp->next;
	}
}

void findInfluencers(GRAPH *graph,int X,int Y){
	int i;
	for(i=1;i<=graph->numOfVertices;i++){
		if((graph->adjLists[i].list->in_degree >= X) && (graph->adjLists[i].list->connection >= Y) && (graph->adjLists[i].list->deleted == 0))
			graph->adjLists[i].list->isInfluencer = 1;
		else
			graph->adjLists[i].list->isInfluencer = 0;
	}
}





