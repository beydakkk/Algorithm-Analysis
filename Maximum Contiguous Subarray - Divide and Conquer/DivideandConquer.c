#include<stdio.h>
#include<stdlib.h>
#define SIZE 100
#define MIN -9999
#include <math.h>
#define MAX2(x, y) (((x) > (y)) ? (x) : (y))
#define MAX3(a, b, c) ((a) <= (b)? (b) <= (c)? (c) : (b) : (a) <= (c)? (c) : (a))

typedef struct res{
	int max;
	int left;
	int right;
}RES;

RES BruteForceSolution(int N , int A[]){
	
	int max=A[0];
	int i,j,sum,right,left;
	RES res;
	
	right=0;
	left=0;
	
	for(i=0;i<N;i++){
		sum = 0;
		for(j=i;j<N;j++){
			sum += A[j];
			if(sum>= max){
				max=sum;
				right=j;
				left=i;
			}
		}	
	}
	
	res.max = max;
	res.left = left;
	res.right = right;
	
	return res;
}



int MaxSum(int arr[], int left, int mid, int right,RES *RecursiveResult){
	
	int sum = 0,i;
	RES left_sum, right_sum;
	
	left_sum.right=mid;
	right_sum.left=mid;
	
	left_sum.max=MIN;
	right_sum.max=MIN;
	
	for (i = mid; i >= left; i--) {
		sum = sum + arr[i];
		if (sum > left_sum.max){
			left_sum.left=i;	
			left_sum.max = sum;
		}
	}
	
	sum = 0;
	for (i = mid; i <= right; i++) {
		sum = sum + arr[i];
		if (sum > right_sum.max){
			right_sum.right=i;
			right_sum.max = sum;
		}
	}
	sum= MAX3(left_sum.max + right_sum.max - arr[mid], left_sum.max, right_sum.max);
	
	if(sum == left_sum.max + right_sum.max - arr[mid]){
		RecursiveResult->left=left_sum.left;
		RecursiveResult->right=right_sum.right;
	}else if(sum == right_sum.max){
		RecursiveResult->left=right_sum.left;
		RecursiveResult->right=right_sum.right;
	}else{
		RecursiveResult->left=left_sum.left;
		RecursiveResult->right=left_sum.right;
	}
	
	return sum;
}

int DivideAndConquer(int arr[], int left, int right,RES *RecursiveResult){
	
	int mid = (left + right) / 2;
	
	if (left > right){
		RecursiveResult->max=MIN;
		return MIN;
	}
	if (left == right){
		RecursiveResult->max=arr[left];
		return arr[left];
	}
			
	return MAX3(DivideAndConquer(arr, left, mid - 1,RecursiveResult) , DivideAndConquer(arr, mid + 1, right,RecursiveResult) , MaxSum(arr, left, mid, right,RecursiveResult));
}


int main(){
	
	int i,j,N,A[SIZE];

	RES BruteForceResult;
	RES *RecursiveResult=(RES*)malloc(sizeof(RES));
	
	printf("\n\n\t\tBEYDA GULER \n\t\t  19011010\n\n\n");
	printf("Please enter the size of your array --> ");
	scanf("%d",&N);
	printf("\nElements :\n\n");
	
	
	for(i=0;i<N;i++){
		printf("%d --> ",i);
		scanf("%d",&A[i]);
	}
	
	BruteForceResult = BruteForceSolution(N,A);
	RecursiveResult->max = DivideAndConquer(A,0,N-1,RecursiveResult);
	
	printf("\n\n----------------USING BRUTE FORCE SOLUTION----------------------\n");
	printf("\nFrom %d'th index to %d'th index , maximum sum is : %d",BruteForceResult.left,BruteForceResult.right,BruteForceResult.max);
	printf("\n\n--------------USING DIVIDE AND CONQUER SOLUTION-------------------\n");
	printf("\nFrom %d'th index to %d'th index ,maximum sum is : %d\n",RecursiveResult->left,RecursiveResult->right,RecursiveResult->max);
	
	free(RecursiveResult);
	return 0;
}
