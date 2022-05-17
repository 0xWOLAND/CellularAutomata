#ifndef COLOR_TRANSITION_H
#define COLOR_TRANSITION_H
#define e 2.71828182845904
class Color{
    public:
        Color() {}
        static float L(int x){
            return 1 / (1 + pow(e, -x));
        }

        static float B(int x){
            return 4 * pow(e, -x) * pow(L(x), 2);
        }
};

#endif