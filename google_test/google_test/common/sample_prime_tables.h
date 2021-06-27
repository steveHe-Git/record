#ifndef SAMPLE_PRIME_TABLES_H_
#define SAMPLE_PRIME_TABLES_H_

#include <algorithm>

class PrimeTable
{
private:
    /* data */
public:
    PrimeTable(/* args */){};
    virtual ~PrimeTable(){};
    virtual bool IsPrime(int n) const = 0;
    virtual int GetNextPrime(int p) const = 0; 
};

class OnTheFlyPrimeTable:public PrimeTable
{
private:
    /* data */
public:
    OnTheFlyPrimeTable(/* args */){};
    ~OnTheFlyPrimeTable(){};
    bool IsPrime(int n) const override {
        if (n <= 1) return false;
        for (int i = 2; i*i <= n; i++) {
            if (n%i == 0) return false;
        }

        return true;
    }

    int GetNextPrime(int p) const override {
        if (p<0) return -1;
        for (int n = p + 1; ;n++) {
            if (IsPrime(n)) {
                return n;
            }
        }
    }
};

class PreCalculatedPrimeTable:public PrimeTable
{
public:
    explicit PreCalculatedPrimeTable(int max):
        is_prime_size(max+1), is_prime(new bool[max + 1]){
        CalculatePrimeUpTo(max);
    }

    ~PreCalculatedPrimeTable() override{
        delete[] is_prime;
    }

    bool IsPrime(int n) const override {
        return n>=0 && is_prime[n] && n< is_prime_size;
    }

    int GetNextPrime(int p) const override {
        for (int n = p+1;n < is_prime_size;n++) {
            if (is_prime[n]) {
                return n;
            }
        }
        return -1;
    }

private:
    /* data */
    void CalculatePrimeUpTo(int max) {
        ::std::fill(is_prime, is_prime + is_prime_size, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i*i <= max; i += i%2 + 1) {
            if (!is_prime[i]) continue;

            for (int j = i*i; j <= max; j+=i) {
                is_prime[j] = false;
            }
        }
    }

    const int is_prime_size;
    bool* const is_prime;
    void operator=(const PreCalculatedPrimeTable& rhs);
};






#endif