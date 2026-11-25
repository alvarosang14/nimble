//HapticEnvironment class implementation

#include <iostream>
#include "HapticEnvironment.h"
#include "HapticTunnelFunctionObject.h"

//Try to retain 4:3 ratio
const double HapticEnvironment::ENV_MAX_X_COORD = 0.08;
const double HapticEnvironment::ENV_MIN_X_COORD = -0.08;
const double HapticEnvironment::ENV_MAX_Y_COORD = 0.09;
const double HapticEnvironment::ENV_MIN_Y_COORD = -0.03;
const double HapticEnvironment::ENV_MAX_Z_COORD = 0.05;
const double HapticEnvironment::ENV_MIN_Z_COORD = 0.0;

HapticEnvironment::HapticEnvironment(): m_hDevice(0), m_boundBox(0), m_boundBoxSurface(0), m_tunnelEffect(0)
{
    //Calculate norm transform coefficients for 2d norm position transform
    calculateNormTransform();

    //Calculate unnorm transform coefficients for 2d unnorm position transform
    calculateUnnormTransform();
}

HapticEnvironment::~HapticEnvironment()
{
  disableHD();
}

void HapticEnvironment::calculateNormTransform()
{
    //Minimum corner
    HAPI::Vec3 minCorner(ENV_MIN_X_COORD,ENV_MIN_Y_COORD,ENV_MIN_Z_COORD);
    //Maximum corner
    HAPI::Vec3 maxCorner(ENV_MAX_X_COORD,ENV_MAX_Y_COORD,ENV_MAX_Z_COORD);
    //Normalized minimum corner
    HAPI::Vec3 normMinCorner(-1.0, -1.0, -1.0);
    //Normalized maximum corner
    HAPI::Vec3 normMaxCorner(1.0, 1.0, 1.0);

    //Scale matrix for scaling the haptic coordinates to the norm coordinates
    HAPI::Matrix4 scaleTransform(
    (normMaxCorner.x - normMinCorner.x)/(maxCorner.x - minCorner.x), 0, 0, 0,
    0, (normMaxCorner.y - normMinCorner.y)/(maxCorner.y - minCorner.y), 0, 0,
    0, 0, (normMaxCorner.z - normMinCorner.z)/(maxCorner.z - minCorner.z), 0,
    0, 0, 0, 1);

    HAPI::Vec4 newMaxCorner = scaleTransform *
    HAPI::Vec4(maxCorner.x, maxCorner.y, maxCorner.z, 1);

    //Translation matrix for moving the haptic coordinates after scaling
    HAPI::Matrix4 translationTransform(
        1, 0, 0, (normMaxCorner.x - newMaxCorner.x),
        0, 1, 0, (normMaxCorner.y - newMaxCorner.y),
        0, 0, 1, (normMaxCorner.z - newMaxCorner.z),
        0, 0, 0, 1);

    //Norm affine transform is to perform the scale matrix multiplication first, then do translation
    m_normTransform = translationTransform * scaleTransform;
}

void HapticEnvironment::calculateUnnormTransform()
{
  HAPI::Vec3 minCorner(ENV_MIN_X_COORD,ENV_MIN_Y_COORD,ENV_MIN_Z_COORD);
  HAPI::Vec3 maxCorner(ENV_MAX_X_COORD,ENV_MAX_Y_COORD,ENV_MAX_Z_COORD);
  HAPI::Vec3 normMinCorner(-1.0, -1.0, -1.0);
  HAPI::Vec3 normMaxCorner(1.0, 1.0, 1.0);

  //Scale matrix for scaling the haptic coordinates to the norm coordinates
  HAPI::Matrix4 scaleTransform(
    (maxCorner.x - minCorner.x)/(normMaxCorner.x - normMinCorner.x), 0, 0, 0,
    0, (maxCorner.y - minCorner.y)/(normMaxCorner.y - normMinCorner.y), 0, 0,
    0, 0, (maxCorner.z - minCorner.z)/(normMaxCorner.z - normMinCorner.z), 0,
    0, 0, 0, 1);

  HAPI::Vec4 newMaxCorner = scaleTransform *
      HAPI::Vec4(normMaxCorner.x, normMaxCorner.y, normMaxCorner.z, 1);

  //Translation matrix for moving the haptic coordinates after scaling
  HAPI::Matrix4 translationTransform(
        1, 0, 0, (maxCorner.x - newMaxCorner.x),
        0, 1, 0, (maxCorner.y - newMaxCorner.y),
        0, 0, 1, (maxCorner.z - newMaxCorner.z),
        0, 0, 0, 1);

  //Unnorm affine transform is to perform the scale matrix multiplication first,
  //then do translation
  m_unnormTransform = translationTransform * scaleTransform;
}

bool HapticEnvironment::initHD()
{
  //initialize haptic device with haptics renderer
  m_hDevice = new HAPI::AnyHapticsDevice();
  m_hDevice->setHapticsRenderer(new HAPI::RuspiniRenderer());
  if(m_hDevice->initDevice(1000) != HAPI::HAPIHapticsDevice::SUCCESS)
  {
    std::cerr << "Could not init haptic device" << std::endl;
    m_hDevice->releaseDevice();
    m_hDevice = 0;
    return false;
  }

  //enable device
  if(m_hDevice->enableDevice() != HAPI::HAPIHapticsDevice::SUCCESS)
  {
    std::cerr << "Could not enable haptic device" << std::endl;
    m_hDevice->releaseDevice();
    m_hDevice = 0;
    return false;
  }

  return true;
}

