#include <iostream>
#include <vector>

namespace mynamespace {

class MyClass {
private:
    int _myVariable;

public:
    MyClass() : _myVariable(0) {}

    int getMyVariable() const {
        return _myVariable;
    }

    void setMyVariable(int value) {
        _myVariable = value;
    }
};

// Function to print a vector of integers
void printVector(const std::vector<int>& vec) {
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

/**
 * @brief The main function of the program.
 * 
 * This function demonstrates the usage of the MyClass and printVector function.
 * 
 * @return int Return status of the program.
 */
int main() {
    MyClass myObject;
    myObject.setMyVariable(5);
    
    std::cout << "MyVariable: " << myObject.getMyVariable() << std::endl;
    
    std::vector<int> myVec{1, 2, 3, 4, 5};
    printVector(myVec);

    return 0;
}
