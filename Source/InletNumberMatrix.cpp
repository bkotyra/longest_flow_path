#include "InletNumberMatrix.h"


InletNumberMatrix::InletNumberMatrix(int height, int width):
  FramedMatrix<char>(height, width, 0)
{
}


InletNumberMatrix::InletNumberMatrix(int height, int width, char fillValue):
  FramedMatrix<char>(height, width, 0, fillValue)
{
}
