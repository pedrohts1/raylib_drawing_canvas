#ifndef DRAWING_CANVAS_UTILS_HPP
#define DRAWING_CANVAS_UTILS_HPP

#include "random"

bool randomBoolean(float true_probability=0.5) {
    static std::default_random_engine generator(std::random_device{}());
    // With p = 0.5 you get equal probability for true and false
    static std::bernoulli_distribution distribution(true_probability);
    return distribution(generator);
}
double map_range(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


#endif //DRAWING_CANVAS_UTILS_HPP
