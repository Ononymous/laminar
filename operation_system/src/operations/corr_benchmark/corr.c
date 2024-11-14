/****************************************************************/
/*                                                              */
/*      Global include files                                    */
/*                                                              */
/****************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "autoc.h"


extern char *optarg;



char *PRED_ARGS = "f:g:";

int
main(argc,argv)
int argc;
char *argv[];
{

	char fname[255];
	char gname[255];
	int c;
	int ierr;
	double f_mu;
	double g_mu;
	double f_var;
	double g_var;
	char *ds1;
	char *ds2;
	double f;
	double g;
	double ts1;
	double ts2;
	int size;
	int s1;
	int s2;
	double rval;
	int done;
	double acc;
	double t;

	if(argc < 2)
	{
		fprintf(stderr,"usage: corr -f filename -g filename\n");
		fflush(stderr);
		exit(1);
	}

	fname[0] = 0;
	gname[0] = 0;

	while((c = getopt(argc,argv,PRED_ARGS)) != EOF)
	{
		switch(c)
		{
			case 'f':
				strcpy(fname,optarg);
				break;
			case 'g':
				strcpy(gname,optarg);
				break;
			default:
				fprintf(stderr,"unrecognized argument %c\n",
						c);
				fflush(stderr);
				break;
		}
	}

	if(fname[0] == 0)
	{
		fprintf(stderr,"usage: corr -f fname -g gname\n");
		fflush(stderr);
		exit(1);
	}

	if(gname[0] == 0)
	{
		fprintf(stderr,"usage: corr -f fname -g gname\n");
		fflush(stderr);
		exit(1);
	}
	
	ierr = InitData(fname,0,0,0,&ds1);
	if(ierr == 0)
	{
		fprintf(stderr,
	"autoc: couldn't init data for file %s\n",fname);
		fflush(stderr);
		exit(1);
	}

	ierr = InitData(gname,0,0,0,&ds2);
	if(ierr == 0)
	{
		fprintf(stderr,
	"autoc: couldn't init data for file %s\n",gname);
		fflush(stderr);
		exit(1);
	}

	ierr = MeanVar(ds1,&f_mu,&f_var);
	
	if(ierr == 0)
	{
		fprintf(stderr,
		"autoc: couldn't calc mean and variance from file %s\n",
		fname);
		fflush(stderr);
	}

	ierr = MeanVar(ds2,&g_mu,&g_var);
	
	if(ierr == 0)
	{
		fprintf(stderr,
		"autoc: couldn't calc mean and variance from file %s\n",
		gname);
		fflush(stderr);
	}
	
	done = 0;

	s1 = SizeOf(ds1);
	s2 = SizeOf(ds2);
	
	if(s1 > s2)
	{
		size = s2;
	}
	else
	{
		size = s1;
	}

	(void)Rewind(ds1);
	(void)Rewind(ds2);
	
	acc = 0;

	while(done == 0)
	{
		ierr = ReadEntry(ds1,&ts1,&f);
		if(ierr == 0)
		{
			break;
		}

		ierr = ReadEntry(ds2,&ts2,&g);
		if(ierr == 0)
		{
			break;
		}
		
		acc = acc + f*g;
		
	}
	
	rval = ((acc / size) - f_mu*g_mu) / 
		(sqrt((size-1)*f_var/size) * sqrt((size-1)*g_var/size));
	
	fprintf(stdout,"rho: %f\n",rval);
	fflush(stdout);

	t = (rval*sqrt((double)size-2.0)) / (sqrt(1.0 - rval*rval));

	fprintf(stdout,"t: %f\n",t);


	
	RemoveData(ds1);
	RemoveData(ds2);
	exit(0);
	
}

