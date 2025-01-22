#ifndef CORPORATERNTER_H
#define CORPORATERNTER_H

#include "Renter.h"

//this class inherits Renter
class CorporateRenter : public Renter {
public:
    CorporateRenter(std::string n, std::string e, double f, std::string p, std::string a)
        : Renter(n, e, f, p, a) {
    }
};

#endif
