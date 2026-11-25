//GazeTraceData.cpp

//Include the user-defined needed files
//Gaze trace data header
#include "GazeTraceData.h"

//Gaze trace data default Constructor
GazeTraceData::GazeTraceData(): totalTime(0)
{
}

//New Gaze trace data (parameterized Constructor)
GazeTraceData::GazeTraceData(const string& traceFile)
{
    //Load YAML file
    loadYamlFile(traceFile);
}


//Load YAML file
void GazeTraceData::loadYamlFile(const string& traceFile)
{
    //Create file with single native data structure
    YAML::Node doc = YAML::LoadFile(traceFile);

    //Save Gaze filename for reference
    filename = traceFile;

    //Clear gaze vector trace events (haptic x, y, and sampling time)
    events.clear();

    //Load metadata
    patientID = doc["Patient-id"].as<std::string>();
    date = doc["Date"].as<std::string>();
    location = doc["Location"].as<std::string>();

    if(doc["Pattern"])
    {
        patternType = doc["Pattern"]["Type"].as<std::string>();
        patternLevel = doc["Pattern"]["Level"].as<int>();
    }

    if(doc["Classification"])
    {
        classificationGroup = doc["Classification"]["Group"].as<int>();
    }
    else
    {
        classificationGroup = 0;
    }

    workspace = doc["Workspace"].as<std::string>();
    coordinateSpace = doc["Coordinate-space"].as<std::string>();
    totalTime = doc["Total-time"].as<double>();

    //Load events metadata
    const YAML::Node& eventsNode = doc["Events"];

    for(size_t i = 1; i < eventsNode.size(); i++)
    {
        events.push_back(eventsNode[i].as<GazeTraceEvent>());
    }

    //Find the minimun and maximum bounds
    findMinMaxBounds();
}

//Find the minimun and maximum bounds
void GazeTraceData::findMinMaxBounds()
{
    //First element in the haptic vector trace events
    maxBound = minBound = events.front();

    //Last sample time record
    maxBound.time = events.back().time;

    //Find min and max for each event spacial dimension
    for(vector<GazeTraceEvent>::const_iterator it = events.begin(); it != events.end(); it++)
    {
        //To find de maximun and minimun bounds on the Haptic X coordinate
        if(it->x > maxBound.x)
        {
            maxBound.x = it->x;
        }
        else if(it->x < minBound.x)
        {
            minBound.x = it->x;
        }

        //To find de maximun and minimun bounds on the Haptic Y coordinate
        if(it->y > maxBound.y)
        {
            maxBound.y = it->y;
        }
        else if(it->y < minBound.y)
        {
            minBound.y = it->y;
        }
    }
}

//Export YAML File with Gaze trace data
void GazeTraceData::exportYamlFile(const string& traceFile) const
{
    //Output Gaze file stream
    ofstream file(traceFile);

    //Additional test data
    file << "%YAML 1.2\n" << exportYamlString() << endl;

    //Gaze Test data
    file << "GAZE_X" << ";" << "GAZE_Y" << ";" << "SAMPLING_TIME" << endl;
    //Loop to save values to file
    for(vector<GazeTraceEvent>::const_iterator it = events.begin(); it != events.end(); it++)
    {
        //Values saved with 4 decimal precision
        file << fixed << setprecision(4) << it -> x << ";" << it -> y << ";" << it ->time << endl;
    }

    //Close file
    file.close();
}

//Export Gaze trace additional data
string GazeTraceData::exportYamlString() const
{
    //Output stream object
    YAML::Emitter out;

    out << YAML::BeginDoc
            << YAML::BeginMap
                << YAML::Key << "Patient-id" << YAML::Value << getPatientID()
                << YAML::Key << "Date" << YAML::Value << getDate()
                << YAML::Key << "Location" << YAML::Value << getLocation()

                << YAML::Key << "Pattern"
                << YAML::BeginMap
                    << YAML::Key << "Type" << YAML::Value << getPatternType()
                    << YAML::Key << "Level" << YAML::Value << getPatternLevel()
                << YAML::EndMap

                << YAML::Key << "Classification"
                << YAML::BeginMap
                    << YAML::Key << "Group" << YAML::Value << getClassificationGroup()
                << YAML::EndMap

                << YAML::Key << "Workspace" << YAML::Value << getWorkspace()
                << YAML::Key << "Coordinate-space" << YAML::Value << getCoordinateSpace()
                << YAML::Key << "Total-time" << YAML::Value << getTotalTime()

                << YAML::Key << "Events:";

    out << YAML::EndMap;

    return out.c_str();
}
