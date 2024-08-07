
#include "base/box.hpp"
#include "base/tools.hpp"

using namespace gs;

int test_box(){
    std::cout << "Test box construction" << std::endl;
    int retVal = 0;
    box<3> testBox(dimensions<3>({2,2,2}, 2), 0);
    retVal += ASSERT_BOOL(testBox[0].at_level(3) == index<3>({0,0,0}, 3));
    retVal += ASSERT_BOOL(testBox[1].at_level(3) == index<3>({0,0,8}, 3));
    retVal += ASSERT_BOOL(testBox[2].at_level(3) == index<3>({0,8,0}, 3));
    retVal += ASSERT_BOOL(testBox[3].at_level(3) == index<3>({0,8,8}, 3));
    retVal += ASSERT_BOOL(testBox[4].at_level(3) == index<3>({8,0,0}, 3));
    retVal += ASSERT_BOOL(testBox[5].at_level(3) == index<3>({8,0,8}, 3));
    retVal += ASSERT_BOOL(testBox[6].at_level(3) == index<3>({8,8,0}, 3));
    retVal += ASSERT_BOOL(testBox[7].at_level(3) == index<3>({8,8,8}, 3));
    {
        box<3> nbox(testBox);
        nbox.to_neighbour(0, box<3>::POSITIVE);
        retVal += ASSERT_BOOL(nbox[0].at_level(3) == index<3>({8,0,0}, 3));
        retVal += ASSERT_BOOL(nbox[1].at_level(3) == index<3>({8,0,8}, 3));
        retVal += ASSERT_BOOL(nbox[2].at_level(3) == index<3>({8,8,0}, 3));
        retVal += ASSERT_BOOL(nbox[3].at_level(3) == index<3>({8,8,8}, 3));
        retVal += ASSERT_BOOL(nbox[4].at_level(3) == index<3>({16,0,0}, 3));
        retVal += ASSERT_BOOL(nbox[5].at_level(3) == index<3>({16,0,8}, 3));
        retVal += ASSERT_BOOL(nbox[6].at_level(3) == index<3>({16,8,0}, 3));
        retVal += ASSERT_BOOL(nbox[7].at_level(3) == index<3>({16,8,8}, 3));
    }
    auto sbox0 = testBox.subbox(0);
    retVal += ASSERT_BOOL(sbox0[0].at_level(3) == index<3>({0,0,0}, 3));
    retVal += ASSERT_BOOL(sbox0[1].at_level(3) == index<3>({0,0,4}, 3));
    retVal += ASSERT_BOOL(sbox0[2].at_level(3) == index<3>({0,4,0}, 3));
    retVal += ASSERT_BOOL(sbox0[3].at_level(3) == index<3>({0,4,4}, 3));
    retVal += ASSERT_BOOL(sbox0[4].at_level(3) == index<3>({4,0,0}, 3));
    retVal += ASSERT_BOOL(sbox0[5].at_level(3) == index<3>({4,0,4}, 3));
    retVal += ASSERT_BOOL(sbox0[6].at_level(3) == index<3>({4,4,0}, 3));
    retVal += ASSERT_BOOL(sbox0[7].at_level(3) == index<3>({4,4,4}, 3));
    auto sbox2 = testBox.subbox(2);
    retVal += ASSERT_BOOL(sbox2[0].at_level(3) == index<3>({0,0,8}, 3));
    retVal += ASSERT_BOOL(sbox2[1].at_level(3) == index<3>({0,0,12}, 3));
    retVal += ASSERT_BOOL(sbox2[2].at_level(3) == index<3>({0,4,8}, 3));
    retVal += ASSERT_BOOL(sbox2[3].at_level(3) == index<3>({0,4,12}, 3));
    retVal += ASSERT_BOOL(sbox2[4].at_level(3) == index<3>({4,0,8}, 3));
    retVal += ASSERT_BOOL(sbox2[5].at_level(3) == index<3>({4,0,12}, 3));
    retVal += ASSERT_BOOL(sbox2[6].at_level(3) == index<3>({4,4,8}, 3));
    retVal += ASSERT_BOOL(sbox2[7].at_level(3) == index<3>({4,4,12}, 3));
    return retVal;
}

int test_box_subpoints(){
    std::cout << "Test box subpoints" << std::endl;
    int retVal = 0;
    box<3> box(dimensions<3>({2,2,2}, 3), 3);
    for (auto sp : box.subpoints()){
        for (uint8_t k = 0; k<3; ++k){
            retVal += ASSERT_BOOL(sp(k) == 0 || sp(k) == 4 || sp(k) == 8);
        }
        retVal += ASSERT_BOOL(sp(0) == 4 || sp(1) == 4 || sp(2) == 4);
    }
    return retVal;
}
