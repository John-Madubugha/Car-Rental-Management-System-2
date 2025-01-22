#ifndef CAR_H
#define CAR_H

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

//this is the base class (abstract)
//that has been inherited by other types of cars
class Car {
protected:
    int carId;
    std::string brand;
    std::string model;
    std::string year;
    std::string plateNum;
    bool rentalStatus;
    bool insuranceStatus;

public:
    Car(int id, std::string b, std::string m, std::string y, std::string p)
        : carId(id), brand(b), model(m), year(y), plateNum(p), rentalStatus(false), insuranceStatus(false) {
    }

    virtual void displayCar(RenderWindow& window, Sprite background, int& xCor, int& yCor) {
        Font font;
        if (!font.loadFromFile("fonts/Roboto-Medium.ttf")) {
            std::cout << "Error loading font file " << std::endl;
            return;
        }

        Text carIdText("Car ID: " + std::to_string(carId), font, 20);
        carIdText.setFillColor(Color::White);
        yCor += 25;
        carIdText.setPosition(xCor, yCor);
        window.draw(carIdText);

        Text brandText("Brand: " + brand, font, 20);
        yCor += 25;
        brandText.setPosition(xCor, yCor);
        brandText.setFillColor(Color::White);
        window.draw(brandText);

        Text modelText("Model: " + model, font, 20);
        yCor += 25;
        modelText.setPosition(xCor, yCor);
        modelText.setFillColor(Color::White);
        window.draw(modelText);

        Text yearText("Year: " + year, font, 20);
        yCor += 25;
        yearText.setPosition(xCor, yCor);
        yearText.setFillColor(Color::White);
        window.draw(yearText);

        Text plateText("Plate: " + plateNum, font, 20);
        yCor += 25;
        plateText.setPosition(xCor, yCor);
        plateText.setFillColor(Color::White);
        window.draw(plateText);
    }

    int getCarId() const { return carId; }
    std::string getModel() const { return model; }
    std::string getPlateNum() const { return plateNum; }
    bool getRentalStatus() const { return rentalStatus; }
    void setRentalStatus(bool status) { rentalStatus = status; }
    bool getInsurableStatus() const { return !insuranceStatus; }
    void setInsuranceStatus(bool status) { insuranceStatus = status; }
    virtual double calculateRent(double distance, bool damaged) = 0;
};

#endif
