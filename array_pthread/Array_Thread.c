#include <pthread.h>
#include <stdio.h>

/* Creo la struttura in cui sono contenute
 * tutte le info riguardanti i Threads */
struct thread_info
{
  int *v;
  long int lim;
  long int start;
  long int sum;
  int dim;
};

/* Apro la funzione che ospita le routine dei
 * threads creati */
void* calcsum (void *arguments)
{
  struct thread_info *p = (struct thread_info *) arguments;
int j=0;
		do
		{
			p->sum += p->v[(p->start)];
			p->start++;
			j++;
		}while (j < (p->lim));
return NULL;
}

void* calcsum2 (void *arguments)
{
  struct thread_info *p = (struct thread_info *) arguments;
			do
			{
				p->sum += p->v[(p->start)];
				p->start++;
			}while ((p->start) < (p->dim));
		
  return NULL;
}

/* Da qua inizia il main che crea i threads, 
 * inizializza i valori nelle loro strutture
 * e crea le basi e i dati per far si che la 
 * routine possa essere svolta correttamente */
int main ()
{
// Dichiarazioni variabili locali
 int n, lim, i = 0;
 long int tot = 0;

/* Inserimento delle dimensioni, ottimizzazione
 * Per la gestione della routine per 4 thread */
 printf ("Inserisci la dimensione n dell'array: ");
 scanf ("%d", &n);
 
// Dichiarazione dell'array con dimensioni n    
 int a[n];

//Riempimento dell'array con numeri interi sequenziali  
  do
    {
      a[i] = (i + 1);
      i++;
    }
  while (i < n);

// Calcolo quante celle deve sommare ciascun thread
 lim = (n / 4);
	if(n%4!=0)
		lim++;

// Dichiaro gli id e gli argomenti dei 4 threads
  pthread_t thread1_id;
  pthread_t thread2_id;
  pthread_t thread3_id;
  pthread_t thread4_id;
  struct thread_info thread1_arg;
  struct thread_info thread2_arg;
  struct thread_info thread3_arg;
  struct thread_info thread4_arg;

/* Inserisco i dati necessari dentro le strutture
 * contenenti gli argomenti per le routine e 
 * creo i thread facendo partire l'esecuzione */
  thread1_arg.v = &a[0];
  thread1_arg.lim = lim;
  thread1_arg.start = lim * 0;
  thread1_arg.sum = 0;
  thread1_arg.dim=n;
  pthread_create (&thread1_id, NULL, &calcsum, &thread1_arg);

  thread2_arg.v = &a[0];
  thread2_arg.lim = lim;
  thread2_arg.start = lim * 1;
  thread2_arg.sum = 0;
  thread2_arg.dim=n;
  pthread_create (&thread2_id, NULL, &calcsum, &thread2_arg);

  thread3_arg.v = &a[0];
  thread3_arg.lim = lim;
  thread3_arg.start = lim * 2;
  thread3_arg.sum = 0;
  thread3_arg.dim=n;
  pthread_create (&thread3_id, NULL, &calcsum, &thread3_arg);

  thread4_arg.v = &a[0];
  thread4_arg.lim = lim;
  thread4_arg.start = lim * 3;
  thread4_arg.sum = 0;
  thread4_arg.dim=n;
  pthread_create (&thread4_id, NULL, &calcsum2, &thread4_arg);

/* Inserisco le pthread_join per far si che il 
 * main attenda che tutti i thread abbiano
 * completato le loro routine */
  pthread_join (thread1_id, NULL);
  pthread_join (thread2_id, NULL);
  pthread_join (thread3_id, NULL);
  pthread_join (thread4_id, NULL);

  tot = (thread1_arg.sum) + (thread2_arg.sum) + (thread3_arg.sum) + (thread4_arg.sum);
  printf ("La somma dei valori nell'Array è: %li\n", tot);

  return 0;
}
