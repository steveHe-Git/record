#ifndef SAMPLE4_H_
#define SAMPLE4_H_

class Counter
{
private:
    /* data */
    int count;
public:
    Counter(/* args */):count(0){}
    ~Counter(){}

    int Increment();

    int Decrement();

    void print() const;
};


#endif