#include "Utils.h"

bool isIn2DBounds(size_t row, size_t col, size_t gridSize)
{
	if (0 <= row && row < gridSize && 0 <= col && col < gridSize)
		return true;

	return false;
}
