#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <vector>
#include "CompactCar.h"
#include "LuxuryCar.h"
#include "SUV.h"
#include "RegularRenter.h"
#include "FrequentRenter.h"
#include "CorporateRenter.h"
#include "Helper.h"
using namespace sf;
using namespace std;

//The CarRentalManager class manages the whole system
//It basically controls the systems basic functionality
//allowing the user to add cars, renters and display them
//it also allows renting and returning cars along with displaying the total fee paid
class CarRentalManager {
private:
    RenderWindow& window;
    Text options[8];
    Font font;
    Sprite background;
    Texture bgTexture;
    vector<Car*> cars;
    vector<Renter*> renter;
    int xCor;
    int yCor;

    //helper functions
    void handleKeyPress(Keyboard::Key key, RenderWindow& window) {
        switch (key) {
        case Keyboard::Num1:
        case Keyboard::Numpad1:
            cout << "Adding cars...\n";
            addCarScreen(window);
            break;
        case Keyboard::Num2:
        case Keyboard::Numpad2:
            cout << "Removing cars...\n";
            removeCars(window);
            break;
        case Keyboard::Num3:
        case Keyboard::Numpad3:
            cout << "Displaying available cars...\n";
            displayCarsScreen(window);
            break;
        case Keyboard::Num4:
        case Keyboard::Numpad4:
            cout << "Adding renter...\n";
            addRenterScreen(window);
            break;
        case Keyboard::Num5:
        case Keyboard::Numpad5:
            cout << "Renting car...\n";
            rentACar(window);
            break;
        case Keyboard::Num6:
        case Keyboard::Numpad6:
            cout << "Returning Car...\n";
            returningACarScreen(window);
            break;
        case Keyboard::Num7:
        case Keyboard::Numpad7:
            cout << "Displaying Renters...\n";
            displayRentersScreen(window);
            break;
        case Keyboard::Num8:
        case Keyboard::Numpad8:
            cout << "Exiting...\n";
            window.close();
        default:
            break;
        }
    }

    void inputText(RenderWindow& window, Font& font, Text& pageTitle, Text& promptText, float& x, float& y, string& value) {
        Text inputText("", font, 24);
        inputText.setFillColor(Color::White);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                else if (event.type == Event::TextEntered) {
                    if (event.text.unicode == 13) { 
                        value = inputText.getString().toAnsiString();
                        return;
                    }
                    else if (event.text.unicode == 8) {
                        if (!inputText.getString().isEmpty()) {
                            string str = inputText.getString();
                            str.pop_back();
                            inputText.setString(str);
                        }
                    }
                    else {
                        inputText.setString(inputText.getString() + static_cast<char>(event.text.unicode));
                    }
                }
            }

