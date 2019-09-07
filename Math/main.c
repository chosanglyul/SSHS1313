///Thanks To : https://rosettacode.org/wiki/Faulhaber's_formula

#include "Frac.h"
ll nCr[1010][1010];
ll binomial(int n, int k) {
    if(n <= 0 || k < 0 || n < k) return 0;
    if(k == 0 || n == k) return 1;
    if(nCr[n][k]) return nCr[n][k];
    return nCr[n][k] = binomial(n-1, k-1)+binomial(n-1, k);
}
ll gcd(ll a, ll b) {
    while(b) {
        ll temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

Frac bernoulli(int n) {
    Frac a[1010];
    if (n < 0) return (Frac){0LL, 0LL};
    for (int m = 0; m <= n; ++m) {
        a[m] = makeFrac(1LL, (ll)m+1LL);
        for (int j = m; j >= 1; --j)
            a[j-1] = multFrac(subFrac(a[j-1], a[j]), makeFrac(j, 1));
    }
    if (n != 1) return a[0];
    return negateFrac(a[0]);
}

void faulhaber(int p) {
    printf("%d : ", p);
    int sign = -1;
    for (int j = 0; j <= p; ++j) {
        sign = -1 * sign;
        Frac coeff = multFrac(
                              multFrac(
                                       multFrac(makeFrac(1, p + 1), makeFrac(sign, 1)),
                                       makeFrac(binomial(p + 1, j), 1)),
                              bernoulli(j));
        if (equalFrac(coeff, makeFrac(0, 1))) continue;
        if (j == 0) {
            if (!equalFrac(coeff, makeFrac(1, 1))) {
                if (equalFrac(coeff, makeFrac(-1, 1))) printf("-");
                else printFrac(coeff);
            }
        } else {
            if (equalFrac(coeff, makeFrac(1, 1))) printf(" + ");
            else if (equalFrac(coeff, makeFrac(-1, 1))) printf(" - ");
            else if (lessFrac(makeFrac(0, 1), coeff)) {
                printf(" + ");
                printFrac(coeff);
            } else {
                printf(" - ");
                printFrac(negateFrac(coeff));
            }
        }
        int pwr = p + 1 - j;
        if (pwr > 1) printf("n^%d", pwr);
        else printf("n");
    }
    printf("\n");
}

int main() {
    int n; scanf("%d", &n);
    for (int i = 0; i <= n; ++i) faulhaber(i);
    return 0;
}
