
#include "box.hpp"
#include "tools.hpp"

using namespace gs;

int test_box(){
    std::cout << "Test box construction" << std::endl;
    box<3> box(3);
    assert_bool(box[0] == std::array<uint32_t, 3>{0,0,0}, "box[0] == std::array<uint32_t, 3>{0,0,0}");
    assert_bool(box[1] == std::array<uint32_t, 3>{8,0,0}, "box[1] == std::array<uint32_t, 3>{8,0,0}");
    assert_bool(box[2] == std::array<uint32_t, 3>{0,8,0}, "box[2] == std::array<uint32_t, 3>{0,8,0}");
    assert_bool(box[3] == std::array<uint32_t, 3>{8,8,0}, "box[3] == std::array<uint32_t, 3>{8,8,0}");
    assert_bool(box[4] == std::array<uint32_t, 3>{0,0,8}, "box[4] == std::array<uint32_t, 3>{0,0,8}");
    assert_bool(box[5] == std::array<uint32_t, 3>{8,0,8}, "box[5] == std::array<uint32_t, 3>{8,0,8}");
    assert_bool(box[6] == std::array<uint32_t, 3>{0,8,8}, "box[6] == std::array<uint32_t, 3>{0,8,8}");
    assert_bool(box[7] == std::array<uint32_t, 3>{8,8,8}, "box[7] == std::array<uint32_t, 3>{8,8,8}");
    auto nbox = box.neighbour(0, index<3>::POSITIVE);
    assert_bool(nbox[0] == std::array<uint32_t, 3>{8,0,0}, "nbox[0] == std::array<uint32_t, 3>{8,0,0}");
    assert_bool(nbox[1] == std::array<uint32_t, 3>{16,0,0}, "nbox[1] == std::array<uint32_t, 3>{16,0,0}");
    assert_bool(nbox[2] == std::array<uint32_t, 3>{8,8,0}, "nbox[2] == std::array<uint32_t, 3>{8,8,0}");
    assert_bool(nbox[3] == std::array<uint32_t, 3>{16,8,0}, "nbox[3] == std::array<uint32_t, 3>{16,8,0}");
    assert_bool(nbox[4] == std::array<uint32_t, 3>{8,0,8}, "nbox[4] == std::array<uint32_t, 3>{8,0,8}");
    assert_bool(nbox[5] == std::array<uint32_t, 3>{16,0,8}, "nbox[5] == std::array<uint32_t, 3>{16,0,8}");
    assert_bool(nbox[6] == std::array<uint32_t, 3>{8,8,8}, "nbox[6] == std::array<uint32_t, 3>{8,8,8}");
    assert_bool(nbox[7] == std::array<uint32_t, 3>{16,8,8}, "nbox[7] == std::array<uint32_t, 3>{16,8,8}");
    auto sbox0 = box.subbox(0);
    assert_bool(sbox0[0] == std::array<uint32_t, 3>{0,0,0}, "sbox0[0] == std::array<uint32_t, 3>{0,0,0}");
    assert_bool(sbox0[1] == std::array<uint32_t, 3>{4,0,0}, "sbox0[1] == std::array<uint32_t, 3>{4,0,0}");
    assert_bool(sbox0[2] == std::array<uint32_t, 3>{0,4,0}, "sbox0[2] == std::array<uint32_t, 3>{0,4,0}");
    assert_bool(sbox0[3] == std::array<uint32_t, 3>{4,4,0}, "sbox0[3] == std::array<uint32_t, 3>{4,4,0}");
    assert_bool(sbox0[4] == std::array<uint32_t, 3>{0,0,4}, "sbox0[4] == std::array<uint32_t, 3>{0,0,4}");
    assert_bool(sbox0[5] == std::array<uint32_t, 3>{4,0,4}, "sbox0[5] == std::array<uint32_t, 3>{4,0,4}");
    assert_bool(sbox0[6] == std::array<uint32_t, 3>{0,4,4}, "sbox0[6] == std::array<uint32_t, 3>{0,4,4}");
    assert_bool(sbox0[7] == std::array<uint32_t, 3>{4,4,4}, "sbox0[7] == std::array<uint32_t, 3>{4,4,4}");
    auto sbox2 = box.subbox(2);
    assert_bool(sbox2[0] == std::array<uint32_t, 3>{0,4,0}, "sbox2[0] == std::array<uint32_t, 3>{0,4,0}");
    assert_bool(sbox2[1] == std::array<uint32_t, 3>{4,4,0}, "sbox2[1] == std::array<uint32_t, 3>{4,4,0}");
    assert_bool(sbox2[2] == std::array<uint32_t, 3>{0,8,0}, "sbox2[2] == std::array<uint32_t, 3>{0,8,0}");
    assert_bool(sbox2[3] == std::array<uint32_t, 3>{4,8,0}, "sbox2[3] == std::array<uint32_t, 3>{4,8,0}");
    assert_bool(sbox2[4] == std::array<uint32_t, 3>{0,4,4}, "sbox2[4] == std::array<uint32_t, 3>{0,4,4}");
    assert_bool(sbox2[5] == std::array<uint32_t, 3>{4,4,4}, "sbox2[5] == std::array<uint32_t, 3>{4,4,4}");
    assert_bool(sbox2[6] == std::array<uint32_t, 3>{0,8,4}, "sbox2[6] == std::array<uint32_t, 3>{0,8,4}");
    assert_bool(sbox2[7] == std::array<uint32_t, 3>{4,8,4}, "sbox2[7] == std::array<uint32_t, 3>{4,8,4}");
    return 0;
}

int test_box_subpoints(){
    std::cout << "Test box subpoints" << std::endl;
    box<3> box(3);
    for (auto sp : box.subpoints()){
        for (uint8_t k = 0; k<3; ++k){
            assert_bool(sp[k] == 0 || sp[k] == 4 || sp[k] == 8, "sp[k] == 0 || sp[k] == 4 || sp[k] == 8");
        }
        assert_bool(sp[0] == 4 || sp[1] == 4 || sp[2] == 4, "sp[0] == 4 || sp[1] == 4 || sp[2] == 4");
    }
    return 0;
}