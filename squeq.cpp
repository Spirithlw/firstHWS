#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/**
 *
 * @param GRT greeting to user
 * @param a coeff x^2
 * @param b coeff x
 * @param c free coeff
 * @return ok or not ok  inputed data
 */

int Input_coeffs(const char GRT[], double* a, double* b, double* c)// gets coefficients and checks its correct
    {
    printf("%s\n", GRT);
    int i = 0;
    while(((scanf("%lg%lg%lg", a, b, c)) != 3) && (i<100))
        {
        while( getchar() != '\n');
        printf("You've entered incorrect data. Try one more time!\n");
        i++;
        }
    if (i == 100)
        {
        printf("You haven't enter after a 100 tries,we are sorry,program ends.\n");
        return 0;
        }
    else
        {
        return 1;
        }
    }

    /**
     *
     * @param b coeff with x in line equation
     * @param c free coeff in line equation
     * @param x1 root
     * @param x2 also root (for comfort);
     * @return number of roots;
     */

int solve_line(double b, double c, double* x1, double* x2)//solves linear equation
    {
    if  ( (b == 0) && (c == 0) )
        {
        return 3;
        }
    if ( (b == 0) )
        {
        return 0;
        }
    if ( (c == 0) )
        {
        *x1 = 0;
        x2 = x1;
        return 1;
        }
    *x1 = (-c) / (b);
    x2 = x1;
    return 1;
    }

    /**
     *
     * @param a coeff x^2
     * @param b coeff x
     * @param c free coeff
     * @param x1 1st root
     * @param x2 2nd root
     * @return number of roots
     */

int solve_square_eq(double a, double b, double c, double* x1, double* x2)//solves square equation
    {
    double d = NAN;
    if ( a == 0 )
        {
        return solve_line(b, c, x1, x2);
        }
    d = b*b - 4*a*c;
    if ( d < 0 )
        {
        return 0;
        }
    if ( d == 0)
        {
        *x1 = ( -b )/( 2*a );
        x2 = x1;
        return 1;
        }
    *x1=( -b - sqrt(d) )/( 2*a );
    *x2=( -b + sqrt(d) )/( 2*a );
    return 2;
    }

    /**
     *
     * @param q
     * @param lim
     * @return
     */

    double giv_rand(double* q, int lim)//generates random double numbers from -lim to +lim
        {
        *q = (2*( ((double)(rand())) / RAND_MAX )-1)*lim ;
        }

    void UNIT_TESTS(double eps)//unit tests
        {
        srand(time(NULL));
        for ( int i = 1; i <= 1000; i++)
            {
            double a, b, c, x1, x2;
            int h = 0;
            a = b = c = x1 = x2 = NAN;
            a = giv_rand( &a, 10000);
            b = giv_rand( &b, 10000);
            c = giv_rand( &c, 10000);
            h = solve_square_eq( a, b, c, &x1, &x2);
            if ( ( h == 1 ) || ( h == 2) )
                {
                if ( ( fabs( a*x1*x1 + b*x1 + c) > eps ) || ( fabs( a*x2*x2 + b*x2 + c) > eps ) )
                    {
                    printf("UNIT TEST Failed with the following conditions a = %lg; b = %lg; c = %lg;\n ", a, b, c);
                    }
                }
            }
        }

        /**
         *
         *
         * @return
         */

 int main ()
    {
    printf("Greetings,my friend! This program can solve square equations.\nV1.0.0 ...\n");
    double a, b, c, x1, x2;
    a = b = c = x1 = x2 = NAN;
    if (Input_coeffs("Please,enter the coefficients!", &a, &b, &c) == 0)
        {
        return 1;
        }
    int k = -1;// k is responsible for the number of roots
    k = solve_square_eq( a, b, c, &x1, &x2);
        if ( k == 0)
            {
            printf("there is 0 roots here");
            }
        if ( k == 3)
            {
            printf("there is infinite number of roots here");
            }
        if ( k == 1)
            {
            printf("x1 = %lg;", x1);
            }
        if ( k == 2)
            {
            printf("x1 = %lg; x2 = %lg;", x1, x2);
            }
     return 0;
     }