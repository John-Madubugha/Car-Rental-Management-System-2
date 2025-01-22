#ifndef RENTER_H
#define RENTER_H

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

//this is the base class (abstract)
//that has been inherited by other different types of renters
class Renter {
protected:
    std::string name;
    std::string email;
    double totalFee;
    std::string phoneNumber;
    std::string address;
    std::vector<Car*> rentedCars;

public:
    Renter(std::string n, std::string e, double f, std::string p, std::string a)
        : name(n), email(e), totalFee(f), phoneNumber(p), address(a) {
    }

    virtual void displayRenter(RenderWindow& window, Sprite background, int& xCor, int& yCor, int renterId) {
        Font font;
        if (!font.loadFromFile("fonts/Roboto-Medium.ttf")) {
            std::cout << "Error loading font file" << std::endl;
            return;
        }

        Text renterIdText("Renter ID: " + std::to_string(renterId), font, 20);
        renterIdText.setFillColor(Color::White);
        yCor += 25;
        renterIdText.setPosition(xCor, yCor);
        window.draw(renterIdText);

        Text nameText("Name: " + name, font, 20);
        nameText.setFillColor(Color::White);
        yCor += 25;
        nameText.setPosition(xCor, yCor);
        window.draw(nameText);

        Text emailText("Email: " + email, font, 20);
        emailText.setFillColor(Color::White);
        yCor += 25;
        emailText.setPosition(xCor, yCor);
        window.draw(emailText);

        Text totalFeeText("Total Fee: $" + std::to_string(totalFee), font, 20);
        totalFeeText.setFillColor(Color::White);
        yCor += 25;
        totalFeeText.setPosition(xCor, yCor);
        window.draw(totalFeeText);

        Text phoneText("Phone: " + phoneNumber, font, 20);
        phoneText.setFillColor(Color::White);
        yCor += 25;
        phoneText.setPosition(xCor, yCor);
        window.draw(phoneText);

        Text addressText("Address: " + address, font, 20);
        addressText.setFillColor(Color::White);
        yCor += 25;
        addressText.setPosition(xCor, yCor);
        window.draw(addressText);
    }


    std::string getName() const { return name; }
    std::string getPhoneNumber() const { return phoneNumber; }
    std::vector<Car*>& getRentedCars() { return rentedCars; }
    double getTotalFee() {
        return totalFee;
    }
    void setTotalFee(double fee) { totalFee = fee; }
};

#endif
