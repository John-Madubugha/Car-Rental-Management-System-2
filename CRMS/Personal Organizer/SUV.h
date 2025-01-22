#ifndef SUV_H
#define SUV_H

#include "Car.h"

//this class inherits Car
//this class overrides the calculateRent function (polymorphism)
class SUV : public Car {
public:
    SUV(int id, std::string b, std::string m, std::string y, std::string p)
        : Car(id, b, m, y, p) {
    }

    double calculateRent(double distance, bool damaged) override {
        double rent = distance * 0.25;
        if (damaged) rent += 40;  // Damage penalty
        return rent;
    }
};

#endif
