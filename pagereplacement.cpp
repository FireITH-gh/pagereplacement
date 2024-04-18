#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

bool validateInput(std::string& s) {

    // Ensure that every character in the string is a digit
    for (auto a : s) {
        if (!isdigit(a)) {
            return false;
        }
    }

    return true;
}

bool findIntInVector(std::vector<int>& v, int t) {

    // Return true if the target is found in the vector
    return (std::find(v.begin(), v.end(), t) != v.end());
}

// Function to predict the page that will be used farthest in the future for Optimal replacement algorithm
int predictPage(std::vector<int>& pageString, std::vector<int>& frames, int numPages, int index) {

    int result = -1;
    int farthest = index;

    // Find the index of the page which will be used farthest in the future
    for (int i = 0; i < frames.size(); ++i) {
        int j;
        for (j = index; j < numPages; ++j) {
            if (frames.at(i) == pageString.at(j)) {
                if (j > farthest) {
                    farthest = j;
                    result = i;
                }
                break;
            }
        }

        // If a page is never referenced again, that is the page that should be replaced
        if (j == numPages) {
            return i;
        }

    }

    // If result was never updated, return zero, otherwise result holds the reference that should be replaced
    if (result == -1) {
        return 0;
    } else {
        return result;
    }
}

int main () {

    bool valid = true;
    std::vector<int> pageString;
    std::string input;

    do {
        
        input = "";
        pageString.clear();

        // Get the page reference string from the user
        std::cout << "Enter a page reference as a string of numbers separated by commas: ";
        std::getline(std::cin, input);

        std::stringstream ss(input);

        // Convert the string to integers and insert into the page vector
        while (ss.good()) {

            std::string substring;
            std::getline(ss, substring, ',');

            // If the input is not an integer, have the user try again
            if (validateInput(substring)) {
                pageString.push_back(std::stoi(substring));
                valid = true;

            } else {
                std::cerr << "Invalid input, please try again!" << std::endl << std::endl;
                valid = false;
                ss.str("");
                break;
            }
        }

    } while (!valid);

    int numFrames;
    std::string frameinput;

    // Get the number of frames from the user
    do {

        std::cout << "Enter the number of frames: ";
        std::getline(std::cin, frameinput);

        // Validate the input, if the input is not an integer, have the user try again
        if (validateInput(frameinput)) {
            numFrames = std::stoi(frameinput);
            valid = true;
        } else {
            std::cerr << "Invalid input, please try again!" << std::endl << std::endl;
            valid = false;
        }

    } while (!valid);   




    // PAGE REPLACEMENT ALGORITHMS


    // 1. Least Recently Used

    std::cout << std::endl << "For LRU Algorithm: " << std::endl;

    int lruFaults = 0;
    std::vector<int> lruFrames;

    for (int i = 0; i < pageString.size(); ++i) {

        int currentPage = pageString.at(i);
        bool fault = false;
        std::string testforfaults;
        

        // If the page is already present in the frame table, bring it to the front
        if (findIntInVector(lruFrames, currentPage)) {

            lruFrames.erase(std::find(lruFrames.begin(), lruFrames.end(), currentPage));

            lruFrames.insert(lruFrames.begin(), currentPage);

        } 
        
        // If the page isn't present and the frame table isn't full, insert the new page
        else if (lruFrames.size() < numFrames) {

            lruFrames.insert(lruFrames.begin(), currentPage);
            fault = true;
            lruFaults++;
        }

        // Otherwise, remove the last element and add the new page
        else {

            lruFrames.pop_back();
            lruFrames.insert(lruFrames.begin(), currentPage);
            fault = true;
            lruFaults++;
        }


        // Printing the results of each step
        if (fault) {
            std::stringstream str;
            str << "Page fault (" << currentPage << ")";
            testforfaults = str.str();
        } else {
            testforfaults = "No page fault";
        }

        std::cout << "      Step " << i + 1 << ": " << testforfaults << " - Frames: [ ";

        for (auto a : lruFrames) {
            std::cout << a << " ";
        }

        std::cout << "], Faults: " << lruFaults << std::endl;
    }

    std::cout << std::endl << "     Total Page Faults for LRU: " << lruFaults << std::endl << std::endl << std::endl;



    // 2. Optimal

    std::cout << "For Optimal Algorithm: " << std::endl;

    int optimalFaults = 0;
    std::vector<int> optimalFrames;

    for (int i = 0; i < pageString.size(); ++i) {

        int currentPage = pageString.at(i);
        bool fault = false;
        std::string testforfaults;

        // If the page is already present in the frame table, do nothing
        if (findIntInVector(optimalFrames, currentPage)) {} 
        
        // If the page isn't present and the frame table isn't full, insert the new page
        else if (optimalFrames.size() < numFrames) {

            optimalFrames.push_back(currentPage);
            fault = true;
            optimalFaults++;
        }

        // Otherwise, remove the predicted element and add the new page
        else {

            optimalFrames.at(predictPage(pageString, optimalFrames, pageString.size(), i + 1)) = currentPage;
            fault = true;
            optimalFaults++;
        }


        // Printing the results of each step
        if (fault) {
            std::stringstream str;
            str << "Page fault (" << currentPage << ")";
            testforfaults = str.str();
        } else {
            testforfaults = "No page fault";
        }

        std::cout << "      Step " << i + 1 << ": " << testforfaults << " - Frames: [ ";

        for (auto a : optimalFrames) {
            std::cout << a << " ";
        }

        std::cout << "], Faults: " << optimalFaults << std::endl;
    }

    std::cout << std::endl << "     Total Page Faults for Optimal: " << optimalFaults << std::endl << std::endl << std::endl;



    // 3. First In First Out

    std::cout << "For FIFO Algorithm: " << std::endl;

    int fifoFaults = 0;
    std::vector<int> fifoFrames;

    for (int i = 0; i < pageString.size(); ++i) {

        int currentPage = pageString.at(i);
        bool fault = false;
        std::string testforfaults;

        // If the page is already present in the frame table, do nothing
        if (findIntInVector(fifoFrames, currentPage)) {} 
        
        // If the page isn't present and the frame table isn't full, insert the new page
        else if (fifoFrames.size() < numFrames) {

            fifoFrames.push_back(currentPage);
            fault = true;
            fifoFaults++;
        }

        // Otherwise, remove the first element and add the new page
        else {

            fifoFrames.erase(fifoFrames.begin());
            fifoFrames.push_back(currentPage);
            fault = true;
            fifoFaults++;
        }


        // Printing the results of each step
        if (fault) {
            std::stringstream str;
            str << "Page fault (" << currentPage << ")";
            testforfaults = str.str();
        } else {
            testforfaults = "No page fault";
        }

        std::cout << "      Step " << i + 1 << ": " << testforfaults << " - Frames: [ ";

        for (auto a : fifoFrames) {
            std::cout << a << " ";
        }

        std::cout << "], Faults: " << fifoFaults << std::endl;
    }

    std::cout << std::endl << "     Total Page Faults for FIFO: " << fifoFaults << std::endl << std::endl << std::endl;
}
