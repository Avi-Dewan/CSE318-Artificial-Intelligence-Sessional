#ifndef CAR_H
#define CAR_H

#include<vector>
#include<string>
#include<map>
#include<iostream>

using namespace std;

class Car{
private:

public:
    string buying;
    string maint;
    string doors;
    string persons;
    string lug_boot;
    string safety;
    string classValue;

    Car(string, string, string, string, string, string, string);
    ~Car();

    void show() {
        cout << buying << "," << maint << "," << doors << "," << persons << "," << lug_boot << "," << safety << "," << classValue << endl;
    }

};


Car::Car(string b, string m, string d, string p, string l, string s, string c)
{   
   buying = b;
   maint = m;
   doors = d;
   persons = p;
   lug_boot = l;
   safety = s;
   classValue = c;
}

Car::~Car()
{
}


#endif