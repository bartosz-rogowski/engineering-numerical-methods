#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int main()
{
	double beta = 1e-03;
	int N = 500;
	double GAMMA = 0.1;
	int t_max = 100;
	double dt = 0.1;
	double TOL = 1e-06;
	int mu_max = 20;
	double u0 = 1;
	double alfa = beta*N - GAMMA;

	//------------------------------------------ metoda Picarda
	double us = u0;
	double un;
	FILE* fp1 = fopen("dane.dat", "w+");
	fprintf(fp1, "%f %f %f\n", 0.0, us, N - us);
	for(double n = dt; n - t_max <= 0; n+=dt)
	{
		double ui = us;
		for(int it = 1; it <= mu_max; it++)
		{
			un = us + (dt/2)*( (alfa*us - beta*pow(us, 2)) + (alfa*ui - beta*pow(ui, 2)) );
			if(fabs(un - ui) - TOL < 0.0)
				break;
			ui = un;
		}
		us = un;
		fprintf(fp1, "%f %f %f\n", n, us, N - us);
	}

	fprintf(fp1, "\n\n");

	//------------------------------------------ metoda Newtona

	us = u0;
	fprintf(fp1, "%f %f %f\n", 0.0, us, N - us);
	for(double n = dt; n - t_max <= 0; n+=dt)
	{
		double ui = us;
		for(int it = 1; it <= mu_max; it++)
		{
			double l = ui - us - (dt/2)*( (alfa*us - beta*pow(us, 2)) + (alfa*ui - beta*pow(ui, 2)) );
			double m = 1.0 - (dt/2)*(alfa - 2*beta*ui);
			un = ui - l/m;
			if(fabs(un - ui) - TOL < 0.0)
				break;
			ui = un;
		}
		us = un;	
		fprintf(fp1, "%f %f %f\n", n, us, N - us);
	}

	fprintf(fp1, "\n\n");

	//----------------------------------------------- niejawna metoda RK2
	
	double a = 0.25;
	double b = 0.5;
	double a12 = 0.25 - sqrt(3)/6;
	double a21 = 0.25 + sqrt(3)/6;
	double c1 = 0.5 - sqrt(3)/6;
	double c2 = 0.5 + sqrt(3)/6;

	double U1, U2, F1, F2;
	U1 = U2 = us = u0;
	fprintf(fp1, "%f %f %f\n", 0.0, us, N - us);
	for(double n = dt; n - t_max <= 0; n+=dt)
	{
		U1 = U2 = us;
		for(int it = 1; it <= mu_max; it++)
		{
			double m11, m12, m21, m22, dU1, dU2;
			F1 = U1 - us - dt*(a*(alfa*U1 - beta*pow(U1, 2)) + a12*(alfa*U2 - beta*pow(U2, 2)) );
			F2 = U2 - us - dt*(a21*(alfa*U1 - beta*pow(U1, 2)) + a*(alfa*U2 - beta*pow(U2, 2)) );
			m11 = 1.0 - dt*a*(alfa - 2*beta*U1);
			m12 = -dt*a12*(alfa - 2*beta*U2);
			m21 = -dt*a21*(alfa - 2*beta*U1);
			m22 = 1.0 - dt*a*(alfa - 2*beta*U2);
			dU1 = (F2*m12 - F1*m22)/(m11*m22 - m12*m21);
			dU2 = (F1*m21 - F2*m11)/(m11*m22 - m12*m21);
			if(fabs(dU1) - TOL < 0.0 && fabs(dU2) - TOL < 0.0)
				break;
			U1+=dU1;
			U2+=dU2;
		}
		us += dt*( b*(alfa*U1 - beta*pow(U1, 2))+b*(alfa*U2 - beta*pow(U2, 2)) );
		fprintf(fp1, "%f %f %f\n", n, us, N - us);
	}

	fclose(fp1);

	return 0;
}