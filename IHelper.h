#ifndef IHELPER_H
#define IHELPER_H

#include <vector>
#include <iostream>

class IHelper
{
public: 
    IHelper(){};
    bool startCode(std::string filename);
    bool endCode(std::string filename);
    bool compile(std::vector<std::string> entries, std::string method);
private:
    int cont;
    float lastResult;

    int isValidOperation(float x, float y, std::string method, bool hasFloat1, bool hasFloat2);
    std::string add(std::string n1, std::string n2, bool flipSignal);
    std::string subtract(std::string n1, std::string n2, bool negative);
    std::string multiply(std::string n1, std::string n2, bool negative);
    std::string divide(std::string n1, std::string n2, bool negative, int cont);
    std::string remainder(std::string n1, std::string n2, bool negative, int cont);
    std::string power(std::string n1, std::string n2, bool negative, int cont);
    std::string output(std::string port, std::string displayMethod, std::string value);
    std::string dealWithEntries(std::string n1, std::string n2);
    std::vector<std::string> split(std::string st);
};

#endif 