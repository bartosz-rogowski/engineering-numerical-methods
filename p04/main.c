#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double epsilon = 1;
double delta = 0.1;
int nx = 150;
int ny = 100;
double V1 = 10;
double V2 = 0;
double TOL = 1e-08;

double ro1(double x, double y)
{
	return exp(-pow(x - 0.35*delta*nx, 2)/pow(0.1*delta*nx, 2) 
		- pow(y - 0.5*delta*ny, 2)/pow(0.1*delta*ny, 2) );
}

double ro2(double x, double y)
{
	return -exp(-pow(x - 0.65*delta*nx, 2)/pow(0.1*delta*nx, 2) 
		- pow(y - 0.5*delta*ny, 2)/pow(0.1*delta*ny, 2) );
}

double ro(double x, double y)
{
	return ro1(x, y) + ro2(x, y);
}


int main()
{
	double w_g[] = {0.6, 1.0};
	double w_l[] = {1.0, 1.4, 1.8, 1.9};
	FILE* fp1 = fopen("rglob_S.dat", "w+");
	FILE* fp2 = fopen("rglob_V.dat", "w+");
	FILE* fp3 = fopen("rglob_blad.dat", "w+");
	FILE* fp4 = fopen("rlok_S.dat", "w+");

	// ----------------------------- relaksacja globalna

	//petla po wagach
	for(int waga = 0; waga <= 1; waga++)
	{
		// ------------------------ przygotowanie tablic
		double **Vs = calloc(nx+1, sizeof(double*));
		double **Vn = calloc(nx+1, sizeof(double*));
		for(int i = 0; i <= nx; i++)
		{
			Vs[i] = calloc(ny+1, sizeof(double));
			Vn[i] = calloc(ny+1, sizeof(double));
			Vs[i][0] = V1;
			Vs[i][ny] = V2;
		}
		// ---------------------------------------------

		for(int i = 0; i <= nx; i++)
		{
			for(int j = 0; j <= ny; j++)
			{
				Vn[i][j] = Vs[i][j];
			}
		}
		int it = 0;
		double Sit_stare, Sit_nowe;
		Sit_stare = 1;
		while(1)
		{

			for(int i = 1; i <= nx-1; i++)
			{
				for(int j = 1; j <= ny-1; j++)
				{
					Vn[i][j] = ( Vs[i+1][j] + Vs[i-1][j] + Vs[i][j+1] + Vs[i][j-1] 
						+ ro(delta*i, delta*j)*pow(delta, 2)/epsilon )/4.0;
				}
			}

			//WB Neumanna
			for(int j = 1; j <= ny-1; j++)
			{
				Vn[0][j] = Vn[1][j];
				Vn[nx][j] = Vn[nx-1][j];
			}

			//mieszanie rozwiazan
			for(int i = 0; i <= nx; i++)
			{
				for(int j = 0; j <= ny; j++)
				{
					Vs[i][j] = (1.0 - w_g[waga])*Vs[i][j] + w_g[waga]*Vn[i][j];
				}
			}

			Sit_nowe = 0;
			for(int i = 0; i <= nx-1; i++)
			{
				for(int j = 0; j <= ny-1; j++)
				{
					Sit_nowe += pow(delta, 2)*(pow((Vs[i+1][j]-Vs[i][j])/delta, 2)/2.0 
						+ pow((Vs[i][j+1]-Vs[i][j])/delta, 2)/2.0 - ro(delta*i, delta*j)*Vs[i][j]);
				}
			}
			if(it > 0)
				fprintf(fp1, "%d %g\n", it, Sit_nowe);
			it++;

			//warunek wyjscia z petli
			if(fabs((Sit_nowe - Sit_stare)/Sit_stare) - TOL < 0 || it >= pow(10, 5))
				break;
			Sit_stare = Sit_nowe;

		}

		for(int i = 0; i <= nx; i++)
		{
			for(int j = 0; j <= ny; j++)
			{
				fprintf(fp2, "%g %g %g\n", delta*i, delta*j, Vs[i][j]);
				if(i > 0 && i <= nx-1 && j > 0 && j <= ny-1)
					fprintf(fp3, "%g %g %g\n", delta*i, delta*j, ( (Vs[i+1][j] - 2.0*Vs[i][j] + Vs[i-1][j])/pow(delta, 2)
				        + (Vs[i][j+1] - 2.0*Vs[i][j] + Vs[i][j-1])/pow(delta, 2) ) + ro(delta*i, delta*j)/epsilon);
				else
					fprintf(fp3, "%g %g %g\n", delta*i, delta*j, 0.0);
			}
		}

		// ------------------------ dealokacja pamieci
		for(int i = 0; i <= nx; i++)
		{
			free(Vs[i]);
			free(Vn[i]);
		}
		free(Vs);
		free(Vn);

		fprintf(fp1, "\n\n");
		fprintf(fp2, "\n\n");
		fprintf(fp3, "\n\n");
	}
	
	fclose(fp1); 
	fclose(fp2);
	fclose(fp3);


	//-------------------------------- relaksacja lokalna
	for(int waga = 0; waga < 4; waga++)
	{
		double **V = calloc(nx+1, sizeof(double*));
		for(int i = 0; i <= nx; i++)
		{
			V[i] = calloc(ny+1, sizeof(double));
			V[i][0] = V1;
			V[i][ny] = V2;
		}

		int it = 0;
		double Sit_stare, Sit_nowe;
		Sit_stare = 1;
		while(1)
		{

			for(int i = 1; i <= nx-1; i++)
			{
				for(int j = 1; j <= ny-1; j++)
				{
					V[i][j] = (1.0 - w_l[waga])*V[i][j] + 0.25*w_l[waga]*
						( V[i+1][j] + V[i-1][j] + V[i][j+1] + V[i][j-1] 
						+ ro(delta*i, delta*j)*pow(delta, 2)/epsilon );
				}
			}

			//WB Neumanna
			for(int j = 1; j <= ny-1; j++)
			{
				V[0][j] = V[1][j];
				V[nx][j] = V[nx-1][j];
			}

			Sit_nowe = 0;
			for(int i = 0; i <= nx-1; i++)
			{
				for(int j = 0; j <= ny-1; j++)
				{
					Sit_nowe += pow(delta, 2)*(pow((V[i+1][j]-V[i][j])/delta, 2)/2.0 
						+ pow((V[i][j+1]-V[i][j])/delta, 2)/2.0 - ro(delta*i, delta*j)*V[i][j]);
				}
			}
			
			if(it > 0)
				fprintf(fp4, "%d %g\n", it, Sit_nowe);
			it++;

			//warunek wyjscia z petli
			if(fabs((Sit_nowe - Sit_stare)/Sit_stare) - TOL < 0 || it >= pow(10, 5))
				break;
			Sit_stare = Sit_nowe;
		}

		fprintf(fp4, "\n\n");

		// ------------------------ dealokacja pamieci
		for(int i = 0; i <= nx; i++)
		{
			free(V[i]);
		}
		free(V);
	}

	fclose(fp4);

	return 0;
}