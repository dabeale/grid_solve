
#include "base/box.hpp"
#include "base/tools.hpp"

using namespace gs;

int test_box(){
    std::cout << "Test box construction" << std::endl;
    int retVal = 0;
    box<3> testBox(dimensions<3>({2,2,2}, 2), 0);
    retVal += assert_bool(testBox[0].at_level(3) == index<3>({0,0,0}, 3), "testBox[0].at_level(3) == index<3>({0,0,0}, 3)");
    retVal += assert_bool(testBox[1].at_level(3) == index<3>({0,0,8}, 3), "testBox[1].at_level(3) == index<3>({0,0,8}, 3)");
    retVal += assert_bool(testBox[2].at_level(3) == index<3>({0,8,0}, 3), "testBox[2].at_level(3) == index<3>({0,8,0}, 3)");
    retVal += assert_bool(testBox[3].at_level(3) == index<3>({0,8,8}, 3), "testBox[3].at_level(3) == index<3>({0,8,8}, 3)");
    retVal += assert_bool(testBox[4].at_level(3) == index<3>({8,0,0}, 3), "testBox[4].at_level(3) == index<3>({8,0,0}, 3)");
    retVal += assert_bool(testBox[5].at_level(3) == index<3>({8,0,8}, 3), "testBox[5].at_level(3) == index<3>({8,0,8}, 3)");
    retVal += assert_bool(testBox[6].at_level(3) == index<3>({8,8,0}, 3), "testBox[6].at_level(3) == index<3>({8,8,0}, 3)");
    retVal += assert_bool(testBox[7].at_level(3) == index<3>({8,8,8}, 3), "testBox[7].at_level(3) == index<3>({8,8,8}, 3)");
    {
        box<3> nbox(testBox);
        nbox.to_neighbour(0, box<3>::POSITIVE);
        retVal += assert_bool(nbox[0].at_level(3) == index<3>({8,0,0}, 3), "nbox[0].at_level(3) == index<3>({8,0,0}, 3)");
        retVal += assert_bool(nbox[1].at_level(3) == index<3>({8,0,8}, 3), "nbox[1].at_level(3) == index<3>({8,0,8}, 3)");
        retVal += assert_bool(nbox[2].at_level(3) == index<3>({8,8,0}, 3), "nbox[2].at_level(3) == index<3>({8,8,0}, 3)");
        retVal += assert_bool(nbox[3].at_level(3) == index<3>({8,8,8}, 3), "nbox[3].at_level(3) == index<3>({8,8,8}, 3)");
        retVal += assert_bool(nbox[4].at_level(3) == index<3>({16,0,0}, 3), "nbox[4].at_level(3) == index<3>({16,0,0}, 3)");
        retVal += assert_bool(nbox[5].at_level(3) == index<3>({16,0,8}, 3), "nbox[5].at_level(3) == index<3>({16,0,8}, 3)");
        retVal += assert_bool(nbox[6].at_level(3) == index<3>({16,8,0}, 3), "nbox[6].at_level(3) == index<3>({16,8,0}, 3)");
        retVal += assert_bool(nbox[7].at_level(3) == index<3>({16,8,8}, 3), "nbox[7].at_level(3) == index<3>({16,8,8}, 3)");
    }
    auto sbox0 = testBox.subbox(0);
    retVal += assert_bool(sbox0[0].at_level(3) == index<3>({0,0,0}, 3), "sbox0[0].at_level(3) == index<3>({0,0,0}, 3)");
    retVal += assert_bool(sbox0[1].at_level(3) == index<3>({0,0,4}, 3), "sbox0[1].at_level(3) == index<3>({0,0,4}, 3)");
    retVal += assert_bool(sbox0[2].at_level(3) == index<3>({0,4,0}, 3), "sbox0[2].at_level(3) == index<3>({0,4,0}, 3)");
    retVal += assert_bool(sbox0[3].at_level(3) == index<3>({0,4,4}, 3), "sbox0[3].at_level(3) == index<3>({0,4,4}, 3)");
    retVal += assert_bool(sbox0[4].at_level(3) == index<3>({4,0,0}, 3), "sbox0[4].at_level(3) == index<3>({4,0,0}, 3)");
    retVal += assert_bool(sbox0[5].at_level(3) == index<3>({4,0,4}, 3), "sbox0[5].at_level(3) == index<3>({4,0,4}, 3)");
    retVal += assert_bool(sbox0[6].at_level(3) == index<3>({4,4,0}, 3), "sbox0[6].at_level(3) == index<3>({4,4,0}, 3)");
    retVal += assert_bool(sbox0[7].at_level(3) == index<3>({4,4,4}, 3), "sbox0[7].at_level(3) == index<3>({4,4,4}, 3)");
    auto sbox2 = testBox.subbox(2);
    retVal += assert_bool(sbox2[0].at_level(3) == index<3>({0,0,8}, 3), "sbox2[0].at_level(3) == index<3>({0,0,8}, 3)");
    retVal += assert_bool(sbox2[1].at_level(3) == index<3>({0,0,12}, 3), "sbox2[1].at_level(3) == index<3>({0,0,12}, 3)");
    retVal += assert_bool(sbox2[2].at_level(3) == index<3>({0,4,8}, 3), "sbox2[2].at_level(3) == index<3>({0,4,8}, 3)");
    retVal += assert_bool(sbox2[3].at_level(3) == index<3>({0,4,12}, 3), "(sbox2[3].at_level(3) == index<3>({0,4,12}, 3)");
    retVal += assert_bool(sbox2[4].at_level(3) == index<3>({4,0,8}, 3), "sbox2[4].at_level(3) == index<3>({4,0,8}, 3)");
    retVal += assert_bool(sbox2[5].at_level(3) == index<3>({4,0,12}, 3), "sbox2[5].at_level(3) == index<3>({4,0,12}, 3)");
    retVal += assert_bool(sbox2[6].at_level(3) == index<3>({4,4,8}, 3), "sbox2[6].at_level(3) == index<3>({4,4,8}, 3)");
    retVal += assert_bool(sbox2[7].at_level(3) == index<3>({4,4,12}, 3), "sbox2[7].at_level(3) == index<3>({4,4,12}, 3)");
    return retVal;
}

int test_box_subpoints(){
    std::cout << "Test box subpoints" << std::endl;
    int retVal = 0;
    box<3> box(dimensions<3>({2,2,2}, 3), 3);
    for (auto sp : box.subpoints()){
        for (uint8_t k = 0; k<3; ++k){
            retVal += assert_bool(sp(k) == 0 || sp(k) == 4 || sp(k) == 8, "sp[k] == 0 || sp[k] == 4 || sp[k] == 8");
        }
        retVal += assert_bool(sp(0) == 4 || sp(1) == 4 || sp(2) == 4, "sp[0] == 4 || sp[1] == 4 || sp[2] == 4");
    }
    return retVal;
}
