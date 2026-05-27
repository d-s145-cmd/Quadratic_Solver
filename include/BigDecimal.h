#pragma once
#include <string>

struct BigDecimal {
    std::string digits;
    std::string exponent10;
    bool isNegative;

    BigDecimal();
    bool isZero() const;
    void normalize();
    bool fromString(const std::string& s);
    std::string toString() const;
};

std::string normalizeString(std::string x);
int compareSignedStrings(std::string x, std::string y);
std::string addSignedStrings(std::string x, std::string y);
std::string subSignedStrings(std::string x, std::string y);

BigDecimal addBigDecimal(BigDecimal a, BigDecimal b);
BigDecimal subBigDecimal(BigDecimal a, BigDecimal b);
BigDecimal mulBigDecimal(const BigDecimal& a, const BigDecimal& b);
BigDecimal divBigDecimal(BigDecimal a, BigDecimal b, int precision = 100);
BigDecimal sqrtBigDecimal(const BigDecimal& a, int precision = 60);
