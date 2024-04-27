#include "ingredient.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include "recipe.h"
using namespace std;
vector <string> ingredients;
ingredient::ingredient(string ingredient_name, string quantity, int importance,int day, int month, int year){ 
    string resultant = ""; 
    tm date = {};  // Initialize to zero
    date.tm_mday = day;
    date.tm_mon = month - 1;  // tm_mon is zero-based, January is 0
    date.tm_year = year - 1900;  // tm_year is years since 1900
    // Normalize and convert to time_t
    time_t date_time = mktime(&date);
    if (date_time == -1) {
     cout << "Wrong date entry" << endl;
    }   

    ofstream output;
    output.open("ingredients.txt",ios::app);
    output << ingredient_name << " " << quantity << " " << importance << " " << date_time << endl;
    ingredients.push_back(ingredient_name);
    output.close();
}
void ingredient::clear(){
    ofstream output;
    output.open("ingredients.txt");
    output << "";
    output.close();
    ofstream output2;
    output.open("recipes.txt");
    output << "";
    output.close();
}
int ingredient::remove_ingredient(const string name) {
    ifstream input;
    ofstream tempout;
    time_t date_time;
    string ingr, quant;
    int imp;

    input.open("ingredients.txt");
    tempout.open("temp.txt");

    if (input.fail()) {
        std::cout << "Error! The file doesn't exist!" << endl;
        return 1;
    }

    while (input >> ingr >> quant >> imp >> date_time) {
        cout << "Checking: " << ingr << " against " << name << endl;
        if (ingr != name) {
            tempout << ingr << " " << quant << " " << imp << " " << date_time << '\n';
            cout << "Writing to temp: " << ingr << " " << quant << " " << imp << " " << date_time << endl;
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
    ifstream input;
    ofstream tempout;
    time_t date_time;
    string ingr, quant;
    int imp;

    input.open("ingredients.txt");
    tempout.open("temp.txt");

    while (input >> ingr >> quant >> imp >> date_time) {
        if (date_time >= today) {  // Updated condition to filter expired ingredients
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
};

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

void findRecipeById( const string& recipeId) {
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
            std::cout << "Recipe Steps for ID " << recipeId << ":\n" << columns[8] << endl;
            file.close();
            return;
        }
    }
}
void recipes(vector<string> IDs){ 
    for (int i=0; i < IDs.size() - 1; i++) { 
        findRecipeById(IDs[i]); 
    }
}



 void ingredient::recipefunc(){ 
    ifstream file("Ingredients-Only.csv");
    if (!file.is_open()) {
        std::cout << "Failed to open file." << std::endl;
    }

    string line;
    std::vector<string> searchIngredientsSet(ingredients.begin(), ingredients.end());

    // Skip the header
    getline(file, line);

    std::vector<string> IDs;
    while (getline(file, line)) {
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

        if (containsAllIngredients(recipeIngredients, searchIngredientsSet)) {
            std::istringstream idStream(line);
            string id;
            getline(idStream, id, ','); // Assuming the first column is the ID
            IDs.push_back(id); 
        }
    }
 file.close();

    if (!IDs.empty()) {

    // Convert vector to a set for efficient ID checking
    unordered_set<string> idSet(IDs.begin(), IDs.end());

    // Open output file once
    ofstream output("recipes.txt", ios::app);

    // Open the CSV file once
    ifstream file("RAW_recipes.csv");
    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return;
    }

    string line;
    bool headerSkipped = false;

    // Read each line of the CSV file
    while (getline(file, line)) {
        if (!headerSkipped) {  // Skip the header line
            headerSkipped = true;
            continue;
        }

        vector<string> columns = split(line, ',');
        if (columns.size() < 9) continue;  // Ensure there are enough columns

        // Clean the ID from quotes if present
        string cleanId = columns[1].erase(0, columns[1].find_first_not_of('\"'));

        // Check if the current row's ID is in the set of IDs we're interested in
        if (idSet.find(cleanId) != idSet.end()) {
            string recipe_name = columns[0];
            string time2cook = columns[2];
            string steps = columns[8];
            output << "Recipe name: " << recipe_name << ", Time to cook: " << time2cook << ", Total steps to cook: " << steps << endl;
        }
    }

    file.close();
    output.close();
}
    else {
        std::cout << "Recipe not found." << std::endl;
    }

    file.close();
}

