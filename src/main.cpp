#include <iostream>
#include <string>
#include "QuadSolver.h"

static void printResult(const QuadResult& res) {
    switch (res.status) {
    case QuadResult::Status::OK:
        std::cout << "OK\n";
        std::cout << res.rootCount << "\n";
        std::cout << res.root1 << "\n";
        if (res.rootCount == 2) {
            std::cout << res.root2 << "\n";
        }
        break;
    case QuadResult::Status::INF:
        std::cout << "INF\n";
        break;
    case QuadResult::Status::WRONG:
        std::cout << "WRONG\n";
        break;
    case QuadResult::Status::NO_SOLUTION:
        std::cout << "NO SOLUTION\n";
        break;
    }
}

int main() {
    QuadSolver solver;
    std::string a, b, c;
    int testNum = 0;

    while (true) {
        std::cout << "\n=== Test " << (++testNum) << " ===\n";
        std::cout << "Enter a b c (or 'exit' to quit): ";

        if (!(std::cin >> a)) {
            break;
        }

        if (a == "exit" || a == "quit" || a == "q") {
            break;
        }

        if (!(std::cin >> b >> c)) {
            std::cerr << "Error: enter 3 coefficients\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

   
        std::cout << "\n[INPUT] a=" << a << " b=" << b << " c=" << c << "\n";
        std::cout << "[OUTPUT]\n";

        QuadResult res = solver.solve(a, b, c);
        printResult(res);
    }

    std::cout << "\nTotal tests run: " << (testNum - 1) << "\n";
    std::cout << "Goodbye!\n";
    return 0;
}
