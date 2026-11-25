#include <iostream>
#include "Correlation.h"

using namespace std;


/**
*	The mean of a vector
*/
double Correlation::mean(vector<double> v1)
{
    double sum = v1[0];
    for (unsigned int i=1; i < v1.size(); i++)
        sum += v1[i];
    return sum / v1.size();
}

/**
*	The Covariance
*/
double Correlation::covariance(vector<double> v1, vector<double> v2)
{
    double mean1 = mean(v1), mean2 = mean(v2);
    double sum = (v1[0] - mean1) * (v2[0] - mean2);
    for (unsigned int i=1; i < v1.size(); i++){
        sum += (v1[i] - mean1) * (v2[i] - mean2);
    }
    return sum / (v1.size()-1);
}

/**
*	standard deviation the covariance where both vectors are the same.
*/
double Correlation::std_dev(vector<double> v1)
{
    return sqrt(covariance(v1, v1));
}

/**
*	Pearson Correlation
*/
/*
double Correlation::pearson(vector<double> v1, vector<double> v2)
{
    if (std_dev(v1) * std_dev(v2) == 0)
    {
        cout << "( a standard deviaton was 0 )" << endl;
        return -2; // I dont know what to do here???
    }
    return covariance(v1,v2) / ( std_dev(v1) * std_dev(v2));
}
*/

double Correlation::pearson(double totalXY, double totalX, double totalY, double totalSquareX, double totalSquareY, double totalX_totalX, double totalY_totalY, int n)
{
    return ((n*totalXY)-(totalX*totalY))/(sqrt(((n*totalSquareX)-(totalX_totalX))*((n*totalSquareY)-(totalY_totalY))));
}

