#include <random>
//#include "display.cpp"


int randomGen(){

    // set seed
    std::random_device rd;
    std::mt19937 gen(rd());

    const int min = 1;
    const int max = 100;

    std::uniform_int_distribution<> distrib(min, max);

    int randVal = distrib(gen);

    return randVal; 
}