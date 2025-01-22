#ifndef HELPER_H
#define HELPER_H

bool isValidYear(const std::string& carYear) {
    // Check if the size is exactly 4
    if (carYear.size() != 4) {
        return false;
    }

    // Check if all characters are digits
    for (char c : carYear) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    // Convert the string to an integer and check the range
    int year = std::stoi(carYear);
    if (year < 1886 || year > 2025) {
        return false;
    }

    return true;
}


#endif