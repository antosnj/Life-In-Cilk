#include<stdio.h>
#include<iostream>
#include <math.h> 

using namespace std;


int main()
{
	int n=5;
	int *a;
	int i,j;
	int r,c,k1,k2;
	int cell[9]={1,1,1,0,1,1,0,0,0};

	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			cout << cell[i*3+j] << " ";
		}
		cout << endl;
	}

	cout << "\n";

	a=(int*)malloc(sizeof(int)*(n*n));

	int aux=0;
	for(i=0;i<n*n;i++){
		a[i]=aux;
		aux++;
	}


	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			cout << a[i*n+j] << " ";
		}
		cout << endl;
	}

	r=floor(n/3);
	cout << "\nr: " << r << endl;
	c=floor(n/5);
	cout << "c: " << c << endl;

	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			k1=j+r;
			k2=i+c;
			a[k2*n+k1]=cell[3*i+j];
		}
	}

	cout << "\n";

	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			cout << a[i*n+j] << " ";
		}
		cout << endl;
	}

}