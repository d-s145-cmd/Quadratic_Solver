#include "QuadSolver.h"

static void fixNegativeZero(BigDecimal& x) {
    if (x.isZero()) x.isNegative = false;
}

QuadResult QuadSolver::solveLinear(const BigDecimal& b,
    const BigDecimal& c) const {
    QuadResult res;
    res.equationType = QuadResult::EquationType::LINEAR;
    if (b.isZero()) {
        res.status = c.isZero() ? QuadResult::Status::INF
            : QuadResult::Status::NO_SOLUTION;
        res.rootCount = 0;
        return res;
    }
    BigDecimal x = divBigDecimal(c, b, PRECISION);
    x.isNegative = !x.isNegative;
    fixNegativeZero(x);

    res.status = QuadResult::Status::OK;
    res.rootCount = 1;
    res.root1 = x.toString();
    return res;
}

QuadResult QuadSolver::solve(const std::string& sa,
    const std::string& sb,
    const std::string& sc) const {
    QuadResult res;
    BigDecimal a, b, c;

    if (!a.fromString(sa) || !b.fromString(sb) || !c.fromString(sc)) {
        res.status = QuadResult::Status::WRONG;
        res.rootCount = 0;
        return res;
    }

    if (a.isZero()) return solveLinear(b, c);

    res.equationType = QuadResult::EquationType::QUADRATIC;

    BigDecimal four;
    four.fromString("4");
    BigDecimal D = subBigDecimal(mulBigDecimal(b, b),
        mulBigDecimal(four, mulBigDecimal(a, c)));

    int dSign = D.isZero() ? 0 : (D.isNegative ? -1 : 1);

    BigDecimal two;
    two.fromString("2");
    BigDecimal twoA = mulBigDecimal(two, a);

    if (dSign < 0) {
       
        res.status = QuadResult::Status::NO_SOLUTION;
        res.rootCount = 0;
        return res;
    }

    if (dSign == 0) {
        BigDecimal x = divBigDecimal(b, twoA, PRECISION);
        x.isNegative = !x.isNegative;
        fixNegativeZero(x);

        res.status = QuadResult::Status::OK;
        res.rootCount = 1;
        res.root1 = x.toString();
        return res;
    }

   
    BigDecimal sqrtD = sqrtBigDecimal(D, PRECISION);

    BigDecimal negB = b;
    if (!negB.isZero()) negB.isNegative = !negB.isNegative;

    BigDecimal x1 = divBigDecimal(addBigDecimal(negB, sqrtD), twoA, PRECISION);
    BigDecimal x2 = divBigDecimal(subBigDecimal(negB, sqrtD), twoA, PRECISION);
    fixNegativeZero(x1);
    fixNegativeZero(x2);

    
    {
        BigDecimal diff = subBigDecimal(x1, x2);
        if (!diff.isZero() && !diff.isNegative) {
            std::swap(x1, x2);
        }
    }

    res.status = QuadResult::Status::OK;
    res.rootCount = 2;
    res.root1 = x1.toString();
    res.root2 = x2.toString();
    return res;
}
