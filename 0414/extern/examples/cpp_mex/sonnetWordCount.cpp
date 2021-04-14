#include "mex.hpp"
#include "mexAdapter.hpp"
#include <unordered_map>
#include <fstream> 

using matlab::mex::ArgumentList;
using namespace matlab::data;

class MexFunction : public matlab::mex::Function {

    // Input stream to read words from file
    std::ifstream inFile;

    // Unordered map to keep track of word count
    std::unordered_map<std::string, int> wordCount;

    // Pointer to MATLAB engine
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();

    // Factory to create MATLAB data arrays
    ArrayFactory factory;

public:
    // Open the file for reading 
    MexFunction() {
        mexLock();
        matlabPtr->eval(u"fname = fullfile(matlabroot, 'examples', 'matlab', 'sonnets.txt');");
        matlab::data::CharArray fileName = matlabPtr->getVariable(u"fname");
        inFile.open(fileName.toAscii());
        if (!inFile.is_open()) {
            std::ostringstream stream;
            stream << "Failed to open sonnets.txt" << std::endl;
            displayOnMATLAB(stream);
        }
    }
    // Close the file when destroying the object
    ~MexFunction() {
        if (inFile.is_open())
            inFile.close();
    }

    void operator()(ArgumentList outputs, ArgumentList inputs) {
        if (inFile.is_open() && !inFile.eof()) {
            std::string word;
            inFile >> word;
            wordCount[word]++;
            std::ostringstream stream;
            stream << "Read : " << "\"" << word << "\"" 
                << ", current count: " << wordCount[word] << std::endl;
            displayOnMATLAB(stream);
        }
        if (!inputs.empty() || !inFile.is_open()) {
            mexUnlock();
        }
    }

    void displayOnMATLAB(const std::ostringstream& stream){
        matlabPtr->feval(u"fprintf", 0, 
            std::vector<Array>({ factory.createScalar(stream.str()) }));
    }
};