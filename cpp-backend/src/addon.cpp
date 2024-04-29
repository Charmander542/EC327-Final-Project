#include "addon.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <sstream>
#include <vector>
#include <unordered_set>

using namespace std;

vector<string> ingredients;

ingredient::ingredient(string ingredient_name, string quantity, int importance, int day, int month, int year) {
    tm date = {};  // Initialize to zero
    date.tm_mday = day;
    date.tm_mon = month - 1;  // tm_mon is zero-based, January is 0
    date.tm_year = year - 1900;  // tm_year is years since 1900
    // Normalize and convert to time_t
    time_t date_time = mktime(&date);
    if (date_time == -1) {
        cout << "Wrong date entry" << endl;
        return;
    }

    ofstream output("ingredients.txt", ios::app);
    output << ingredient_name << " " << quantity << " " << importance << " " << date_time << endl;
    output.close();
    ingredients.push_back(ingredient_name);
}

void populateIngredients() {
    string ingredient_name;
    int importance;
    string quantity;
    time_t date_time;
    ifstream input("ingredients.txt");
    while (input >> ingredient_name >> quantity >> importance >> date_time) {
        ingredients.push_back(ingredient_name);
    }
    input.close();
}

void ingredient::clear() {
    ofstream output("ingredients.txt");
    output << "";
    output.close();
    ofstream output2("recipes.txt");
    output << "";
    output.close();
}

int ingredient::remove_ingredient(const string name) {
    ifstream input("ingredients.txt");
    ofstream tempout("temp.txt");

    if (input.fail()) {
        cout << "Error! The file doesn't exist!" << endl;
        return 1;
    }

    string ingr, quant;
    int imp;
    time_t date_time;

    while (input >> ingr >> quant >> imp >> date_time) {
        if (ingr != name) {
            tempout << ingr << " " << quant << " " << imp << " " << date_time << '\n';
        }
    }

    input.close();
    tempout.close();
    remove("ingredients.txt");
    rename("temp.txt", "ingredients.txt");

    return 0;
}

void ingredient::expired() {
    time_t today = time(0);
    ifstream input("ingredients.txt");
    ofstream tempout("temp.txt");

    string ingr, quant;
    int imp;
    time_t date_time;

    while (input >> ingr >> quant >> imp >> date_time) {
        if (date_time >= today) {
            tempout << ingr << " " << quant << " " << imp << " " << date_time << '\n';
        }
    }

    input.close();
    tempout.close();
    remove("ingredients.txt");
    rename("temp.txt", "ingredients.txt");
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v'\"");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v'\"");
    return str.substr(first, (last - first + 1));
}

bool containsAllIngredients(const unordered_set<string>& recipeIngredients, const vector<string>& searchIngredients) {
    for (const auto& ing : searchIngredients) {
        if (recipeIngredients.find(ing) == recipeIngredients.end()) {
            return false;
        }
    }
    return true;
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    bool inside_quotes = false;
    ostringstream currentToken;

    for (char c : s) {
        if (c == '\"') {
            inside_quotes = !inside_quotes;
        } else if (c == delimiter && !inside_quotes) {
            tokens.push_back(currentToken.str());
            currentToken.str("");
            currentToken.clear();
        } else {
            currentToken << c;
        }
    }
    tokens.push_back(currentToken.str()); // add the last token
    return tokens;
}

void findRecipeById(const string& recipeId) {
    ifstream file("RAW_recipes.csv");
    string line;
    bool headerSkipped = false;

    while (getline(file, line)) {
        if (!headerSkipped) {  // Skip the header line
            headerSkipped = true;
            continue;
        }
        vector<string> columns = split(line, ',');

        if (columns.size() < 9) {
            continue;
        }

        if (columns[1].erase(0, columns[1].find_first_not_of('\"')) == recipeId) {
            cout << "Recipe Steps for ID " << recipeId << ":\n" << columns[8] << endl;
            file.close();
            return;
        }
    }
}

void recipes(const vector<string>& IDs) {
    for (const auto& id : IDs) {
        findRecipeById(id);
    }
}

