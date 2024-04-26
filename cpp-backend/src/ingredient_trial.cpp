#include "ingredient.h" 
#include <string> 
#include <iostream> 

using namespace std; 

int main() {
string name = "chicken";
string quant = "2_breasts";
int importance = 5;
int day = 25;
int month = 4;
int year = 2024;
ingredient(name,quant,importance,day,month,year);

name = "rice";
quant = "5_oz";
importance = 4;
day = 20;
month = 4;
year = 2024;
ingredient(name,quant,importance,day,month,year);
//int worked = ingredient::remove_ingredient("chicken");
//cout << worked << endl;
ingredient::expired();
return 0;
}