/* Intentionally cumbersome program to test simulator performance. */

#define N 30


unsigned int fib(unsigned int n)
{
	if(n < 2) return 1;

	return fib(n-1) + fib(n-2);
}


unsigned int fib_array[N];

int main(void)
{
	unsigned int i;
	for(i = 0; i < N; i++) {
		fib_array[i] = fib(i);
	}

	return 0;
}



