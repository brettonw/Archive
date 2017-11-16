#ifndef UTILITY
#define UTILITY

inline real degreesToRadians (real angle) {
    return angle * (PI / R (180.0));
}

inline real radiansToDegrees (real angle) {
    return angle * (R (180.0) / PI);
}

inline real sgn (real v) {
    return (v > R (0.0)) ? R (1.0) : (v == R (0.0)) ? R (0.0) : R (-1.0);
}

inline bool odd (int v) {
    return bool (v&  1);
}

inline real fract (real v) {
    v = v - FLOOR (v);
    if (v < R (0.0)) v += R (1.0);
    return v;
}

inline real unitRandom (void) {
    return rand () / real (RAND_MAX);
}

#endif //UTILITY
