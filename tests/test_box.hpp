
#include "box.hpp"
#include "tools.hpp"

using namespace gs;

int test_box(){
    std::cout << "Test box construction" << std::endl;
    int retVal = 0;
    box<3> testBox(3);
    retVal += assert_bool(testBox[0] == std::array<uint32_t, 3>{0,0,0}, "testBox[0] == std::array<uint32_t, 3>{0,0,0}");
    retVal += assert_bool(testBox[1] == std::array<uint32_t, 3>{8,0,0}, "testBox[1] == std::array<uint32_t, 3>{8,0,0}");
    retVal += assert_bool(testBox[2] == std::array<uint32_t, 3>{0,8,0}, "testBox[2] == std::array<uint32_t, 3>{0,8,0}");
    retVal += assert_bool(testBox[3] == std::array<uint32_t, 3>{8,8,0}, "testBox[3] == std::array<uint32_t, 3>{8,8,0}");
    retVal += assert_bool(testBox[4] == std::array<uint32_t, 3>{0,0,8}, "testBox[4] == std::array<uint32_t, 3>{0,0,8}");
    retVal += assert_bool(testBox[5] == std::array<uint32_t, 3>{8,0,8}, "testBox[5] == std::array<uint32_t, 3>{8,0,8}");
    retVal += assert_bool(testBox[6] == std::array<uint32_t, 3>{0,8,8}, "testBox[6] == std::array<uint32_t, 3>{0,8,8}");
    retVal += assert_bool(testBox[7] == std::array<uint32_t, 3>{8,8,8}, "testBox[7] == std::array<uint32_t, 3>{8,8,8}");
    {
        box<3> nbox(testBox);
        nbox.to_neighbour(0, box<3>::POSITIVE);
        retVal += assert_bool(nbox[0] == std::array<uint32_t, 3>{8,0,0}, "nbox[0] == std::array<uint32_t, 3>{8,0,0}");
        retVal += assert_bool(nbox[1] == std::array<uint32_t, 3>{16,0,0}, "nbox[1] == std::array<uint32_t, 3>{16,0,0}");
        retVal += assert_bool(nbox[2] == std::array<uint32_t, 3>{8,8,0}, "nbox[2] == std::array<uint32_t, 3>{8,8,0}");
        retVal += assert_bool(nbox[3] == std::array<uint32_t, 3>{16,8,0}, "nbox[3] == std::array<uint32_t, 3>{16,8,0}");
        retVal += assert_bool(nbox[4] == std::array<uint32_t, 3>{8,0,8}, "nbox[4] == std::array<uint32_t, 3>{8,0,8}");
        retVal += assert_bool(nbox[5] == std::array<uint32_t, 3>{16,0,8}, "nbox[5] == std::array<uint32_t, 3>{16,0,8}");
        retVal += assert_bool(nbox[6] == std::array<uint32_t, 3>{8,8,8}, "nbox[6] == std::array<uint32_t, 3>{8,8,8}");
        retVal += assert_bool(nbox[7] == std::array<uint32_t, 3>{16,8,8}, "nbox[7] == std::array<uint32_t, 3>{16,8,8}");
    }
    auto sbox0 = testBox.subbox(0);
    retVal += assert_bool(sbox0[0] == std::array<uint32_t, 3>{0,0,0}, "sbox0[0] == std::array<uint32_t, 3>{0,0,0}");
    retVal += assert_bool(sbox0[1] == std::array<uint32_t, 3>{4,0,0}, "sbox0[1] == std::array<uint32_t, 3>{4,0,0}");
    retVal += assert_bool(sbox0[2] == std::array<uint32_t, 3>{0,4,0}, "sbox0[2] == std::array<uint32_t, 3>{0,4,0}");
    retVal += assert_bool(sbox0[3] == std::array<uint32_t, 3>{4,4,0}, "sbox0[3] == std::array<uint32_t, 3>{4,4,0}");
    retVal += assert_bool(sbox0[4] == std::array<uint32_t, 3>{0,0,4}, "sbox0[4] == std::array<uint32_t, 3>{0,0,4}");
    retVal += assert_bool(sbox0[5] == std::array<uint32_t, 3>{4,0,4}, "sbox0[5] == std::array<uint32_t, 3>{4,0,4}");
    retVal += assert_bool(sbox0[6] == std::array<uint32_t, 3>{0,4,4}, "sbox0[6] == std::array<uint32_t, 3>{0,4,4}");
    retVal += assert_bool(sbox0[7] == std::array<uint32_t, 3>{4,4,4}, "sbox0[7] == std::array<uint32_t, 3>{4,4,4}");
    auto sbox2 = testBox.subbox(2);
    retVal += assert_bool(sbox2[0] == std::array<uint32_t, 3>{0,4,0}, "sbox2[0] == std::array<uint32_t, 3>{0,4,0}");
    retVal += assert_bool(sbox2[1] == std::array<uint32_t, 3>{4,4,0}, "sbox2[1] == std::array<uint32_t, 3>{4,4,0}");
    retVal += assert_bool(sbox2[2] == std::array<uint32_t, 3>{0,8,0}, "sbox2[2] == std::array<uint32_t, 3>{0,8,0}");
    retVal += assert_bool(sbox2[3] == std::array<uint32_t, 3>{4,8,0}, "sbox2[3] == std::array<uint32_t, 3>{4,8,0}");
    retVal += assert_bool(sbox2[4] == std::array<uint32_t, 3>{0,4,4}, "sbox2[4] == std::array<uint32_t, 3>{0,4,4}");
    retVal += assert_bool(sbox2[5] == std::array<uint32_t, 3>{4,4,4}, "sbox2[5] == std::array<uint32_t, 3>{4,4,4}");
    retVal += assert_bool(sbox2[6] == std::array<uint32_t, 3>{0,8,4}, "sbox2[6] == std::array<uint32_t, 3>{0,8,4}");
    retVal += assert_bool(sbox2[7] == std::array<uint32_t, 3>{4,8,4}, "sbox2[7] == std::array<uint32_t, 3>{4,8,4}");
    return retVal;
}

int test_box_subpoints(){
    std::cout << "Test box subpoints" << std::endl;
    int retVal = 0;
    dimensions<3> dims({2,2,2}, 3);
    box<3> box(3);
    for (auto sp : box.subpoints(dims)){
        for (uint8_t k = 0; k<3; ++k){
            retVal += assert_bool(sp(k) == 0 || sp(k) == 4 || sp(k) == 8, "sp[k] == 0 || sp[k] == 4 || sp[k] == 8");
        }
        retVal += assert_bool(sp(0) == 4 || sp(1) == 4 || sp(2) == 4, "sp[0] == 4 || sp[1] == 4 || sp[2] == 4");
    }
    return retVal;
}