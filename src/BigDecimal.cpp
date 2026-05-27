#include "BigDecimal.h"
#include "UnsignedArith.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

std::string normalizeString(std::string x) {
    if (x.empty()) return "0";
    if (x[0] == '+') x = x.substr(1);
    if (x[0] == '-') {
        std::string n = x.substr(1);
        std::size_t p = n.find_first_not_of('0');
        return (p == std::string::npos) ? "0" : "-" + n.substr(p);
    }
    std::size_t p = x.find_first_not_of('0');
    return (p == std::string::npos) ? "0" : x.substr(p);
}

int compareSignedStrings(std::string x, std::string y) {
    x = normalizeString(x); y = normalizeString(y);
    bool xNeg = (!x.empty() && x[0] == '-');
    bool yNeg = (!y.empty() && y[0] == '-');
    std::string absX = xNeg ? x.substr(1) : x;
    std::string absY = yNeg ? y.substr(1) : y;
    if (xNeg && !yNeg) return -1;
    if (!xNeg && yNeg) return 1;
    if (!xNeg && !yNeg) return compareStrings(absX, absY);
    int cmp = compareStrings(absX, absY);
    if (cmp == 1) return -1;
    if (cmp == -1) return 1;
    return 0;
}

std::string addSignedStrings(std::string x, std::string y) {
    x = normalizeString(x); y = normalizeString(y);
    bool xNeg = (!x.empty() && x[0] == '-');
    bool yNeg = (!y.empty() && y[0] == '-');
    std::string absX = xNeg ? x.substr(1) : x;
    std::string absY = yNeg ? y.substr(1) : y;
    if (xNeg == yNeg) {
        std::string res = normalizeString(add(absX, absY));
        if (res == "0") return "0";
        return xNeg ? "-" + res : res;
    }
    int cmp = compareStrings(absX, absY);
    if (cmp == 0) return "0";
    if (cmp > 0) {
        std::string res = normalizeString(sub(absX, absY));
        return (res == "0") ? "0" : (xNeg ? "-" + res : res);
    }
    std::string res = normalizeString(sub(absY, absX));
    return (res == "0") ? "0" : (yNeg ? "-" + res : res);
}

std::string subSignedStrings(std::string x, std::string y) {
    y = normalizeString(y);
    if (y == "0") return normalizeString(x);
    if (!y.empty() && y[0] == '-') y = y.substr(1);
    else y = "-" + y;
    return addSignedStrings(x, y);
}

static bool isValidMantissa(const std::string& s) {
    if (s.empty()) return false;
    int dotCount = 0, digitCount = 0;
    for (std::size_t i = 0; i < s.size(); i++) {
        if (s[i] == '.') {
            dotCount++;
            if (i == 0 || i == s.size() - 1 || dotCount > 1) return false;
        }
        else if (s[i] == '-') {
            if (i != 0) return false;
        }
        else if (s[i] >= '0' && s[i] <= '9') {
            digitCount++;
        }
        else return false;
    }
    return digitCount > 0;
}

static bool isExponentValid(const std::string& s) {
    if (s.empty()) return false;
    int digitCount = 0;
    for (std::size_t i = 0; i < s.size(); i++) {
        if (s[i] == '.') return false;
        if (s[i] == '+' || s[i] == '-') { if (i != 0) return false; }
        else if (s[i] >= '0' && s[i] <= '9') digitCount++;
        else return false;
    }
    return digitCount > 0;
}

static bool validateDecimalString(const std::string& s) {
    if (s.empty()) return false;
    std::size_t ePos = s.find_first_of('e');
    if (ePos != std::string::npos) {
        if (s.find_first_of('e', ePos + 1) != std::string::npos) return false;
        return isValidMantissa(s.substr(0, ePos)) && isExponentValid(s.substr(ePos + 1));
    }
    return isValidMantissa(s);
}

static bool stringToSizeT(const std::string& s, std::size_t& out) {
    if (s.empty() || s[0] == '-') return false;
    out = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
        out = out * 10 + (c - '0');
    }
    return true;
}

static bool appendZeros(std::string& digits, const std::string& delta) {
    std::string d = normalizeString(delta);
    if (d.empty() || d[0] == '-') return false;
    if (d == "0") return true;
    std::size_t count = 0;
    if (!stringToSizeT(d, count)) return false;
    digits.append(count, '0');
    return true;
}

BigDecimal::BigDecimal() : digits("0"), exponent10("0"), isNegative(false) {}

bool BigDecimal::isZero() const { return digits == "0"; }

void BigDecimal::normalize() {
    digits = normalizeString(digits);
    if (digits == "0") { exponent10 = "0"; isNegative = false; return; }
    while (!digits.empty() && digits.back() == '0') {
        digits.pop_back();
        exponent10 = addSignedStrings(exponent10, "1");
    }
    exponent10 = normalizeString(exponent10);
}

