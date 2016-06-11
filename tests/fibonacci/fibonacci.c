/* Intentionally cumbersome program to test simulator performance. */


#define N 10


unsigned int fib_array[N];

unsigned int fib(unsigned int n)
{
	if(n < 2) return 1;

	return fib(n-1) + fib(n-2);
}

int main(void)
{
	unsigned int i;
	for(i = 0; i < N; i++) {
		fib_array[i] = fib(i);
	}

//	return 2;
	return fib_array[N-1];
}


