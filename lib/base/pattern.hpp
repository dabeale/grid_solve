// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_PATTERN_HPP_
#define LIB_BASE_PATTERN_HPP_

#include <vector>

namespace gs {
/**
 * \brief A means to traverse the grid.
 */
enum PatternComponent {
    FINE_TO_COARSE,
    COARSE_TO_FINE,
    PARSE_FINEST
};

/**
 * \brief A V traversal pattern.
 */
std::vector<PatternComponent> v_pattern() {
    return {
        COARSE_TO_FINE,
        FINE_TO_COARSE
    };
}
/**
 * \brief A /\ traversal pattern.
 */
std::vector<PatternComponent> inverse_v_pattern() {
    return {
        FINE_TO_COARSE,
        COARSE_TO_FINE
    };
}
}  // namespace gs

#endif  // LIB_BASE_PATTERN_HPP_
