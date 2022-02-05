//Cs 424 Program 3
//Developed by Shane Flynn

#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>
#include "ManualMode.h"
#include "AutoMode.h"
#include "Banker.h"

void doManualMode(char* []);
void doAutoMode(char* []);

int main(int argc, char* argv[]) {
    try 	{        
        std::string mode = argv[1];
        if (mode == "manual" || mode == "Manual") {
            std::cout << "Manual Mode Activated\n";
            doManualMode(argv);
        }
        else if (mode == "auto" || mode == "Auto") {
            std::cout << "Auto mode activated" << std::endl;
            doAutoMode(argv);
        }
        else {
            throw std::invalid_argument("Invalid Mode!");
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::cerr << "\nHit enter to terminate program...\n";
        std::cin.get();
        return EXIT_FAILURE;
    }
    
    std::cout << "Thank You For Banking With Us!" << std::endl;
    std::cin.get();
    return 0;
}

void doManualMode(char* argv[]) {
    ManualMode manual(argv);
    std::string inputLine;

    while (true) 	{
        std::cout << "\nEnter a Command: ";
        std::getline(std::cin, inputLine, '\n');
        if (inputLine == "end")
            break;
        manual.processInput(inputLine);
    }
}
void doAutoMode(char* argv[]) {
    AutoMode autoZone(argv);
    autoZone.startAutomationOfProcesses();
}