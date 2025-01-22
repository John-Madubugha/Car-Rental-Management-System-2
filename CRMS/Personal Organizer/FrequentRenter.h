#ifndef FREQUENTRENTER_H
#define FREQUENTRENTER_H

#include "Renter.h"

//this class inherits Renter
class FrequentRenter : public Renter {
public:
    FrequentRenter(std::string n, std::string e, double f, std::string p, std::string a)
        : Renter(n, e, f, p, a) {
    }
};

#endif
