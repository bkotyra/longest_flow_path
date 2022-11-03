#include "DoubleDropSeqLfp.h"


const int DoubleDropSeqLfp::UNDETERMINED = -1;
const int DoubleDropSeqLfp::OUT_OF_BASIN = -2;


DoubleDropSeqLfp::IntermediateResult DoubleDropSeqLfp::mainAlgorithm(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  const int height = directionMatrix.height;
  const int width = directionMatrix.width;

  FramedMatrix<FlowPathLength> lengthMatrix(height, width, {OUT_OF_BASIN, OUT_OF_BASIN}, {UNDETERMINED, UNDETERMINED});
  lengthMatrix.value[outlet.row][outlet.col] = {0, 0};

  CellLocation longestPathSource = outlet;
  FlowPathLength longestPathLength{0, 0};

  for (int row = 1; row <= height; ++row)
  {
    for (int col = 1; col <= width; ++col)
    {
      if ((lengthMatrix.value[row][col].straight == UNDETERMINED) && (directionMatrix.value[row][col] != DIRECTION_NONE))
      {
        int pathRow = row;
        int pathCol = col;
        int pathStraight = 0;
        int pathDiagonal = 0;

        do
        {
          switch (directionMatrix.value[pathRow][pathCol])
          {
            case DIRECTION_RIGHT:                 ++pathCol; ++pathStraight; break;
            case DIRECTION_DOWN_RIGHT: ++pathRow; ++pathCol; ++pathDiagonal; break;
            case DIRECTION_DOWN:       ++pathRow;            ++pathStraight; break;
            case DIRECTION_DOWN_LEFT:  ++pathRow; --pathCol; ++pathDiagonal; break;
            case DIRECTION_LEFT:                  --pathCol; ++pathStraight; break;
            case DIRECTION_UP_LEFT:    --pathRow; --pathCol; ++pathDiagonal; break;
            case DIRECTION_UP:         --pathRow;            ++pathStraight; break;
            case DIRECTION_UP_RIGHT:   --pathRow; ++pathCol; ++pathDiagonal; break;
          }
        }
        while ((lengthMatrix.value[pathRow][pathCol].straight == UNDETERMINED) && (directionMatrix.value[pathRow][pathCol] != DIRECTION_NONE));

        if (lengthMatrix.value[pathRow][pathCol].straight >= 0)
        {
          pathStraight += lengthMatrix.value[pathRow][pathCol].straight;
          pathDiagonal += lengthMatrix.value[pathRow][pathCol].diagonal;

          if (pathStraight + pathDiagonal * SQRT_2 > longestPathLength.length())
          {
            longestPathSource = {row, col};
            longestPathLength = {pathStraight, pathDiagonal};
          }

          pathRow = row;
          pathCol = col;

          do
          {
            lengthMatrix.value[pathRow][pathCol] = {pathStraight, pathDiagonal};

            switch (directionMatrix.value[pathRow][pathCol])
            {
              case DIRECTION_RIGHT:                 ++pathCol; --pathStraight; break;
              case DIRECTION_DOWN_RIGHT: ++pathRow; ++pathCol; --pathDiagonal; break;
              case DIRECTION_DOWN:       ++pathRow;            --pathStraight; break;
              case DIRECTION_DOWN_LEFT:  ++pathRow; --pathCol; --pathDiagonal; break;
              case DIRECTION_LEFT:                  --pathCol; --pathStraight; break;
              case DIRECTION_UP_LEFT:    --pathRow; --pathCol; --pathDiagonal; break;
              case DIRECTION_UP:         --pathRow;            --pathStraight; break;
              case DIRECTION_UP_RIGHT:   --pathRow; ++pathCol; --pathDiagonal; break;
            }
          }
          while (lengthMatrix.value[pathRow][pathCol].straight == UNDETERMINED);
        }

        else
        {
          lengthMatrix.value[pathRow][pathCol] = {OUT_OF_BASIN, OUT_OF_BASIN};

          pathRow = row;
          pathCol = col;

          do
          {
            lengthMatrix.value[pathRow][pathCol] = {OUT_OF_BASIN, OUT_OF_BASIN};

            switch (directionMatrix.value[pathRow][pathCol])
            {
              case DIRECTION_RIGHT:                 ++pathCol; break;
              case DIRECTION_DOWN_RIGHT: ++pathRow; ++pathCol; break;
              case DIRECTION_DOWN:       ++pathRow;            break;
              case DIRECTION_DOWN_LEFT:  ++pathRow; --pathCol; break;
              case DIRECTION_LEFT:                  --pathCol; break;
              case DIRECTION_UP_LEFT:    --pathRow; --pathCol; break;
              case DIRECTION_UP:         --pathRow;            break;
              case DIRECTION_UP_RIGHT:   --pathRow; ++pathCol; break;
            }
          }
          while (lengthMatrix.value[pathRow][pathCol].straight == UNDETERMINED);
        }
      }
    }
  }

  return IntermediateResult{std::move(lengthMatrix), longestPathSource, longestPathLength};
}


CellLocation DoubleDropSeqLfp::execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix, outlet);
  return result.longestPathSource;
}


std::vector<CellLocation> DoubleDropSeqLfp::executeAll(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix, outlet);

  const int height = result.lengthMatrix.height;
  const int width = result.lengthMatrix.width;

  std::vector<CellLocation> sourceVector;

  for (int row = 1; row <= height; ++row)
  {
    for (int col = 1; col <= width; ++col)
    {
      if (result.lengthMatrix.value[row][col] == result.longestPathLength)
      {
        sourceVector.push_back({row, col});
      }
    }
  }

  return sourceVector;
}
