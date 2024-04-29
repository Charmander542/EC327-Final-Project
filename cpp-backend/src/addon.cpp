#include "addon.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype> 


using namespace std;

vector<string> ingredients;

    //Ingredients Profile Code: Lines 18-479

//Constructor for Ingredients
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

//Loads ingredients into arrays
void populateIngredients() {
    ingredients.clear();
    ifstream input("ingredients.txt");
    string line;

    // Check if the file can be opened
    if (!input) {
        cerr << "Failed to open ingredients.txt" << endl;
        return;
    }

    // Skip the header line
    getline(input, line);

    // Process each subsequent line
    while (getline(input, line)) {
        istringstream iss(line);
        string ingredient_name;
        iss >> ingredient_name;  // Extract the first word from the line
        transform(ingredient_name.begin(), ingredient_name.end(), ingredient_name.begin(), ::tolower);
        ingredients.push_back(ingredient_name);  // Store the ingredient name
        cout << "Ingredient added: " << ingredient_name << endl;  // Optional: Output for confirmation
    }

    input.close();  // Close the file stream
}

//clears the text files
void ingredient::clear() {
    ofstream output("ingredients.txt");
    output << "";
    output.close();
    ofstream output2("recipes.txt");
    output << "";
    output.close();
}

//removes an ingredient from the list
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

//Tracks an ingredient's expiration date and sets it as expired when it expires
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

//Helps display text in a readable form
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v'\"");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v'\"");
    return str.substr(first, (last - first + 1));
}

//Checks if a recipe contains the ingredients we want it to have
bool containsAllIngredients(const unordered_set<string>& recipeIngredients, const vector<string>& searchIngredients) {
    for (const auto& ing : searchIngredients) {
    bool found = false;
    for (const auto& recipeIng : recipeIngredients) {
        if (recipeIng.find(ing) != string::npos) {
            found = true;
            break;
        }
    }
    if (!found) {
        return false;
    }
}
return true;
}

//Helps display text in a readable form
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


