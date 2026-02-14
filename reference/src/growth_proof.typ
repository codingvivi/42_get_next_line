= Math notes
== Memory growth factor of $2$
Memory used with growth factor of 2
$
  sum_(i=0)^n 2^i
      &= 2^0 + 2^1 + 2^2 + 2^3 + ... + 2^(n-1) + 2^n \
      &= 1 + 2 + 4 + 8 + ... + 2^n
$

Previous memory step: 
$
  sum_(i=0)^(n-1) 2^i  &= 1 + 2 + 4 + 8 + ... + 2^(n-1)
$

Let $S$ be total history of memory freed
$
  S  &= 1 + 2 + 4 + 8 + ... + 2^(n-1) \
  2S &= 2 + 4 + 8 + 16 + ... + 2^n \ 
$
To get S again:
$
  S &= 2S - S \
    &= (2 + 4 + 8 + ... + 2^(n-1) + 2^n) - (1 + 2 + 4 + 8 + ... + 2^(n-1)) \
    &= 2^n - 1
$

This will always be smaller
than the new memory needed:

$
  2^n + 1 < 2^n 
$

== Optimal growth factor
Let
  - $x$ be generalized growth factor
  - $n$ be realloc iteration 

$
   1, x, x^2, x^3, ..., x^n
$

=== Summing series
Using formula for a geometric series $S_k = (r^k - 1) / (r - 1)$:

$ (x^(n-1) - 1) / (x - 1) >= x^n $

=== Determining equation for limit calc
Let $R_n$ be ratio between reusable blocks and new block:

$
  R_n = (sum_(i=0)^(n-2) x^i) / x^n
$

If $R_n >= 1$
we should be able to reuse previously freed memory.

$
  lim_(n arrow infinity)& R_n >= 1 \
  lim_(n arrow infinity)& (sum_(i=0)^(n-2) x^i) / x^n >= 1 \
  lim_(n arrow infinity)& (((x^(n-1) - 1) / (x - 1))/ x^n) >= 1 \
  lim_(n arrow infinity)& [ x^(n-1) / (x^n (x - 1)) - 1 / (x^n (x - 1)) ] >= 1
$

Quotient rule lets us subtract exponents of the same base:

$
  &x^(n-1) / x ^n  \
    &= x^(n - 1 -n ) \
    &= x^(-1) \
    &= 1/x \
$

Thus
$
  lim_(n arrow infinity)& [ x^(n-1) / (x^n (x - 1)) - 1 / (x^n / (x - 1)) ] >= 1 \
  lim_(n arrow infinity)& [ 1 / ( x (x - 1)) - 1 / (x^n (x - 1)) ] >= 1
$

=== Evaluating limit
$
  &lim_(n arrow infinity) [ 1 / ( x (x - 1)) - 1 / (x^n (x - 1)) ] \
    &= lim_(n arrow infinity) [ 1 / ( x (x - 1)) - 0 ] \
    &= 1 / ( x (x - 1))
$

=== Evaluating inequality 
$
  1 / (x (x - 1)) &>=1 \
  1 &>= x (x - 1) \
  1 &>= x^2 - x \
  0 &>= x^2 - x - 1 \
$

Solvng this quadratic equation yields:

$
  x_1 &= (1 + sqrt(5)) / 2 approx 1.618 \
  x_2 &= (1 - sqrt(5)) / 2 approx -0.618 \ 
$

the first of which is the golden ratio.

=== Compromise operation
$
  n times 1.5 = n + (n/2)
$
which is 
```C
  n + (n >> 1)
```
which should be pretty fast for a computer to do 
