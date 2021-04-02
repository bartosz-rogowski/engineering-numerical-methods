#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double delta = 0.2;
int nx = 128;
int ny = 128;
double TOL = 1e-08;


int main()
{
	int k[] = {16, 8, 4, 2, 1};
	FILE* fp1 = fopen("S.dat", "w+");
	FILE* fp2 = fopen("V.dat", "w+");

	// ----------------------- przygotowanie i wypelnienie tablic (warunki poczatkowe)
	double **V = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		V[i] = calloc(ny+1, sizeof(double));
		V[i][0] = sin(2.0*M_PI*delta*i/(delta*nx));
		V[i][ny] = -sin(2.0*M_PI*delta*i/(delta*nx));
	}
	for(int j = 0; j <= ny; j++)
	{
		V[0][j] = sin(M_PI*delta*j/(delta*ny));
		V[nx][j] = sin(M_PI*delta*j/(delta*ny));
	}

	// ------------------------------------------------------------------------------

	double Sit_stare, Sit_nowe;
	Sit_stare = 1;
	int it = 0;

	for(int kk = 0; kk < sizeof(k)/sizeof(k[0]); kk++)
	{
		while(1)
		{

			for(int i = k[kk]; i <= nx-k[kk]; i += k[kk])
			{
				for(int j = k[kk]; j <= ny-k[kk]; j += k[kk])
				{
					V[i][j] = 0.25*(V[i+k[kk]][j] + V[i-k[kk]][j] + V[i][j+k[kk]] + V[i][j-k[kk]]);
				}
			}


			Sit_nowe = 0;
			for(int i = 0; i <= nx-k[kk]; i += k[kk])
			{
				for(int j = 0; j <= ny-k[kk]; j += k[kk])
				{
					Sit_nowe += 0.5*pow(k[kk]*delta, 2) * 
						( pow((V[i+k[kk]][j] - V[i][j] + V[i+k[kk]][j+k[kk]] - V[i][j+k[kk]])/(2.0*k[kk]*delta), 2) 
						+ pow((V[i][j+k[kk]] - V[i][j] + V[i+k[kk]][j+k[kk]] - V[i+k[kk]][j])/(2.0*k[kk]*delta), 2) );
				}
			}

			fprintf(fp1, "%d %g\n", it, Sit_nowe);
			it++;

			//warunek wyjscia z petli
			if(fabs((Sit_nowe - Sit_stare)/Sit_stare) - TOL < 0 || it >= pow(10, 5))
				break;
			Sit_stare = Sit_nowe;
		}

		for(int i = 0; i <= nx; i += k[kk])
		{
			for(int j = 0; j <= ny; j += k[kk])
			{
				fprintf(fp2, "%g %g %g\n", delta*i, delta*j, V[i][j]);
			}
			fprintf(fp2, "\n");
		}
		
		fprintf(fp1, "\n\n");
		fprintf(fp2, "\n\n");

		// ------------------------------------ zageszczanie siatki
		if(kk < sizeof(k)/sizeof(k[0]) - 1)
		{
			for(int i = 0; i <= nx - k[kk]; i += k[kk])
			{
				for(int j = 0; j <= ny - k[kk]; j += k[kk])
				{
					V[i+k[kk+1]][j+k[kk+1]] = 0.25*(V[i][j] + V[i+k[kk]][j] + V[i][j+k[kk]] + V[i+k[kk]][j+k[kk]]);
					if(i != nx-k[kk])
						V[i+k[kk]][j+k[kk+1]] = 0.5*(V[i+k[kk]][j] + V[i+k[kk]][j+k[kk]]);
					if(j != ny-k[kk])
						V[i+k[kk+1]][j+k[kk]] = 0.5*(V[i][j+k[kk]] + V[i+k[kk]][j+k[kk]]);
					if(j != 0)
						V[i+k[kk+1]][j] = 0.5*(V[i][j] + V[i+k[kk]][j]);
					if(i != 0)
						V[i][j+k[kk+1]] = 0.5*(V[i][j] + V[i][j+k[kk]]);
				}
			}
		}
		// ---------------------------------------------------------
	} //koniec petli po k

	fclose(fp1);
	fclose(fp2);

	// ------------------------ dealokacja pamieci
	for(int i = 0; i <= nx; i++)
	{
		free(V[i]);
	}
	free(V);


	return 0;
}