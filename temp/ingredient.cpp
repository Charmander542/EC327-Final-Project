#include "ingredient.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include "recipe.h"
#include "profile.h"

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

int populateRecipies() {
    populateIngredients();
    ingredient::recipefunc();
    return 1;
}

