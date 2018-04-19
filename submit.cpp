/**
 * Life in Cilk
 * Antonio Javier Samaniego Jurado
 * Pablo Martin Garcia
 *
 **/

#include "life.h"
#include <time.h>
#include <math.h> 
#include <fstream>
#include <sstream>

using namespace std;

//Generate the life matrix
void genlife(int *a, unsigned int n)
{
	int i,j;
	int k1,k2,r,c;
	int initial[25]={1,0,0,0,1, 1,0,0,1,1, 0,0,0,0,1, 1,1,1,0,0, 1,0,1,0,1};
	int cell_value;

	r=floor(n/3);
	c=floor(n/5);

	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			k1=j+r;
			k2=i+c;
			cell_value=initial[i*5+j];
			a[k2*n+k1]=cell_value;
		}
	}

	printArray(a,n);

}

//Read the life matrix from a file
void readlife(int *a, unsigned int n, char *filename)
{	
	int i,j;
	int row_size_char=2*n+2;
	char *row;
	FILE* pFile;

	int m;

	pFile=fopen(filename,"r");

	row=(char*)malloc(sizeof(char)*(row_size_char));

	if (pFile==NULL){
		perror("Error reading file");
		exit(-1);
	}
	else{

		for(i=0;i<n;i++){
			fgets(row,row_size_char,pFile);


			for(j=0;j<n;j++){
				a[i*n+j]=atoi(&row[2*j]);
			}
		}

	}

}

//Life function
void life(int *a, unsigned int n, unsigned int iter, int *livecount)
{

	int i,j;
	int k1,k2;
	int lives,pos_live;
	unsigned int num_neighbors;
	int value;
	clock_t t1,t2;
	float diff;
	int a_aux[n*n];
	int k;
	

		t1=clock();

		for(i=0;i<iter;i++){


			for (k=0;k<n*n;k++){
					a_aux[k]=a[k];
			}

			cilk_for(k2=0;k2<n;k2++){
				for(k1=0;k1<n;k1++){
								
					value=cellValue(k2,k1,n,a_aux);
					num_neighbors=numNeighbors(k2,k1,n,a_aux);

					//Rules of the game:
					if (value==0){
						if(num_neighbors==3){
							a[k2*n+k1]=1;
						}
					}else if (value==1){

						if (num_neighbors==2 || num_neighbors==3){
							a[k2*n+k1]=1;
						}else if (num_neighbors<2 || num_neighbors>3){
							a[k2*n+k1]=0;							
						}
					
					}
					
				}

			}

			//Debug:
			#if DEBUG == 1

				if(iter%10==0){
					if(((i+1)%(iter/10))==0){
						lives=countlive(a,n);
						pos_live=((i+1)/(iter/10))-1;
						livecount[pos_live]=lives;
					}
				}

			#endif
		}

		t2=clock();

		diff=((t2-t1)/CLOCKS_PER_SEC);

}


int cellValue(int i, int j, int size, int * arr)  
{
	int value;

	value=arr[size*i+j];

	return value;
}

int numNeighbors(int i, int j, int size, int * arr)
{
	
	int *val_neighbor = (int *)malloc(sizeof(int)*8);

	int r,r_up,r_down;
	int c,c_right,c_left;
	int k;
	int num_neighbors=0;
	

		//Horizontal neighbors:
		r=i;
		c_right=(j+1+size)%size;
		c_left=(j-1+size)%size;
		val_neighbor[0]=cellValue(r,c_right,size,arr); //Right neighbor.
		val_neighbor[1]=cellValue(r,c_left,size,arr); //Left neighbor.

		//Vertical neighbors:
		c=j;
		r_up=(i-1+size)%size;
		r_down=(i+1+size)%size;
		val_neighbor[2]=cellValue(r_up,c,size,arr); //Up neighbor.
		val_neighbor[3]=cellValue(r_down,c,size,arr); //Down neighbor.

		//Diagonal neighbors:
		val_neighbor[4]=cellValue(r_up,c_right,size,arr); //Up right neighbor.
		val_neighbor[5]=cellValue(r_down,c_right,size,arr); //Down right neighbor.
		val_neighbor[6]=cellValue(r_up,c_left,size,arr); //Up left neighbor.
		val_neighbor[7]=cellValue(r_down,c_left,size,arr); //Down left neighbor.

		for(k=0;k<8;k++){
			num_neighbors=num_neighbors+val_neighbor[k];
		}

	return num_neighbors;
}

//Prints the generated matrix into a .txt file:
void printArray(int * arr, int size)
{
	int i,j;

	ofstream myfile; 

	myfile.open ("genlife.txt");

	for (i=0;i<size;i++){
		for (j=0;j<size;j++){
			myfile << arr[i*size+j] << " " ;   
		}
		myfile << "\n";
	}

	myfile.close();


}