//////////////////////////////////////////
//helper methods to clean recipe.txt input
string cleanName(string name){
    name[0]=toupper(name[0]);
    for (int i=1;i<name.size();i++){
        if (name[i-1]==' '){
            name[i]=toupper(name[i]);
        }
    }
    return name;
}
//Helps display text in a readable form
string cleanCookTime(string cooktime){
    return cooktime+" minutes";
}
/*expected output example:
(1) Preheat oven to 475 F.
(2) Cut carrots.
*/
//Helps display text in a readable form
string cleanSteps(string steps){
    steps=steps.substr(1, steps.size() - 2); //remove brackets
    unsigned int index=1; //skip first '
    unsigned int step_number=2;
    string clean_steps=" (1) ";
    clean_steps+=toupper(steps[index]);
    index++;
    while(index < steps.size()-1){
        if(steps[index]=='\''){
            index+=4; //skip ' ,'
            clean_steps=clean_steps+ ".\n ("+to_string(step_number)+") "; 
            while(steps[index]==' '){ //ignore more spaces
                index++;
            }
            clean_steps+=toupper(steps[index]); //capitalize first character of next line
            step_number++;
        }else if (steps[index]==' ' && steps[index+1]==','){
            clean_steps+=','; //skip annoying extra spaces before commas
            index++;
        }else if(steps[index-1]=='.'){
            clean_steps+=toupper(steps[index]);
          
        } else{
            clean_steps+=steps[index];
        }

        index++;

        
    }
    return clean_steps;
}
//////////////////////////////////////
//Creates a Vector of food restrictions based on profile
vector<string> noNoIngredients;
void populateNoNos(profile p1){
    noNoIngredients.clear();
    if(p1.noNos[0]){
        noNoIngredients.push_back("beef");
        noNoIngredients.push_back("ham");
        noNoIngredients.push_back("ground beef");
        noNoIngredients.push_back("pork");
        noNoIngredients.push_back("chicken");
        noNoIngredients.push_back("turkey");
        noNoIngredients.push_back("salmon");
        noNoIngredients.push_back("tuna");
        noNoIngredients.push_back("bacon");
        noNoIngredients.push_back("lobster");
        noNoIngredients.push_back("shrimp");
        noNoIngredients.push_back("crab");
    }
    if(p1.noNos[1]){
        noNoIngredients.push_back("beef");
        noNoIngredients.push_back("bacon");
        noNoIngredients.push_back("ham");
        noNoIngredients.push_back("pork");
        noNoIngredients.push_back("chicken");
        noNoIngredients.push_back("turkey");
        noNoIngredients.push_back("salmon");
        noNoIngredients.push_back("tuna");
        noNoIngredients.push_back("yoghurt");
        noNoIngredients.push_back("cheese");
        noNoIngredients.push_back("milk");
        noNoIngredients.push_back("lobster");
        noNoIngredients.push_back("shrimp");
        noNoIngredients.push_back("crab");
    }
    if(p1.noNos[2]){
        noNoIngredients.push_back("wine");
        noNoIngredients.push_back("gin");
    }
    if(p1.noNos[3]){
        noNoIngredients.push_back("pasta");
        noNoIngredients.push_back("bread");
        noNoIngredients.push_back("tortilla");
    }
    if(p1.noNos[4]){
        noNoIngredients.push_back("cheese");
        noNoIngredients.push_back("milk");
        noNoIngredients.push_back("yoghurt");
        noNoIngredients.push_back("butter");
    }
    if(p1.noNos[5]){
        noNoIngredients.push_back("eggs");
        noNoIngredients.push_back("egg");
    }
    if(p1.noNos[6]){
        noNoIngredients.push_back("soy");
    }
    if(p1.noNos[7]){
        noNoIngredients.push_back("wheat");
    }
    if(p1.noNos[8]){
        noNoIngredients.push_back("tuna");
        noNoIngredients.push_back("salmon");
        noNoIngredients.push_back("trout"); 
        noNoIngredients.push_back("cod");
        noNoIngredients.push_back("haddock");
        noNoIngredients.push_back("swordfish");
    }
    if(p1.noNos[9]){
        noNoIngredients.push_back("lobster");
        noNoIngredients.push_back("shrimp");
        noNoIngredients.push_back("crab");
    }
    if(p1.noNos[10]){
        noNoIngredients.push_back("hazelnut");
        noNoIngredients.push_back("walnut");
        noNoIngredients.push_back("chestnut");
        noNoIngredients.push_back("almond");
        noNoIngredients.push_back("cashew");
        noNoIngredients.push_back("macademia");
        noNoIngredients.push_back("pecan");
        noNoIngredients.push_back("pistachio");
    }
    if(p1.noNos[11]){
        noNoIngredients.push_back("peanut");
    }
}

//Checks to make sure the recipe doesn't contain food restrictions
bool doesNotContainNoNoIngredients(const unordered_set<string>& recipeIngredients, const vector<string>& noNoIngredients) {
    for (const auto& ing : noNoIngredients) {
        for (const auto& recipeIng : recipeIngredients) {
        if (recipeIng.find(ing) != string::npos) {
            return false;
        }
        }
    }
    return true;
}

