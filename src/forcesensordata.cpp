#include "ForceSensorData.h"

//Force Sensor data default Constructor
ForceSensorData::ForceSensorData(): totalTime(0)
{
}

//Parameterized Constructor
ForceSensorData::ForceSensorData(const string& traceFile)
{
    //Load YAML file
    loadYamlFile(traceFile);
}

//Load YAML file
void ForceSensorData::loadYamlFile(const string& traceFile)
{
    YAML::Node doc = YAML::LoadFile(traceFile);
    filename = traceFile;
    events.clear();

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

    const YAML::Node& eventsNode = doc["Events"];
    for(size_t i = 0; i < eventsNode.size(); i++)
    {
        events.push_back(eventsNode[i].as<ForceSensorEvent>());
    }
}

//Export YAML File with Force Sensor data
void ForceSensorData::exportYamlFile(const string& traceFile) const
{
    ofstream file(traceFile);

    file << "%YAML 1.2" << endl << "---" << endl;
    file << exportYamlString() << endl;
    file << "sensor1; sensor2; sensor3; sensor4; time" << endl;

    if (events.empty()) {
        cout << "El vector de eventos está vacío." << endl;
    }

    for(vector<ForceSensorEvent>::const_iterator it = events.begin(); it != events.end(); it++)
    {
        //Values saved with 4 decimal precision
        file << fixed << setprecision(4) << it -> sensor1 << "; " << it->sensor2 << "; "<< it->sensor3 << "; "<< it->sensor4 << "; "<< it->time << endl;
    }
    file.close();
}

//Export Sensor additional data
string ForceSensorData::exportYamlString() const
{
    YAML::Emitter out;

    out << YAML::BeginMap
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
        << YAML::Key << "Events";

    out << YAML::EndMap;

    return out.c_str();
}
