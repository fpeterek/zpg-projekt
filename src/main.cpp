#include <iostream>

#include "application.hpp"


int main(int argc, const char * argv[]) {

    std::cout << __PRETTY_FUNCTION__ << std::endl;
    Application & app = Application::application;

   app.run();

}


