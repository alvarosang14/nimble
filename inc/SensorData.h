//SensorData.h
#ifndef SENSORDATA_H
#define SENSORDATA_H

//Include standard files
//Header providing file stream classes
#include <fstream>
//Defines the standard input/output stream objects
#include <iostream>
//Provides string stream classes
#include <sstream>
//Provides parametric manipulators
#include <iomanip>
//Introduces random number generation facilities
#include <random>
//Defines a collection of functions especially designed to be used on ranges of elements
#include <algorithm>
//Defines the vector container class
#include <vector>
//Introduces string types, character traits and a set of converting functions
#include <string>

//3rd party libraries
#include <yaml-cpp/yaml.h>

//Standard namespace
using namespace std;

//Sensor Event structure
struct SensorEvent
{
    //Sensor event default Constructor
    SensorEvent(): wrist_flex_ext(0), wrist_abd_add(0), elbow_flex_ext(0), elbow_pron_sup(0), shoulder_flex_ext(0), shoulder_abd_add(0), shoulder_ext_int_rot(0), time(0)
    {
    }

    //Sensor event parameterized Constructor
    SensorEvent(double WRIST_FLEX_EXT, double WRIST_ABD_ADD, double ELBOW_FLEX_EXT, double ELBOW_PRON_SUP, double SHOULDER_FLEX_EXT, double SHOULDER_ABD_ADD, double SHOULDER_EXT_INT_ROT, double Time): wrist_flex_ext(WRIST_FLEX_EXT), wrist_abd_add(WRIST_ABD_ADD), elbow_flex_ext(ELBOW_FLEX_EXT), elbow_pron_sup(ELBOW_PRON_SUP), shoulder_flex_ext(SHOULDER_FLEX_EXT), shoulder_abd_add(SHOULDER_ABD_ADD), shoulder_ext_int_rot(SHOULDER_EXT_INT_ROT), time(Time)
    {
    }

    //Event data
    double wrist_flex_ext, wrist_abd_add, elbow_flex_ext, elbow_pron_sup, shoulder_flex_ext, shoulder_abd_add, shoulder_ext_int_rot, time;
};

//Sensor Data class
class SensorData
{   
    //Public access members (members are accesible from outside the class)
    public:

        //Sensor data default Constructor
        SensorData();
        //Sensor data parameterized Constructor
        SensorData(const string& traceFile);

        //Input/Output functions
        void loadYamlFile(const string& traceFile);
        void exportYamlFile(const string& traceFile) const;
        string exportYamlString() const;

        //Access methods
        string getPatientID() const;
        string getDate() const;
        string getLocation() const;
        string getPatternType() const;
        int getPatternLevel() const;
        int getClassificationGroup() const;
        string getWorkspace() const;
        string getCoordinateSpace() const;
        double getTotalTime() const;
        bool getArm() const;

        //Modifier methods
        void setPatientID(string patientID_str);
        void setDate(string date_str);
        void setLocation(string location_str);
        void setPatternType(string patternType_str);
        void setPatternLevel(int patternLevel_int);
        void setClassificationGroup(int classificationGroup_int);
        void setWorkspace(string workspace_str);
        void setCoordinateSpace(string coordinateSpace_str);
        void setTotalTime(double totalTime_dbl);
        void setArm(bool arm_bool);

        //Vector methods
        //Returns a reference to the element at the position specified in the vector
        SensorEvent& at(size_t pos);
        const SensorEvent& at(size_t pos) const;
        //Returns a reference to the element at the position specified in the vector container
        SensorEvent& operator[](size_t pos);
        const SensorEvent& operator[](size_t pos) const;
        //Adds a new element (value) at the end of the vector, after its current last element
        void push_back(const SensorEvent& value);
        //Removes the last element in the vector, effectively reducing the container size by one
        void pop_back();
        //Returns whether the vector is empty (size = 0)
        bool empty() const;
        //Returns the number of elements in the vector
        size_t size() const;
        //Returns an iterator pointing to the first element in the vector
        vector<SensorEvent>::iterator begin();
        vector<SensorEvent>::const_iterator begin() const;
        //Returns an iterator referring to the past-the-end element in the vector container
        vector<SensorEvent>::iterator end();
        vector<SensorEvent>::const_iterator end() const;
        //Removes all the elements from the vector, leaving the container with a size of 0
        void clear();

    //Private access members (members are not accesible (or viewed) from outside the class)
    private:

        //Find the minimun and maximum bounds
        void findMinMaxBounds();

        //Private attributes
        string filename;
        string patientID;
        string date;
        string location;
        string patternType;
        int patternLevel;
        int classificationGroup;
        string workspace;
        string coordinateSpace;
        double totalTime;
        //Select the arm
        bool arm = false;

        //Vector with sensor events (Sensor wrist_flex_ext, wrist_abd_add, elbow_flex_ext, elbow_pron_sup, shoulder_flex_ext, shoulder_abd_add, shoulder_ext_int_rot, and sampling time)
        vector<SensorEvent> events;
        SensorEvent maxBound;
        SensorEvent minBound;
};

