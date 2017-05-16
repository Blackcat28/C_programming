#include <pthread.h>
#include <stdio.h>

int main()
{
int i=0, n=0; 
long int sum=0;
	do
	{
		printf("Inserisci le dimensioni dell'array: ");
		scanf("%d", &n);
	}while(n<=0);

int v[n];

	do
	{			
		v[i]=(i+1);
		sum += v[i];
		i++;
	}while(i<n);

printf("La somma dei valori nell'Array è: %li\n", sum);
return 0;
}
