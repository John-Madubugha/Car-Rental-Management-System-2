#ifndef REGULARRENTER_H
#define REGULARRENTER_H

#include "Renter.h"

//this class inherits Renter
class RegularRenter : public Renter {
public:
    RegularRenter(std::string n, std::string e, double f, std::string p, std::string a)
        : Renter(n, e, f, p, a) {
    }
};

#endif
