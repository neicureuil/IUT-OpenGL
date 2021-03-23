#include "demoGLEngine/utils.hpp"

float Utils::random(int min, int max) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

float Utils::random() {
    return (float)((float)rand() / RAND_MAX);
}

glm::vec3 Utils::vecProd(const glm::vec3& a, const glm::vec3& b) {
    return glm::vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
}

float Utils::scalProd(const glm::vec3& a, const glm::vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Utils::norme(const glm::vec3& a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