            window.clear();
            window.draw(background);
            window.draw(pageTitle);
            window.draw(promptText);
            inputText.setPosition(x, y + 30);
            window.draw(inputText);
            window.display();
        }
    }

    template <typename T>
    void inputNumber(RenderWindow& window, Font& font, Text& pageTitle, Text& promptText, float& x, float& y, T& object, void (T::* setter)(int)) {
        Text inputText("", font, 24);
        inputText.setFillColor(Color::White);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                else if (event.type == Event::TextEntered) {
                    if (event.text.unicode == 13) {  // Enter key
                        (object.*setter)(stoi(inputText.getString().toAnsiString()));
                        return;
                    }
                    else if (event.text.unicode == 8) {  // Backspace key
                        // Handle backspace
                        if (!inputText.getString().isEmpty()) {
                            string str = inputText.getString();
                            str.pop_back();
                            inputText.setString(str);
                        }
                    }
                    else {
                        // Handle other key presses
                        inputText.setString(inputText.getString() + static_cast<char>(event.text.unicode));
                    }
                }
            }

            window.clear();
            window.draw(background);
            window.draw(pageTitle);
            window.draw(promptText);
            inputText.setPosition(x, y + 30);
            window.draw(inputText);
            window.display();
        }
    }

    template <typename T>
    void inputBool(RenderWindow& window, Font& font, Text& pageTitle, Text& promptText, float& x, float& y, T& object, void (T::* setter)()) {
        Text inputText("", font, 24);
        inputText.setFillColor(Color::White);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                else if (event.type == Event::TextEntered) {
                    if (event.text.unicode == '0' || event.text.unicode == '1') {
                        (object.*setter)();
                        return;
                    }
                }
            }

            window.clear();
            window.draw(background);
            window.draw(pageTitle);
            window.draw(promptText);
            inputText.setPosition(x, y + 30);
            window.draw(inputText);
            window.display();
        }
    }

    bool inputTextWithEscape(RenderWindow& window, Font& font, Text& title, Text& prompt, float x, float y, string& input) {
        Text key3;
        key3.setString("Main Menu [esc]");
        key3.setFont(font);
        key3.setCharacterSize(16);
        key3.setFillColor(Color::White);
        key3.setPosition(680, 580);

        input.clear();
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                    return false; // Handle window close
                }
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    return false; // Exit input and return to main menu
                }
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode == '\b') { // Handle backspace
                        if (!input.empty()) input.pop_back();
                    }
                    else if (event.text.unicode == '\r') { // Handle Enter
                        return true; // Input complete
                    }
                    else if (event.text.unicode < 128) { // Ignore non-ASCII characters
                        input += static_cast<char>(event.text.unicode);
                    }
                }
            }

            // Display current input
            window.clear();
            window.draw(background);
            window.draw(title);
            window.draw(key3);
            window.draw(prompt);

            Text inputDisplay(input, font, 30);
            inputDisplay.setFillColor(Color::White);
            inputDisplay.setPosition(x, y + 30);
            window.draw(inputDisplay);

            window.display();
        }
        return false;
    }


