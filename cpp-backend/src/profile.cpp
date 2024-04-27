#include "profile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

//Constructor with arguments, saving the users and reading from the file "users.cpp"
profile::profile(string name, int noNo){

    //turns the integer into the boolean equivalent
    bool noNos[12];

    for(int i = 0; i<12;i++){
        noNos[11-i] = noNo%2;
        noNo = noNo / 2;
    }

    //Checks if user is already stored
    string storedName;
    bool storedNoNos[12];
    
    ifstream input;

    input.open("users.txt");
    
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

//This returns a pointer to the array, probably don't need it but it's here
bool* profile::getNoNos(){
    return noNos;
}



//this function turns the boolean array into a integer
int array2int(bool array[12]){
    int sum = 0;
    for(int i = 0; i<12; i++){
        sum = sum + (short)array[i] * pow(i,2);
    }
    return sum;
}

//this function turns the array into a boolean integer
bool* integer2array(int num){
    bool array[12];

    for(int i = 0; i<12;i++){
        array[i] = num%2;
        num = num / 2;
    }
    return array;
}

//addProfile 

    //This returns the object of a new profile.
    void addProfile(string name, int noNo){
        
        profile newProfile(name,noNo);
        return;
    }

    Napi::Boolean addProfileWrapped(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        //check if arguments are integer only.
        if(info.Length()<2 || !info[0].IsString() || !info[1].IsNumber()){
            Napi::TypeError::New(env, "arg1::String, arg2::Number expected").ThrowAsJavaScriptException();
        }
        //convert javascripts datatype to c++
        Napi::String first = info[0].As<Napi::String>();
        Napi::Number second = info[1].As<Napi::Number>();

        addProfile(first.ToString(),second.Int32Value());
        //run c++ function return value and return it in javascript
        Napi::Boolean returnValue = Napi::Boolean::New(env, true);
    }

//loadProfile

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

        int noNo = array2int(storedNoNos);

        profile newProfile(storedName,noNo);
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

    //Wrapper for loadProfile
    Napi::Number loadProfileWrapped(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        //check if argument is a string
        if(info.Length()<1 || !info[0].IsString()){
            Napi::TypeError::New(env, "arg1::String expected").ThrowAsJavaScriptException();
        }
        
        //turns into a javascript string
        Napi::String first = info[0].As<Napi::String>();

        //Error Checks ti make sure the profile exists
        if(checkLoadProfile(first.ToString())){
            Napi::TypeError::New(env, "Profile does not exist").ThrowAsJavaScriptException();
        }
        
        //run c++ function
        profile temp = loadProfile(first.ToString());
        
        //generates an integer that stored the list of booleans
        int num;
        for(int i = 0; i < 12; i++){
            num | ((short)temp.noNos[i] << i); 
        }

        //Returns value to Javascript
        Napi::Number returnValue = Napi::Number::New(env, num);
        return returnValue;
    }

//loadLastProfile

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

        int noNo = array2int(storedNoNos);

        profile newProfile(storedName,noNo);
        return newProfile;
    }

    //Wrapper for loadLastProfile
    Napi::Object loadLastProfileWrapped(){
        
        profile temp = loadLastProfile();

        //run c++ function return value and return it in javascript
        Napi::Object returnValue;
        returnValue.Set(uint32_t(0),temp.name);
        
        int num;
        for(int i = 0; i < 12; i++){
            num | ((short)temp.noNos[i] << i); //todo CHARLIE CHECK THIS
        }
        returnValue.Set(uint32_t(1),num);
    
        return returnValue;
    }

//NAPI requirements

Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
  //export Napi function
  exports.Set("loadLastProfile", Napi::Function::New(env, loadLastProfileWrapped));
  exports.Set("loadProfile", Napi::Function::New(env, loadProfileWrapped));
  exports.Set("addProfile", Napi::Function::New(env, addProfileWrapped));
  return exports;
}



/*Napi::String loadProfileName(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::String::New(env, p1.name);
}

Napi::Number loadProfileNoNos(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    int num;
    for(int i = 0; i < 12; i++){
        num | ((short)p1.noNos[i] << i); //todo CHARLIE CHECK THIS
    }

    return Napi::Number::New(env, (double)num);
}
*/

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

/*This is our Destructor - Don't think we need it, but just in case we do I have it
profile::~profile(){
    delete[] noNos;

}*/