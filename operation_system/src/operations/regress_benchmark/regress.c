#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "array.h"


#define PI (3.14159265)


/*
 * Lower tail quantile for standard normal distribution function.
 *
 * This function returns an approximation of the inverse cumulative
 * standard normal distribution function.  I.e., given P, it returns
 * an approximation to the X satisfying P = Pr{Z <= X} where Z is a
 * random variable from the standard normal distribution.
 *
 * The algorithm uses a minimax approximation by rational functions
 * and the result has a relative error whose absolute value is less
 * than 1.15e-9.
 *
 * Author:      Peter J. Acklam
 * Time-stamp:  2002-06-09 18:45:44 +0200
 * E-mail:      jacklam@math.uio.no
 * WWW URL:     http://www.math.uio.no/~jacklam
 *
 * C implementation adapted from Peter's Perl version
 */


/* Coefficients in rational approximations. */
static const double a[] =
{
	-3.969683028665376e+01,
	 2.209460984245205e+02,
	-2.759285104469687e+02,
	 1.383577518672690e+02,
	-3.066479806614716e+01,
	 2.506628277459239e+00
};

static const double b[] =
{
	-5.447609879822406e+01,
	 1.615858368580409e+02,
	-1.556989798598866e+02,
	 6.680131188771972e+01,
	-1.328068155288572e+01
};

static const double c[] =
{
	-7.784894002430293e-03,
	-3.223964580411365e-01,
	-2.400758277161838e+00,
	-2.549732539343734e+00,
	 4.374664141464968e+00,
	 2.938163982698783e+00
};

static const double d[] =
{
	7.784695709041462e-03,
	3.224671290700398e-01,
	2.445134137142996e+00,
	3.754408661907416e+00
};

#define LOW 0.02425
#define HIGH 0.97575

/*
 * http://home.online.no/~pjacklam/notes/invnorm/impl/sprouse/ltqnorm.c
 */
