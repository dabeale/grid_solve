
#include "test_index.hpp"
#include "test_box.hpp"

int main(int args, char* argv[]){
    std::cout << argv[0] << " test suite" << std::endl;
    int error = 0;
    error += test_construction();
    error += test_neighbours();
    error += test_sub2ind();
    error += test_box();
    error += test_box_subpoints();
    if (error){
        std::cout << error << " tests failed" << std::endl;
    }
    else {
        std::cout << "All tests passed" << std::endl;
    }
}