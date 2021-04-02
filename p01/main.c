#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double lambda = -1.0;

double f(double x)
{
	return exp(lambda*x);
}


int main()
{
	FILE* fp1 = fopen("data.dat", "w+");
	double y_n;
	double delta_t[] = {0.01, 0.1, 1.0};

	//-------------------------------------------------------------- zad 1
	for(int i=0; i<3; i++)
	{
		double y_s = 1.0;
		fprintf(fp1, "%f %f %f\n",0.0, y_s, fabs(y_s - f(0.0)));
		for(double t_n = delta_t[i]; t_n - 5.0 <= 0; t_n+=delta_t[i] )
		{
			y_n = y_s + delta_t[i]*lambda*y_s;
			fprintf(fp1, "%f %f %f\n",t_n, y_n, fabs(y_n - f(t_n)));
			y_s = y_n;
		}
		fprintf(fp1, "\n\n");
	}

	//-------------------------------------------------------------- zad 2
	double k1, k2;
	for(int i=0; i<3; i++)
	{
		double y_s = 1.0;
		fprintf(fp1, "%f %f %f\n",0.0, y_s, fabs(y_s - f(0.0)));
		for(double t_n = delta_t[i]; t_n - 5.0 <= 0; t_n+=delta_t[i] )
		{
			k1 = lambda * y_s;
			k2 = lambda * (y_s + delta_t[i]*k1);
			y_n = y_s + delta_t[i]*(k1 + k2)/2;
			fprintf(fp1, "%f %f %e\n",t_n, y_n, fabs(y_n - f(t_n)));
			y_s = y_n;
		}
		fprintf(fp1, "\n\n");
	}

	//-------------------------------------------------------------- zad 3
	double k3, k4;
	for(int i=0; i<3; i++)
	{
		double y_s = 1.0;
		fprintf(fp1, "%f %f %f\n",0.0, y_s, fabs(y_s - f(0.0)));
		for(double t_n = delta_t[i]; t_n - 5.0 <= 0; t_n+=delta_t[i] )
		{
			k1 = lambda * y_s;
			k2 = lambda * (y_s + delta_t[i] * k1/2);
			k3 = lambda * (y_s + delta_t[i] * k2/2);
			k4 = lambda * (y_s + delta_t[i] * k3);
			y_n = y_s + delta_t[i] * (k1 + 2*k2 + 2*k3 + k4)/6;
			fprintf(fp1, "%f %f %e\n",t_n, y_n, fabs(y_n - f(t_n)));
			y_s = y_n;
		}
		fprintf(fp1, "\n\n");
	}

	//------------------------- zapisanie analitycznych wartosci funkcji
	for(double t_n = 0.0; t_n - 5.0 <= 0; t_n+=delta_t[0] )
	{
		fprintf(fp1, "%f %f\n", t_n, f(t_n));
	}

	fclose(fp1);
	//------------------------------------------------------------ obwod
	FILE* fp2 = fopen("obwod.dat", "w+");
	double dT = 1e-04;
	double R = 100;
	double L = 0.1;
	double C = 1e-03;
	double omega_0 = 1.0/sqrt(L*C);
	double T_0 = 2*M_PI/omega_0; //t od 0 do 4*T_0
	double omega_V[] = {0.5*omega_0, 0.8*omega_0, omega_0, 1.2*omega_0};
	double k1_I, k2_I, k3_I, k4_I, k1_Q, k2_Q, k3_Q, k4_Q;

	for(int i=0; i<4; i++)
	{
		double Q_s, I_s, Q_n, I_n;
		Q_s = I_s = 0.0;
		fprintf(fp2, "%f %f %f\n",0.0, Q_n, I_n);
		for(double t_n = dT; t_n - 4*T_0 <= 0; t_n+=dT )
		{
			double V_n = 10*sin(omega_V[i]*t_n);
			double V_n_half = 10*sin(omega_V[i]*(t_n+dT/2));
			double V_n_1 = 10*sin(omega_V[i]*(t_n+dT));
			k1_Q = Q_s;
			k1_I = V_n/L + Q_s/(L*C) - R*I_s/L; 
			k2_Q = I_s + dT/2 * k1_I;
			k2_I = V_n_half/L - (Q_s + k1_Q*dT/2)/(L*C) - R*(I_s + k1_I*dT/2)/L;
			k3_Q = I_s + dT/2 * k2_I;
			k3_I = V_n_half/L - (Q_s + k2_Q*dT/2)/(L*C) - R*(I_s + k2_I*dT/2)/L;
			k4_Q = I_s + dT * k3_I;
			k4_I = V_n_1/L - (Q_s + k3_Q*dT)/(L*C) - R*(I_s + k3_I*dT)/L;
			Q_n = Q_s + dT/6 * (k1_Q + 2*k2_Q + 2*k3_Q + k4_Q);
			I_n = I_s + dT/6 * (k1_I + 2*k2_I + 2*k3_I + k4_I);
			fprintf(fp2, "%f %f %f\n",t_n, Q_n, I_n);
			Q_s = Q_n;
			I_s = I_n;
		}
		fprintf(fp2, "\n\n");
	}
	fclose(fp2);

	return 0;
}