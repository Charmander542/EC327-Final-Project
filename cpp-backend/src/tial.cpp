#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>

// Helper function to trim whitespace and quotes from the beginning and end of a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v'\"");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v'\"");
    return str.substr(first, (last - first + 1));
}

bool containsAllIngredients(const std::unordered_set<std::string>& recipeIngredients, const std::vector<std::string>& searchIngredients) {
    for (const auto& ing : searchIngredients) {
        if (recipeIngredients.find(ing) == recipeIngredients.end()) {
            return false;
        }
    }
    return true;
}
// 9024,"['cream cheese', 'salmon', 'salt', 'lemon juice', 'onions', 'liquid smoke']"
int main() {
    std::ifstream file("Ingredients-Only.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> searchIngredients = {"cream cheese", "salt", "lemon juice","onions","liquid smoke"};
    std::unordered_set<std::string> searchIngredientsSet(searchIngredients.begin(), searchIngredients.end());

    // Skip the header
    std::getline(file, line);

    while (getline(file, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        std::unordered_set<std::string> recipeIngredients;

        // Read until the ingredients field (assuming index 1)
        getline(lineStream, cell, ','); // Skip the ID
        getline(lineStream, cell); // Read the full ingredient list

        // Remove the enclosing brackets
        cell = cell.substr(1, cell.size() - 2);
        std::istringstream cellStream(cell);
        std::string ingredient;

        while (getline(cellStream, ingredient, ',')) {
            ingredient = trim(ingredient); // Trim spaces and single quotes
            recipeIngredients.insert(ingredient);
        }

        if (containsAllIngredients(recipeIngredients, searchIngredients)) {
            std::istringstream idStream(line);
            std::string id;
            getline(idStream, id, ','); // Assuming the first column is the ID
            std::cout << "Matching Recipe ID: " << id << std::endl;
        }
    }

    file.close();
    return 0;
}