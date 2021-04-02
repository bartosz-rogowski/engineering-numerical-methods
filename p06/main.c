#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mgmres.h"
#include "mgmres.c"

double TOL = 1e-08;
int itr_max = 500;
double delta = 0.1;

double ro(double x, double y, int n, int przypadek)
{
	switch(przypadek)
	{
		case 1:
		{
			return 0.0;
		} break;

		case 2:
		{
			return exp(-pow(x - 0.25*delta*n, 2)/pow(delta*n/10, 2)
				-pow(y - 0.5*delta*n, 2)/pow(delta*n/10, 2))
				-exp(-pow(x - 0.75*delta*n, 2)/pow(delta*n/10, 2)
				-pow(y - 0.5*delta*n, 2)/pow(delta*n/10, 2));
		}
	}
}


double e(double l, double nx, double eps1, double eps2)
{
	int j = l/(nx+1);
	int i = l - j*(nx+1);
	if(i > nx/2)
		return eps2;
	return eps1;
}


double* fun(double V1, double V2, double V3, double V4, double eps1, double eps2, int nx, int ny, int przypadek_ro, int zapis)
{
	int N = (nx + 1)*(ny + 1);
	double *a = calloc(5*N, sizeof(double));
	int *ja = calloc(5*N, sizeof(int));
	int *ia = calloc(N+1, sizeof(int));

	for(int i = 0; i < N+1; i++)
	{
		ia[i] = -1;
	}

	double *b = calloc(N, sizeof(double));
	double *V = calloc(N, sizeof(double));

	FILE* fp1 = fopen("a.dat", "a+");
	FILE* fp2 = fopen("b.dat", "a+");


	int k = -1;
	for(int l = 0; l < N; l++)
	{
		int brzeg = 0; //wskaznik polozenia
		double Vb = 0; //potencjal na brzegu
		int j = l/(nx+1);
		int i = l - j*(nx+1);

		//lewy brzeg
		if(i == 0)
		{
			brzeg = 1;
			Vb = V1;
		}

		//gorny brzeg
		if(j == ny)
		{
			brzeg = 1;
			Vb = V2;
		}

		//prawy brzeg
		if(i == nx)
		{
			brzeg = 1;
			Vb = V3;
		}

		//dolny brzeg
		if(j == 0)
		{
			brzeg = 1;
			Vb = V4;
		}

		b[l] = -ro(i*delta, j*delta, nx, przypadek_ro);

		if(brzeg == 1)
			b[l] = Vb;

		ia[l] = -1;

		//lewa skrajna przekatna
		if(l-nx-1 >= 0 && brzeg == 0)
		{
			k++;
			if(ia[l] < 0)
				ia[l] = k;
			a[k] = e(l, nx, eps1, eps2)/pow(delta, 2);
			ja[k] = l-nx-1;
		}

		//poddiagonala
		if(l-1 >= 0 && brzeg == 0)
		{
			k++;
			if(ia[l] < 0)
				ia[l] = k;
			a[k] = e(l, nx, eps1, eps2)/pow(delta, 2);
			ja[k] = l-1;
		}

		//diagonala
		k++;
		if(ia[l] < 0)
			ia[l] = k;
		if(brzeg == 0)
			a[k] = -(2.0*e(l, nx, eps1, eps2)+e(l+1, nx, eps1, eps2)+e(l+nx+1, nx, eps1, eps2))/pow(delta, 2);
		else
			a[k] = 1;
		ja[k] = l;

		//naddiagonala
		if(l < N && brzeg == 0)
		{
			k++;
			a[k] = e(l+1, nx, eps1, eps2)/pow(delta, 2);
			ja[k] = l+1;
		}

		//prawa skrajna przekatna
		if(l < N-nx-1 && brzeg == 0)
		{
			k++;
			a[k] = e(l+nx+1, nx, eps1, eps2)/pow(delta, 2);
			ja[k] = l+nx+1;
		}

		//zapis do plikow
		if(zapis == 1)
			fprintf(fp2, "%d %d %d %g\n", l, i, j, b[l]);
	}

	if(zapis == 1)
	{
		for(int k = 0; k < 5*N; k++)
		{
			fprintf(fp1, "%d %g\n", k, a[k]);
		}
	}

	fclose(fp1);
	fclose(fp2);
	
	int nz_num = k+1; // ilosc niezerowych elementow (1 element ma indeks 0)
	ia[N]= nz_num;

	pmgmres_ilu_cr(N, nz_num, ia, ja, a, V, b, itr_max, itr_max, TOL, TOL);

	free(a);
	free(b);
	free(ja);
	free(ia);
	return V;
}

int main()
{
	fun(10, -10, 10, -10, 1.0, 1.0, 4, 4, 1, 1);

	FILE* fp = fopen("mapy.dat", "w+");
	double* wynik = NULL;

	wynik = fun(10, -10, 10, -10, 1.0, 1.0, 50, 50, 1, 0);
	for(int i = 0; i <= 50; i++)
	{
		for(int j = 0; j <= 50; j++)
		{
			fprintf(fp, "%g %g %g\n", i*delta, j*delta, wynik[j*51+i]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n\n");

	free(wynik);
	wynik = fun(10, -10, 10, -10, 1.0, 1.0, 100, 100, 1, 0);
	for(int i = 0; i <= 100; i++)
	{
		for(int j = 0; j <= 100; j++)
		{
			fprintf(fp, "%g %g %g\n", i*delta, j*delta, wynik[j*101+i]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n\n");

	
	free(wynik);
	wynik = fun(10, -10, 10, -10, 1.0, 1.0, 200, 200, 1, 0);
	for(int i = 0; i <= 200; i++)
	{
		for(int j = 0; j <= 200; j++)
		{
			fprintf(fp, "%g %g %g\n", i*delta, j*delta, wynik[j*201+i]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n\n");
	fclose(fp);
	free(wynik);

	FILE* fptr = fopen("mapy2.dat", "w+");

	wynik = fun(0, 0, 0, 0, 1.0, 1.0, 100, 100, 2, 0);
	for(int i = 0; i <= 100; i++)
	{
		for(int j = 0; j <= 100; j++)
		{
			fprintf(fptr, "%g %g %g\n", i*delta, j*delta, wynik[j*101+i]);
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "\n\n");
	free(wynik);

	wynik = fun(0, 0, 0, 0, 1.0, 2.0, 100, 100, 2, 0);
	for(int i = 0; i <= 100; i++)
	{
		for(int j = 0; j <= 100; j++)
		{
			fprintf(fptr, "%g %g %g\n", i*delta, j*delta, wynik[j*101+i]);
		}
		fprintf(fptr, "\n");
	}
	free(wynik);
	fprintf(fptr, "\n\n");

	wynik = fun(0, 0, 0, 0, 1.0, 10.0, 100, 100, 2, 0);
	for(int i = 0; i <= 100; i++)
	{
		for(int j = 0; j <= 100; j++)
		{
			fprintf(fptr, "%g %g %g\n", i*delta, j*delta, wynik[j*101+i]);
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "\n\n");
	fclose(fptr);
	free(wynik);
	return 0;
}