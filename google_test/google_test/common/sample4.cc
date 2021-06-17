#include "sample4.h"
#include "gtest/gtest.h"

int Counter::Increment() {
    return count++;
}

int Counter::Decrement() {
    if (count == 0) {
        return count;
    } else {
        return count--;
    }
}

void Counter::print() const {
    printf("count: %d", count);
}