#include "AdvRoom.h"



AdvRoom::AdvRoom()
{
    
}

bool AdvRoom::readRoom(ifstream &roomFile)
{
    // Reads room data from an open file.
    // Returns true if successful.
    if (roomFile.fail()){
        //cout << "Error opening room file." << endl;
        return false;
    }
    
    string roomNum;
    getline(roomFile, roomNum);
    stringstream ss(roomNum);
    ss >> rmNum;
    
    getline(roomFile, name); //read room name
    string temp;
    
    while (temp.substr(0,5).compare("-----") != 0){
        getline(roomFile, temp); //using while loop to get all description in the room
        description.push_back(temp); //push back to the description vector
    }
    
    
    
    string line;
    string direction;
    int destinationRoom;
    
    getline(roomFile, line);
    
    //store the whole line in the result vector if not empty line
    while (line.length() > 1){
        vector <string> result;
        stringstream s(line);
        
        for (string line; s >> line;){
            result.push_back(line); // push back to the result vector
        }
        
        direction = result.at(0); //at 0 = direction
        stringstream geek(result.at(1)); //at 1 = destinationRoom;
        geek >> destinationRoom; //into integer
        
        if (result.size() > 2){
            AdvMotionTableEntry temp2(direction, destinationRoom, result.at(2)); //direction, destinationRoom, keyName
            motion.push_back(temp2);
        }
        
        else {
            AdvMotionTableEntry temp2(direction, destinationRoom); // if there is no keyName, only store direction and destinationRoom;
            motion.push_back(temp2);
        }
        
        if (roomFile.eof()) {
            return false;
        }
        
        getline(roomFile, line); //get next line
        
    }
    
    return true;
    
}

// Return the room description.
vector<string> AdvRoom::getDescription()
{
    return description;
}

string AdvRoom::getName()
{
    return name; // Returns the room name
}

void AdvRoom::addObject(AdvObject obj)
{
    // This function should add the obj to the room.
    // It however, should not add the object to the room
    // if the room already contains the object.
    objects.push_back(obj);
    
}

AdvObject AdvRoom::removeObject(string objName)
{
    // This function should remove the object with objName.
    AdvObject removeObject = AdvObject();
    for (int i = 0; i < objects.size(); i++){
        if (objects.at(i).getName().substr(0, objects.at(i).getName().length()-1) == objName ){ //if the objName is in the vector
            removeObject = objects.at(i);
            objects.erase(objects.begin() + i); //remove objName
            break;
        }
    }
    return removeObject;
}


bool AdvRoom::containsObject(string objName)
{
    // Returns true if object with objName is in the room.
    for (int i = 0; i < objects.size(); i++){
        if (objects.at(i).getName().substr(0, objects.at(i).getName().length()-1) == objName){
            return true;
        }
    }
    return false;
}

int AdvRoom::objectCount()
{
    // Returns how many objects are in the room.
    return objects.size();
}

AdvObject AdvRoom::getObject(int index)
{
    return objects[index];
}

bool AdvRoom::hasBeenVisited()
{
    // Returns true if the room has been visited. False otherwise.
    return visited;
}

void AdvRoom::setVisited(bool flag)
{
    // Sets if the room has been visited.
    visited = flag;
}

vector<AdvMotionTableEntry> AdvRoom::getMotionTable()
{
    return motion; // Returns a motion table for the room.
}


int AdvRoom::getRoomNumber()
{
    return rmNum; // Returns the room number.
}
