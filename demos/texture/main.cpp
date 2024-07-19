#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
    int x, y, n;
    unsigned char *image = stbi_load("../gengar.png", &x, &y, &n, 0);
    std::cout << x << ", " << y << ", " << n << std::endl;
}
