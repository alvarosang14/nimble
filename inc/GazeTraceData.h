//GazetraceData.h
#ifndef GAZE_TRACE_DATA_H
#define GAZE_TRACE_DATA_H

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

//Gaze Trace Event structure
struct GazeTraceEvent
{
    //Gaze trace event default Constructor
    GazeTraceEvent(): x(0), y(0), time(0)
    {
    }

    //Gaze trace event parameterized Constructor
    GazeTraceEvent(double X, double Y, double Time): x(X), y(Y), time(Time)
    {
    }

    //Operator "-" overloaded
    GazeTraceEvent operator-(const GazeTraceEvent& rhs) const
    {
        return GazeTraceEvent(x-rhs.x, y-rhs.y, time-rhs.time);
    }

    //Operator "+" overloaded
    GazeTraceEvent operator+(const GazeTraceEvent& rhs) const
    {
        return GazeTraceEvent(x+rhs.x, y+rhs.y, time+rhs.time);
    }

    //Operator "*" overloaded
    GazeTraceEvent operator*(const double rhs) const
    {
        return GazeTraceEvent(x*rhs,y*rhs,time*rhs);
    }

    //Operator "+=" overloaded
    GazeTraceEvent& operator+=(const GazeTraceEvent& rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        time = time + rhs.time;

        return *this;
    }

    //Event data
    double x, y, time;
};

//Gaze Trace Data class
class GazeTraceData
{
    //Public access members (members are accesible from outside the class)
    public:

        //Gaze trace data default Constructor
        GazeTraceData();
        //Gaze trace data parameterized Constructor
        GazeTraceData(const string& traceFile);

        //Input/Output functions
        void loadYamlFile(const string& traceFile);
        void exportYamlFile(const string& traceFile) const;
        string exportYamlString() const;

        //Access methods
        string getPatientID() const;
        string getDate() const; //TODO: select standared date format
        string getLocation() const;
        string getPatternType() const;
        int getPatternLevel() const;
        int getClassificationGroup() const;
        string getWorkspace() const;
        string getCoordinateSpace() const;
        double getTotalTime() const;

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

        //Vector methods
        //Returns a reference to the element at the position specified in the vector
        GazeTraceEvent& at(size_t pos);
        const GazeTraceEvent& at(size_t pos) const;
        //Returns a reference to the element at the position specified in the vector container
        GazeTraceEvent& operator[](size_t pos);
        const GazeTraceEvent& operator[](size_t pos) const;
        //Adds a new element (value) at the end of the vector, after its current last element
        void push_back(const GazeTraceEvent&);
        //Removes the last element in the vector, effectively reducing the container size by one
        void pop_back();
        //Returns whether the vector is empty (size = 0)
        bool empty() const;
        //Returns the number of elements in the vector
        size_t size() const;
        //Returns an iterator pointing to the first element in the vector
        vector<GazeTraceEvent>::iterator begin();
        vector<GazeTraceEvent>::const_iterator begin() const;
        //Returns an iterator referring to the past-the-end element in the vector container
        vector<GazeTraceEvent>::iterator end();
        vector<GazeTraceEvent>::const_iterator end() const;
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

        //Vector with trace events (Gaze x, y, and sampling time)
        vector<GazeTraceEvent> events;
        GazeTraceEvent maxBound;
        GazeTraceEvent minBound;

};

//Inline methods (more efficient at runtime)

//Access methods
inline string GazeTraceData::getPatientID() const
{
    return patientID;
}

inline string GazeTraceData::getDate() const
{
    return date;
}

inline string GazeTraceData::getLocation() const
{
    return location;
}

inline string GazeTraceData::getPatternType() const
{
    return patternType;
}

inline int GazeTraceData::getPatternLevel() const
{
    return patternLevel;
}

inline int GazeTraceData::getClassificationGroup() const
{
    return classificationGroup;
}

inline string GazeTraceData::getWorkspace() const
{
    return workspace;
}

inline string GazeTraceData::getCoordinateSpace() const
{
    return coordinateSpace;
}

inline double GazeTraceData::getTotalTime() const
{
    return totalTime;
}

//Modifier methods
inline void GazeTraceData::setPatientID(string patientID_str)
{
    patientID = patientID_str;
}

inline void GazeTraceData::setDate(string date_str)
{
    date = date_str;
}

inline void GazeTraceData::setLocation(string location_str)
{
    location = location_str;
}

inline void GazeTraceData::setPatternType(string patternType_str)
{
    patternType = patternType_str;
}

inline void GazeTraceData::setPatternLevel(int patternLevel_int)
{
    patternLevel = patternLevel_int;
}

inline void GazeTraceData::setClassificationGroup(int classificationGroup_int)
{
    classificationGroup = classificationGroup_int;
}

inline void GazeTraceData::setWorkspace(string workspace_str)
{
    workspace = workspace_str;
}

inline void GazeTraceData::setCoordinateSpace(string coordinateSpace_str)
{
    coordinateSpace = coordinateSpace_str;
}

inline void GazeTraceData::setTotalTime(double totalTime_dbl)
{
    totalTime = totalTime_dbl;
}

//Vector methods
inline GazeTraceEvent& GazeTraceData::at(size_t pos)
{
    return events.at(pos);
}

inline const GazeTraceEvent& GazeTraceData::at(size_t pos) const
{
    return events.at(pos);
}

inline GazeTraceEvent& GazeTraceData::operator[](size_t pos)
{
    return events[pos];
}

inline const GazeTraceEvent& GazeTraceData::operator[](size_t pos) const
{
    return events[pos];
}

inline void GazeTraceData::push_back(const GazeTraceEvent& value)
{
    events.push_back(value);
}

inline void GazeTraceData::pop_back()
{
    events.pop_back();
}

inline bool GazeTraceData::empty() const
{
    return events.empty();
}

inline size_t GazeTraceData::size() const
{
    return events.size();
}

inline vector<GazeTraceEvent>::iterator GazeTraceData::begin()
{
    return events.begin();
}

inline vector<GazeTraceEvent>::const_iterator GazeTraceData::begin() const
{
    return events.begin();
}

inline vector<GazeTraceEvent>::iterator GazeTraceData::end()
{
    return events.end();
}

inline vector<GazeTraceEvent>::const_iterator GazeTraceData::end() const
{
    return events.end();
}

inline void GazeTraceData::clear()
{
    events.clear();
}

//Define YAML namespace to provide a method for preventing name conflicts
namespace YAML
{
    //Template
    template<>

    //Gaze Trace Event conversion structure
    struct convert<GazeTraceEvent>
    {
        //x, y, time encoded as [x, y, time] in YAML
        static Node encode(const GazeTraceEvent& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.time);
            return node;
        }

        static bool decode(const Node& node, GazeTraceEvent& rhs)
        {
            if(!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<double>();
            rhs.y = node[1].as<double>();
            rhs.time = node[2].as<double>();
            return true;
        }
    };
}

#endif //TRACE_DATA_H
