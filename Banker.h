#ifndef BANKER_H
#define BANKER_H
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
class Banker 				{
public:
	Banker(char* []);

    bool bankerAlgorithm(const int&, const int&);
    bool makeResourceRequest(const int&, const int&, const int&);
    void makeResourceRelease(const int&, const int&, const int&);

    int getNumResources() const;
    int getNumProcesses() const;
    int getProcessMaxClaims(const int&, const int&) const;
    int getProcessAllocatedResources(const int&, const int&) const;

    bool validRequestQty(const int&, const int&, const int&);
    bool validReleaseQty(const int&, const int&, const int&);
private:
    int numResources,
        numProcesses,
        numFinishedProcess = 0;

	std::vector<int> availableResources;
    std::vector<int> work;
    std::vector<int> total;
    std::vector<bool> finish;

    std::vector<std::vector<int>> need;
	std::vector<std::vector<int>> maxClaims;
	std::vector<std::vector<int>> requestEdges;
	std::vector<std::vector<int>> allocatedResources;

    void initializeFromFile(char* argv[]);
    void initializeOtherVectors();
    void validateInitialConditions();
    void checkIfInSafeState();
    bool bankerGraphReduction(const int&, std::vector<bool>&);
};

Banker::Banker(char* argv[]) {
    initializeFromFile(argv);
    initializeOtherVectors();
    validateInitialConditions();
    checkIfInSafeState();
}

void Banker::initializeFromFile(char* argv[]) {
    std::string fileName = argv[2];
    std::ifstream setUpFile(fileName, std::ios::in);

    if (!setUpFile) {
        throw std::invalid_argument("Failed to open file!\n");
    }

    int valueFromFile;

    setUpFile >> numResources;
    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    setUpFile >> numProcesses;
    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore 'Available'

    for (int i = 0; i < numResources; i++) 		{
        setUpFile >> valueFromFile;
        availableResources.push_back(valueFromFile);
    }
    
    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore 'Max'

    for (int i = 0; i < numProcesses; i++) 		{
        maxClaims.push_back(std::vector<int>());
        for (int j = 0; j < numResources; j++) 		{
            setUpFile >> valueFromFile;
            maxClaims[i].push_back(valueFromFile);
        }
    }

    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    setUpFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore 'Allocation'

    for (int i = 0; i < numProcesses; i++) {
        allocatedResources.push_back(std::vector<int>());
        for (int j = 0; j < numResources; j++) {
            setUpFile >> valueFromFile;
            allocatedResources[i].push_back(valueFromFile);
        }
    }
 
    setUpFile.close();
}
void Banker::initializeOtherVectors() {
    for (int i = 0; i < numProcesses; i++) { //need
        need.push_back(std::vector<int>());
        for (int j = 0; j < numResources; j++) {
            need[i].push_back(maxClaims[i][j] - allocatedResources[i][j]);
        }
    }

    work = availableResources; //work

    for (int i = 0; i < numProcesses; i++) //finish
        finish.push_back(false);


    for (int i = 0; i < numProcesses; i++) { //requestEdges
        requestEdges.push_back(std::vector<int>());
        for (int j = 0; j < numResources; j++)
            requestEdges[i].push_back(0);
    }

    for (int j = 0; j < numResources; j++) 		{ //total
        int sum = 0;
        for (int i = 0; i < numProcesses; i++)
            sum += allocatedResources[i][j];

        total.push_back(sum + availableResources[j]);
    }
}
void Banker::validateInitialConditions() {
    for (int i = 0; i < numProcesses; i++) //check if allocatedResources[Pi][Rj] > maxClaims[Pi][Rj]
        for (int j = 0; j < numResources; j++) 
            if (allocatedResources[i][j] > maxClaims[i][j])
                throw std::out_of_range("Allocated Resources Exceed Max Claims!\n");

    for (int j = 0; j < numResources; j++) { //check if allocatedResources[Pi][Rj] + availableResources[Rj] != total[Rj]
        int sum = 0;
        for (int i = 0; i < numProcesses; i++)
            sum += allocatedResources[i][j];

        if ((sum + availableResources[j]) != total[j])
            throw std::out_of_range("Invalid Total Resource Amount!\n");
    }
}

bool Banker::validRequestQty(const int& processID, const int& resourceID, const int& resourceAmount) {
    if (resourceAmount > maxClaims[processID][resourceID] || resourceAmount < 0)
        return false;
    else
        return true;
}
bool Banker::validReleaseQty(const int& processID, const int& resourceID, const int& resourceAmount) {
    if (resourceAmount > allocatedResources[processID][resourceID] || resourceAmount < 0)
        return false;
    else
        return true;
}

