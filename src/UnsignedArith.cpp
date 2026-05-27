#include "UnsignedArith.h"
#include <algorithm>
#include <vector>

int compareStrings(std::string x, std::string y) {
    std::size_t posX = 0, posY = 0;
    while (posX < x.size() && x[posX] == '0') posX++;
    while (posY < y.size() && y[posY] == '0') posY++;
    x = (posX == x.size()) ? "0" : x.substr(posX);
    y = (posY == y.size()) ? "0" : y.substr(posY);
    if (x.size() > y.size()) return 1;
    if (x.size() < y.size()) return -1;
    for (std::size_t i = 0; i < x.size(); i++) {
        if (x[i] > y[i]) return 1;
        if (x[i] < y[i]) return -1;
    }
    return 0;
}

std::string add(std::string x, std::string y) {
    if (y.size() > x.size()) std::swap(x, y);
    std::string result;
    int carry = 0;
    std::size_t n1 = x.size(), n2 = y.size();
    for (std::size_t i = 0; i < n1; i++) {
        int digitX = x[n1 - 1 - i] - '0';
        int digitY = (i < n2) ? y[n2 - 1 - i] - '0' : 0;
        int sum = digitX + digitY + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
    }
    if (carry > 0) result.push_back(carry + '0');
    std::reverse(result.begin(), result.end());
    std::size_t pos = result.find_first_not_of('0');
    return (pos == std::string::npos) ? "0" : result.substr(pos);
}

std::string sub(std::string x, std::string y) {
    if (compareStrings(x, y) == -1) std::swap(x, y);
    std::string result;
    int borrow = 0;
    std::size_t n1 = x.size(), n2 = y.size();
    for (std::size_t i = 0; i < n1; i++) {
        int digitX = x[n1 - 1 - i] - '0';
        int digitY = (i < n2) ? y[n2 - 1 - i] - '0' : 0;
        int s;
        if (digitX - borrow < digitY) { s = digitX - borrow + 10 - digitY; borrow = 1; }
        else { s = digitX - borrow - digitY;       borrow = 0; }
        result.push_back(s + '0');
    }
    std::reverse(result.begin(), result.end());
    std::size_t pos = result.find_first_not_of('0');
    return (pos == std::string::npos) ? "0" : result.substr(pos);
}

std::string mul(const std::string& x, const std::string& y) {
    if (x == "0" || y == "0") return "0";
    std::size_t n1 = x.size(), n2 = y.size();
    std::vector<int> res(n1 + n2, 0);
    for (int i = (int)n1 - 1; i >= 0; i--) {
        for (int j = (int)n2 - 1; j >= 0; j--) {
            int p = (x[i] - '0') * (y[j] - '0');
            int sum = p + res[i + j + 1];
            res[i + j + 1] = sum % 10;
            res[i + j] += sum / 10;
        }
    }
    std::string result;
    for (int d : res)
        if (!(result.empty() && d == 0)) result.push_back(d + '0');
    return result.empty() ? "0" : result;
}

std::pair<std::string, std::string> divmod(const std::string& x, const std::string& y) {
    if (compareStrings(x, y) == -1) return { "0", x };
    std::string quotient;
    std::string current = "0";
    for (std::size_t i = 0; i < x.size(); i++) {
        if (current == "0") current = std::string(1, x[i]);
        else                current.push_back(x[i]);

        int lo = 0, hi = 9, digit = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (compareStrings(mul(y, std::to_string(mid)), current) <= 0) { digit = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        quotient.push_back(digit + '0');
        current = sub(current, mul(y, std::to_string(digit)));
    }
    std::size_t pos = quotient.find_first_not_of('0');
    quotient = (pos == std::string::npos) ? "0" : quotient.substr(pos);
    return { quotient, current };
}
