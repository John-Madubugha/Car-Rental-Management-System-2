#ifndef LUXURYCAR_H
#define LUXURYCAR_H

#include "Car.h"

//this class inherits Car
//this class overrides the calculateRent function (polymorphism)
class LuxuryCar : public Car {
public:
    LuxuryCar(int id, std::string b, std::string m, std::string y, std::string p)
        : Car(id, b, m, y, p) {
    }

    double calculateRent(double distance, bool damaged) override {
        double rent = distance * 0.3;
        if (damaged) rent += 50;
        return rent;
    }
};

#endif
