#include "hk_keyboard.h"
uintptr_t Hk::render = 0;
int Hk::i = -1;
int Hk::sz = 0;
std::vector<std::vector<vec2>> Hk::marked(20);