bool Banker::makeResourceRequest(const int& processID, const int& resourceID, const int& resourceAmount) {
    requestEdges[processID][resourceID] = resourceAmount;
    return bankerAlgorithm(processID, resourceID);
}
void Banker::makeResourceRelease(const int& processID, const int& resourceID, const int& resourceAmount) {
    availableResources[resourceID] += resourceAmount;
    allocatedResources[processID][resourceID] -= resourceAmount;
}

int Banker::getNumResources() const { return numResources; }
int Banker::getNumProcesses() const { return numProcesses; }
int Banker::getProcessMaxClaims(const int& processID, const int& resourceID) const { return maxClaims[processID][resourceID]; }
int Banker::getProcessAllocatedResources(const int& processID, const int& resourceID) const { return allocatedResources[processID][resourceID]; }

void Banker::checkIfInSafeState() {
    if (!bankerAlgorithm(0, 0))
        throw std::logic_error("Initial State Fails Banker's Algorithm!\n");
}

bool Banker::bankerAlgorithm(const int& processID, const int& resourceID) {
    std::vector<bool> blockedProcesses(numProcesses);
    numFinishedProcess = 0;
    work = availableResources;
    for (int i = 0; i < numProcesses; i++)
        finish[i] = false;

    int numBlockedProcesses = 0;

    if (requestEdges[processID][resourceID] > work[resourceID]) {//check if requests is more than available
        requestEdges[processID][resourceID] = 0;
        return false;
    }

    int grantedRequest = requestEdges[processID][resourceID]; //grant request
    requestEdges[processID][resourceID] = 0;
    allocatedResources[processID][resourceID] += grantedRequest;
    need[processID][resourceID] -= grantedRequest;
    work[resourceID] -= grantedRequest;

    for (int process = 0; process < numProcesses; process++) 		{ //if need > availableResources, process is blocked
        bool blocked = false;
        for (int j = 0; j < numResources; j++) 		{
            if (need[process][j] > work[j]) {
                blocked = true;
                break;
            }
        }
        if (blocked) {
            blockedProcesses[process] = true;
            numBlockedProcesses++;
        }
        else
            blockedProcesses[process] = false;
    }
    
    if (numBlockedProcesses == numProcesses) { //All blocked, deny request
        allocatedResources[processID][resourceID] -= grantedRequest;
        need[processID][resourceID] += grantedRequest;
        work[resourceID] += grantedRequest;
        return false;
    }

    for (int i = 0; i < numProcesses; i++) 		{ //Apply graph reduction
        if (blockedProcesses[i] == false) 	{
            if (bankerGraphReduction(i, blockedProcesses)) 	{
                availableResources[resourceID] -= grantedRequest;
                return true;
            }
        }
    }

    return false; //if no path led to successful reduction
}
bool Banker::bankerGraphReduction(const int& processID, std::vector<bool>& blockedProcesses) {//graph reduction
    finish[processID] = true;
    numFinishedProcess++;
    if (numFinishedProcess == numProcesses) //successful reduction
        return true;
    
    std::vector<int> releasedResources(numResources);
    for (int j = 0; j < numResources; j++) 		{ //release resources
        releasedResources[j] = allocatedResources[processID][j];
        work[j] += releasedResources[j];
        allocatedResources[processID][j] = 0;
    }


    for (int process = 0; process < numProcesses; process++) { //if need > availableResources, process is blocked
        if (finish[process] == true)
            continue;

        bool blocked = false;
        for (int j = 0; j < numResources; j++) {
            if (need[process][j] > work[j]) {
                blocked = true;
                break;
            }
        }
        if (blocked)
            blockedProcesses[process] = true;
        else
            blockedProcesses[process] = false;
    }

    for (int i = 0; i < numProcesses; i++) { //Continue graph reduction
        if (blockedProcesses[i] == false && finish[i] != true) {
            if (bankerGraphReduction(i, blockedProcesses)) {
                for (int j = 0; j < numResources; j++) 		{
                    work[j] -= releasedResources[j];
                    allocatedResources[processID][j] = releasedResources[j];
                }
                return true;
            }
        }
    }

    for (int j = 0; j < numResources; j++) { //give back resources
        work[j] -= releasedResources[j];
        allocatedResources[processID][j] = releasedResources[j];
    }
    finish[processID] = false;
    numFinishedProcess--;
    return false; //if no path was accepted
}

#endif // !BANKER_H