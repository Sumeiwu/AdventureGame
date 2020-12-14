
#include "Adventure.h"


Adventure::Adventure()
{
    
}

Adventure::Adventure(string objfile, string rmfile, string cmdfile)
{
    // Setup of the Adventure game using some files.
    LoadObjects(objfile); //call function
    LoadRooms(rmfile);
    LoadSynonyms(cmdfile);
    
    for(int i=0; i<objects.size(); i++){
        // cout<<objects.at(i).getInitialLocation()<<endl;
        rooms.at(objects.at(i).getInitialLocation()-1).addObject(objects.at(i)); //add the objects to the room
    }
    
    currRoomInd = 1; //start with room 1
    LookCmd();
}

void Adventure::Play()
{
    while (currRoomInd != 0) {
        string cmd, objName;
        GetUserCmd(cmd, objName);
        if (cmd == "QUIT") {
            QuitCmd();
            cout << "Goodbye!" << endl;
            return;
        }
        else if (cmd == "HELP") {
            HelpCmd();
        }
        
        // The rest of the commands go here.
        
        else if (cmd == "LOOK"){
            LookCmd();
        }
        
        else if (cmd == "TAKE"){
            TakeCmd(objName);
            
        }
        
        else if (cmd == "DROP"){
            DropCmd(objName);
        }
        
        else if (cmd == "INVENTORY"){
            InventoryCmd();
        }
        
        else{
            MotionCmd(cmd);
        }
    }
    
    cout << "Goodbye!" << endl;
}


void Adventure::GetUserCmd(string &verb, string &obj)
{
    string cmdOrg = verb;
    
    cout << "> ";
    string line;
    getline(cin, line);
    
    // Add code here to figure out the verb and object from the line
    stringstream s;
    s.str(line);
    s >> verb >> obj;
    
    verb = GetSynonym(verb);
    obj = GetSynonym(obj);
    
    //cout << verb << " " << obj;
    
}

int Adventure::MotionCmd(string motion)
{
    // Get the motion table of the room number where the player is currently.
    // For each table entry, check to see if there is an entry that matches
    // "motion".  If the entry matches, check if the motion requires
    // a key and if the player has the has the key. A successful match
    // moves the player into the destination room.
    
    rooms.at(currRoomInd-1).setVisited(true); //call setVisited function to flag the room that is visited

    vector <AdvMotionTableEntry> temp = rooms.at(currRoomInd-1).getMotionTable();
    
    for (int i = 0; i < temp.size();i++){
        
        if (temp.at(i).getDirection() == motion && temp.at(i).getKeyName().empty()){ //if the direction=motion and keyName is empty
            //cout << temp.at(i).getDirection() << "-->" << motion << endl;
            currRoomInd = temp.at(i).getDestinationRoom(); //set the currRoomInd to the destinationRoom
    
            if (rooms.at(currRoomInd-1).hasBeenVisited()){ //if the room hasBeenVisited
                cout << rooms.at(currRoomInd-1).getName() << endl; //only cout the currRoom name
            }
            
            //otherwise call LookCmd()
            else{
                LookCmd();
            }
            
            return currRoomInd;
            //cout << currRoomInd << endl;
            //cout << rooms.at(currRoomInd-1).getDescription().at(i);
        }
        
        
        else if (temp.at(i).getDirection() == motion && !temp.at(i).getKeyName().empty()){ //if the direction=motion and keyName is NOT empty
            for (int j = 0 ; j < playersItems.size(); j++){
                //cout << temp.at(i).getKeyName() << "."<< endl;
                //cout << playersItems.at(j).getName() << "."<< endl;
                if (temp.at(i).getKeyName() == playersItems.at(j).getName().substr(0, playersItems.at(j).getName().length()-1)){ //check if the keyName in the playersItems vector
                    currRoomInd = temp.at(i).getDestinationRoom(); //if yes, set the currRoomInd to the destinationRoom

                    if (rooms.at(currRoomInd-1).hasBeenVisited()){ //if the room hasBeenVisited
                        cout << rooms.at(currRoomInd-1).getName() << endl; //only cout the currRoom name
                    }
                    
                    //otherwise call LookCmd()
                    else{
                        LookCmd();
                    }

                    return currRoomInd;
                }
            }
        }
        
        //if the direction is FORCED, the player is forced to destinationRoom; display a message
        else if (temp.at(i).getDirection() == "FORCED"){
            cout << endl;
            cout << "--NO DIRECTION to move in this room. You are FORCED to another room!" << endl;
            cout << endl;
            currRoomInd = temp.at(i).getDestinationRoom();
            
            if (rooms.at(currRoomInd-1).hasBeenVisited()){ //if the room hasBeenVisited
                cout << rooms.at(currRoomInd-1).getName() << endl; //only cout the currRoom name
            }
            
            //otherwise call LookCmd()
            else{
                LookCmd();
            }

            return currRoomInd;
        }
        
    }
    
    cout << "There is no way to move in that direction." << endl; //all others, cout a message to the player
    
    return -1;
}

void Adventure::LoadObjects(string filename)
{
    // Should load all the objects from a file.
    ifstream file;
    //filename = "SmallObjects.txt";
    file.open(filename);
    while (!file.eof()){
        AdvObject* temp = new AdvObject();
        temp->readObject(file);
        objects.push_back(*temp); //push back all the objects from the file to the vector
    }
    
}

