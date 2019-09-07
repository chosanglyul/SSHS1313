#include "Frac.h"

Frac makeFrac(ll n, ll d) {
    if(d == 0LL) return (Frac){0LL, 0LL};
    if(n == 0LL) d = 1LL;
    else if(d < 0LL) n *= -1LL, d *= -1LL;
    ll g = abs(gcd(n, d));
    if (g > 1LL) n /= g, d /= g;
    return (Frac){n, d};
}

Frac negateFrac(Frac f) {
    return makeFrac(-f.num, f.denom);
}

Frac subFrac(Frac lhs, Frac rhs) {
    return makeFrac(lhs.num * rhs.denom - lhs.denom * rhs.num, rhs.denom * lhs.denom);
}

Frac multFrac(Frac lhs, Frac rhs) {
    return makeFrac(lhs.num * rhs.num, lhs.denom * rhs.denom);
}

bool equalFrac(Frac lhs, Frac rhs) {
    return (lhs.num == rhs.num) && (lhs.denom == rhs.denom);
}

bool lessFrac(Frac lhs, Frac rhs) {
    return (lhs.num * rhs.denom) < (rhs.num * lhs.denom);
}

void printFrac(Frac f) {
    printf("%lld", f.num);
    if(f.denom != 1LL) printf("/%lld", f.denom);
}
