#include <iostream>
#include <stdexcept>

#include "App.hpp"

int main(int argc, char* argv[]) {

    try {
        App app(argc, argv);
        app.run();
    } catch (const std::exception& e){
        std::cerr << "An error occured: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}