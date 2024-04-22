#include "profile.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Constructor with arguments, saving the users and reading from the file "users.cpp"
profile::profile(string name, bool noNos[12]){

    //Checks if user is already stored
    string storedName;
    bool storedNoNos[12];
    bool exists;
    ifstream input;

    input.open("users.txt");

    if(input.fail()){
        exists = false;
    }
    else{
        exists = false;
        while(input >> storedName >> " " >> storedNoNos[0] >> " " >> storedNoNos[1] >> " " >> storedNoNos[2] >> " " >> storedNoNos[3] >> " " >> storedNoNos[4] >> " " >> storedNoNos[5] >> " " >> storedNoNos[6] >> " " >> storedNoNos[7] >> " " >> storedNoNos[8] >> " " >> storedNoNos[9] >> " " >> storedNoNos[10] >> " " >> storedNoNos[11]){
            if(name == storedName && storedNoNos[0] == noNos[0] && storedNoNos[1] == noNos[1] && storedNoNos[2] == noNos[2] && storedNoNos[3] == noNos[3] && storedNoNos[4] == noNos[4] && storedNoNos[5] == noNos[5] && storedNoNos[6] == noNos[6] && storedNoNos[7] == noNos[7] && storedNoNos[8] == noNos[8] && storedNoNos[9] == noNos[9] && storedNoNos[10] == noNos[10] && storedNoNos[11] == noNos[11]){
                exists = true;
                break;
            }
        }
    }
    
    //Adds the user to the file/updates their profile
    if(exists){
        this->name = name;
        for(int i = 0; i < 12; i++){
            this->noNos[i] = noNos[i];
        }
    }
    else{
        ofstream temp2;
        temp2.open("temp2.txt", ios::app);

        if(exists){
            while(input >> storedName >> " " >> storedNoNos[0] >> " " >> storedNoNos[1] >> " " >> storedNoNos[2] >> " " >> storedNoNos[3] >> " " >> storedNoNos[4] >> " " >> storedNoNos[5] >> " " >> storedNoNos[6] >> " " >> storedNoNos[7] >> " " >> storedNoNos[8] >> " " >> storedNoNos[9] >> " " >> storedNoNos[10] >> " " >> storedNoNos[11]){
                if(name != storedName){
                        temp2 << storedName << " " << storedNoNos[1] << " " << storedNoNos[2] << " " << storedNoNos[3] << " " << storedNoNos[4] << " " << storedNoNos[5] << " " << storedNoNos[6] << " " << storedNoNos[7]<< " " << storedNoNos[8] << " " << storedNoNos[9] << " " << storedNoNos[10] << " " << storedNoNos[11] << endl;
                }
            }
        }
        temp2 << name << " " << noNos[0] << " " << noNos[1] << " " << noNos[2] << " " << noNos[3] << " " << noNos[4] << " " << noNos[5] << " " << noNos[6] << " " << noNos[7] << " " << noNos[8] << " " << noNos[9] << " " << noNos[10] << " " << noNos[11] << endl;
        temp2.close();
        input.close();
        remove("users.txt");
        rename("temp2.txt", "users.txt");

        //Creates the object
        this->name = name;
        for(int i = 0; i < 12; i++){
            this->noNos[i] = noNos[i];
        }
    }
}

//This returns a pointer to the array, probably don't need it but it's here
bool* profile::getNoNos(){
    return noNos;
}