//Generates the recipe based on the ingredients and food restrictions entered
void ingredient::recipefunc() {
    const int MAX_RECIPES_GENERATED=50;
    int num_recipes_generated=0;
    profile p = loadLastProfile();
    populateNoNos(p);
    std::vector<string> searchNoNosSet(noNoIngredients.begin(), noNoIngredients.end());

    
    /*for (int i=0; i < 12; i++){ 
        cout << "p.noNos[" << i << "] = " << (p.noNos[i] ? "true" : "false") << endl;
    }*/

    vector<string> searchIngredientsSet(ingredients.begin(), ingredients.end());

    searchIngredientsSet.erase( //remove nonos from the pantry search
        remove_if(searchIngredientsSet.begin(), searchIngredientsSet.end(), 
            [&searchNoNosSet](const string& ingredient) {
                return find(searchNoNosSet.begin(), searchNoNosSet.end(), ingredient) != searchNoNosSet.end();
            }
        ),
        searchIngredientsSet.end()
    );

    cout << "Remaining ingredients after removal:\n";
    for (const auto& ingredient : searchIngredientsSet) {
        cout << ingredient << "\n";
    }
    cout << "All nonos:\n";
    for (const auto& ingredient : searchNoNosSet) {
        cout << ingredient << "\n";
    }

    std::vector<string> IDs;
    while(!searchIngredientsSet.empty()){
        ifstream file("Ingredients-Only.csv");
        if (!file.is_open()) {
            std::cout << "Failed to open file." << std::endl;
        }

        string line;
        // Skip the header
        getline(file, line);
        while (getline(file, line) && num_recipes_generated<MAX_RECIPES_GENERATED) {
            std::istringstream lineStream(line);
            std::string cell;
            std::unordered_set<string> recipeIngredients;

            // Read until the ingredients field (assuming index 1)
            getline(lineStream, cell, ','); // Skip the ID
            getline(lineStream, cell); // Read the full ingredient list

            // Remove the enclosing brackets
            cell = cell.substr(1, cell.size() - 2);
            std::istringstream cellStream(cell);
            string ingredient1;

            while (getline(cellStream, ingredient1, ',')) {
                ingredient1 = trim(ingredient1); // Trim spaces and single quotes
                recipeIngredients.insert(ingredient1);
            }
            
            if (containsAllIngredients(recipeIngredients, searchIngredientsSet) && doesNotContainNoNoIngredients(recipeIngredients,searchNoNosSet)) {
                std::istringstream idStream(line);
                string id;
                getline(idStream, id, ','); // Assuming the first column is the ID
                IDs.push_back(id);
                num_recipes_generated++; 
            }
            
        }
        file.close();

        cout << IDs.size() << endl;


        if (!IDs.empty()) {
            unordered_set<string> idSet(IDs.begin(), IDs.end());
            ofstream output("recipes.txt");
            ifstream file("RAW_recipes.csv");
            //cout << "writing" << endl;
            if (!file.is_open()) {
                cout << "Failed to open file." << endl;
                return;
            }

            bool headerSkipped = false;

        
            
            //old while loop that only saves recipe name, time to cook ,total steps to cook.
            num_recipes_generated = 0;
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
                    recipe_name=cleanName(recipe_name);
                    time2cook = cleanCookTime(time2cook);
                    steps=cleanSteps(steps);
                    output << recipe_name << "====" << time2cook << "====" << steps << endl;
                }
            }
            
            file.close();
            output.close();
            return;
        }else if(searchIngredientsSet.empty()){
            ofstream output("recipes.txt");
            output << "Recipe Not Found" << "====" << "N/A" << "====" << "Recipe not Found" << endl;
            output.close();
            
            cout << "Recipe not found." << endl;
        }else{
            searchIngredientsSet.pop_back();
            cout << "Remaining ingredients after removal:\n";
            for (const auto& ingredient : searchIngredientsSet) {
                cout << ingredient << "\n";
            }
        }
    }
    ofstream output("recipes.txt");
    output << "Recipe Not Found" << "====" << "N/A" << "====" << "Recipe not Found" << endl;
    output.close();
    
    cout << "Recipe not found." << endl;
}

int populateRecipes() {
    populateIngredients();
    ingredient::recipefunc();
    return 1;
}

//Wrapper Function for JavaScript 
Napi::Number populateRecipesWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Number returnValue = Napi::Number::New(env, populateRecipes());
    return returnValue;
}


    //Code for the Profile Class: lines 480-end

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