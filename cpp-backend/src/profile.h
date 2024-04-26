#ifndef PROFILE
#define PROFILE

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
//checks to see if loadProfile was successfull
bool checkLoadProfile(string name);
//adds a profile to the list of existing profiles
profile addProfile(string name, bool noNos[12]);
//loads the last profile
profile loadLastProfile();


#endif
