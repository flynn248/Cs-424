#ifndef MANUALMODE_H
#define MANUALMODE_H
#include "Banker.h"
#include <iostream>
#include <memory>
#include <string>

class ManualMode 				{
public:
	ManualMode(char* []);
	void processInput(std::string&);
private:
	std::unique_ptr<Banker> banker;
	void doRequestCommand(const int&, const int&, const int&);
	void doReleaseCommand(const int&, const int&, const int&);
};

ManualMode::ManualMode(char* argv[]) 				{
	banker = std::make_unique<Banker>(argv);
}

void ManualMode::processInput(std::string& inputLine) {
	std::string command,
		processIDStr,
		resourceIDStr,
		resourceQtyStr;
	int bufferInString = 0,
		processID,
		resourceID,
		resourceQty;

	command = inputLine.substr(0, inputLine.find(" "));
	resourceQtyStr = inputLine.substr(command.size() + 1 , inputLine.find(" ", command.size() + 1) - command.size() - 1);
	bufferInString = command.size() + resourceQtyStr.size() + 5;
	resourceIDStr = inputLine.substr(bufferInString, inputLine.find(" ", bufferInString) - bufferInString);
	bufferInString += resourceIDStr.size() + 5;
	processIDStr = inputLine.substr(bufferInString);

	processID = std::stoi(processIDStr);
	resourceID = std::stoi(resourceIDStr);
	resourceQty = std::stoi(resourceQtyStr);

	if (processID >= banker->getNumProcesses() || processID < 0) 	{
		std::cout << "\nInvalid Process Number!\n";
		return;
	}
	if (resourceID >= banker->getNumResources() || resourceID < 0) 	{
		std::cout << "\nInvalid Resource Number!\n";
		return;
	}

	
	if (command == "request") {
		doRequestCommand(processID, resourceID, resourceQty);
	}
	else if (command == "release") {
		doReleaseCommand(processID, resourceID, resourceQty);
	}
	else
		std::cout << "\nInput Command Not Recognized!\n";
}

void ManualMode::doRequestCommand(const int& processID, const int& resourceID, const int& resourceQty) {
	if (banker->validRequestQty(processID, resourceID, resourceQty) == false) {
		std::cout << "\nInvalid Request Amount!\n";
		return;
	}
	bool safeRequest = banker->makeResourceRequest(processID, resourceID, resourceQty);
	std::cout << "Process " << processID << " requests " << resourceQty << " of resource " << resourceID << ": ";
	if (safeRequest == false)
		std::cout << "Denied\n";
	else
		std::cout << "Accepted\n";
}
void ManualMode::doReleaseCommand(const int& processID, const int& resourceID, const int& resourceQty) {
	if (banker->validReleaseQty(processID, resourceID, resourceQty) == false) {
		std::cout << "\nInvalid Release Amount!\n";
		return;
	}
	banker->makeResourceRelease(processID, resourceID, resourceQty);
	std::cout << "Process " << processID << " releases " << resourceQty << " of resource " << resourceID << std::endl;
}
#endif // !MANUALMODE_H