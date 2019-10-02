//! @file

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

const  int COUNT_OF_TESTS = 1000;
const  int LIMIT_OF_COEFFS = 10000;
const double EPSILON = 0.001;

/**
 * @brief checks correction of input data
 * @param GRT greeting to user
 * @param a ptr to coeff x^2
 * @param b ptr to coeff x
 * @param c ptr to free coeff
 * @return (0 if OK) (1 if user couldn't input correct data after 100 tries)
 */

int Input_coeffs(const char GRT[], double* a, double* b, double* c)// gets coefficients and checks its correct
    {
    assert(a);
    assert(b);
    assert(c);

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
     * @brief calculate roots of linear equations
     * @param b coeff with x in line equation
     * @param c free coeff in line equation
     * @param x1 ptr ro root
     * @param x2 also ptr to root (for comfort);
     * @return number of roots;
     */

int Solve_line(double b, double c, double* x1, double* x2)//solves linear equation
    {
    assert(x1);
    assert(x2);

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
     * @brief calculate roots of square equation
     * @param a coeff x^2
     * @param b coeff x
     * @param c free coeff
     * @param x1 ptr to 1st root
     * @param x2 ptr to 2nd root
     * @return number of roots
     */

int Solve_square_eq(double a, double b, double c, double* x1, double* x2)//solves square equation
    {
    assert(x1);
    assert(x2);

    double d = NAN;
    if ( a == 0 )
        {
        return Solve_line(b, c, x1, x2);
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
     * @brief generates random doubles in [ - lim; + lim }
     * @param q is ptr to double
     * @param lim defines border of double
     * @return
     */

    double Giv_rand(double* q, int lim)//generates random double numbers from -lim to +lim
        {
        assert(q);

        *q = (2*( ((double)(rand())) / RAND_MAX )-1)*lim ;
        }

    void Unit_tests(double eps)//unit tests
        {
        srand(time(NULL));

        for ( int i = 1; i <= COUNT_OF_TESTS; i++)
            {
            double a, b, c, x1, x2;
            int h = 0;
            a = b = c = x1 = x2 = NAN;

            a = Giv_rand( &a, LIMIT_OF_COEFFS);
            b = Giv_rand( &b, LIMIT_OF_COEFFS);
            c = Giv_rand( &c, LIMIT_OF_COEFFS);

            h = Solve_square_eq( a, b, c, &x1, &x2);

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
        * @brief printing results
        * @param n number of roots
        * @param x1 ptr to 1st root
        * @param x2 ptr to 2nd root
        */

 void Printing (int n, double* x1, double* x2)
    {
        if ( n == 0)
        {
            printf("there is 0 roots here");
        }
        if ( n == 3)
        {
            printf("there is infinite number of roots here");
        }
        if ( n == 1)
        {
            printf("x1 = %lg;", *x1);
        }
        if ( n == 2)
        {
            printf("x1 = %lg; x2 = %lg;", *x1, *x2);
        }
    }

        /**
         * @brief main
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
    k = Solve_square_eq( a, b, c, &x1, &x2);
    Printing( k, &x1, &x2);

     return 0;

     }