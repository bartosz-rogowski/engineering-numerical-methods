#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define j1 50

int nx = 400;
int ny = 90;
int i1 = 200;
int i2 = 210;
double delta = 0.01;
double sigma = 0.1;
double x_A = 0.45;
double y_A = 0.45;
int IT_MAX = 10000; //wartosc dobrana empirycznie

//zwraca wartosc w punkcie, w ktorym najwiekszy jest modul predkosci
double find_max(double** tab1, double** tab2)
{
	double temp = sqrt(pow(tab1[0][0], 2) + pow(tab2[0][0], 2));
	for(int i = 0; i <= nx; i++)
	{
		for(int j = 0; j <= ny; j++)
		{
			double val = sqrt(pow(tab1[i][j], 2) + pow(tab2[i][j], 2));
			if(temp < val)
				temp = val;
		}
	}
	return temp;
}

int zastawka(int i, int j)
{
	if(i >= i1 && i <= i2 && j<= j1)
		return 1;
	return 0;
}

void iteracje(double D, double** v_x, double** v_y, double delta_t, FILE** fpu, FILE** fpcx)
{

	double** u0 = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		u0[i] = calloc(ny+1, sizeof(double));
	}

	double** u1 = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		u1[i] = calloc(ny+1, sizeof(double));
	}

	//inicjalizacja gestosci u0
	for(int i = 0; i <= nx; i++)
	{
		for(int j = 0; j <= ny; j++)
		{
			u0[i][j] = exp( -(pow(i*delta - x_A, 2) + pow(j*delta - y_A, 2))/(2.0*pow(sigma, 2)) )
				/(2.0*M_PI*pow(sigma, 2));
		}
	}

	for(int IT = 1; IT <= IT_MAX; IT++)
	{
		//start iteracji Picarda

		//inicjalizacja u1 = u0
		for(int i = 0; i <= nx; i++)
		{
			for(int j = 0; j <= ny; j++)
			{
				u1[i][j] = u0[i][j];
			}
		}
		

		for(int k = 1; k <= 20; k++)
		{
			for(int i = 0; i <= nx; i++)
			{
				for(int j = 1; j <= ny-1; j++)
				{					
					if(zastawka(i,j) == 1)
					{
						continue;
					}
					else if(i == 0 || i == nx)
					{
						if(i == 0)
						{
							u1[i][j] = (1.0/(1.0 + 2.0*D*delta_t/pow(delta, 2)))
								*(u0[i][j] - 0.5*delta_t*v_x[i][j]*
								(0.5*(u0[i+1][j] - u0[nx][j])/delta + (0.5*(u1[i+1][j] - u1[nx][j])/delta) ) 
								- 0.5*delta_t*v_y[i][j] * ( (0.5*(u0[i][j+1] - u0[i][j-1])/delta) + (0.5*(u1[i][j+1] - u1[i][j-1])/delta) ) 
								+ 0.5*D*delta_t * ( (u0[i+1][j] + u0[nx][j] + u0[i][j+1] + u0[i][j-1] - 4.0*u0[i][j])/pow(delta, 2)
								+ (u1[i+1][j] + u1[nx][j] + u1[i][j+1] + u1[i][j-1])/pow(delta, 2) ) );
						}
						if(i == nx)
						{
							u1[i][j] = (1.0/(1.0 + 2.0*D*delta_t/pow(delta, 2)))
								*(u0[i][j] - 0.5*delta_t*v_x[i][j]*
								(0.5*(u0[0][j] - u0[i-1][j])/delta + (0.5*(u1[0][j] - u1[i-1][j])/delta) ) 
								- 0.5*delta_t*v_y[i][j] * ( (0.5*(u0[i][j+1] - u0[i][j-1])/delta) + (0.5*(u1[i][j+1] - u1[i][j-1])/delta) ) 
								+ 0.5*D*delta_t * ( (u0[0][j] + u0[i-1][j] + u0[i][j+1] + u0[i][j-1] - 4.0*u0[i][j])/pow(delta, 2)
								+ (u1[0][j] + u1[i-1][j] + u1[i][j+1] + u1[i][j-1])/pow(delta, 2) ) );
						}
					}
					else
					{
						u1[i][j] = (1.0/(1.0 + 2.0*D*delta_t/pow(delta, 2)))
							*(u0[i][j] - 0.5*delta_t*v_x[i][j]*
							(0.5*(u0[i+1][j] - u0[i-1][j])/delta + (0.5*(u1[i+1][j] - u1[i-1][j])/delta) ) 
							- 0.5*delta_t*v_y[i][j] * ( (0.5*(u0[i][j+1] - u0[i][j-1])/delta) + (0.5*(u1[i][j+1] - u1[i][j-1])/delta) ) 
							+ 0.5*D*delta_t * ( (u0[i+1][j] + u0[i-1][j] + u0[i][j+1] + u0[i][j-1] - 4.0*u0[i][j])/pow(delta, 2)
							+ (u1[i+1][j] + u1[i-1][j] + u1[i][j+1] + u1[i][j-1])/pow(delta, 2) ) );
					}
				}
			}
		}

		//zachowanie rozwiazania
		for(int i = 0; i <= nx; i++)
		{
			for(int j = 0; j <= ny; j++)
			{
				u0[i][j] = u1[i][j];
			}
		}

		//tu wyznaczyc c i x_sr i zapisac do pliku
		double c = 0.0;
		double x_sr = 0.0;
		for(int i = 0; i <= nx; i++)
		{
			for(int j = 0; j <= ny; j++)
			{
				c += u0[i][j];
				x_sr += delta*i * u0[i][j];

				if(IT > 0 && IT%(IT_MAX/5) == 0)
				{
					fprintf(*fpu, "%g %g %g\n", i*delta, j*delta, u0[i][j]);
				}
			}
			if(IT > 0 && IT%(IT_MAX/5) == 0)
			{
				fprintf(*fpu, "\n");
			}
		}	
		fprintf(*fpcx, "%g %g %g\n", IT*delta_t, c*pow(delta, 2), x_sr*pow(delta, 2));
		if(IT > 0 && IT%(IT_MAX/5) == 0)
		{
			fprintf(*fpu, "\n\n");
		}
	}

	fprintf(*fpcx, "\n\n");

	//dealokacja pamieci
	for(int i = 0; i <= nx; i++)
	{	
		free(u0[i]);
		free(u1[i]);
	}
	free(u0);
	free(u1);

}


