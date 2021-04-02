#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define j1 55

double delta = 0.01;
double ro = 1.0;
double mu = 1.0;
int nx = 200;
int ny = 90;
int i1 = 50;
int IT_MAX = 20000;

double** WB_psi(double Qwe, double Qwy)
{
	double** psi = calloc(nx+1, sizeof(double*));
	for(int i = 0; i <= nx; i++)
	{
		psi[i] = calloc(ny+1, sizeof(double));
	}

	//brzeg A
	for(int j = j1; j <= ny; j++)
	{
		double y = j*delta;
		psi[0][j] = (Qwe/(2.0*mu))*( pow(y, 3)/3.0 
			- (delta*j1 + ny*delta)*pow(y, 2)/2.0 + y*j1*delta*ny*delta);
	}

	//brzeg C
	for(int j = 0; j <= ny; j++)
	{
		double y = j*delta;
		psi[nx][j] = (Qwy/(2.0*mu))*( pow(y, 3)/3.0 
			- (ny*delta)*pow(y, 2)/2.0) + (Qwe*pow(delta*j1, 2)*(3.0*delta*ny - delta*j1))/(12.0*mu);
	}

	//brzeg B
	for(int i = 1; i <= nx-1; i++)
	{
		psi[i][ny] = psi[0][ny];
	}

	//brzeg D
	for(int i = i1; i <= nx-1; i++)
	{
		psi[i][0] = psi[0][j1];
	}

	//brzeg E
	for(int j = 1; j <= j1; j++)
	{
		psi[i1][j] = psi[0][j1];
	}

	//brzeg F
	for(int i = 1; i <= i1; i++)
	{
		psi[i][j1] = psi[0][j1];
	}
	return psi;
}

double** WB_zeta(double Qwe, double Qwy, double** psi, double** zeta)
{

	//brzeg A
	for(int j = j1; j <= ny; j++)
	{
		double y = j*delta;
		zeta[0][j] = 0.5*(Qwe/mu)*(2.0*y - delta*j1 - delta*ny);
	}

	//brzeg C
	for(int j = 0; j <= ny; j++)
	{
		double y = j*delta;
		zeta[nx][j] = 0.5*(Qwy/mu)*(2.0*y - delta*ny);
	}

	//brzeg B
	for(int i = 1; i <= nx-1; i++)
	{
		zeta[i][ny] = (2.0/pow(delta, 2))*(psi[i][ny-1] - psi[i][ny]);
	}

	//brzeg D
	for(int i = i1+1; i <= nx-1; i++)
	{
		zeta[i][0] = (2.0/pow(delta, 2))*(psi[i][1] - psi[i][0]);
	}

	//brzeg E
	for(int j = 1; j <= j1-1; j++)
	{
		zeta[i1][j] = (2.0/pow(delta, 2))*(psi[i1+1][j] - psi[i1][j]);
	}

	//brzeg F
	for(int i = 1; i <= i1; i++)
	{
		zeta[i][j1] = (2.0/pow(delta, 2))*(psi[i][j1+1] - psi[i][j1]);
	}

	//wierzcholek E/F
	zeta[i1][j1] = 0.5*(zeta[i1-1][j1] + zeta[i1][j1-1]);

	return zeta;
}

//sprawdza czy indeksy oznaczaja brzegi, jesli tak, zwraca 1
int brzeg(int i, int j)
{
	if(i <= i1 && j <= j1)
		return 1;
	return 0;
}

int main()
{
	double Q[] = {-1000.0, -4000.0, 4000.0};
	FILE* fp1 = fopen("psi.dat", "w+");
	FILE* fp2 = fopen("zeta.dat", "w+");
	FILE* fp3 = fopen("u.dat", "w+");
	FILE* fp4 = fopen("v.dat", "w+");

	//petla po Q
	for(int q = 0; q < sizeof(Q)/sizeof(Q[0]); q++)
	{
		double Qwe = Q[q];
		double Qwy = Qwe*(pow(delta*ny, 3) - pow(delta*j1, 3) 
			- 3.0*delta*j1*pow(delta*ny, 2) + 3.0*delta*ny*pow(delta*j1, 2))/pow(delta*ny, 3);
		
		//definicje psi i zeta
		double** psi = WB_psi(Qwe, Qwy);
		double** zeta = calloc(nx+1, sizeof(double*));
		for(int i = 0; i <= nx; i++)
		{
			zeta[i] = calloc(ny+1, sizeof(double));
		}
		
		//iteracje
		for(int IT = 1; IT <= IT_MAX; IT++)
		{
			double omega = 0;
			if(IT >= 2000)
				omega = 1.0;
			for(int i = 1; i <= nx-1; i++)
			{
				for(int j = 1; j <= ny-1; j++)
				{
					if(brzeg(i, j) == 0)
					{
						psi[i][j] = 0.25*(psi[i+1][j] + psi[i-1][j] + psi[i][j+1] + psi[i][j-1] - pow(delta, 2)*zeta[i][j]);
						zeta[i][j] = 0.25*(zeta[i+1][j] + zeta[i-1][j] + zeta[i][j+1] + zeta[i][j-1])
							- omega*(ro/(16.0*mu))*( ((psi[i][j+1] - psi[i][j-1]) * (zeta[i+1][j] - zeta[i-1][j])) 
							- ((psi[i+1][j] - psi[i-1][j]) * (zeta[i][j+1] - zeta[i][j-1])) );
					}
				}
			}
			//modyfikacja WB_zeta
			zeta = WB_zeta(Qwe, Qwy, psi, zeta);

			//kontrola bledu
			double S = 0;
			for(int i = 1; i <= nx-1; i++)
			{
				S += (psi[i+1][j1+2] + psi[i-1][j1+2] + psi[i][j1+2+1] + psi[i][j1+2-1] - 4.0*psi[i][j1+2] - zeta[i][j1+2]*pow(delta, 2));
			}
		}
		//zapis do pliku
		for(int i = 0; i <= nx; i++)
		{
			for(int j = 0; j <= ny; j++)
			{
				fprintf(fp1, "%g %g %g\n", i*delta, j*delta, psi[i][j]);
				if(q < 2)
				{
					fprintf(fp2, "%g %g %g\n", i*delta, j*delta, zeta[i][j]);
					double u = 0.0;
	      			double v = 0.0;
	      			if( (i > 0 && i <= i1 && j > j1 && j <= ny-1) || (i > i1 && i <= nx-1 && j > 0 && j <= ny-1) )
	      			{
	                	u = (psi[i][j+1] - psi[i][j-1])/(2.0*delta);
	                	v = -(psi[i+1][j] - psi[i-1][j])/(2.0*delta);
	            	}
	            	fprintf(fp3, "%g %g %g\n", i*delta, j*delta, u);
					fprintf(fp4, "%g %g %g\n", i*delta, j*delta, v);
				}
			}
			fprintf(fp1, "\n");
			fprintf(fp2, "\n");
			fprintf(fp3, "\n");
			fprintf(fp4, "\n");
			free(psi[i]);
			free(zeta[i]);
		}
		free(psi);
		free(zeta);
		fprintf(fp1, "\n\n");
		fprintf(fp2, "\n\n");
		fprintf(fp3, "\n\n");
		fprintf(fp4, "\n\n");
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);

	return 0;
}