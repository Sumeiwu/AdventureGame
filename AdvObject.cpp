#include "AdvObject.h"

AdvObject::AdvObject()
{
    
}

AdvObject::AdvObject(string objName, string objDes, int loc)
{
    name = objName;
    description = objDes;
    initialrm = loc;
}

bool AdvObject::readObject(ifstream &objFile)
{
    // True if able to read name,
    // description, and initial location.
    // Otherwise returns false.
    if (objFile.fail()){
        //cout << "Error opening object file." << endl;
        return false;
    }
    getline(objFile, name); // read the object name
    
    getline(objFile, description); // read description
    
    string roomNum;
    getline(objFile, roomNum); //get roomNum string
    stringstream s(roomNum);
    s >> initialrm; //convert the roomNum into int initialrm;
    
    string empty = "";
    getline(objFile, empty); //empty line
    
    return true;
}

string AdvObject::getName()
{
    return name;
}

string AdvObject::getDescription()
{
    return description;
}

int AdvObject::getInitialLocation()
{
    return initialrm;
}


