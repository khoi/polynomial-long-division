//
//  main.cpp
//  long-division-polynominal
//
//  Created by Khoi Lai on 5/10/18.
//  Copyright © 2018 Khoi Lai. All rights reserved.
//

#include "polynomial.hpp"
#include "polynomial_linkedlist.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>

#define DEBUG_MODE false

using namespace std;

struct InputPair {
    Polynomial *p;
    PolynomialLL *p_ll;
};

bool strContainDot(const string &str) {
    return str.find('.') != string::npos;
}

void replaceSubstr(std::string& str,
               const std::string& oldStr,
               const std::string& newStr)
{
    std::string::size_type pos = 0u;
    while((pos = str.find(oldStr, pos)) != std::string::npos){
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

InputPair validateInput(const string &s) {
    string input(s);
    string delim = "+";
    int degree = INT_MIN;
    replaceSubstr(input, "-", "+-");
    vector<string> terms = split(input, delim);
    map<int, double> powAndCoeff;
    for (vector<string>::const_iterator i = terms.begin(); i != terms.end(); ++i) {
        string term = *i;
        unsigned long positionOfX = term.find('x');
        bool hasX = positionOfX != string::npos;

        if (hasX) {
            unsigned long positionOfPowChar = term.find('^');
            bool hasPow = positionOfPowChar != string::npos;
            if (hasPow) {
                string powString = term.substr(positionOfPowChar + 1);
                stringstream ss1(powString);
                int pow;
                double coeff;
                if (!(ss1 >> pow && ss1.eof())) {
                    cout << term << " is not a valid term." << endl;
                    return {nullptr, nullptr};
                }

                string coeffString = term.substr(0, positionOfX);

                if (coeffString.length() == 0) {
                    coeff = 1.0;
                } else if (coeffString == "-") {
                    coeff = -1.0;
                } else {
                    stringstream ss2(coeffString);
                    bool coeffConversionFail = (ss2 >> coeff).fail();
                    string remainString;
                    getline(ss2, remainString);

                    if (coeffConversionFail || remainString != "*") {
                        cout << term << " is not a valid term." << endl;
                        return {nullptr, nullptr};;
                    }

                    if (strContainDot(coeffString)) { // Only integer is accepted as coeff according to the spec :( 3.0 is not valid
                        cout << term << " is not a valid term. Coeff can only be integer." << endl;
                        return {nullptr, nullptr};;
                    }
                }

                if (coeff == 0) {
                    cout << "Coeff can't be zero." << endl;
                    return {nullptr, nullptr};;
                }
                if (DEBUG_MODE) cout << " has pow " << pow << " and coeff " << coeff;
                powAndCoeff[pow] += coeff;
                if (pow > degree) degree = pow;
            } else { // Having pow == 1. For instance: -3*x or just x
                int pow = 1;
                double coeff = 0;
                string coeffString = term.substr(0, positionOfX);

                if (coeffString.length() == 0) { // the term is only x
                    coeff = 1.0;
                } else if (coeffString == "-") { // The term is only -x
                    coeff = -1.0;
                } else {
                    stringstream ss(coeffString);
                    int coeffConversionFail = (ss >> coeff).fail();
                    string remainString;
                    getline(ss, remainString);

                    if (coeffConversionFail || remainString != "*") {
                        cout << term << " is not a valid term." << endl;
                        return {nullptr, nullptr};;
                    }

                    if (strContainDot(coeffString)) {
                        cout << term << " is not a valid term. Coeff can only be integer." << endl;
                        return {nullptr, nullptr};;
                    }
                }

                if (coeff == 0) {
                    cout << "Coeff can't be zero." << endl;
                    return {nullptr, nullptr};;
                }

                if (DEBUG_MODE) cout << " has pow " << pow << " and coeff " << coeff;
                powAndCoeff[pow] += coeff;
                if (pow > degree) degree = pow;
            }
        } else {
            double coeff;
            int pow = 0;
            stringstream ss(term);
            if (!(ss >> coeff && ss.eof())) {
                cout << term << " is not a valid term." << endl;
                return {nullptr, nullptr};;
            }

            if (strContainDot(term)) {
                cout << term << " is not a valid term. Coeff can only be integer." << endl;
                return {nullptr, nullptr};;
            }

            if (coeff == 0) {
                cout << "Coeff can't be zero." << endl;
                return {nullptr, nullptr};;
            }

            if (DEBUG_MODE) cout << " has pow " << pow << " and coeff " << coeff;
            powAndCoeff[pow] += coeff;
            if (pow > degree) degree = pow;
        }
        if (DEBUG_MODE) cout << " " << term << endl;
    }

    auto p_ll = new PolynomialLL();
    int size = degree + 1;
    auto *coeffs = new double[size];

    for (int i = 0; i < size; ++i) {
        coeffs[i] = 0;
    }

    for(auto elem : powAndCoeff)
    {
        int pow = elem.first;
        double coeff = elem.second;
        coeffs[degree - pow] = coeff;
        if (coeff != 0) {
            p_ll->add(coeff, pow);
        }
    }

    return {new Polynomial(coeffs, size), p_ll};
}

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        cout << "Please pass 0 or 1 as the program argument" << endl;
        return 1;
    }

    if (argc > 2) {
        cout << "Only 1 argument (0|1) is allowed." << endl;
        return 1;
    }

    const char *option = argv[1];

    int isPrintingName = strcmp(option, "0") == 0;
    int isInNormalMode = strcmp(option, "1") == 0;

    if (!isInNormalMode && !isPrintingName) {
        cout << option << " is not a valid input." << endl;
        return 1;
    }

    if (isPrintingName) {
        cout << "Email goes here" << endl;
        return 0;
    }

    if (isInNormalMode) {
        string a;
        string b;

        cout << "Enter dividend: ";
        cin >> a;
        auto p1 = validateInput(a);

        if (p1.p == nullptr || p1.p_ll == nullptr) {
            return 1;
        }

        cout << "Enter divisor: ";
        cin >> b;
        auto p2 = validateInput(b);

        if (p2.p == nullptr || p2.p_ll == nullptr) {
            return 1;
        }


        /*Method 1*/
        cout << "Method 1: Dynamic Array" << endl;
        auto dividend = *p1.p;
        auto divisor = *p2.p;

        if (divisor.isZero()) {
            cout << "Divisor is not allowed to be 0" << endl;
            return  1;
        }

        if (dividend.isZero()) {
            cout << "Quotient = " << 0 << endl;
            cout << "Remainder = " << 0 << endl;
            return 0;
        }

        if (dividend.getDegree() < divisor.getDegree()) {
            cout << "Quotient = " << 0 << endl;
            cout << "Remainder = " << dividend << endl;
            return 0;
        }

        Polynomial q = Polynomial();
        Polynomial r = Polynomial(dividend);

        while (!r.isZero() && r.getDegree() >= divisor.getDegree()) {
            int tmpSize = r.getDegree() - divisor.getDegree() + 1;
            auto *tmpCoeffs = new double[tmpSize];
            tmpCoeffs[0] = r.getCoeffs()[0] / divisor.getCoeffs()[0];
            for (int i = 1; i < tmpSize; ++i) {
                tmpCoeffs[i] = 0;
            }
            Polynomial t(tmpCoeffs, tmpSize);

            q += t;
            r -= t * divisor;
        }

        cout << "Quotient = " << q << endl;
        cout << "Remainder = " << r << endl;

        /*Method 2*/

        PolynomialLL pasdf = PolynomialLL(*p1.p_ll);
        PolynomialLL pasdf2 = PolynomialLL(*p2.p_ll);
        pasdf += pasdf2;
        cout << pasdf << endl;
    }

    return 0;
}
