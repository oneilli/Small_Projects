/*Program: trig.c
  Written by: Ian O'Neill
  Written: 17/11/2020
  Description: Program to calculate a series of values for sin(x) using the Taylor Series method
  User input lower limit, upper limit and step size for list of x values for which to calculate sin(x).
  Results are printed to screen in table form.
  */

/* Preprocessing Directives*/
#include <stdio.h>
#include <math.h>

/*Define constants to be used*/
#define TOLERANCE 1e-6
#define PI 3.141592654
#define MAX_LENGTH 50

/*Function prototypes*/
float calcsin(float x,float tolerance);
double calc_factorial(int x);
void printout(const float x_values[],const float sinx_values[], int length);

int main()
{
    float x_uplim, x_lowlim, step_size,counter;
    float x_vals[MAX_LENGTH] = {},sinx_vals[MAX_LENGTH] = {};
    int i,array_len;

    /*Take user input*/
    printf("Enter the lower limit on x: ");
    scanf("%f",&x_lowlim);
    printf("Enter the upper limit on x: ");
    scanf("%f",&x_uplim);
    printf("Enter the step size: ");
    scanf("%f",&step_size);

    array_len = 1 + round((x_uplim-x_lowlim)/step_size); /*calculates the length of the input array defined by the user*/

    /*Error messages - the program stops running if these are triggered*/
    /*Note: The program is configured to allow the lower limit to be greater than the upper limit if the step size is negative*/
    if (array_len > MAX_LENGTH) /*Checks that the array length is within the accepted limit*/
    {
        printf("Error: The array sizes are too big for this program! Try a smaller range or a larger step size.\n");
        return 0;
    }
    else if (((step_size > 0) && (x_lowlim > x_uplim)) || ((step_size < 0) && (x_lowlim < x_uplim))) /*Checks that the lower limit is less than the upper limit if the step size is positive*/
    {                                                                                                /*Alternatively, a negative step size means that we must make sure the lower limit is greater than the upper limit*/
        printf("Error: The step size is inconsistent with the limits chosen.\n");
        return 0;
    }
    else if (step_size == 0) /*Make sure that the step size is nonzero*/
    {
        printf("Error: The step size must be a non-zero number.\n");
        return 0;
    }

    counter = x_lowlim;
    for(i = 0;i < array_len;i++) /*For loop populates the x-values array from lower to upper limit*/
    {
        x_vals[i] = counter;
        while ((x_vals[i] >= 2*PI) || x_vals[i] < 0) /*Small loop here maps each x value to the range [0,2pi] for input into the function*/
            (x_vals[i] > 0) ? (x_vals[i]-=2*PI):(x_vals[i] += 2*PI);
        sinx_vals[i] = calcsin(x_vals[i],TOLERANCE); /*The function below is used to calculate the corresponding sin(x) values*/
        x_vals[i] = counter; /*x value, which has been mapped to [0,2pi], is returned to its original value*/
        counter += step_size;
    }

    printout(x_vals,sinx_vals,array_len); /*The table is printed using this function call*/
    return 0;
}

/*Function to calculate the sin(x) values for an x-value*/
/*Uses the Taylor Expansion to calculate sin(x) to a given error tolerance. Returns float value for sin(x)*/
/*Arguments: float x = x value to calculate sin(x) with, float tolerance = tolerance detailing the point at which to stop adding taylor terms*/
float calcsin(float x,float tolerance)
{
    float total_sum = -1,numerator,denominator; /*total sum = -1 to offset taylor_term = 1*/
    long double taylor_term=1; /*Taylor term is initialised to 1 so that the for loop will begin running (if set to 0 loop won't execute)*/
    int n;

    for (n=0;fabs(taylor_term)>tolerance;n++)
    {
        total_sum += taylor_term; /*We add the taylor term to the running sum at the beginning of each iteration*/
                                  /*This ensures that only terms that are greater than the tolerance get added*/
        numerator = (pow((-1),n)*(pow(x,(2*n + 1))));
        denominator = calc_factorial(2*n+1); /*The factorial function is used in the calculation of each taylor series term*/
        taylor_term = numerator/denominator;
    }

    return total_sum;
}

/*Function to print the table*/
/*Arguments: x_values[] = array of values to be printed in x column
             sinx_values[] = array of values to be printed in sin(x) column
             length = length of the arrays (both arrays should have equal length*/
void printout(const float x_values[],const float sinx_values[], int length)
{
    int i;
    printf("================================\n");
    printf("|  x (radians)  |     sin(x)   |\n");
    printf("|==============================|\n");
    for (i=0;i<length;i++)
        printf("|%10.4f     |%10.4f    |\n",x_values[i],sinx_values[i]);
    printf("================================\n");
}

/*Function to calculate the factorial of an integer input*/
/*Arguments: x = integer input to take the factorial of, e.g. function returns x!*/
double calc_factorial(int x)
{
    double factorial_sum = 1,i; /*we make this a double as larger integers will have a factorial that does not fit into long long int datatype*/
    for (i=0;i < x;i++)
            factorial_sum *= (x-i);
    return factorial_sum;
}