double
InvNormal(double p, double mu, double sigma)
{
	double q, r;
	double value;
	double x;
	double u;
	double e;

	errno = 0;

	if (p < 0 || p > 1)
	{
		errno = EDOM;
		return 0.0;
	}
	else if (p == 0)
	{
		errno = ERANGE;
		return -HUGE_VAL /* minus "infinity" */;
	}
	else if (p == 1)
	{
		errno = ERANGE;
		return HUGE_VAL /* "infinity" */;
	}
	else if (p < LOW)
	{
		/* Rational approximation for lower region */
		q = sqrt(-2*log(p));
		value = (((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
			((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
		x = value;
	}
	else if (p > HIGH)
	{
		/* Rational approximation for upper region */
		q  = sqrt(-2*log(1-p));
		value =  -(((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
			((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
		x = value;
	}
	else
	{
		/* Rational approximation for central region */
    		q = p - 0.5;
    		r = q*q;
		value =  (((((a[0]*r+a[1])*r+a[2])*r+a[3])*r+a[4])*r+a[5])*q /
			(((((b[0]*r+b[1])*r+b[2])*r+b[3])*r+b[4])*r+1);
		x = value;
	}

	if(( 0 < p)&&(p < 1))
	{
		e = 0.5 * erfc(-x/sqrt(2)) - p;
		u = e * sqrt(2*M_PI) * exp(x*x/2);
		x = x - u/(1 + x*u/2);
	}

	return(x*sigma+mu);

}


double RSquared(Array2D *x, Array2D *b, Array2D *y)
{
	Array2D *f;
	int i;
	double ss_res;
	double ss_reg;
	double ss_total;
	double y_bar;
	double temp;
	double rsq;

	f = MultiplyArray2D(x,b);
	if(f == NULL) {
		return(-1.0);
	}

	ss_res = 0;
	for(i=0; i < f->ydim; i++) {
		temp = (y->data[i*y->xdim+0] -
			   f->data[i*f->xdim+0]);
		ss_res += (temp*temp);
	}

	y_bar = 0;
	for(i=0; i < y->ydim; i++) {
		y_bar += y->data[i*y->xdim+0];
	}
	y_bar = y_bar / (double)(y->ydim);

	ss_reg = 0;
	for(i=0; i < f->ydim; i++) {
		temp = (y_bar -
			   f->data[i*f->xdim+0]);
		ss_reg += (temp*temp);
	}

	ss_total = ss_reg + ss_res;

	rsq = 1 - (ss_res / ss_total);

	FreeArray2D(f);

	return(rsq);
}

double RMSE(Array2D *x, Array2D *b, Array2D *y)
{
	Array2D *f;
	int i;
	double ss_res;
	double temp;
	double rmse;
	double count;

	f = MultiplyArray2D(x,b);
	if(f == NULL) {
		return(-1.0);
	}

	ss_res = 0;
	count = 0;
	for(i=0; i < f->ydim; i++) {
		temp = (y->data[i*y->xdim+0] -
			   f->data[i*f->xdim+0]);
		ss_res += (temp*temp);
		count++;
	}

	rmse = sqrt(ss_res/count);

	FreeArray2D(f);

	return(rmse);
}

Array2D *Residuals(Array2D *x, Array2D *b, Array2D *y)
{
	Array2D *f;
	int i;
	double temp;
	Array2D *resid;

	f = MultiplyArray2D(x,b);
	if(f == NULL) {
		return(NULL);
	}

	resid = MakeArray1D(f->ydim);
	if(resid == NULL) {
		FreeArray2D(f);
		return(NULL);
	}

	for(i=0; i < f->ydim; i++) {
		temp = (y->data[i*y->xdim+0] -
			   f->data[i*f->xdim+0]);
		resid->data[i*resid->xdim+0] = temp;
	}


	FreeArray2D(f);

	return(resid);
}

double SER(Array2D *resid, int p)
{
	Array2D *rt;
	Array2D *rtr;
	double n;
	double se;

	rt = TransposeArray2D(resid);
	if(rt == NULL) {
		return(-1);
	}
	
	rtr = MultiplyArray2D(rt,resid);
	if(rtr == NULL) {
		FreeArray2D(rt);
		return(-1);
	}
	FreeArray2D(rt);

	n = (double)resid->ydim;
	se = rtr->data[0] / (n - (double)p);

	FreeArray2D(rtr);

	return(se);
}

Array2D *CIBeta(Array2D *x, Array2D *b, Array2D *y, double alpha)
{
	double sigmasq;
	double ssq;
	Array2D *resid;
	Array2D *ci;
	Array2D *Qxx;
	Array2D *xt;
	Array2D *xtx;
	double q;
	int i;
	double temp;
	

	resid = Residuals(x,b,y);
	if(resid == NULL) {
		return(NULL);
	}

	ssq = SER(resid,x->xdim);
	if(ssq < 0) {
		FreeArray2D(resid);
		return(NULL);
	}
	FreeArray2D(resid);

	sigmasq = ((double)(x->ydim - x->xdim) / (double)x->ydim) * ssq;
	if(sigmasq < 0) {
		return(NULL);
	}

	xt = TransposeArray2D(x);
	if(xt == NULL) {
		return(NULL);
	}
	xtx = MultiplyArray2D(xt,x);
	FreeArray2D(xt);
	if(xtx == NULL) {
		return(NULL);
	}
	Qxx = InvertArray2D(xtx);
	if(Qxx == NULL) {
		printf("error xtx: \n");
		PrintArray2D(xtx);
	}
	FreeArray2D(xtx);
	if(Qxx == NULL) {
		return(NULL);
	}

	q = InvNormal(1.0-(alpha/2.0),0.0,1.0);

	/*
	 * - in col 0, + in col 1
	 */
	ci = MakeArray2D(b->ydim,2);
	if(ci == NULL) {
		FreeArray2D(Qxx);
		return(NULL);
	}

	for(i=0; i < ci->ydim; i++) {
		/*
		 * from diagonal
		 */
		temp = q * sqrt(sigmasq*Qxx->data[i*Qxx->xdim+i]);
		ci->data[i*ci->xdim+0] =
			b->data[i*b->xdim+0] - temp;
		ci->data[i*ci->xdim+1] =
			b->data[i*b->xdim+0] + temp;
	}

	FreeArray2D(Qxx);

	return(ci);
}

/*
 * normal equations are
 * X^tXB = X^ty
 *
 * B = (X^tX)^-1 * X^t * y
 */
Array1D *RegressMatrix2DSimple(Array2D *x, Array2D *y)
{
	Array1D *B;
	Array2D *xt; /* x^t */
	Array2D *xtx; /* x^t * x */
	Array2D *xtxI; /* (x^t * x)^-1 */
	Array2D *xtxIxt; /* ((x^t * x)^-1) * x^t */
	int i;

	xt = TransposeArray2D(x);
	if(xt == NULL)
	{
		return(NULL);
	}

	xtx = MultiplyArray2D(xt,x);
	if(xtx == NULL)
	{
		FreeArray2D(xt);
		return(NULL);
	}

	xtxI = InvertArray2D(xtx);
	FreeArray2D(xtx);
	if(xtxI == NULL)
	{
		FreeArray2D(xt);
		return(NULL);
	}

	xtxIxt = MultiplyArray2D(xtxI,xt);
	FreeArray2D(xt);
	FreeArray2D(xtxI);
	if(xtxIxt == NULL)
	{
		return(NULL);
	}
	B = MultiplyArray2D(xtxIxt,y);
	FreeArray2D(xtxIxt);
	if(B == NULL)
	{
		return(NULL);
	}

	return(B);
}


Array1D *RegressMatrix2D(Array2D *x, Array2D *y)
{
	return(RegressMatrix2DSimple(x,y));
}

	

	



