#ifndef PROFILE
#define PROFILE

#include <napi.h>
#include <string>
using namespace std;

class profile {
    public:
        string name;
        //noNos are our Food Restrictions and Allergies, it is true if the user has them and false if they don't
        //Order of noNos: [Vegetarian, Vegan, Alcohol-Free, Gluten, Dairy, Eggs, Soy, Wheat, Fish, Shellfish, Tree Nuts, Peanuts]
        bool noNos[12];
        
        profile(string name, bool noNos[12]);
        // ~profile(); don't think we need it, but we have it just in case

        bool* getNoNos();


};


//loads last profile into an object 
profile loadProfile(string name);
//todo add wrapper function and only return nonos
//checks to see if loadProfile was successfull
bool checkLoadProfile(string name);
//todo call this one in loadProfile and throw an exception if ass
//adds a profile to the list of existing profiles
void addProfile(string name, bool noNos[12]);
//todo add wrapper but it don't return shiiii
//loads the last profile
profile loadLastProfile();
Napi::Object loadLastProfileWrapped();

//Export shiii
Napi::Object Init(Napi::Env env, Napi::Object exports);
 NODE_API_MODULE(addon, Init)

//string loadProfileName(profile p1);
//Napi::String loadProfileNameWrapped(const Napi::CallbackInfo& info);





/*
//returns the name as a javascript string
Napi::String loadProfileName(const Napi::CallbackInfo& info);
//returns the bool array as a javascript array
Napi::Number loadProfileNoNos(const Napi::CallbackInfo& info);

*/

//Napi::Boolean checkLoadProfileWrapped(const Napi::CallbackInfo& info);


#endif