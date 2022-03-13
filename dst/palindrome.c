#include<stdio.h>
int main()
{
	int n, n_copy, m = 0 ; // reverse n and store in m
	scanf("%d",&n);
	n_copy = n;
	while (n_copy) {
		m = m * 10 + n_copy % 10;
		n_copy /= 10;
	}
	if(m == n){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
