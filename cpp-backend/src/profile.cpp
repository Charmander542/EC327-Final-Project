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
    
    ifstream input;

    input.open("users.txt");

    /* This Code checks if the user already exists, I dont think we need it but it's here just in case

    bool exists;
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
    } */
    
    //Adds the user to the file/updates their profile
    ofstream temp2;
    temp2.open("temp2.txt", ios::app);

    
    while(input >> storedName  >> storedNoNos[0] >> storedNoNos[1] >> storedNoNos[2] >> storedNoNos[3] >> storedNoNos[4] >> storedNoNos[5] >> storedNoNos[6] >> storedNoNos[7] >> storedNoNos[8] >> storedNoNos[9] >> storedNoNos[10] >> storedNoNos[11]){
        if(name != storedName){
                temp2 << storedName << " " <<storedNoNos[0] << " " << storedNoNos[1] << " " << storedNoNos[2] << " " << storedNoNos[3] << " " << storedNoNos[4] << " " << storedNoNos[5] << " " << storedNoNos[6] << " " << storedNoNos[7]<< " " << storedNoNos[8] << " " << storedNoNos[9] << " " << storedNoNos[10] << " " << storedNoNos[11] << endl;
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

/*This is our Destructor - Don't think we need it, but just in case we do I have it
profile::~profile(){
    delete[] noNos;

}*/


//This returns a pointer to the array, probably don't need it but it's here
bool* profile::getNoNos(){
    return noNos;
}


//This returns the object of a new profile. It should be cachilling
void addProfile(string name, bool noNos[12]){
    profile newProfile(name,noNos);
    return;
}

//This loads the profile with the designated name
profile loadProfile(string name){
    string storedName;
    bool storedNoNos[12];
    ifstream input;

    input.open("users.txt");
    
    while(input >> storedName  >> storedNoNos[0] >> storedNoNos[1] >> storedNoNos[2] >> storedNoNos[3] >> storedNoNos[4] >> storedNoNos[5] >> storedNoNos[6] >> storedNoNos[7] >> storedNoNos[8] >> storedNoNos[9] >> storedNoNos[10] >> storedNoNos[11]){
        if(name == storedName){
                break;
        }
    }
    
    input.close();

    profile newProfile(storedName,storedNoNos);
    return newProfile;

}

//checks if loadProfile with the name provided is accurate or not
bool checkLoadProfile(string name){
    string storedName;
    bool storedNoNos[12];
    ifstream input;
    bool existed = false;

    input.open("users.txt");
    
    while(input >> storedName  >> storedNoNos[0] >> storedNoNos[1] >> storedNoNos[2] >> storedNoNos[3] >> storedNoNos[4] >> storedNoNos[5] >> storedNoNos[6] >> storedNoNos[7] >> storedNoNos[8] >> storedNoNos[9] >> storedNoNos[10] >> storedNoNos[11]){
        if(name == storedName){
            existed = true;
            break;
        }
    }
    
    input.close();

    return existed;
}

//loads the last profile uploaded 
profile loadLastProfile(){
    string storedName;
    bool storedNoNos[12];
    ifstream input;
    bool existed = false;

    input.open("users.txt");
    
    while(input >> storedName  >> storedNoNos[0] >> storedNoNos[1] >> storedNoNos[2] >> storedNoNos[3] >> storedNoNos[4] >> storedNoNos[5] >> storedNoNos[6] >> storedNoNos[7] >> storedNoNos[8] >> storedNoNos[9] >> storedNoNos[10] >> storedNoNos[11]){
        
    }
    
    input.close();

    profile newProfile(storedName,storedNoNos);
    return newProfile;
}

Napi::String loadProfileName(const Napi::CallbackInfo& info, profile p1){
    Napi::Env env = info.Env();
    return Napi::String::New(env, p1.name);
}

Napi::Object loadProfileNoNos(const Napi::CallbackInfo& info, profile p1){
    
}

num | (1 << i)

