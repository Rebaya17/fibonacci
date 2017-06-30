#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	unsigned long long *val;
	size_t size;
} num_t;

typedef struct {
	int n;
	num_t c;
} arg;

void num_init (num_t *num, const unsigned long long x) {
	num->val = (unsigned long long *) malloc(sizeof(unsigned long long));
	num->val[0] = x;
	num->size = 1;
}

void add (num_t *a, num_t *b) {
	const unsigned long long lim = 1000000000000000000;
	unsigned long long digi = 0, carry = 0;
	const size_t size_a = a->size;
	const size_t size_b = b->size;
	size_t ind = 0;
	
	for (; ind < size_a; ind++) {
		digi = a->val[ind] + b->val[ind] + carry;
		if (carry != 0) carry = 0;
		
		if (digi >= lim) {
			digi -= lim;
			carry = 1;
		}
		
		a->val[ind] = b->val[ind];
		b->val[ind] = digi;
	}
	
	if (ind < size_b) {
		a->size++;
		a->val = (unsigned long long *) realloc(a->val, size_b * sizeof(unsigned long long));
		a->val[ind] = b->val[ind];
		
		b->val[ind] += carry;
	}
	else if (carry) {
		b->size++;
		b->val = (unsigned long long *) realloc(b->val, b->size * sizeof(unsigned long long));
		b->val[size_b] = 1;
	}
}

void show (const num_t *x) {
	size_t ind = x->size - 1;
	
	printf("%llu", x->val[ind--]);
	
	for (; ind < x->size; ind--)
		printf("%018llu", x->val[ind]);
	
	printf("\n");
	
	free(x->val);
}

void *F (void *x) {
	int n = ((arg *)x)->n;
	
	if (n > 0) {
		int ind;
		num_t a, b;
		
		num_init(&a, 0);
		num_init(&b, 1);
		
		for (ind = 1; ind <= n; ind++)
			add(&b, &a);
		
		
		free(a.val);
		
		((arg *)x)->c = b;
	}
	
	pthread_exit(NULL);
}

int main (int argc, char **argv) {
	if (argc <= 1)
		return -1;
	
	
	pthread_t tid[argc-1];
	arg args[argc-1];
	int n, ind;
	
	for (ind = 1; ind < argc; ind++) {
		args[ind - 1].n = atoi(argv[ind]);
		
		pthread_create(&tid[ind - 1], NULL, &F, &args[ind - 1]);
	}
	
	for (ind = 1; ind < argc; ind++) {
		pthread_join(tid[ind - 1], NULL);
		n = atoi(argv[ind]);
		
		printf("F(%d) = ", n);
		
		if (n < 0) printf("No existe!\n");
		else if (n == 0) printf("1\n");
		else show(&(args[ind - 1].c));
	}
	
	return 0;
}