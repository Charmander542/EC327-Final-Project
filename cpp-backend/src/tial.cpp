#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include "recipe.h"
using namespace std;

// Helper function to trim whitespace and quotes from the beginning and end of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v'\"");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v'\"");
    return str.substr(first, (last - first + 1));
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
            cout << "Recipe Steps for ID " << recipeId << ":\n" << columns[8] << endl;
            file.close();
            return;
        }
    }

    cout << "Recipe ID not found." << endl;
    file.close();
}



void recipes(vector<string> IDs){ 
    for (int i=0; i < IDs.size() - 1; i++) { 
        findRecipeById(IDs[i]); 
    }
}


bool containsAllIngredients(const unordered_set<string>& recipeIngredients, const vector<string>& searchIngredients) {
    for (const auto& ing : searchIngredients) {
        if (recipeIngredients.find(ing) == recipeIngredients.end()) {
            return false;
        }
    }
    return true;
}
// 9024,"['cream cheese', 'salmon', 'salt', 'lemon juice', 'onions', 'liquid smoke']"
int main() {
   ifstream file("Ingredients-Only.csv");
 
    string line;
    vector<string> searchIngredients = {"chicken", "salt","rice"};
    unordered_set<string> searchIngredientsSet(searchIngredients.begin(), searchIngredients.end());

    // Skip the header
    getline(file, line);

    vector<string> IDs;
    while (getline(file, line)) {
        istringstream lineStream(line);
        string cell;
        unordered_set<string> recipeIngredients;

        // Read until the ingredients field (assuming index 1)
        getline(lineStream, cell, ','); // Skip the ID
        getline(lineStream, cell); // Read the full ingredient list

        // Remove the enclosing brackets
        cell = cell.substr(1, cell.size() - 2);
        istringstream cellStream(cell);
        string ingredient;

        while (getline(cellStream, ingredient, ',')) {
            ingredient = trim(ingredient); // Trim spaces and single quotes
            recipeIngredients.insert(ingredient);
        }

        
        if (containsAllIngredients(recipeIngredients, searchIngredients)) {
            istringstream idStream(line);
            string id;
            getline(idStream, id, ','); // Assuming the first column is the ID
            IDs.push_back(id); 
        }
    }
    Recipe A1 = Recipe(IDs[0]) ;
    std::cout << A1.time2cook;
    file.close();
    return 0;
}
