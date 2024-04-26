#include <napi.h>
#include <iostream>
#include <profile.h>
using namespace std;

//add number function
int add(int x, int y);
//add function wrapper
Napi::Number addWrapped(const Napi::CallbackInfo& info);
//Export API
Napi::Object Init(Napi::Env env, Napi::Object exports);
NODE_API_MODULE(addon, Init)
