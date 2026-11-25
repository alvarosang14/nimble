#ifndef HAPTICTUNNELFUNCTIONOBJECT_H
#define HAPTICTUNNELFUNCTIONOBJECT_H

#include <vector>
#include <HAPI/HAPIFunctionObject.h>

class HapticTunnelFunctionObject : public HAPI::HAPIFunctionObject
{
    public:
        HapticTunnelFunctionObject(std::vector<HAPI::Vec3> curve, HAPI::HAPIFloat force, unsigned int dimension);

        virtual ~HapticTunnelFunctionObject();

        HAPI::HAPIFloat evaluate(HAPI::HAPIFloat *input);

    private:
        HAPI::Vec3 findNearestCurveIntersection(HAPI::Vec3 loc);

        std::vector<HAPI::Vec3> m_curve;
        HAPI::HAPIFloat m_force;
        unsigned int m_dimension;
};

#endif // HAPTICTUNNELFUNCTIONOBJECT_H