int main()
{
	FILE* fppsi = fopen("psi.dat", "r");
	FILE* fp_u0 = fopen("u_D0.txt", "w+");
	FILE* fp_u1 = fopen("u_D1.txt", "w+");
	FILE* fp1 = fopen("cx.txt", "w+");
	
	double** psi = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		psi[i] = calloc(ny+1, sizeof(double));
	}

	double** v_x = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		v_x[i] = calloc(ny+1, sizeof(double));
	}

	double** v_y = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		v_y[i] = calloc(ny+1, sizeof(double));
	}

	//wczytanie psi z pliku
	for(int i = 0; i <= nx; i++)
	{
		for(int j = 0; j <= ny; j++)
		{
			long int a, b;
			fscanf(fppsi, "%ld %ld %lf", &a, &b, &psi[i][j]);
		}
	}

	//wyznaczenie pol predkosci
	for(int i = 1; i <= nx-1; i++)
	{
		for(int j = 1; j <= ny-1; j++)
		{
			if(i >= i1 && i <= i2 && j <= j1)
			{
				v_x[i][j] = v_y[i][j] = 0.0;
			}
			else
			{
				v_x[i][j] = 0.5*(psi[i][j+1] - psi[i][j-1])/delta;
				v_y[i][j] = -0.5*(psi[i+1][j] - psi[i-1][j])/delta;
			}
		}
		v_x[i][0] = v_x[i][ny] = v_y[i][ny] = 0.0;
	}
	for(int j = 0; j <= ny; j++)
	{
		v_x[0][j] = v_x[1][j];
		v_x[nx][j] = v_x[nx-1][j];
		v_y[0][j] = v_y[1][j];
		v_y[nx][j] = v_y[nx-1][j];
	}

	FILE* fp_v = fopen("v.txt", "w+");
	for(int i = 0; i <= nx; i++)
	{
		for(int j = 0; j <= ny; j++)
		{
			fprintf(fp_v, "%g %g %g %g\n", i*delta, j*delta, v_x[i][j], v_y[i][j]);
		}
		fprintf(fp_v, "\n");
	}
	fclose(fp_v);

	//wyznaczanie v_max i delta_t
	double v_max = find_max(v_x, v_y);
	double delta_t = 0.25*delta/v_max;
	
	printf("Obliczenia trwaja dla D = 0. Prosimy czekac...\n");
	iteracje(0.0, v_x, v_y, delta_t, &fp_u0, &fp1);
	printf("Obliczenia trwaja dla D = 0.1. Prosimy czekac...\n");
	iteracje(0.1, v_x, v_y, delta_t, &fp_u1, &fp1);

	fclose(fp_u0);
	fclose(fp_u1);
	fclose(fp1);

	//dealokacja pamieci
	for(int i = 0; i <= nx; i++)
	{
		free(psi[i]);
		free(v_x[i]);
		free(v_y[i]);
	}
	free(psi);
	free(v_x);
	free(v_y);
	
	return 0;
}