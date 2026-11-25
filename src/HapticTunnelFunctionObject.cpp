#include "HapticTunnelFunctionObject.h"

HapticTunnelFunctionObject::HapticTunnelFunctionObject(
  std::vector<HAPI::Vec3> curve, HAPI::HAPIFloat force, unsigned int dimension)
 :HAPI::HAPIFunctionObject(),
  m_curve(curve),
  m_force(force),
  m_dimension(dimension)
{
}

HapticTunnelFunctionObject::~HapticTunnelFunctionObject()
{
}

HAPI::HAPIFloat HapticTunnelFunctionObject::evaluate(HAPI::HAPIFloat *input)
{
  HAPI::Vec3 loc(input[0], input[1], input[2]);

  //Find point on curve nearest to loc
  HAPI::Vec3 target = findNearestCurveIntersection(loc);

  //Create output force vector
  HAPI::Vec3 outputVector = target - loc;
  HAPI::HAPIFloat outputVectorLength =
    H3DUtil::H3DSqrt(H3DUtil::H3DPow(outputVector.x, 2) +
    H3DUtil::H3DPow(outputVector.y, 2) +
    H3DUtil::H3DPow(outputVector.z, 2));
  outputVector /= outputVectorLength;
  if(outputVectorLength > 0.001)
  {
    outputVector *= m_force;
  }
  else
  {
    //When sufficiently close, apply a quarter of the force requested
    outputVector *= m_force / 4.0;
  }

  //Choose output for the function based on the dimension requested
  HAPI::HAPIFloat result;
  switch(m_dimension)
  {
    case 0:
      result = outputVector.x;
      break;
    case 1:
      result = outputVector.y;
      break;
    case 2:
      result = outputVector.z;
      break;
  }

  return result;
}

HAPI::Vec3 HapticTunnelFunctionObject::
  findNearestCurveIntersection(HAPI::Vec3 loc)
{
  HAPI::Vec3 point;

  HAPI::Vec3 a, b;
  HAPI::Vec3 p;
  HAPI::HAPIFloat t0;
  HAPI::HAPIFloat dist;
  HAPI::HAPIFloat minDist;
  for(int i = 0; i < m_curve.size()-1; i++)
  {
    a = m_curve[i];
    b = m_curve[i+1];

    t0 = ((b - a)*(loc - a)) / ((b - a)*(b - a));

    if(t0 <= 0)
    {
      p = a;
    }
    else if(t0 < 1)
    {
      p = a + (b - a) * t0;
    }
    else
    {
      p = b;
    }
    HAPI::Vec3 outputVector = p - loc;
    dist = H3DUtil::H3DSqrt(H3DUtil::H3DPow(outputVector.x, 2) +
                            H3DUtil::H3DPow(outputVector.y, 2) +
                            H3DUtil::H3DPow(outputVector.z, 2));
    if(i == 0 || dist < minDist)
    {
      minDist = dist;
      point = p;
    }
  }

  return point;
}
