#ifndef RECIPE
#define RECIPE
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;


class Recipe{ 
    public: 
        string recipe_name; 
        string time2cook;
        string steps;

        Recipe();
        Recipe(const string& recipeId){ 
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
                    steps = columns[8];
                    time2cook = columns[2];
                    recipe_name = columns[0];
                    file.close();
                    
                }
            }

            file.close();
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
};

#endif
