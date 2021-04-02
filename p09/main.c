#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h> 

int nx = 40;
int ny = 40;
double delta = 1.0;
double delta_t = 1.0;
double T_A = 40;
double T_B = 0;
double T_C = 30;
double T_D = 0;
double k_B = 0.1;
double k_D = 0.6;
int IT_MAX = 2000;

int main()
{
	int N = (nx+1)*(ny+1);
	gsl_matrix *A = gsl_matrix_calloc(N, N);
	gsl_matrix *B = gsl_matrix_calloc(N, N);
	gsl_vector *C = gsl_vector_calloc(N);

	//wypelnienie macierzy A, B, C

	//wnetrze obszaru
	for(int i = 1; i <= nx-1; i++)
	{
		for(int j = 1; j <= ny-1; j++)
		{
			int l = i + j*(nx+1);
			gsl_matrix_set(A, l, l-nx-1, delta_t/(2.0*pow(delta, 2))); 
			gsl_matrix_set(A, l, l-1, delta_t/(2.0*pow(delta, 2))); 
			gsl_matrix_set(A, l, l+1, delta_t/(2.0*pow(delta, 2))); 
			gsl_matrix_set(A, l, l+nx+1, delta_t/(2.0*pow(delta, 2)));

			gsl_matrix_set(A, l, l, -2.0*delta_t/pow(delta, 2) - 1.0); 

			gsl_matrix_set(B, l, l-nx-1, -delta_t/(2.0*pow(delta, 2))); 
			gsl_matrix_set(B, l, l-1, -delta_t/(2.0*pow(delta, 2))); 
			gsl_matrix_set(B, l, l+1, -delta_t/(2.0*pow(delta, 2))); 
			gsl_matrix_set(B, l, l+nx+1, -delta_t/(2.0*pow(delta, 2)));	

			gsl_matrix_set(B, l, l, 2.0*delta_t/pow(delta, 2) - 1.0); 
		}
	}

	//WB D lewy i prawy brzeg
	for(int i = 0; i < nx+1; i += nx)
	{
		for(int j = 0; j <= ny; j++)
		{
			int l = i + j*(nx+1);
			gsl_matrix_set(A, l, l, 1.0); 
			gsl_matrix_set(B, l, l, 1.0); 
			gsl_vector_set(C, l, 0.0); 
		}
	}

	//WB v N na gornym brzegu
	for(int i = 1; i <= nx-1; i++)
	{
		int j = ny;
		int l = i + j*(nx+1);
		gsl_matrix_set(A, l, l-nx-1, -1.0/(k_B*delta)); 
		gsl_matrix_set(A, l, l, 1.0 + 1.0/(k_B*delta)); 
		gsl_vector_set(C, l, T_B); 
		for(int a = 0; a <= ny; a++)
		{
			gsl_matrix_set(B, l, a, 0.0); 
		}
		//WB v N na dolnym brzegu
		j = 0;
		l = i + j*(nx+1);
		gsl_matrix_set(A, l, l, 1.0 + 1.0/(k_D*delta)); 
		gsl_matrix_set(A, l, l+nx+1, -1.0/(k_D*delta)); 
		gsl_vector_set(C, l, T_D); 
		for(int a = 0; a <= ny; a++)
		{
			gsl_matrix_set(B, l, a, 0.0); 
		}
	}

	gsl_vector *T = gsl_vector_calloc(N);
	
	for(int i = 0; i <= nx; i++)
	{
		for(int j = 0; j <= ny; j++)
		{
			int l = i + j*(nx+1);
			if(i == 0)
			{
				gsl_vector_set(T, l, T_A);
			}
			else if(i == nx)
			{
				gsl_vector_set(T, l, T_C);
			}
			else //poxostaly obszar
			{
				gsl_vector_set(T, l, 0.0);
			}
		}
	}

	//rozklad LU i jego rozw
	gsl_permutation *perm = gsl_permutation_alloc(N);
	int signum = 0;
	gsl_linalg_LU_decomp(A, perm, &signum);

	FILE* fp1 = fopen("T.dat", "w+");
	FILE* fp2 = fopen("laplasjan.dat", "w+");	

	gsl_vector *d = gsl_vector_calloc(N);
	for(int IT = 0; IT <= IT_MAX; IT++)
	{
		// d = B · ⃗T +⃗c //T = Tn
		for(int a = 0; a < N; a++)
		{
			gsl_vector_set(d, a, gsl_vector_get(C, a));
		}
		gsl_blas_dgemv(CblasNoTrans, 1.0, B, T, 1.0, d);
		
		//A · ⃗T = ⃗d //T = Tn+1
		gsl_linalg_LU_solve(A, perm, d, T);

		//zapis T do pliku
		if(IT == 100 || IT == 200 || IT == 500 || IT == 1000 || IT == 2000)
		{
			printf("Zapisuje do pliku %d. iteracje...\n", IT);
			for(int l = 0; l < N; l++)
			{
				int j = l/(nx+1);
				int i = l - j*(nx+1);
				fprintf(fp1, "%g %g %g\n", i*delta, j*delta, gsl_vector_get(T, l));
				if(i > 0 && i < nx && j > 0 && j < ny)
				{
					double laplasjan_1 = (gsl_vector_get(T, l-1) - 2.0*gsl_vector_get(T, l) + gsl_vector_get(T, l+1))/pow(delta, 2);
					double laplasjan_2 = (gsl_vector_get(T, l-nx-1) - 2.0*gsl_vector_get(T, l) + gsl_vector_get(T, l+nx+1))/pow(delta, 2);
					fprintf(fp2, "%g %g %g\n", i*delta, j*delta, laplasjan_1+laplasjan_2);
					if(i == nx-1)
						fprintf(fp2, "\n");
				}

				if(i == nx)
					fprintf(fp1, "\n");
			}
			fprintf(fp1, "\n\n");
			fprintf(fp2, "\n\n");
		}
	}

	fclose(fp1);
	fclose(fp2);

	gsl_matrix_free(A); 
	gsl_matrix_free(B); 
	gsl_vector_free(C); 
	gsl_vector_free(T); 
	gsl_permutation_free(perm); 
	gsl_vector_free(d); 
	return 0;
}