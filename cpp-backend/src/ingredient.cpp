#include "ingredient.h"
#include <ctime>
#include <iostream>
#include <fstream>

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
        cout << "Error! The file doesn't exist!" << endl;
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