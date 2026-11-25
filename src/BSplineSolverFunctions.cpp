#include <cmath>
#include <iostream>
#include "BSplineSolverFunctions.h"
#include <algorithm>

//Constructor
BSplineSolverFunctions::BSplineSolverFunctions(ts::BSpline spline): m_spline(spline)
{
}

//Smooth Measure
double BSplineSolverFunctions::smoothMeas(const ::std::vector<double> &Q,
                                          std::vector<double> &grad, void *data)
{
    int numOfQ = static_cast<int>(Q.size() / 2);

    //Create a clamped spline of degree 5 in 2D consisting of sPoints.size() control points
    ts::BSpline spline(5, 2, numOfQ, TS_CLAMPED);
    spline.setCtrlp(Q);

    BSplineSolverFunctions bFuncs(spline);

    std::vector<double> result(2, 0.0);
    std::vector<double> secondDiff;

    for(int i = 1; i < numOfQ - 2; i++)
    {
        secondDiff = bFuncs.weighted2ndDiff(i);

        result[0] += pow(secondDiff[0], 2.0);
        result[1] += pow(secondDiff[1], 2.0);
    }

    if(!grad.empty())
    {
        for(int i = 0; i < numOfQ; i++)
        {
            grad[2*i] = 0.0;
            grad[2*i+1] = 0.0;
        }
    }

    return sqrt(pow(result[0], 2) + pow(result[1], 2));
}

//Smooth Measure Constraint
double BSplineSolverFunctions::smoothMeasConstraint(const ::std::vector<double> &Q,
                                                    std::vector<double> &grad, void *data)
{
    SmoothMeasConstraintData *d = reinterpret_cast<SmoothMeasConstraintData*>(data);
    std::vector<double> contour = d->contour;
    int numOfQ = static_cast<int>(Q.size() / 2);

    // Create a clamped spline of degree 5 in 2D consisting of sPoints.size() control points.
    ts::BSpline spline(5, 2, numOfQ, TS_CLAMPED);
    spline.setCtrlp(Q);

    BSplineSolverFunctions bFuncs(spline);

    bool constrained = true;

    for(int i = 0; i < numOfQ; i++)
    {
        std::vector<double> S = bFuncs.S(i);

        for(int j = 0; j < 8; j+=2)
        {
            std::vector<double> corner;
            corner.push_back(S[j]);
            corner.push_back(S[j+1]);
            constrained &= BSplineSolverFunctions::pointInPolygon(corner, contour);
        }
    }

    if(!grad.empty())
    {
        for(int i = 0; i < numOfQ; i++)
        {
            grad[2*i] = 0.0;
            grad[2*i+1] = 0.0;
        }
    }

    if(constrained)
    {
        return 1.0;
    }
    else
    {
        return -1.0;
    }
}


std::vector<double> BSplineSolverFunctions::calcGrevAbsc()
{
  std::vector<double> grevAbsc(m_spline.nCtrlp());
  int degree = m_spline.deg();
  std::vector<double> knots = m_spline.knots();

  for (int i = 0; i < grevAbsc.size(); i++)
  {
    double avg = 0;
	for (int j = i+1; j <= i+degree; j++)
	{
	  avg += knots[j];
    }

	grevAbsc[i] = avg / degree;
  }

  return grevAbsc;
}

std::vector<double> BSplineSolverFunctions::weighted2ndDiff(int i)
{
  std::vector<double> p(2);

  std::vector<double> controls = m_spline.ctrlp();
  int numControls = m_spline.nCtrlp();
  std::vector<double> u = calcGrevAbsc();

  //Cap i at 1 and numControls-1
  int j = (i < 1 ? 1 : (i > numControls-2 ? numControls-2 : i));
  
  std::vector<double> control0(2);
  std::vector<double> control1(2);
  control0[0] = (controls[2*j] - controls[2*(j-1)]) / (u[j] - u[j-1]);
  control0[1] = (controls[2*j+1] - controls[2*(j-1)+1]) / (u[j] - u[j-1]);
  control1[0] = (controls[2*(j+1)] - controls[2*j]) / (u[j+1] - u[j]);
  control1[1] = (controls[2*(j+1)+1] - controls[2*j+1]) / (u[j+1] - u[j]);

  p[0] = control1[0] - control0[0];
  p[1] = control1[1] - control0[1];

  return p;
}

