#ifndef RECIPE
#define RECIPE
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;
// Sample row from RAW_recipes.csv
/*
name,id,minutes,contributor_id,submitted,tags,nutrition,n_steps,steps,description,ingredients,n_ingredients
arriba   baked winter squash mexican style	137739	55	47892	9/16/2005	['60-minutes-or-less', 'time-to-make', 'course', 'main-ingredient', 'cuisine', 'preparation', 'occasion', 'north-american', 'side-dishes', 'vegetables', 'mexican', 'easy', 'fall', 'holiday-event', 'vegetarian', 'winter', 'dietary', 'christmas', 'seasonal', 'squash']	[51.5, 0.0, 13.0, 0.0, 2.0, 0.0, 4.0]	11	['make a choice and proceed with recipe', 'depending on size of squash , cut into half or fourths', 'remove seeds', 'for spicy squash , drizzle olive oil or melted butter over each cut squash piece', 'season with mexican seasoning mix ii', 'for sweet squash , drizzle melted honey , butter , grated piloncillo over each cut squash piece', 'season with sweet mexican spice mix', 'bake at 350 degrees , again depending on size , for 40 minutes up to an hour , until a fork can easily pierce the skin', 'be careful not to burn the squash especially if you opt to use sugar or butter', 'if you feel more comfortable , cover the squash with aluminum foil the first half hour , give or take , of baking', 'if desired , season with salt']	"autumn is my favorite time of year to cook! this recipe 
can be prepared either spicy or sweet, your choice!
two of my posted mexican-inspired seasoning mix recipes are offered as suggestions."	['winter squash', 'mexican seasoning', 'mixed spice', 'honey', 'butter', 'olive oil', 'salt']	7
*/


//recipe currently only stores NAME, TIME TO PREPARE, and STEPS of each recipe.
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
