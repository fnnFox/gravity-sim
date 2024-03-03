#include "Forces3.h"
#include <math.h>

Vector2f getUnit(Vector2f vector) {
    return Vector2f(vector / getLength(vector));
}

float getLength(Vector2f vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vector2f operator+ (Vector2f vec, float sca) {
    vec.x += sca;
    vec.y += sca;
    return vec;
}