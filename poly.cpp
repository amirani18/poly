#include "poly.h"

// constructor
polynomial::polynomial() : terms{{0, 0}} {}

// constructor from iterators
// polynomial::polynomial(Iter begin, Iter end) {
//     for (auto it = begin; it != end; ++it) {
//         if (it->second != 0) {  // ignore 0 coeffs
//             terms[it->first] += it->second;
//         }    
//     }
// }

// copy constructor
polynomial::polynomial(const polynomial &other) : terms(other.terms) {}

// assignment operator
polynomial &polynomial::operator=(const polynomial &other) {
    if (this != &other) {
        terms = other.terms;
    }
    return *this;
}

// function for printing
void polynomial::print() const {
    bool first = true;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        if (first) first = false;
        else std::cout << " + ";
        std::cout << it->second << "x^" << it->first;        
    }
    if (first) std::cout << "0";   // empty polynomial
    std::cout << std::endl;
}


// -----------------------------------------------------------------------------------------------------
// 
//              helpers
//
// -----------------------------------------------------------------------------------------------------
// helper to simplify polynomial coeff
// void polynomial::simplify() {
//     for (auto it = terms.begin(); it != terms.end();) {
//         if (it -> second == 0) {
//             it = terms.erase(it);       // remove zero terms
//         }
//         else {
//             ++it;
//         }
//     }
//     if (terms.empty()) {
//         terms[0] = 0;
//     }
// }

// helper for modulus: subtract two polynomials
polynomial polynomial::operator-(const polynomial &rhs) const {
    polynomial result = *this;
    for (const auto &[p, c] : rhs.terms) {
        result.terms[p] -= c;
        if (result.terms[p] == 0) result.terms.erase(p);        // remove zero coeff
    }
    // result.simplify();
    return result;
}

// -----------------------------------------------------------------------------------------------------
//  * Addition (+) should support
//  * 1. polynomial + polynomial
//  * 2. polynomial + int
//  * 3. int + polynomial
// -----------------------------------------------------------------------------------------------------
polynomial polynomial::operator+(const polynomial &rhs) const {
    polynomial result = *this;
    for (const auto &[p, c] : rhs.terms) {
        result.terms[p] += c;
        if (result.terms[p] == 0) result.terms.erase(p);        // remove zero coeff
    }
    // result.simplify();
    return result;
}

polynomial polynomial::operator+(const coeff scalar) const {
    polynomial result = *this;
    result.terms[0] += scalar;
    // result.simplify;
    return result;
}

polynomial operator+(const coeff scalar, const polynomial &poly) {
    return poly + scalar;
}

// -----------------------------------------------------------------------------------------------------
//  * Multiplication (*) should support
//  * 1. polynomial * polynomial
//  * 2. polynomial * int
//  * 3. int * polynomial
// -----------------------------------------------------------------------------------------------------
polynomial polynomial::operator*(const polynomial &rhs) const {
    polynomial result;
    for (const auto &[p1, c1] : terms) {
        for (const auto &[p2, c2] : rhs.terms) {
            result.terms[p1 + p2] += c1 * c2;
        }
    }
    // result.simplify;
    return result;
}

polynomial polynomial::operator*(const coeff scalar) const{
    polynomial result;
    for (const auto &[p, c] : terms) {
        if (scalar != 0) result.terms[p] = c * scalar;
    }
    // result.simplify;
    return result;
}

polynomial operator*(const coeff scalar, const polynomial &rhs) {
    return rhs * scalar;
}

// -----------------------------------------------------------------------------------------------------
//  * Modulo (%) should support
//  * 1. polynomial % polynomial
// -----------------------------------------------------------------------------------------------------
polynomial polynomial::operator%(const polynomial &rhs) const {
    if (rhs.terms.empty() || (rhs.terms.size() == 1 && rhs.terms.begin()->second == 0)) {
        return polynomial();    // return zero polynomial
    }

    polynomial dividend = *this;
    polynomial divisor = rhs;
    polynomial result;

    size_t divisor_degree = divisor.find_degree_of();
    
    while (!dividend.terms.empty() && dividend.find_degree_of() >= divisor_degree) {
        size_t dividend_degree = dividend.find_degree_of();
        coeff leading_coeff = dividend.terms.rbegin()->second / divisor.terms.rbegin()->second;
        // size_t deg_diff = dividend.terms.rbegin()->first - divisor.terms.rbegin()->first;
        // int coeff_div = dividend.terms.rbegin()->second / divisor.terms.rbegin()->second;

        polynomial temp;
        temp.terms[dividend_degree - divisor_degree] = leading_coeff;

        dividend = dividend - (temp * divisor);
    }
    
    return dividend;    // return remainder after division
}

// helper to find degree of polynomial coeff
size_t polynomial::find_degree_of() {
    if (terms.empty()) return 0;
    return terms.rbegin()->first;
}

// canonical form
std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> result;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        if (it->second != 0) {
            result.emplace_back(it->first, it->second);
        }
    }
    if (result.empty()) result.emplace_back(0, 0);  // for zero polynomial
    return result;
}


