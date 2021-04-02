#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double delta = 1e-10;
double x0 = 0.01;
double v0 = 0;
double dt0 = 1;
double S = 0.75;
int p = 2;
double t_max = 40;
double ALFA = 5;

double g(double t, double x, double v, double alpha)
{
	return (alpha*(1.0 - pow(x, 2))*v - x);
}

//zwraca x_(n+1) oraz v_(n+1)
double* trapezy(double x_n, double v_n, double dt, double alfa)
{
	double x_n1, v_n1, dx, dv;
	x_n1 = x_n;
	v_n1 = v_n;

	double x_k, v_k;
	x_k = x_n1;
	v_k = v_n1;
	do
	{
		double F = x_n1 - x_n - (dt/2)*(v_n + v_n1);
		double G = v_n1 - v_n - (dt/2)*( g(0, x_n, v_n, alfa) + g(0, x_n1, v_n1, alfa) );
		double a11 = 1.0;
		double a12 = -(dt/2);
		double a21 = -(dt/2)*(-2*alfa*x_k*v_k - 1.0);
		double a22 = 1.0 - (dt/2)*alfa*(1.0 - pow(x_k, 2));
		dx = ( -F*a22 + G*a12 )/(a11*a22 - a12*a21);
		dv = ( -G*a11 + F*a21 )/(a11*a22 - a12*a21);
		x_k += dx;
		v_k += dv;
	} while(fabs(dx) - delta < 0 && fabs(dv) - delta < 0);
	x_n1 = x_k;
	v_n1 = v_k;

	double* solv = calloc(2, sizeof(double));
	solv[0] = x_n1;
	solv[1] = v_n1;
	return solv;
}


double* RK2(double x_n, double v_n, double dt, double alfa)
{
	double k_1x = v_n;
	double k_1v = g(0, x_n, v_n, alfa);
	double k_2x = v_n + dt*k_1v;
	double k_2v = alfa*(1.0 - pow(x_n+dt*k_1x, 2))*(v_n+dt*k_1v) - (x_n + dt*k_1x);
	double* solv = calloc(2, sizeof(double));
	solv[0] = x_n + (dt/2)*(k_1x+k_2x); 
	solv[1] = v_n + (dt/2)*(k_1v+k_2v);
	return solv;
}

int main()
{
	double TOL[] = {1e-02, 1e-05};

	typedef double*(*fun_type)(double, double, double, double);
	fun_type metoda[2] = {trapezy, RK2};

	double Ex, Ev;
	FILE* fp1 = fopen("dane.dat", "w+");
	//petla po dwoch metodach
	for(int m = 0; m < 2; m++)
	{
		//petla po dwoch wartosciach TOL
		for(int tol = 0; tol < 2; tol++) 
		{
			//algorytm skoku czasowego
			double x_n = x0;
			double v_n = v0;
			double dt = dt0;
			double t = 0;
			fprintf(fp1, "%f %f %f %f\n", t, dt, x_n, v_n);
			do{
				double* dwa_kroki_1 = calloc(2, sizeof(double));
				double* dwa_kroki_2 = calloc(2, sizeof(double));
				double* jeden_krok = calloc(2, sizeof(double));
				// stawiamy dwa kroki ∆t
				dwa_kroki_1 = metoda[m](x_n, v_n, dt, ALFA);
				dwa_kroki_2 = metoda[m](dwa_kroki_1[0], dwa_kroki_1[1], dt, ALFA);
				// stawiamy jeden krok 2·∆t
				jeden_krok = metoda[m](x_n, v_n, 2*dt, ALFA);
				
				Ex = (dwa_kroki_2[0] - jeden_krok[0]) / (pow(2, p) - 1.0);
				Ev = (dwa_kroki_2[1] - jeden_krok[1]) / (pow(2, p) - 1.0);
				double Emax = fmax(fabs(Ex),fabs(Ev));
				// sprawdzamy czy wynik jest akceptowany
				if(Emax - TOL[tol] < 0)
				{
					t += 2*dt;
					x_n = dwa_kroki_2[0];
					v_n = dwa_kroki_2[1];
					fprintf(fp1, "%f %f %f %f\n", t, dt, x_n, v_n);
				}
				// zmiana  kroku  następuje  zawsze
				dt *= pow(S*TOL[tol] / Emax, 1.0/(p+1.0) );
				// t += dt;
				free(dwa_kroki_1);
				free(dwa_kroki_2);
				free(jeden_krok);
			} while(t - t_max < 0);
			fprintf(fp1, "\n\n");
		}
		fprintf(fp1, "\n\n");
	}
	
	return 0;
}