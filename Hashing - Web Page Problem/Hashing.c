#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LINK_SIZE 100
#define WORD_SIZE 100
#define WORD_LENGTH	100

typedef struct hash{
  unsigned long long int key;
  char value[100];
  char links[50][100];
  int count;
}HASH;

typedef struct data{
	char word[WORD_LENGTH][WORD_SIZE];
	int wordCounter;
	char link[LINK_SIZE];
}DATA;

int readFile(DATA *data,FILE* fp);
unsigned long long int hornerKey(char* string);
unsigned long long int power(int a, int b);
HASH* createHash(HASH* hash,DATA *data,int keyCounter,float loadFactor);
int searchWord(HASH* hash,char search[],int hashTableSize);
void listLinks(HASH* hash,char search[],int hashTableSize);
void printTable(int hashTableSize,HASH* hash);
int isPrime(int n);
int nextPrime(int N);

int main(){
	
    FILE* file;
    int keyCounter,hashTableSize;
    float loadFactor;
	char str[100];
	char search[50];
	int run=1,slct;
	float tmp;
	
	printf("\n\n\t\tBEYDA GULER \n\t\t  19011010\n\n\n");
	printf("Please enter the load factor --> ");
	scanf("%f",&loadFactor);
	
	DATA* data =NULL;
	data = (DATA*)malloc(100*sizeof(DATA));
	
	HASH* hash ;        // allocating hash table
	
	file = fopen("sample.txt","r");
    if(file != NULL)
    	keyCounter = readFile(data,file);
	else
		printf("Couldn't find file...\n");
	
	tmp = 1.0*keyCounter / loadFactor;
	hashTableSize = nextPrime((int)tmp);
	
	hash = createHash(hash,data,keyCounter,loadFactor);
	
	while(run){
		system("cls");
		printf("\n\n\t\tBEYDA GULER \n\t\t  19011010\n\n\n");
		printf("\n1.Normal Mode\n2.Detailed Mode\n3.EXIT\n\n");
		printf("Select a mode --> ");
		scanf("%d",&slct);
		printf("\n\n\n");
		switch(slct){
			
			case 1:     //normal mode
				printf("Enter word(s) to search -->");
				
				scanf(" %[^\n]s",search);
				listLinks(hash,search,hashTableSize);
				
				printf("\n\nPress any key to continue!");
				getch();
				break;
			case 2:      //detailed mode
			
				printf("Length of the table --> %d\n",hashTableSize);
				printTable(hashTableSize,hash);
				printf("\n\nPress any key to continue!\n");
				getch();
				break;
			case 3:     //exit
				run = 0;
				break;
		}
		
	}
	
	free(hash);
	free(data);	
	return 0;
  }
  
int readFile(DATA *data,FILE* file){
	
	int i=0,j=0,keyCounter=0;
	char c;
	char buff[50];	
	int a,b,flag=0;
	
	while(fscanf(file,"%s",buff)!=EOF){
		data[i].wordCounter=0;
		if(!strncmp(buff,"http",4)){
			strcpy((data[i].link),buff);
			i++;
			j=0;
			
		}else{
			strcpy(data[i-1].word[j],buff);
			data[i-1].wordCounter++;
			j++;
			keyCounter++;
		}
	}
	
	
	fclose(file);
	return keyCounter;
}

HASH* createHash(HASH* hash,DATA *data,int keyCounter,float loadFactor){
	
	int i=0,j,k,count=1;
	float tmp = 1.0*keyCounter / loadFactor;
	int hashTableSize = nextPrime((int)tmp);
	int index;
	unsigned long long int hKey;
	hash = (HASH*)malloc(hashTableSize*sizeof(HASH)); 
	
	for(j=0;j<hashTableSize;j++)
		hash[j].key = 9999;
	
	
	while(!strncmp(data[i].link,"http",4)){
		
		j=0;
		while(j<data[i].wordCounter){
			hKey = hornerKey(data[i].word[j]);
			index = hKey % hashTableSize;
			
			while(hash[index].key!=9999 && strcmp(hash[index].value,data[i].word[j])!=0){
				index=(index+1) % hashTableSize;
				count++;
			}
			hash[index].count=count;
			hash[index].key=hKey;
			strcpy(hash[index].value,data[i].word[j]);
			
			k=0;
			while(!strncmp(hash[index].links[k],"http",4)){
				k++;
			}
			strcpy(hash[index].links[k],data[i].link);	
			j++;
		}
		count=1;
		k=0;
		
		i++;
	}
		
	j=0;

	
	return hash;
}

