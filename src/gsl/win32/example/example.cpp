#include <stdio.h>
#include <gsl/gsl_linalg.h>

////////////////////////////////////////////////////////////
// Solve Ax = b with LU and cholesky
int main(int argc, char **argv)
{
    printf("=========== tst2 ===========\n");
    double a_data[] = { 2,1,1,3,2,
                        1,2,2,1,1,
                        1,2,9,1,5,
                        3,1,1,7,1,
                        2,1,5,1,8 };

    double b_data[] = { -2,4,3,-5,1 };

    gsl_vector *x = gsl_vector_alloc (5);
    gsl_permutation * p = gsl_permutation_alloc (5);

    gsl_matrix_view m 
        = gsl_matrix_view_array(a_data, 5, 5);

    gsl_vector_view b
        = gsl_vector_view_array(b_data, 5);


    int s;

    gsl_linalg_LU_decomp (&m.matrix, p, &s);

    gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);

    printf ("x = \n");
    gsl_vector_fprintf(stdout, x, "%g");

    double a2_data[] = { 2,1,1,3,2,
                        1,2,2,1,1,
                        1,2,9,1,5,
                        3,1,1,7,1,
                        2,1,5,1,8 };

    double b2_data[] = { -2,4,3,-5,1 };
    gsl_matrix_view m2 = gsl_matrix_view_array(a2_data, 5, 5);
    gsl_vector_view b2 = gsl_vector_view_array(b2_data, 5);
    gsl_linalg_cholesky_decomp(&m2.matrix);
    gsl_linalg_cholesky_solve(&m2.matrix, &b2.vector, x);
    printf ("x = \n");
    gsl_vector_fprintf(stdout, x, "%g");

    gsl_permutation_free (p);
    gsl_vector_free(x);
}
