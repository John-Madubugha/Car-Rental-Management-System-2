#ifndef COMPACTCAR_H
#define COMPACTCAR_H

#include "Car.h"

//this class inherits Car
//this class overrides the calculateRent function (polymorphism)
class CompactCar : public Car {
public:
    CompactCar(int id, std::string b, std::string m, std::string y, std::string p)
        : Car(id, b, m, y, p) {
    }

    double calculateRent(double distance, bool damaged) override {
        double rent = distance * 0.2;
        if (damaged) rent += 30;
        return rent;
    }
};

#endif
