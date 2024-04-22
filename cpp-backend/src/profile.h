#ifndef PROFILE
#define PROFILE

#include <string>
using namespace std;

class profile {
    public:
        string name;
        //noNos are our Food Restrictions and Allergies, it is true if the user has them and false if they don't
        //Order of noNos: [Vegetarian, Vegan, Alcohol-Free, Gluten, Dairy, Eggs, Soy, Wheat, Fish, Shellfish, Tree Nuts, Peanuts]
        bool noNos[12];
        
        profile(string name, bool noNos[12]);
        bool* getNoNos();


};

#endif
