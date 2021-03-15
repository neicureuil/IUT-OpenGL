#include "demoGLEngine/utils.hpp"

float Utils::random(int min, int max) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}