public:

    CarRentalManager(RenderWindow& window) : window(window) {
        //loading font and background texture
        if (!font.loadFromFile("fonts/Roboto-Medium.ttf")) {
            cerr << "Error loading font." << endl;
        }
        bgTexture.loadFromFile("img/background.png");
        background.setTexture(bgTexture);
        background.setScale(1, 2);

        //initializing cars
        Car* car1 = new SUV(1, "Tesla", "Model X", "2022", "TES123");
        Car* car2 = new SUV(2, "BMW", "X5", "2021", "BMW456");
        Car* car3 = new SUV(3, "Audi", "Q7", "2023", "AUD789");
        Car* car4 = new CompactCar(4, "Toyota", "Camry", "2021", "TYT123");
        Car* car5 = new CompactCar(5, "Honda", "Accord", "2020", "HND456");
        Car* car6 = new CompactCar(6, "BMW", "5 Series", "2022", "BMW789");

        cars = { car1, car2, car3, car4, car5, car6 };

        //initializing Renters
        Renter* renter1 = new RegularRenter("Andrew McDonald", "andy@gmail.com", 0, "1234567890", "123 St");
        Renter* renter2= new FrequentRenter("Kylian Mbappe", "mbappe@gmail.com", 0, "1234567891", "456 St");
        Renter* renter3 = new CorporateRenter("Steve Nicol", "steve@gmail.com", 0, "1234567892", "789 St");

        renter = { renter1, renter2, renter3 };

        //initializing CarRentalManager options
        options[0].setString("1. Add Car");
        options[1].setString("2. Remove Car");
        options[2].setString("3. Display Available Cars");
        options[3].setString("4. Add Renter");
        options[4].setString("5. Rent Car");
        options[5].setString("6. Return Car");
        options[6].setString("7. Display Renters");
        options[7].setString("8. Exit");

        for (auto& option : options) {
            option.setFont(font);
            option.setCharacterSize(30);
            option.setFillColor(Color::White);
        }

        options[0].setPosition(270, 160);
        options[1].setPosition(270, 200);
        options[2].setPosition(270, 240);
        options[3].setPosition(270, 280);
        options[4].setPosition(270, 320);
        options[5].setPosition(270, 360);
        options[6].setPosition(270, 400);
        options[7].setPosition(270, 440);

        window.setTitle("CRMS");
        xCor = 20;
        yCor = 60;
    }

    void display() {
        Text pageTitle;
        pageTitle.setString("CRMS");
        pageTitle.setFont(font);
        pageTitle.setCharacterSize(60);
        pageTitle.setFillColor(Color::White);
        pageTitle.setPosition(250, 30);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::KeyPressed) {
                    handleKeyPress(event.key.code, window);
                }
            }

            window.clear();
            window.draw(background);
            window.draw(pageTitle);

            for (const auto& option : options) {
                window.draw(option);
            }

            window.display();
        }
    }

    void displayCarsScreen(RenderWindow& window) {
        window.clear();
        xCor = 20;
        yCor = 100;
        
        Text pageTitle;
        pageTitle.setString("Available Cars");
        pageTitle.setFont(font);
        pageTitle.setCharacterSize(60);
        pageTitle.setFillColor(Color::White);
        pageTitle.setPosition(300, 30);

        Text key1;
        key1.setString("Next Page [n]");
        key1.setFont(font);
        key1.setCharacterSize(16);
        key1.setFillColor(Color::White);
        key1.setPosition(700, 520);

        Text key2;
        key2.setString(" Prev Page [p]");
        key2.setFont(font);
        key2.setCharacterSize(16);
        key2.setFillColor(Color::White);
        key2.setPosition(700, 550);

        Text key3;
        key3.setString("Main Menu [esc]");
        key3.setFont(font);
        key3.setCharacterSize(16);
        key3.setFillColor(Color::White);
        key3.setPosition(680, 580);

        size_t startIndex = 0; 
        size_t endIndex = min(startIndex + 2, cars.size()); 

        while (window.isOpen()) {
            window.draw(background);
            window.draw(pageTitle);
            window.draw(key1);
            window.draw(key2);
            window.draw(key3);

            for (size_t i = startIndex; i < endIndex; ++i) {
                if (!cars[i]->getRentalStatus()) {
                    cars[i]->displayCar(window, background, xCor, yCor);
                    yCor += 25;
                }
            }

            window.display();
            Event event;
            while (window.waitEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        return;  
                    }
                    else if (event.key.code == Keyboard::N) {
                        startIndex += 2;
                        endIndex = min(startIndex + 2, cars.size());
                        window.clear();
                        yCor = 100;
                        break;
                    }
                    else if (event.key.code == Keyboard::P && startIndex >= 2) {
                        startIndex -= 2;
                        endIndex = min(startIndex + 2, cars.size());
                        window.clear();
                        yCor = 100;
                        break;
                    }
                }
                else if (event.type == Event::Closed) {
                    window.close();
                    return;
                }
            }

        }
    }

    //input handling has been done in all those functions below
    //those of which that required it
    void addCarScreen(RenderWindow& window) {
        string carBrand, carModel, plateNumber, carYear, carType;
        int id = cars.size() + 1;

        while (true) {
            window.clear();
            window.draw(background);
            Text pageTitle;
            pageTitle.setString("Add Car");
            pageTitle.setFont(font);
            pageTitle.setCharacterSize(60);
            pageTitle.setFillColor(Color::White);
            pageTitle.setPosition(250, 30);
            window.draw(pageTitle);

            Text promptText;
            promptText.setFont(font);
            promptText.setCharacterSize(30);
            promptText.setFillColor(Color::White);
            promptText.setPosition(0, 100);
            float x = 20.0f;
            float y = 110.0f;

            string value;

            // Input Car Brand
            promptText.setString("Enter car brand: ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, carBrand)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (carBrand.empty()) {
                cout << "Invalid input! Car brand cannot be empty.\n";
                continue;
            }

            // Input Car Model
            promptText.setString("Enter car model: ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, carModel)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (carModel.empty()) {
                cout << "Invalid input! Car model cannot be empty.\n";
                continue;
            }

            // Input Plate Number
            promptText.setString("Enter plate number(only letters, digits, or hyphens): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, plateNumber)) {
                return; // Exit to the main menu if ESC is pressed
            }
            // Validate plate number input
            if (plateNumber.empty()) {
                ::cout << "Invalid input! Plate number cannot be empty.\n";
            }
            else if (plateNumber.size() > 10) {
                ::cout << "Invalid input! Plate number is too long.\n";
            }
            else {
                for (char c : plateNumber) {
                    if (!std::isalnum(c) && c != '-') {
                        ::cout << "Invalid input! Plate number must contain only letters, digits, or hyphens.\n";
                        break;
                    }
                }
            }

            // Input Car Year
            promptText.setString("Enter car year(must be a 4-digit number between 1886 and 2025): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, carYear)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (!isValidYear(carYear)) {
                std::cout << "Invalid input! Car year must be a 4-digit number between 1886 and 2025.\n";
                continue;
            }

            // Input Car Type
            promptText.setString("Enter car type (Compact/Luxury/SUV): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, carType)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (carType != "Compact" && carType != "Luxury" && carType != "SUV") {
                cout << "Invalid input! Car type must be 'Compact', 'Luxury', or 'SUV'.\n";
                continue;
            }

            // Add the car based on type
            if (carType == "Compact") {
                cars.push_back(new CompactCar(id, carBrand, carModel, carYear, plateNumber));
            }
            else if (carType == "Luxury") {
                cars.push_back(new LuxuryCar(id, carBrand, carModel, carYear, plateNumber));
            }
            else if (carType == "SUV") {
                cars.push_back(new SUV(id, carBrand, carModel, carYear, plateNumber));
            }

            cout << "Car added successfully.\n";
            break; // Exit the loop if all inputs are valid
        }
    }

    void removeCars(RenderWindow& window) {
        window.draw(background);
        Text pageTitle;
        pageTitle.setString("Remove Car");
        pageTitle.setFont(font);
        pageTitle.setCharacterSize(60);
        pageTitle.setFillColor(Color::White);
        pageTitle.setPosition(250, 30);
        window.draw(pageTitle);

        Text promptText;
        promptText.setFont(font);
        promptText.setCharacterSize(30);
        promptText.setFillColor(Color::White);
        promptText.setPosition(0, 100);
        float x = 20.0f;
        float y = 110.0f;

        string plateNum;
        bool validInput = false;

        // Loop until valid input is provided
        while (!validInput) {
            promptText.setString("Enter Plate Number to remove(only letters, digits, or hyphens): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, plateNum)) {
                return; // Exit to the main menu if ESC is pressed
            }

            // Validate plate number input
            if (plateNum.empty()) {
                ::cout << "Invalid input! Plate number cannot be empty.\n";
            }
            else if (plateNum.size() > 10) { 
                ::cout << "Invalid input! Plate number is too long.\n";
            }
            else {
                validInput = true;
                for (char c : plateNum) {
                    if (!std::isalnum(c) && c != '-') {
                        ::cout << "Invalid input! Plate number must contain only letters, digits, or hyphens.\n";
                        validInput = false;
                        break;
                    }
                }
            }
        }

        // Search and remove the car
        bool found = false;

        for (auto car = cars.begin(); car != cars.end(); ++car) {
            if ((*car)->getPlateNum() == plateNum) {
                cars.erase(car);
                ::cout << "Car removed successfully.\n";
                found = true;
                break;
            }
        }

        if (!found) {
            ::cout << "Car with Plate Number " << plateNum << " not found.\n";
        }
    }

    void returningACarScreen(RenderWindow& window) {
        Font font;
        if (!font.loadFromFile("fonts/Roboto-Medium.ttf")) {
            ::cout << "Error loading font file." << ::endl;
            return;
        }

        window.draw(background);
        float x = 20.0f;
        float y = 110.0f;

        Text titleText("Return a Car", font, 40);
        titleText.setFillColor(Color::White);
        titleText.setPosition(200, 30);
        window.draw(titleText);

        ::string renterName, phone, carModel, distanceString, damageString;
        double distance = 0.0;
        bool damaged = false;

        Text promptText;
        promptText.setFont(font);
        promptText.setCharacterSize(20);
        promptText.setFillColor(Color::White);

        // Validate renter's name
        while (true) {
            promptText.setString("Enter renter's name: ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, renterName)) {
                return; // Exit to the main menu if ESC is pressed
            }

            if (renterName.empty()) {
                ::cout << "Invalid input! Renter's name cannot be empty.\n";
                continue;
            }
            break;
        }

        // Validate phone number
        while (true) {
            promptText.setString("Enter renter's phone number: ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, phone)) {
                return; // Exit to the main menu if ESC is pressed
            }

            bool isValidPhone = !phone.empty() && phone.size() >= 10;
            for (char c : phone) {
                if (!isdigit(c)) {
                    isValidPhone = false;
                    break;
                }
            }

            if (!isValidPhone) {
                ::cout << "Invalid input! Phone number must be at least 10 digits long and contain only numbers.\n";
                continue;
            }

            break;
        }

        Renter* renter1 = nullptr;
        for (auto& r : renter) {
            if (r->getName() == renterName && r->getPhoneNumber() == phone) {
                renter1 = r;
                break;
            }
        }

        if (renter1 == nullptr) {
            promptText.setString("Renter not found.");
            promptText.setPosition(20, 180);
            window.draw(promptText);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        // Validate car model
        while (true) {
            promptText.setString("Enter car model to return: ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, carModel)) {
                return; // Exit to the main menu if ESC is pressed
            }

            if (carModel.empty()) {
                ::cout << "Invalid input! Car model cannot be empty.\n";
                continue;
            }
            break;
        }

        Car* car = nullptr;
        for (auto& rentedCar : renter1->getRentedCars()) {
            if (rentedCar->getModel() == carModel) {
                car = rentedCar;
                break;
            }
        }

        if (car == nullptr) {
            promptText.setString("Rented car not found.");
            promptText.setPosition(20, 180);
            window.draw(promptText);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        // Validate distance traveled
        while (true) {
            promptText.setString("Enter distance traveled (km): ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, distanceString)) {
                return; // Exit to the main menu if ESC is pressed
            }

            try {
                distance = std::stod(distanceString);
                if (distance < 0) {
                    throw std::invalid_argument("Distance cannot be negative.");
                }
                break;
            }
            catch (...) {
                ::cout << "Invalid input! Distance must be a positive number.\n";
            }
        }

        // Validate damage status
        while (true) {
            promptText.setString("Was the car damaged? (yes/no): ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, damageString)) {
                return; // Exit to the main menu if ESC is pressed
            }

            if (damageString == "yes") {
                damaged = true;
                break;
            }
            else if (damageString == "no") {
                damaged = false;
                break;
            }
            else {
                cout << "Invalid input! Please enter 'yes' or 'no'.\n";
            }
        }

        // Calculate rent and update records
        double rent = car->calculateRent(distance, damaged);
        renter1->setTotalFee(renter1->getTotalFee() + rent);
        car->setRentalStatus(false);

        promptText.setString("Car returned. Total fee paid: " + ::to_string(renter1->getTotalFee()));
        promptText.setPosition(20, 300);
        window.draw(promptText);
        window.display();
        sf::sleep(sf::seconds(4));

        auto& rentedCars = renter1->getRentedCars();
        rentedCars.erase(remove(rentedCars.begin(), rentedCars.end(), car), rentedCars.end());
        if (rentedCars.empty()) {
            renter.erase(remove(renter.begin(), renter.end(), renter1), renter.end());
            promptText.setString("Renter removed from the system.");
            promptText.setPosition(20, 180);
            window.draw(promptText);
            window.display();
            sf::sleep(sf::seconds(2));
        }
    }

    void displayRentersScreen(RenderWindow& window) {
        window.clear();
        xCor = 20;
        yCor = 100;

        Text pageTitle;
        pageTitle.setString("Renters");
        pageTitle.setFont(font);
        pageTitle.setCharacterSize(60);
        pageTitle.setFillColor(Color::White);
        pageTitle.setPosition(300, 30);

        Text key1;
        key1.setString("Next Page [n]");
        key1.setFont(font);
        key1.setCharacterSize(16);
        key1.setFillColor(Color::White);
        key1.setPosition(700, 520);

        Text key2;
        key2.setString(" Prev Page [p]");
        key2.setFont(font);
        key2.setCharacterSize(16);
        key2.setFillColor(Color::White);
        key2.setPosition(700, 550);

        Text key3;
        key3.setString("Main Menu [esc]");
        key3.setFont(font);
        key3.setCharacterSize(16);
        key3.setFillColor(Color::White);
        key3.setPosition(680, 580);

        size_t startIndex = 0;  
        size_t endIndex = min(startIndex + 2, renter.size()); 

        while (window.isOpen()) {
            window.draw(background);
            window.draw(pageTitle);
            window.draw(key1);
            window.draw(key2);
            window.draw(key3);
            yCor = 100;

            for (size_t i = startIndex; i < endIndex; ++i) {
                renter[i]->displayRenter(window, background, xCor, yCor, i + 1);
                yCor += 25; 
            }

            window.display();
            Event event;
            while (window.waitEvent(event)) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        return; 
                    }
                    else if (event.key.code == Keyboard::N) {
                        startIndex += 2;
                        endIndex = min(startIndex + 2, renter.size());
                        window.clear();
                        yCor = 100;
                        break;
                    }
                    else if (event.key.code == Keyboard::P && startIndex >= 2) {
                        startIndex -= 2;
                        endIndex = min(startIndex + 2, renter.size());
                        window.clear();
                        yCor = 100;
                        break;
                    }
                }
                else if (event.type == Event::Closed) {
                    window.close();
                    return;
                }
            }
        }
    }

    void addRenterScreen(RenderWindow& window) {
        window.draw(background);
        Text pageTitle;
        pageTitle.setString("Add Renter");
        pageTitle.setFont(font);
        pageTitle.setCharacterSize(60);
        pageTitle.setFillColor(Color::White);
        pageTitle.setPosition(250, 30);
        window.draw(pageTitle);

        Text promptText;
        promptText.setFont(font);
        promptText.setCharacterSize(24);
        promptText.setFillColor(Color::White);
        promptText.setPosition(0, 100);
        float x = 20.0f;
        float y = 110.0f;

        string name, email, phoneNumber, address, renterType;

        // Input and validation for name
        do {
            promptText.setString("Enter Name (at least 2 characters): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, name)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (name.empty() || name.size() < 2) {
                ::cout << "Invalid input! Name must be at least 2 characters long.\n";
            }
        } while (name.empty() || name.size() < 2);

        // Input and validation for email
        do {
            promptText.setString("Enter Email (valid email format): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, email)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (email.find('@') == string::npos || email.find('.') == string::npos || email.empty()) {
                ::cout << "Invalid input! Enter a valid email address.\n";
            }
        } while (email.find('@') == string::npos || email.find('.') == string::npos || email.empty());

        // Input and validation for phone number
        bool isValidPhone;
        do {
            promptText.setString("Enter Phone Number (at least 10 digits): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, phoneNumber)) {
                return; // Exit to the main menu if ESC is pressed
            }

            isValidPhone = !phoneNumber.empty() && phoneNumber.size() >= 10;
            for (char c : phoneNumber) {
                if (!isdigit(c)) {
                    isValidPhone = false;
                    break;
                }
            }

            if (!isValidPhone) {
                ::cout << "Invalid input! Phone number must be at least 10 digits long and contain only numbers.\n";
            }
        } while (!isValidPhone);

        // Input and validation for address
        do {
            promptText.setString("Enter Address (at least 5 characters): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, address)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (address.empty() || address.size() < 5) {
                cout << "Invalid input! Address must be at least 5 characters long.\n";
            }
        } while (address.empty() || address.size() < 5);

        // Input and validation for renter type
        do {
            promptText.setString("Enter Renter Type (Regular/Frequent/Corporate): ");
            if (!inputTextWithEscape(window, font, pageTitle, promptText, x, y, renterType)) {
                return; // Exit to the main menu if ESC is pressed
            }
            if (renterType != "Regular" && renterType != "Frequent" && renterType != "Corporate") {
                cout << "Invalid input! Renter type must be Regular, Frequent, or Corporate.\n";
            }
        } while (renterType != "Regular" && renterType != "Frequent" && renterType != "Corporate");

        // Add renter to the appropriate category
        if (renterType == "Regular") {
            renter.push_back(new RegularRenter(name, email, 0, phoneNumber, address));
        }
        else if (renterType == "Frequent") {
            renter.push_back(new FrequentRenter(name, email, 0, phoneNumber, address));
        }
        else if (renterType == "Corporate") {
            renter.push_back(new CorporateRenter(name, email, 0, phoneNumber, address));
        }
    }

    void rentACar(RenderWindow& window) {
        Font font;
        if (!font.loadFromFile("fonts/Roboto-Medium.ttf")) {
            ::cout << "Error loading font file." << ::endl;
            return;
        }

        window.draw(background);
        float x = 20.0f;
        float y = 110.0f;

        Text titleText("Rent a Car", font, 40);
        titleText.setFillColor(Color::White);
        titleText.setPosition(200, 30);
        window.draw(titleText);

        ::string renterName, phone, carModel;
        Text promptText;
        promptText.setFont(font);
        promptText.setCharacterSize(20);
        promptText.setFillColor(Color::White);

        // Input renter's name with validation
        do {
            promptText.setString("Enter renter's name: ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, renterName)) {
                return; // Exit to the main menu if ESC is pressed
            }

            if (renterName.empty() || renterName.size() < 3) {
                ::cout << "Invalid name! It must be at least 3 characters long.\n";
            }
        } while (renterName.empty() || renterName.size() < 3);

        // Input renter's phone number with validation
        bool isValidPhone;
        do {
            promptText.setString("Enter renter's phone number (at least 10 digits): ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, phone)) {
                return; // Exit to the main menu if ESC is pressed
            }

            isValidPhone = !phone.empty() && phone.size() >= 10;
            for (char c : phone) {
                if (!isdigit(c)) {
                    isValidPhone = false;
                    break;
                }
            }

            if (!isValidPhone) {
                ::cout << "Invalid phone number! It must be at least 10 digits long and contain only numbers.\n";
            }
        } while (!isValidPhone);

        // Validate renter existence
        Renter* renter1 = nullptr;
        for (auto& r : renter) {
            if (r->getName() == renterName && r->getPhoneNumber() == phone) {
                renter1 = r;
                break;
            }
        }

        if (renter1 == nullptr) {
            promptText.setString("Renter not found.");
            promptText.setPosition(20, 180);
            window.draw(promptText);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        // Input car model with validation
        do {
            promptText.setString("Enter car model to rent: ");
            promptText.setPosition(0, 100);
            window.draw(promptText);
            window.display();
            if (!inputTextWithEscape(window, font, titleText, promptText, x, y, carModel)) {
                return; // Exit to the main menu if ESC is pressed
            }

            if (carModel.empty() || carModel.size() < 2) {
                ::cout << "Invalid car model! It must be at least 2 characters long.\n";
            }
        } while (carModel.empty() || carModel.size() < 2);

        // Validate car availability
        for (auto& car : cars) {
            if (car->getModel() == carModel && !car->getRentalStatus()) {
                car->setRentalStatus(true);
                renter1->getRentedCars().push_back(car);

                promptText.setString("Car rented successfully.");
                promptText.setPosition(20, 180);
                window.draw(promptText);
                window.display();
                sf::sleep(sf::seconds(2));
                return;
            }
        }

        // If no matching car is found
        promptText.setString("Car not available.");
        promptText.setPosition(20, 220);
        window.draw(promptText);
        window.display();
        sf::sleep(sf::seconds(2));
    }
};

int main() {
    RenderWindow window(VideoMode(800, 600), "CRMS");
    Font font;

    if (!font.loadFromFile("fonts/Roboto-Medium.ttf")) {
        return -1;
    }
    CarRentalManager CRM(window);
    CRM.display();

    return 0;
}