#ifndef CORRELATION_H
#define CORRELATION_H

#include <vector>

using namespace std;

class Correlation
{
public:
    /**
     *	Pearson Correlation
     */
    //static double pearson(vector<double> v1, vector<double> v2);
    static double pearson(double totalXY, double totalX, double totalY, double totalSquareX, double totalSquareY, double totalX_totalX, double totalY_totalY, int n);

private:
    /**
     *	The mean of a vector
     */
    static double mean(vector<double> v1);

    /**
     *	The Covariance
     */
    static double covariance(vector<double> v1, vector<double> v2);

    /**
     *	standard deviation the covariance where both vectors are the same.
     */
    static double std_dev(vector<double> v1);
};
#endif // CORRELATION_H