//Disable Haptic Device
void HapticEnvironment::disableHD()
{
    //If Haptic Device is enable
    if(isDeviceEnabled())
    {
        //Disable Haptic Device
        m_hDevice->disableDevice();
    }

    //If any Haptic device
    if(m_hDevice != 0)
    {
        //Release the Haptic Device
        m_hDevice->releaseDevice();
    }

    //Clear Haptic Device
    m_hDevice = 0;
}

void HapticEnvironment::createBoundBox(HAPI::HAPIFloat stiffness,
                                       HAPI::HAPIFloat damping,
                                       HAPI::HAPIFloat friction)
{
  //remove old bounding box from haptic loop
  if(isDeviceEnabled() &&
     m_boundBox != 0)
  {
    m_hDevice->removeShape(m_boundBox);
  }

  //Define the friction surface for bounding box
  m_boundBoxSurface = new HAPI::FrictionSurface(stiffness, damping, 0, friction);
  //Bound box primitive to use for the haptic shape
  HAPI::Vec3 minCorner(ENV_MIN_X_COORD,ENV_MIN_Y_COORD,ENV_MIN_Z_COORD);
  HAPI::Vec3 maxCorner(ENV_MAX_X_COORD,ENV_MAX_Y_COORD,ENV_MAX_Z_COORD);
  HAPI::Collision::AABox *boxPrimitive =
    new HAPI::Collision::AABox(minCorner,maxCorner);
  //Create bound box with defined friction surface
  m_boundBox = new HAPI::HapticPrimitive(
              boxPrimitive,
              m_boundBoxSurface,
              HAPI::Collision::BACK);

  //add bounding box to haptic loop
  if(isDeviceEnabled())
  {
    m_hDevice->addShape(m_boundBox);
    m_hDevice->transferObjects();
  }
}

//Note: expects a curve with normalized coordinates
void HapticEnvironment::createTunnelEffect(std::vector<HAPI::Vec3> curve,
  HAPI::HAPIFloat force)
{
  //remove old tunnel effect from haptic loop
  if(isDeviceEnabled() &&
     m_tunnelEffect != 0)
  {
    removeEffect(m_tunnelEffect);
  }

  //Unnormalize the curve's coordinates
  curve = unnormalizeCurve(curve);

  HapticTunnelFunctionObject *xForce =
    new HapticTunnelFunctionObject(curve, force, 0);
  HapticTunnelFunctionObject *yForce =
    new HapticTunnelFunctionObject(curve, force, 1);
  HapticTunnelFunctionObject *zForce =
    new HapticTunnelFunctionObject(curve, force, 2);

  m_tunnelEffect =
    new HAPI::HapticPositionFunctionEffect(xForce, yForce, zForce);

  //add bounding box to haptic loop
  addEffect(m_tunnelEffect);
}

void HapticEnvironment::addEffect(HAPI::HAPIForceEffect *effect)
{
  if(isDeviceEnabled())
  {
    m_hDevice->addEffect(effect);
    m_hDevice->transferObjects();
  }
}

void HapticEnvironment::removeEffect(HAPI::HAPIForceEffect *effect)
{
  if(isDeviceEnabled())
  {
    m_hDevice->removeEffect(effect);
  }
}

HAPI::Vec3 HapticEnvironment::getPosition()
{
  if(isDeviceEnabled())
  {
    return m_hDevice->getPosition();
  }
  //return origin by default
  return HAPI::Vec3(0,0,0);
}

HAPI::Vec3 HapticEnvironment::getNormPosition()
{
  HAPI::Vec3 p = getPosition();

  //Perform transform equations
  HAPI::Vec4 normP = m_normTransform * HAPI::Vec4(p.x, p.y, p.z, 1);

  //Cap at normalized bounds
  normP.x = H3DUtil::H3DMax(normP.x, -1.0);
  normP.y = H3DUtil::H3DMax(normP.y, -1.0);
  normP.z = H3DUtil::H3DMax(normP.z, -1.0);
  normP.x = H3DUtil::H3DMin(normP.x, 1.0);
  normP.y = H3DUtil::H3DMin(normP.y, 1.0);
  normP.z = H3DUtil::H3DMin(normP.z, 1.0);

  return HAPI::Vec3(normP.x, normP.y, normP.z);
}

//Get Button Status
bool HapticEnvironment::getButtonStatus(unsigned int button)
{
    //If
    if(isDeviceEnabled())
    {
        return m_hDevice->getButtonStatus(button);
    }
        return false;
}

std::vector<HAPI::Vec3> HapticEnvironment::unnormalizeCurve(
  std::vector<HAPI::Vec3> curve)
{
  for(int i = 0; i < curve.size(); i++)
  {
    HAPI::Vec4 unnormP =
      m_unnormTransform * HAPI::Vec4(curve[i].x, curve[i].y, curve[i].z, 1);

    curve[i].x = unnormP.x;
    curve[i].y = unnormP.y;
    curve[i].z = unnormP.z;
  }

  return curve;
}

bool HapticEnvironment::isDeviceEnabled()
{
  return (m_hDevice != 0 && m_hDevice->getDeviceState() == HAPI::HAPIHapticsDevice::ENABLED);
}