//Inline methods (more efficient at runtime)

//Access methods
inline string SensorData::getPatientID() const
{
    return patientID;
}

inline string SensorData::getDate() const
{
    return date;
}

inline string SensorData::getLocation() const
{
    return location;
}

inline string SensorData::getPatternType() const
{
    return patternType;
}

inline int SensorData::getPatternLevel() const
{
    return patternLevel;
}

inline int SensorData::getClassificationGroup() const
{
    return classificationGroup;
}

inline string SensorData::getWorkspace() const
{
    return workspace;
}

inline string SensorData::getCoordinateSpace() const
{
    return coordinateSpace;
}

inline double SensorData::getTotalTime() const
{
    return totalTime;
}

inline bool SensorData::getArm() const
{
    return arm;
}

//Modifier methods
inline void SensorData::setPatientID(string patientID_str)
{
    patientID = patientID_str;
}

inline void SensorData::setDate(string date_str)
{
    date = date_str;
}

inline void SensorData::setLocation(string location_str)
{
    location = location_str;
}

inline void SensorData::setPatternType(string patternType_str)
{
    patternType = patternType_str;
}

inline void SensorData::setPatternLevel(int patternLevel_int)
{
    patternLevel = patternLevel_int;
}

inline void SensorData::setClassificationGroup(int classificationGroup_int)
{
    classificationGroup = classificationGroup_int;
}

inline void SensorData::setWorkspace(string workspace_str)
{
    workspace = workspace_str;
}

inline void SensorData::setCoordinateSpace(string coordinateSpace_str)
{
    coordinateSpace = coordinateSpace_str;
}

inline void SensorData::setTotalTime(double totalTime_dbl)
{
    totalTime = totalTime_dbl;
}

inline void SensorData::setArm(bool arm_bool)
{
    arm = arm_bool;
}

//Vector methods
inline SensorEvent& SensorData::at(size_t pos)
{
    return events.at(pos);
}

inline const SensorEvent& SensorData::at(size_t pos) const
{
    return events.at(pos);
}

inline SensorEvent& SensorData::operator[](size_t pos)
{
    return events[pos];
}

inline const SensorEvent& SensorData::operator[](size_t pos) const
{
    return events[pos];
}

inline void SensorData::push_back(const SensorEvent& value)
{
    events.push_back(value);
}

inline void SensorData::pop_back()
{
    events.pop_back();
}

inline bool SensorData::empty() const
{
    return events.empty();
}

inline size_t SensorData::size() const
{
    return events.size();
}

inline vector<SensorEvent>::iterator SensorData::begin()
{
    return events.begin();
}

inline vector<SensorEvent>::const_iterator SensorData::begin() const
{
    return events.begin();
}

inline vector<SensorEvent>::iterator SensorData::end()
{
    return events.end();
}

inline vector<SensorEvent>::const_iterator SensorData::end() const
{
    return events.end();
}

inline void SensorData::clear()
{
    events.clear();
}

//Define YAML namespace to provide a method for preventing name conflicts
namespace YAML
{
    //Template
    template<>

    //Sensor Event conversion structure
    struct convert<SensorEvent>
    {
        //wrist_flex_ext, wrist_abd_add, elbow_flex_ext, elbow_pron_sup, shoulder_flex_ext, shoulder_abd_add, shoulder_ext_int_rot, and sampling time encoded as [wrist_flex_ext, wrist_abd_add, elbow_flex_ext, elbow_pron_sup, shoulder_flex_ext, shoulder_abd_add, shoulder_ext_int_rot, time] in YAML
        static Node encode(const SensorEvent& rhs)
        {
            Node node;
            node.push_back(rhs.wrist_flex_ext);
            node.push_back(rhs.wrist_abd_add);
            node.push_back(rhs.elbow_flex_ext);
            node.push_back(rhs.elbow_pron_sup);
            node.push_back(rhs.shoulder_flex_ext);
            node.push_back(rhs.shoulder_abd_add);
            node.push_back(rhs.shoulder_ext_int_rot);
            node.push_back(rhs.time);
            return node;
        }

        static bool decode(const Node& node, SensorEvent& rhs)
        {
            if(!node.IsSequence() || node.size() != 8)
            {
                return false;
            }

            rhs.wrist_flex_ext = node[0].as<double>();
            rhs.wrist_abd_add = node[1].as<double>();
            rhs.elbow_flex_ext = node[2].as<double>();
            rhs.elbow_pron_sup = node[3].as<double>();
            rhs.shoulder_flex_ext = node[4].as<double>();
            rhs.shoulder_abd_add = node[5].as<double>();
            rhs.shoulder_ext_int_rot = node[6].as<double>();
            rhs.time = node[7].as<double>();
            return true;
        }
    };
}

#endif // SENSORDATA_H
