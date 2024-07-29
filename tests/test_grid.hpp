
#include <set>

#include "grid.hpp"

using namespace gs;

int test_grid(){
    std::cout << "Test grid" << std::endl;
    int retVal = 0;
    dimensions<3> dims({2,2,2}, 2);
    grid<3, std::vector<double>> grid(dims);
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 0);
        retVal += assert_bool(nPts == 8, "nPts == 8");
        retVal += assert_bool(allVals.size() == 3, "allVals.size() == 3");
        retVal += assert_bool(allVals.find(0) != allVals.end(), "allVals.find(0) != allVals.end()");
        retVal += assert_bool(allVals.find(4) != allVals.end(), "allVals.find(4) != allVals.end()");
        retVal += assert_bool(allVals.find(8) != allVals.end(), "allVals.find(8) != allVals.end()");
    }
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 1);
        retVal += assert_bool(nPts == 64, "nPts == 64");
        retVal += assert_bool(allVals.size() == 5, "allVals.size() == 5");
        retVal += assert_bool(allVals.find(0) != allVals.end(), "allVals.find(0) != allVals.end()");
        retVal += assert_bool(allVals.find(2) != allVals.end(), "allVals.find(2) != allVals.end()");
        retVal += assert_bool(allVals.find(4) != allVals.end(), "allVals.find(4) != allVals.end()");
        retVal += assert_bool(allVals.find(6) != allVals.end(), "allVals.find(6) != allVals.end()");
        retVal += assert_bool(allVals.find(8) != allVals.end(), "allVals.find(8) != allVals.end()");
    }
    return retVal;
}