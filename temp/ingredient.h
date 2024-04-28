#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <napi.h>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class ingredient {
public:
    string ingredient_name;
    int importance;
    string quantity;
    int day;
    int month;
    int year;
    ingredient(string ingredient_name, string quantity, int importance, int day, int month, int year);
    static void expired();
    static void clear();
    static void recipefunc();
    static int remove_ingredient(const string name);
};

int populateRecipies();
void populateIngredients();

Napi::Number populateRecipiesWrapped(const Napi::CallbackInfo& info);

Napi::Object Init(Napi::Env env, Napi::Object exports);

NODE_API_MODULE(addon, Init)

#endif
