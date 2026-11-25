//HapticEnvironment class
//Description: Class to implement haptic environment in HAPI

#ifndef HAPTICENVIRONMENT_H
#define HAPTICENVIRONMENT_H

#include <HAPI/AnyHapticsDevice.h>
#include <HAPI/RuspiniRenderer.h>
#include <HAPI/FrictionSurface.h>
#include <HAPI/HapticPrimitive.h>
#include <HAPI/HAPIForceEffect.h>
#include <HAPI/HapticPositionFunctionEffect.h>

class HapticEnvironment
{
    public:
        HapticEnvironment();

        ~HapticEnvironment();

        bool initHD();

        void disableHD();

        void createBoundBox(HAPI::HAPIFloat stiffness, HAPI::HAPIFloat damping, HAPI::HAPIFloat friction);

        void createTunnelEffect(std::vector<HAPI::Vec3> curve, HAPI::HAPIFloat force);

        void addEffect(HAPI::HAPIForceEffect *effect);

        void removeEffect(HAPI::HAPIForceEffect *effect);

        HAPI::Vec3 getPosition();

        HAPI::Vec3 getNormPosition();

        bool getButtonStatus(unsigned int button);

        std::vector<HAPI::Vec3> unnormalizeCurve(std::vector<HAPI::Vec3> curve);

        bool isDeviceEnabled();

        //Constants that impose spatial limits to the haptic environment
        //Coordinates are in haptic coordinate system
        static const double ENV_MAX_X_COORD;
        static const double ENV_MIN_X_COORD;
        static const double ENV_MAX_Y_COORD;
        static const double ENV_MIN_Y_COORD;
        static const double ENV_MAX_Z_COORD;
        static const double ENV_MIN_Z_COORD;

    private:

        void calculateNormTransform();
        void calculateUnnormTransform();

        HAPI::Matrix4 m_normTransform;
        HAPI::Matrix4 m_unnormTransform;

        HAPI::AnyHapticsDevice *m_hDevice;
        HAPI::HapticPrimitive *m_boundBox;
        HAPI::FrictionSurface *m_boundBoxSurface;
        HAPI::HapticPositionFunctionEffect *m_tunnelEffect;
};

#endif // HAPTICENVIRONMENT_H
