#ifndef INGREDIENT
#define INGREDIENT

#include <string>
using namespace std;

class ingredient { 
    public: 
        string ingredient_name; 
        int importance;
        string quantity;
        int day;
        int month; 
        int year;
        ingredient(string ingredient_name, string quantity, int importance,int day, int month, int year);
        static void expired();
        static void clear();
        static void recipefunc();
        static int remove_ingredient(string name);
};

#endif
