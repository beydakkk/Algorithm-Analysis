#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct color{
	char colorName[20];
	int colorNo;
}COLOR;

COLOR** CreateMatrix(COLOR** matrix , int N);            //Create matrix with values received from user
void PrintMatrix(COLOR** matrix, int N);  
COLOR* RotateTheRow(COLOR* row,int N);                  //Cyclic rotate operation
int IsSafe(COLOR** matrix, int row, int N);             //Compare the input row with the upper row(s). Return 0 if column values conflict, return 1 otherwise.
int Solve(COLOR** matrix, int N, int row,int mode);     //Backtracking method to avoid conflicting 

int main(){
	
	int N,i,j,mode;
	char color[20];
	COLOR** matrix = NULL;
	printf("\n\n\t\t\t   19011010\n\t\t\t  BEYDA GULER\n");
	printf("\n\n\t\t\t 1-NORMAL MODE\n\t\t\t 2-DETAILED MODE\n\n");
	printf("\t\t\tSelect Mode -> ");
	scanf("%d",&mode);
	printf("\n\t\tEnter the number of colors -> ");
	scanf("%d",&N);
	matrix = (COLOR**)malloc(N*sizeof(COLOR*));
	for(i =0; i<N;i++)
		matrix[i] = (COLOR*)malloc(N*sizeof(COLOR));
	matrix = CreateMatrix(matrix,N);
	switch(mode){
		
		case 1:
			system("cls");
			printf("\n\n\t\t  -------- NORMAL MODE --------\n\n");
			printf("\t\t\t-------------------\n");
			printf("\t\t\t  INITIAL MATRIX\n");
			printf("\t\t\t-------------------\n\n");
			PrintMatrix(matrix,N);
			printf("\t\t\t-------------------\n");
			printf("\t\t\t  ROTATED MATRIX\n");
			printf("\t\t\t-------------------\n\n");
			if(Solve(matrix,N,1,mode) == 0)
				printf("\t\t\tNO SOLUTION!!\n\n");
			else
				PrintMatrix(matrix,N);
			break;
			
			
		case 2:
			printf("\n\n\t\t\t -------- DETAILED MODE --------\n\n");
			
			if(Solve(matrix,N,1,mode) == 0)
				printf("\t\t\tNO SOLUTION!!\n\n");
			
			break;
	}

	
	return 0;
}

void PrintMatrix(COLOR** matrix, int N){
	int i,j;
	for(i = 0 ; i < N ; i++){
		printf("\t\t\t");
		for(j = 0 ; j < N ; j++)
			printf("   %s ",matrix[i][j].colorName);
		printf("\n");
	}
	printf("\n");
}

COLOR** CreateMatrix(COLOR** matrix , int N){
	
	int i,j,k;
	for(i = 0 ; i < N ; i++){
		printf("\n\t\t\t  ***%dst row***\n",i+1);
		for(j = 0 ; j < N ; j++){
			printf("\n\t\t\t%dst column ---> ",j+1);
			scanf("%s",matrix[i][j].colorName);
			if(i == 0)
				matrix[i][j].colorNo = j+1;    //If it is first row then generate color numbers in an ascending order.
			else{                              //If it is not first row 
				k=0;
				while(strcmp(matrix[0][k].colorName,matrix[i][j].colorName) != 0)  //Compare the color name with first row
					k++;
				matrix[i][j].colorNo = matrix[0][k].colorNo;    //Apply the same color number for the same color name
			}
		}
	}
	return matrix;
}

COLOR* RotateTheRow(COLOR* row,int N){  //Cyclic rotate operation
	int i;
	COLOR tmp = row[N-1];
	for(i = N-1 ; i > 0 ; i--)
		row[i] = row[i-1]; 
	
	row[0] = tmp;
	return row;
}

int IsSafe(COLOR** matrix, int row, int N){    //Compare the input row with the upper row(s). Return 0 if column values conflict, return 1 otherwise.
	int i,j;
	for(i = 0; i<row ; i++){
		for(j = 0 ; j<N ; j++){
			if(matrix[row][j].colorNo == matrix[i][j].colorNo)
				return 0;
		}
	}
	return 1;
}


int Solve(COLOR** matrix, int N, int row,int mode){
	int i;
	for(i=0;i<N;i++){
		if(IsSafe(matrix,row,N)){                         // IF IT IS SAFE
			if(row!=N-1){     //If it is not the last row
				if(Solve(matrix,N,row+1,mode))    //Call solve function for the next row.
					return 1;
				else{  // If the next row doesn't return 1, then rotate the current row again.
					matrix[row]=RotateTheRow(matrix[row],N);
					if(mode==2)
						PrintMatrix(matrix,N);
				}	
			}else   //If last row is safe then return 1
				return 1;
		}else{                                           // IF IT IS NOT SAFE
			matrix[row]=RotateTheRow(matrix[row],N);     
			if(mode==2)
				PrintMatrix(matrix,N);
		}
	}
	return 0;
}