bool BigDecimal::fromString(const std::string& s) {
    if (!validateDecimalString(s)) return false;
    std::size_t ePos = s.find_first_of('e');
    std::string mantissa = (ePos != std::string::npos) ? s.substr(0, ePos) : s;
    std::string explicitExponent = (ePos != std::string::npos) ? s.substr(ePos + 1) : "0";

    if (mantissa[0] == '-') { isNegative = true; mantissa = mantissa.substr(1); }
    else isNegative = false;

    std::size_t dotPos = mantissa.find_first_of('.');
    std::string mantissaExponent;
    if (dotPos != std::string::npos) {
        std::string right = mantissa.substr(dotPos + 1);
        digits = mantissa.substr(0, dotPos) + right;
        mantissaExponent = "-" + std::to_string(right.size());
    }
    else {
        digits = mantissa;
        mantissaExponent = "0";
    }

    explicitExponent = normalizeString(explicitExponent);
    exponent10 = addSignedStrings(mantissaExponent, explicitExponent);
    exponent10 = normalizeString(exponent10);
    digits = normalizeString(digits);
    normalize();
    return true;
}

std::string BigDecimal::toString() const {
    if (digits == "0") return "0";
    std::string exp = normalizeString(exponent10);
    std::string sign = isNegative ? "-" : "";
    if (exp == "0") return sign + digits;
    if (exp[0] == '-') {
        std::string absExp = exp.substr(1);
        std::string digSize = std::to_string(digits.size());
        int cmp = compareStrings(absExp, digSize);
        if (cmp == -1) {
            std::size_t shift = 0;
            stringToSizeT(absExp, shift);
            std::string dig = digits;
            dig.insert(dig.size() - shift, ".");
            return sign + dig;
        }
        else if (cmp == 0) {
            return sign + "0." + digits;
        }
        else {
            std::string zerosCount = sub(absExp, digSize);
            std::string dig = "0.";
            appendZeros(dig, zerosCount);
            dig += digits;
            return sign + dig;
        }
    }
    else {
        std::string dig = digits;
        appendZeros(dig, exp);
        return sign + dig;
    }
}

static void alignExponents(BigDecimal& a, BigDecimal& b) {
    int cmp = compareSignedStrings(a.exponent10, b.exponent10);
    if (cmp == 0) return;
    if (cmp == 1) {
        std::string delta = subSignedStrings(a.exponent10, b.exponent10);
        if (appendZeros(a.digits, delta)) a.exponent10 = b.exponent10;
    }
    else {
        std::string delta = subSignedStrings(b.exponent10, a.exponent10);
        if (appendZeros(b.digits, delta)) b.exponent10 = a.exponent10;
    }
}

BigDecimal addBigDecimal(BigDecimal a, BigDecimal b) {
    alignExponents(a, b);
    BigDecimal result;
    result.exponent10 = a.exponent10;
    if (a.isNegative == b.isNegative) {
        result.isNegative = a.isNegative;
        result.digits = add(a.digits, b.digits);
    }
    else {
        int cmp = compareStrings(a.digits, b.digits);
        if (cmp == 1) { result.digits = sub(a.digits, b.digits); result.isNegative = a.isNegative; }
        else if (cmp == -1) { result.digits = sub(b.digits, a.digits); result.isNegative = b.isNegative; }
        else { result.digits = "0"; result.isNegative = false; }
    }
    result.normalize();
    return result;
}

BigDecimal subBigDecimal(BigDecimal a, BigDecimal b) {
    if (!b.isZero()) b.isNegative = !b.isNegative;
    return addBigDecimal(a, b);
}

BigDecimal mulBigDecimal(const BigDecimal& a, const BigDecimal& b) {
    BigDecimal result;
    if (a.isZero() || b.isZero()) return result;
    result.digits = mul(a.digits, b.digits);
    result.exponent10 = addSignedStrings(a.exponent10, b.exponent10);
    result.isNegative = (a.isNegative != b.isNegative);
    result.normalize();
    return result;
}

BigDecimal divBigDecimal(BigDecimal a, BigDecimal b, int precision) {
    BigDecimal result;
    if (b.isZero() || a.isZero()) return result;
    result.isNegative = (a.isNegative != b.isNegative);

    int extraZeros = precision + (int)b.digits.size();
    std::string dividend = a.digits;
    dividend.append(extraZeros, '0');

    auto qr = divmod(dividend, b.digits);

    result.digits = qr.first;
    result.exponent10 = subSignedStrings(
        subSignedStrings(a.exponent10, b.exponent10),
        std::to_string(extraZeros)
    );
    result.normalize();
    return result;
}



BigDecimal sqrtBigDecimal(const BigDecimal& a, int precision) {
    BigDecimal zero;
    if (a.isZero() || a.isNegative) return zero;

    std::string s = a.toString();
    std::string sShort = s.substr(0, std::min((int)s.size(), 15));
    double approx = 1.0;
    try { approx = std::sqrt(std::stod(sShort)); }
    catch (...) {}
    if (approx <= 0) approx = 1.0;

    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.10e", approx);
    BigDecimal x;
    x.fromString(std::string(buf));

    BigDecimal two;
    two.fromString("2");

    for (int i = 0; i < 200; i++) {
        BigDecimal ax = divBigDecimal(a, x, precision + 20);
        BigDecimal xNew = divBigDecimal(addBigDecimal(x, ax), two, precision + 20);

        BigDecimal diff = subBigDecimal(xNew, x);
        diff.isNegative = false;

        BigDecimal eps;
        eps.digits = "1";
        eps.exponent10 = std::to_string(-(precision + 5));

        x = xNew;
        if (diff.isZero() || compareSignedStrings(diff.exponent10, eps.exponent10) == -1)
            break;
    }
    return x;
}
