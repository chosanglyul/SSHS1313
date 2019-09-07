#ifndef FRAC_H_INCLUDED
#define FRAC_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef long long ll;
typedef struct tFrac { ll num, denom; }Frac;
ll gcd(ll a, ll b);
ll binomial(int n, int k);
Frac bernoulli(int n);
void faulhaber(int p);
Frac makeFrac(ll n, ll d);
Frac negateFrac(Frac f);
Frac subFrac(Frac lhs, Frac rhs);
Frac multFrac(Frac lhs, Frac rhs);
bool equalFrac(Frac lhs, Frac rhs);
bool lessFrac(Frac lhs, Frac rhs);
void printFrac(Frac f);
#endif // FRAC_H_INCLUDED
