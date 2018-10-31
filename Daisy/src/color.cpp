#include "../include/color.h"

using namespace daisy;

const color color::BLACK = color{ 0, 0, 0 };
const color color::WHITE = color{ 255, 255, 255 };
const color color::RED = color{ 255, 0, 0 };
const color color::GREEN = color{ 0, 255, 0 };
const color color::BLUE = color{ 0, 0, 255 };

bool color::operator==(color const &c) const
{
	return ((r == c.r) && (g == c.g) && (b == c.b));
}