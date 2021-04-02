#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int nx = 150;
int nt = 1000;
double delta = 0.1;
double delta_t = 0.05;
double xA = 7.5;
double sigma = 0.5; 
double xF = 2.5;

double aF(int i, int n, double alpha)
{
	if(i*delta == xF && alpha > 0.5)
		return alpha*cos(50*n/nt);
	return 0.0;
}

double Verlet(double alfa, double beta, FILE** fp1, FILE** fp2)
{
	double* u0 = calloc(nx+1, sizeof(double));
	double* u = calloc(nx+1, sizeof(double));
	double* v = calloc(nx+1, sizeof(double));
	double* vp = calloc(nx+1, sizeof(double));
	double* a = calloc(nx+1, sizeof(double));

	//warunki brzegowe u, v
	u[0] = u[nx] = v[0] = v[nx] = 0.0;

	//warunki poczatkowe
	for(int i = 1; i < nx; i++)
	{
		u[i] = alfa > 0.5 ? 0.0 : exp( -pow(i*delta - xA, 2) / (2.0*pow(sigma, 2)) );
		//zerowanie v nastepuje za pomoca calloca
	}

	for(int i = 0; i <= nx; i++)
	{
		u0[i] = u[i];
	}

	for(int i = 1; i < nx; i++)
	{
		a[i] = (u[i+1] - 2.0*u[i] + u[i-1])/pow(delta, 2) - beta*(u[i] - u0[i])/delta_t + aF(i, 0, alfa);
	}

	for(int n = 1; n <= nt; n++)
	{
		// u[0] = u[nx] = v[0] = v[nx] = 0.0;
		for(int i = 1; i < nx; i++)
		{
			vp[i] = v[i] + a[i]*delta_t/2.0;
			u0[i] = u[i];
			u[i] += delta_t*vp[i];
		}

		for(int i = 1; i < nx; i++)
		{
			a[i] = (u[i+1] - 2.0*u[i] + u[i-1])/pow(delta, 2) - beta*(u[i] - u0[i])/delta_t + aF(i, n, alfa);
			v[i] = vp[i] + a[i]*delta_t/2.0;
		}

		double temp = 0;
		for(int i = 0; i <= nx; i++)
		{
			if(i > 0 && i < nx)
			{
				temp += pow(v[i], 2) + pow( 0.5*(u[i+1]-u[i-1])/delta, 2);
			}
			fprintf(*fp1, "%g %g %g\n", n*delta_t, i*delta, u[i]);
		}

		double E = (delta/4.0)*(pow( (u[1] - u[0])/delta, 2) + pow((u[nx] - u[nx-1])/delta, 2)) + temp*delta/2.0;
		//zapis E oraz u
		fprintf(*fp2, "%g\t%g\n", n*delta_t, E);
		fprintf(*fp1, "\n");
	}

	fprintf(*fp2, "\n\n");
	
	// dealokacja pamieci
	free(u0);
	free(u);
	free(v);
	free(vp);
	free(a);
}

int main()
{
	FILE* fp1 = fopen("u_0.0.dat", "w+");
	FILE* fp2 = fopen("u_0.1.dat", "w+");
	FILE* fp3 = fopen("u_1.0.dat", "w+");
	FILE* fpE = fopen("E.dat", "w+");
	Verlet(0.0, 0.0, &fp1, &fpE);
	Verlet(0.0, 0.1, &fp2, &fpE);
	Verlet(0.0, 1.0, &fp3, &fpE);

	FILE* fp4 = fopen("u_a1.0.dat", "w+");
	FILE* fpEa = fopen("Ea.dat", "w+");
	Verlet(1.0, 1.0, &fp4, &fpEa);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fpE);
	fclose(fpEa);

	return 0;
}