#ifndef ADDON
#define ADDON

#include <napi.h>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class ingredient {
public:
    string ingredient_name;
    int importance;
    string quantity;
    int day;
    int month;
    int year;
    ingredient(string ingredient_name, string quantity, int importance, int day, int month, int year);
    static void expired();
    static void clear();
    static void recipefunc();
    static int remove_ingredient(const string name);
};

class profile {
    public:
        string name;
        //noNos are our Food Restrictions and Allergies, it is true if the user has them and false if they don't
        //Order of noNos: [Vegetarian, Vegan, Alcohol-Free, Gluten, Dairy, Eggs, Soy, Wheat, Fish, Shellfish, Tree Nuts, Peanuts]
        bool noNos[12];
        
        profile(string name, int noNo);
        profile();
        // ~profile(); don't think we need it, but we have it just in case

        bool* getNoNos();


};


int populateRecipies();
void populateIngredients();
Napi::Number populateRecipiesWrapped(const Napi::CallbackInfo& info);
void populateNoNos(profile); //Justin added this line, remove it if needed

int array2int(bool array[12]);
bool* integer2array(int num);

//loads last profile into an object 
profile loadProfile(string name);
//checks to see if loadProfile was successfull
bool checkLoadProfile(string name);
//adds a profile to the list of existing profiles
void addProfile(string name, int noNo);
//loads the last profile
profile loadLastProfile();

//Export shiii


int masterProfile(string name, int noNoNumber);
Napi::Number masterWrapper(const Napi::CallbackInfo& info);

Napi::Object Init(Napi::Env env, Napi::Object exports) ;
NODE_API_MODULE(addon, Init)

#endif