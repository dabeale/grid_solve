
#include <set>

#include "base/grid.hpp"

using namespace gs;

int test_grid(){
    std::cout << "Test grid" << std::endl;
    int retVal = 0;
    dimensions<3> dims({2,2,2}, 2);
    grid<3, double, double> grid(dims);
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 0);
        retVal += assert_bool(nPts == 8, "nPts == 8");
        //
        // Note that there are 8 boxes in level 0, each of which have
        // 4 corners. This means that each dimension contains 3 points
        // defining the boundry of the 2 segments, and that we should
        // expect to see 3 values (including the boundary points).
        //
        retVal += assert_bool(allVals.size() == 3, "allVals.size() == 3");
        retVal += assert_bool(allVals.find(0) != allVals.end(), "allVals.find(0) != allVals.end()");
        retVal += assert_bool(allVals.find(4) != allVals.end(), "allVals.find(4) != allVals.end()");
        retVal += assert_bool(allVals.find(8) != allVals.end(), "allVals.find(8) != allVals.end()");
    }
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in, double&){
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
    {
        uint32_t nPts = 0;
        std::set<uint32_t> allVals;
        grid.iterate([&](box<3>& in, double&){
            for (auto corner : in){
                for (auto x : corner.at_level(2)){
                    allVals.insert(x);
                }
            }
            ++nPts;
        }, 2);
        retVal += assert_bool(nPts == 512, "nPts == 512");
        retVal += assert_bool(allVals.size() == 9, "allVals.size() == 9");
        for( int i=0; i<=8; ++i){
            retVal += assert_bool(allVals.find(i) != allVals.end(), "allVals.find(i) != allVals.end()");
        }
    }
    return retVal;
}