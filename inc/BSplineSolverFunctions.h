#ifndef BSPLINE_SOLVER_FUNCTIONS
#define BSPLINE_SOLVER_FUNCTIONS

#include <vector>
#include "tinysplinecpp.h"

struct SmoothMeasConstraintData
{
  std::vector<double> contour;
};

class BSplineSolverFunctions
{
  public:
	//Accepts a 2D b-spline
	BSplineSolverFunctions(ts::BSpline spline);

    //Smoothness measure function for fitting clamped bspline curve to 2D points.
    //Implemented for NLopt library c++ interface, so need to unroll/flatten
    //the x and y coordinates of control points Q_i into a 1 dimensional array
    //to input into the NLopt library (first half is x, second half is y).
    //The same applies to the gradient wrt Q_i.
    static double smoothMeas(const ::std::vector<double> &Q,
      std::vector<double> &grad, void *data);

	//Smoothness measure constraints for fitting clamped bspline curve to
	//2D points. Implemented for NLopt library c++ interface.
    static double smoothMeasConstraint(const ::std::vector<double> &Q,
      std::vector<double> &grad, void *data);

	//Calculates the Greville Abscissae of the spline
	std::vector<double> calcGrevAbsc();
	  
	//Weighted second difference of control point i, 
    //for i = 1 to number of control points minus 1
	std::vector<double> weighted2ndDiff(int i);

	// Access basis value N(i,d)(t), where d is the degree
	double basis(int i, int d, double t);

    //Over the interval between two consecutive Greville abscissae[u_k,u_(k+1)]
    //(where k=0,1,...,number of control points - 2) the contribution of the jth B-spline 
    //basis function N(j,d) to the distance between the B-spline X and
    //its control polygon L(t)  is captured by this non-negative and convex 
    //function
	double beta(int k, int j, double t);

    //Axis-aligned bounding box S_k bounding B-spline X at Greville abscissae 
	//u[k] formed from envelopes at X(u[k]). Saved as the 4 corner points
	//of the box.
	std::vector<double> S(int k);

	//Point in polygon test
	static bool pointInPolygon(std::vector<double> point, std::vector<double> contour);

  private:

	ts::BSpline m_spline;
};

#endif