unsigned long long int hornerKey(char* string){
  
  int R=31;    // horner's parameter
  int L = strlen(string);    // string's length
  unsigned long long int key = 0;
  int i;
  for(i=0; i<L; i++){
  	if(string[i]>91)
    	key = key + power(R,L-i-1) * (string[i] - 'a' + 1); // function for horner's method
    else
    	key = key + power(R,L-i-1) * (string[i] - 'A' + 1);
  }
  return key;
}

int searchWord(HASH* hash,char search[],int hashTableSize){
	
	int i=0;
	unsigned long long int key;
	do{
		key = hornerKey(search);
		key = (key+i)%hashTableSize;
		i++;
	}while(strcmp(search, hash[key].value) != 0 && hash[key].key!=9999);
	
	if(hash[key].key != 9999)
		return key;
	else
		return -1;
}
  
void listLinks(HASH* hash,char search[],int hashTableSize){
	int i,j;
	int len;
	int key,key1,key2;
	int flag=0;
	int eq;
	
	len = strlen(search);
	char *p = strtok(search," ");
	char *words[3] = {0};
	
	while(p!= NULL){
		words[i++] = p;
		p = strtok(NULL," ");
	}
	
	if(words[1]==NULL){
		
		key = searchWord(hash,words[0],hashTableSize);
		if(key!=-1){
			printf("LINKS : \n");
			i=0;
			while(!strncmp(hash[key].links[i],"http",4)){
				printf("      %s\n",hash[key].links[i]);
				i++;
			}
				
		}
		else{
			printf("The word you are searching doesn't exists in this document!\n\n");
		}
	}
	else{
		if(strcmp(words[1],"or")==0){
			key1=searchWord(hash,words[0],hashTableSize);
			key2=searchWord(hash,words[2],hashTableSize);
			
			i=0;
			while(!strncmp(hash[key1].links[i],"http",4)){
				
				printf("      %s\n",hash[key1].links[i]);
				i++;
			}

			j=0;
			while(!strncmp(hash[key2].links[j],"http",4)){
				i=0;
				while(!strncmp(hash[key1].links[i],"http",4)){
					if(strcmp(hash[key1].links[i],hash[key2].links[j])!=0)
						eq=0;
					i++;
				}
				if(eq==0){
					printf("      %s\n",hash[key2].links[j]);
				}
				j++;
			}
		}
		else if(strcmp(words[1],"and")==0){
			key1=searchWord(hash,words[0],hashTableSize);
			key2=searchWord(hash,words[2],hashTableSize);
			
			i=0;
			while(!strncmp(hash[key1].links[i],"http",4)){
				j=0;
				while(!strncmp(hash[key2].links[j],"http",4)){
					if(strcmp(hash[key1].links[i],hash[key2].links[j])==0){
						printf("      %s\n",hash[key2].links[j]);
						flag++;
					}
						
					j++;
				}
				i++;
			}
			if(flag==0)
				printf("No common links!\n");
		}
	}
	
	
	
}



void printTable(int hashTableSize,HASH* hash){
	int i,k;
	
	for(i=0;i<hashTableSize;i++){
		if(hash[i].key!=9999){
			
			printf("WORD ->%s\n",hash[i].value);
			printf("Inserted at %dth step\n",hash[i].count);
			k=0;
			printf("LINKS :\n");
			while(!strncmp(hash[i].links[k],"http",4)){
				
				printf("%d - %s\n",k+1,hash[i].links[k]);
				k++;
			}
			printf("\n");
		}
			
		
	}
	
}


unsigned long long int power(int a, int b){
	if(b==0) 
    	return 1;
  	
  	else if(b%2==0) 
    	return power(a,b/2) * power(a,b/2);
	
  	else
   		return a * power(a,b/2) * power(a,b/2);
  
}

int isPrime(int n){
	int i;
    if (n <= 1)  return 0;
    if (n <= 3)  return 1;
   
    if (n%2 == 0 || n%3 == 0) return 0;
   
    for (i=5; i*i<=n; i=i+6)
        if (n%i == 0 || n%(i+2) == 0)
           return 0;
   
    return 1;
}

int nextPrime(int N){
 
    if (N <= 1)
        return 2;
 
    int prime = N;
    int found = 0;
    while (!found) {
        prime++;
 
        if (isPrime(prime))
            found = 1;
    }
 
    return prime;
}