void Adventure::LoadRooms(string filename)
{
    // Should load all rooms from a file
    ifstream file;
    //filename = "SmallRooms.txt";
    file.open(filename);
    while (!file.eof()){
        AdvRoom* temp = new AdvRoom();
        temp->readRoom(file);
        rooms.push_back(*temp); //push back all the objects from the file to the vector
    }
}

void Adventure::LoadSynonyms(string filename)
{
    // Should load all synonyms from a file.
    ifstream file;
    //filename = "SmallSynonyms.txt";
    file.open(filename);
    string s;
    getline(file, s);
    while (!file.eof() && s.length() > 1){
        Synonym temp;
        vector<string> result;
        
        istringstream iss(s);
        for (string s; iss >> s;) {
            result.push_back(s);
        }
        
        temp.word = result.at(0); //word goes first, for example -> Q
        temp.synonym = result.at(1); //synonym follows, for example -> QUIT
        //cout << temp.synonym << "   " << temp.word << endl;
        
        synonyms.push_back(temp); //push back all the synonyms from the file to the vector
        getline(file, s);
    }
    
}

//Returns a synonym if one exists.  Otherwise returns original word.
string Adventure::GetSynonym(string word)
{
    
    ConvertToUpper(word);
    
    //cout << synonyms.size() << endl;
    for (int i = 0; i < synonyms.size(); i++){
        //cout << synonyms.at(i).synonym << " "<< synonyms.at(i).word << endl;
        if (synonyms.at(i).word == word) //if Q=Q
            word = synonyms.at(i).synonym; //word = QUIT
    }
    return word;
}


void Adventure::QuitCmd()
{
    // Ask if the uses would like to quit.  Should only take a yes or no.
    while (true) {
        string ans;
        cout << "Are you sure you want to quit now? ";
        getline(cin, ans);
        ConvertToUpper(ans);
        if (ans == "Y" || ans == "YES") {
            currRoomInd = 0;
            break;
        }
        if (ans == "N" || ans == "NO") {
            break;
        }
        cout << "Please answer yes or no." << endl;
    }
}

void Adventure::HelpCmd()
{
    cout << "Welcome to Adventure!" << endl;
    cout << "Somewhere nearby is Colossal Cave, where others have found fortunes in" << endl;
    cout << "treasure and gold, though it is rumored that some who enter are never" << endl;
    cout << "seen again. Magic is said to work in the cave.I will be your eyes" << endl;
    cout << "and hands. Direct me with natural English commands; I don't understand" << endl;
    cout << "all of the English language, but I do a pretty good job." << endl << endl;
    
    cout << "It's important to remember that cave passages turn a lot, and that" << endl;
    cout << "leaving a room to the north does not guarantee entering the next from" << endl;
    cout << "the south, although it often works out that way. You'd best make yourself" << endl;
    cout << "a map as you go along." << endl << endl;
    
    cout << "Much of my vocabulary describes places and is used to move you there." << endl;
    cout << "To move, try words like IN, OUT, EAST, WEST, NORTH, SOUTH, UP, or DOWN." << endl;
    cout << "I also know about a number of objects hidden within the cave which you" << endl;
    cout << "can TAKE or DROP.To see what objects you're carrying, say INVENTORY." << endl;
    cout << "To reprint the detailed description of where you are, say LOOK.If you" << endl;
    cout << "want to end your adventure, say QUIT." << endl;
}


void Adventure::LookCmd()
{
    for (int i = 0; i < rooms.at(currRoomInd-1).getDescription().size()-1; i++){ //size-1, so it won't print -----
        cout << rooms.at(currRoomInd-1).getDescription().at(i);
    }
    
    int index = rooms.at(currRoomInd-1).objectCount(); //get how many objects in the current room
    //cout << index << endl;
    for (int j = 0; j < index; j++){
        cout << "There is " << rooms.at(currRoomInd-1).getObject(j).getDescription(); //get the object description
    }
    
}

void Adventure::TakeCmd(string obj)
{
    
    // cout<< rooms.at(currRoomInd-1).containsObject(obj)<<endl;
    if (rooms.at(currRoomInd-1).containsObject(obj)) { //make sure the obj is in the currRoom
        AdvObject takeObj = rooms.at(currRoomInd-1).removeObject(obj); //if yes, remove from the currRoom
        cout << "Took " << takeObj.getDescription()<<endl;
        playersItems.push_back(takeObj); //push back to the playersItems
    }
    
    
    else
        cout << obj << " is not in the room" << endl;
    
}



void Adventure::DropCmd(string obj)
{
    AdvObject dropObj;
    
    for (int i = 0; i < playersItems.size(); i++){
        //cout << playersItems.at(i).getName() << "+++"  << obj << "." << endl;
        
        //if the obj you want to drop is in the playersItems
        if (playersItems.at(i).getName().substr(0, playersItems.at(i).getName().length()-1) == obj){
            dropObj = playersItems.at(i);
            rooms.at(currRoomInd-1).addObject(dropObj); //add back to the currRoom
            cout << "Dropped " << dropObj.getDescription() << endl;
            playersItems.erase(playersItems.begin() + i); //erase/drop from the playersItems
        }
        
    }
}

void Adventure::InventoryCmd()
{
    
    //if no inventory, cout a message
    if (playersItems.size() == 0)
        cout << "You are empty-handed." << endl;
    
    //Display the inventory
    else if (playersItems.size() != 0){
        cout << "You are carrying:" << endl;
        for (int i = 0; i < playersItems.size(); i++){
            cout << "       " << playersItems.at(i).getName() << endl;
        }
    }
}

