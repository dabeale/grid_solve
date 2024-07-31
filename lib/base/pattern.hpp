
#ifndef _GS_PATTERN_
#define _GS_PATTERN_

#include <vector>

namespace gs {
/**
 * A means to traverse the grid.
 */
enum PatternComponent {
    FINE_TO_COARSE,
    COARSE_TO_FINE
};

/**
 * A V traversal pattern.
 */
std::vector<PatternComponent> v_pattern(){
    return {
        COARSE_TO_FINE,
        FINE_TO_COARSE
    };
}
/**
 * A /\ traversal pattern.
 */
std::vector<PatternComponent> inverse_v_pattern(){
    return {
        FINE_TO_COARSE,
        COARSE_TO_FINE
    };
}
}

#endif