void ingredient::recipefunc() {
    const int MAX_RECIPES_GENERATED=50;
    int num_recipes_generated=0;
    profile p = loadLastProfile();
    p.noNos;
    ifstream file("Ingredients-Only.csv");

    string line;
    vector<string> searchIngredientsSet(ingredients.begin(), ingredients.end());

    // Skip the header
    getline(file, line);

    vector<string> IDs;
    while (getline(file, line)) {
        istringstream lineStream(line);
        string cell;
        unordered_set<string> recipeIngredients;

        //print out what is in recipeIngredients
        for (const auto& elem : recipeIngredients)
            cout << elem << ' ';
        cout << '\n';

        // Read until the ingredients field (assuming index 1)
        getline(lineStream, cell, ','); // Skip the ID
        getline(lineStream, cell); // Read the full ingredient list

        // Remove the enclosing brackets for the ingredient list for each recipe
        cell = cell.substr(1, cell.size() - 2);
        istringstream cellStream(cell);
        string ingredient1;
        cout<<ingredient1<<endl;

        while (getline(cellStream, ingredient1, ',')) {
            ingredient1 = trim(ingredient1); // Trim spaces and single quotes
            recipeIngredients.insert(ingredient1);
        }



        if (containsAllIngredients(recipeIngredients, searchIngredientsSet)) {
            istringstream idStream(line);
            string id;
            getline(idStream, id, ','); // Assuming the first column is the ID
            IDs.push_back(id);
        }
    }
    file.close();

    if (!IDs.empty()) {
        unordered_set<string> idSet(IDs.begin(), IDs.end());
        ofstream output("recipes.txt", ios::app);
        ifstream file("RAW_recipes.csv");
        if (!file.is_open()) {
            cout << "Failed to open file." << endl;
            return;
        }

        bool headerSkipped = false;

     
        
        //old while loop that only saves recipe name, time to cook ,total steps to cook.
         
        while (getline(file, line)) {
            if(num_recipes_generated==MAX_RECIPES_GENERATED){
                break;
            }
            if (!headerSkipped) {  // Skip the header line
                headerSkipped = true;
                continue;
            }

            vector<string> columns = split(line, ',');
            if (columns.size() < 9) continue;  // Ensure there are enough columns

            string cleanId = columns[1].erase(0, columns[1].find_first_not_of('\"'));

            //idSet contains the recipes that were found as matches
            
            //columns of RAW_recipes.csv:
            //name	id	minutes	contributor_id	submitted	tags	nutrition	n_steps	steps	description	ingredients	n_ingredients

            if (idSet.find(cleanId) != idSet.end()) {
                string recipe_name = columns[0];
                string time2cook = columns[2];
                string steps = columns[8];
                output << recipe_name << "~" << time2cook << "~" << steps << endl;
            }
        }
        
        file.close();
        output.close();
    } else {
        cout << "Recipe not found." << endl;
    }
}

int populateRecipes() {
    populateIngredients();
    ingredient::recipefunc();
    return 1;
}
Napi::Number populateRecipesWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Number returnValue = Napi::Number::New(env, populateRecipes());
    return returnValue;
}


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

profile::profile(){}

//This returns a pointer to the array, probably don't need it but it's here
bool* profile::getNoNos(){
    return noNos;
}



//this function turns the boolean array into a integer
int array2int(bool array[12]){
    int sum = 0;
    for(int i = 0; i<12; i++){
        sum = sum + (short)array[i] * pow(2,11-i);
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



int masterProfile(string name, int noNoNumber){
    cout << "I am Alive" << endl;
    
    if(checkLoadProfile(name)){
        loadProfile(name);
    }
    else{
        addProfile(name, noNoNumber);
    }

    return 1;
}

//MasterWrapper

Napi::Number masterWrapper(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
 //check if arguments are integer only.
 if(info.Length()<2 || !info[0].IsString() || !info[1].IsNumber()){
    Napi::TypeError::New(env, "arg1::String, arg2::Number expected").ThrowAsJavaScriptException();
 }
 //convert javascripts datatype to c++
 Napi::String first = info[0].As<Napi::String>();
 Napi::Number second = info[1].As<Napi::Number>();
//run c++ function return value and return it in javascript
 Napi::Number returnValue = Napi::Number::New(env, masterProfile(first, second.Int32Value()));
 
 return returnValue;
}



//NAPI requirements

Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
  //export Napi function
  exports.Set("masterProfile", Napi::Function::New(env, masterWrapper));
  exports.Set("populateRecipes", Napi::Function::New(env, populateRecipesWrapped));
  return exports;
    
}