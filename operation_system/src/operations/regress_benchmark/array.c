#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "array.h"


Array2D *MakeArray2D(int rows, int cols)
{
	Array2D *a;

	a = (Array2D *)malloc(sizeof(Array2D));
	if(a == NULL)
	{
		return(NULL);
	}

	a->data = (double *)malloc(rows*cols*sizeof(double));
	if(a->data == NULL) {
		free(a);
		return(NULL);
	}

	a->xdim = cols;
	a->ydim = rows;

	return(a);
}

void FreeArray2D(Array2D *a)
{
	if(a->data != NULL) {
		free(a->data);
	}
	free(a);
	return;
}

void PrintArray2D(Array2D *a)
{
	int i;
	int j;

	for(i=0; i < a->ydim; i++)
	{
//		fprintf(stdout,"\t[");
		for(j=0; j < a->xdim; j++)
		{
			if(j < a->xdim-1)
			{
				fprintf(stdout,"%f ",a->data[i*a->xdim+j]);
			}
			else
			{
//				fprintf(stdout,"%f]\n",a->data[i*a->xdim+j]);
				fprintf(stdout,"%f\n",a->data[i*a->xdim+j]);
			}
		}
	}

	return;
}

Array2D *CopyArray2D(Array2D *a)
{
	Array2D *b;
	int i;

	b = MakeArray2D(a->ydim,a->xdim);
	if(b == NULL)
	{
		return(NULL);
	}

	for(i=0; i < ((a->xdim)*(a->ydim)); i++)
	{
		b->data[i] = a->data[i];
	}

	return(b);
}

Array2D *TransposeArray2D(Array2D *a)
{
	Array2D *t;
	int i;
	int j;

	t = MakeArray2D(a->xdim,a->ydim);
	if(t == NULL)
	{
		return(NULL);
	}

	for(i=0; i < a->ydim; i++)
	{
		for(j=0; j < a->xdim; j++)
		{
			t->data[j*t->xdim+i] =
				a->data[i*a->xdim+j];
		}
	}

	return(t);
}



#define EL(a,i,j,r) ((a)[(i)*(r)+(j)])

/*
 * adapted from Gauss-Jordon w/o pivoting
 */
Array2D *InvertArray2D(Array2D *a)
{
	int i;
	int j;
	int row;
	int pivot;
	double *temp;
	double *orig_a;
	Array2D *or_mio;
	double *new_a;
	Array2D *na_mio;
	double mult;
	double *new_b;
	int r;
	int c;
	double *inverse;
	Array2D *in_mio;
	double *new_inverse;
	Array2D *nin_mio;
	double t;
	Array2D *result;

	/*
	 * rows and columns from a
	 */
	r = a->ydim;
	c = a->xdim;

	/*
	 * allocate space for build
	 */
	na_mio = MakeArray2D(r,c);
	if(na_mio == NULL)
	{
		fprintf(stderr,"no space for swap in Invert2D\n");
		fflush(stderr);
		exit(1);
	}
	new_a = (double *)na_mio->data;

	or_mio = MakeArray2D(r,c);
	if(or_mio == NULL)
	{
		fprintf(stderr,"no space for orig in Invert2D\n");
		fflush(stderr);
		exit(1);
	}
	orig_a = (double *)or_mio->data;

	/*
	 * get space for the identity to make into the inverse
	 */
	in_mio = MakeArray2D(r,c);
	if(in_mio == NULL)
	{
		fprintf(stderr,"no space for inverse in Invert2D\n");
		fflush(stderr);
		exit(1);
	}
	inverse = (double *)in_mio->data;
	memset(inverse,0,r*c*sizeof(double));

	/*
	 * the idea here is to run the identity matrix into the inverse
	 * while turninhg a into the identity matrix
	 */
	for(pivot = 0; pivot < r; pivot++)
	{
		EL(inverse,pivot,pivot,c) = 1.0;
	}

	nin_mio = MakeArray2D(r,c);
	if(nin_mio == NULL)
	{
		fprintf(stderr,"no space for new inverse in Invert2D\n");
		fflush(stderr);
		exit(1);
	}
	new_inverse = (double *)nin_mio->data;

	for(i=0; i < (r*c); i++)
	{
		orig_a[i] = a->data[i];
	}

	for(pivot=0; pivot < r; pivot++)
	{
		for(row=0; row < r; row++)
		{
			t = EL(orig_a,pivot,pivot,c);
			/*
			 * check here for 0 pivot element
			 */
			if(t == 0)
			{
				FreeArray2D(or_mio);
				FreeArray2D(na_mio);
				FreeArray2D(in_mio);
				FreeArray2D(nin_mio);
				return(NULL);
			}
			mult = EL(orig_a,row,pivot,c) / t;

			if(row == pivot)
			{
				for(j=0; j < c; j++)
				{
					EL(new_a,row,j,c) =
						EL(orig_a,row,j,c) /
						   EL(orig_a,pivot,pivot,c);
					EL(new_inverse,row,j,c) =
						EL(inverse,row,j,c) /
                                                   EL(orig_a,pivot,pivot,c);
				}
			}
			else
			{
				for(j=0; j < c; j++)
				{
					EL(new_a,row,j,c) =
						EL(orig_a,row,j,c) - mult *
							EL(orig_a,pivot,j,c);
					EL(new_inverse,row,j,c) =
						EL(inverse,row,j,c) - mult *
                                                        EL(inverse,pivot,j,c);
				}
			}
		}

		temp = new_a;
		new_a = orig_a;
		orig_a = temp;
		or_mio->data = (void *)orig_a;
		na_mio->data = (void *)new_a;

		temp = new_inverse;
		new_inverse = inverse;
		inverse = temp;
		in_mio->data = (void *)inverse;
		nin_mio->data = (void *)new_inverse;
	}

	FreeArray2D(or_mio);
	FreeArray2D(na_mio);
	FreeArray2D(nin_mio);

	result = MakeArray2D(c,r);
	if(result == NULL)
	{
		FreeArray2D(in_mio);
		return(NULL);
	}
	else
	{
		/*
		 * throw away the mio in the array
		 */
		free(result->data);
		/*
		 * keep the inverse mio
		 */
		result->data = (double *)in_mio->data;
		in_mio->data = NULL;
		free(in_mio);
		return(result);
	}

}

Array2D *MultiplyArray2D(Array2D *a, Array2D *b)
{
	Array2D *c;	/* result */
	int i;
	int j;
	int k;
	double acc;
	int new_r;
	int new_c;
	double *data;

	if(a->xdim != b->ydim)
	{
		return(NULL);
	}

	new_r = a->ydim;
	new_c = b->xdim;

	c = MakeArray2D(new_r,new_c);
	if(c == NULL)
	{
		return(NULL);
	}

	data = c->data;

	for(i=0; i < new_r; i++)
	{
		for(j=0; j < new_c; j++)
		{
			acc = 0.0;
			for(k=0; k < a->xdim; k++)
			{
				acc += (a->data[i*(a->xdim)+k] *
					b->data[k*(b->xdim)+j]);
			}
			data[i*new_c+j] = acc;
		}
	}

	return(c);
}





