#include <iostream>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix_double.h>


using namespace std;

gsl_rng * r;  /* global generator */

int main()
{
	cout << "### RNG ###" << endl;
	const gsl_rng_type * T;
	gsl_rng * r;
	int i, n = 10;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);
	for (i = 0; i < n; i++) 
	{
		double u = gsl_rng_uniform (r);
		printf ("%.5f\n", u);
	}
	gsl_rng_free (r);
	
	cout << "### MATRIX ###" << endl;
	int j; 
	gsl_matrix * m = gsl_matrix_alloc (10, 3);
	for (i = 0; i < 10; i++)
	for (j = 0; j < 3; j++)
		gsl_matrix_set (m, i, j, 0.23 + 100*i + j);
	for (i = 0; i < 100; i++)  /* OUT OF RANGE ERROR */
	for (j = 0; j < 3; j++)
		printf ("m(%d,%d) = %g\n", i, j, 
				gsl_matrix_get (m, i, j));
	gsl_matrix_free (m);

	char c;
	cin >> c;
     
	return 0;
}