double BSplineSolverFunctions::basis(int i, int d, double t)
{
  std::vector<double> knots = m_spline.knots();

  if(d == 0)
  {
	if(knots[i] <= t && t < knots[i+1])
	{
	  return 1.0;
	}
	else
	{
	  return 0.0;
	}
  }

  double a1 = (t - knots[i]) / (knots[i+d] - knots[i]);
  double a2 = (knots[i+d+1] - t) / (knots[i+d+1] - knots[i+1]);

  return a1*basis(i,d-1,t) + a2*basis(i+1,d-1,t);
}

double BSplineSolverFunctions::beta(int k, int j, double t)
{
  double sum = 0.0;
  int numControls = m_spline.nCtrlp();
  int degree = m_spline.deg();
  std::vector<double> u = calcGrevAbsc();

  if(k < 0 || k > numControls-1)
  {
    return sum;
  }

  ts::DeBoorNet dbNet = m_spline.evaluate(t);

  int imax = dbNet.k();
  int imin = imax - degree;
  

  if(j > k)
  {
    for(int i = j; i <= imax; i++)
	{
      sum += (u[i] - u[j]) * basis(i, degree, t);
	}
  }
  else
  {
    for(int i = imin; i <= j; i++)
	{
      sum += (u[i] - u[j]) * basis(i, degree, t);
	}
  }

  return sum;
}

std::vector<double> BSplineSolverFunctions::S(int k)
{
  std::vector<double> corners(8, 0.0);
  int degree = m_spline.deg();
  int numControls = m_spline.nCtrlp();
  std::vector<double> controls = m_spline.ctrlp();
  std::vector<double> u = calcGrevAbsc();

  if(k < 0 || k > numControls-1)
  {
    return corners;
  }

  ts::DeBoorNet dbNet = m_spline.evaluate(u[k]);

  int imax = dbNet.k();
  int imin = imax - degree;

  std::vector<double> E_lower(2);
  E_lower[0] = controls[2*k];
  E_lower[1] = controls[2*k+1];
  std::vector<double> E_upper(2);
  E_upper[0] = controls[2*k];
  E_upper[1] = controls[2*k+1];
  std::vector<double> w2ndDiff;
  double B;
  for(int j = imin; j <= imax; j++)
  {
	w2ndDiff = weighted2ndDiff(j);
	B = beta(k, j, u[k]);
	E_lower[0] += std::min(w2ndDiff[0], 0.0) * B;
	E_lower[1] += std::min(w2ndDiff[1], 0.0) * B;

	E_upper[0] += std::max(w2ndDiff[0], 0.0) * B;
	E_upper[1] += std::max(w2ndDiff[1], 0.0) * B;
  }

  corners[0] = E_lower[0]; 
  corners[1] = E_lower[1];
  corners[2] = E_upper[0];
  corners[3] = E_lower[1];
  corners[4] = E_upper[0];
  corners[5] = E_upper[1];
  corners[6] = E_lower[0];
  corners[7] = E_upper[1];

  return corners;
}

bool BSplineSolverFunctions::pointInPolygon(std::vector<double> point, 
  std::vector<double> contour)
{
  int i, j, nvert = contour.size()/2;
  bool c = false;

  for(i = 0, j = nvert - 1; i < nvert; j = i++) {
    if( ( (contour[2*i+1] >= point[1] ) != (contour[2*j+1] >= point[1]) ) &&
        (point[0] <= (contour[2*j] - contour[2*i]) * (point[1] - contour[2*i+1]) / 
		  (contour[2*j+1] - contour[2*i+1]) + contour[2*i])
      )
      c = !c;
  }

  return c;
}
