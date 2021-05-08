#include <iostream>
#include <memory>
#include <sstream>

int main () {
	std::istringstream stream("You are the best");
	std::string str, text;
    char a = ' ';
    getline(stream, text, a);
    std::cout <<"text: "<< text <<"\n";

	while(stream >> str) {
		std::cout <<"str: "<< str<<"\n";
        std::cout <<"stream: "<< stream.str() <<"\n";
        char a = ' ';
        getline(stream, text, a);
        std::cout <<"text: "<< text <<"\n";
    }
	return 0;
}