#include "DoubleDropOmpLfp.h"


const int DoubleDropOmpLfp::UNDETERMINED = -1;
const int DoubleDropOmpLfp::OUT_OF_BASIN = -2;


DoubleDropOmpLfp::IntermediateResult DoubleDropOmpLfp::mainAlgorithm(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  const int height = directionMatrix.height;
  const int width = directionMatrix.width;

  FramedMatrix<FlowPathLength> lengthMatrix(height, width, {OUT_OF_BASIN, OUT_OF_BASIN}, {UNDETERMINED, UNDETERMINED}, ProcessingMode::PARALLEL);
  lengthMatrix.value[outlet.row][outlet.col] = {0, 0};

  CellLocation longestPathSourceGlobal = outlet;
  FlowPathLength longestPathLengthGlobal{0, 0};

  #pragma omp parallel
  {
    CellLocation longestPathSourceThread = outlet;
    FlowPathLength longestPathLengthThread{0, 0};

    #pragma omp for schedule(guided) nowait
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

            if (pathStraight + pathDiagonal * SQRT_2 > longestPathLengthThread.length())
            {
              longestPathSourceThread = {row, col};
              longestPathLengthThread = {pathStraight, pathDiagonal};
            }

            pathRow = row;
            pathCol = col;

            do
            {
              lengthMatrix.value[pathRow][pathCol].diagonal = pathDiagonal;
              lengthMatrix.value[pathRow][pathCol].straight = pathStraight;

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

    #pragma omp critical
    {
      if (longestPathLengthThread > longestPathLengthGlobal)
      {
        longestPathSourceGlobal = longestPathSourceThread;
        longestPathLengthGlobal = longestPathLengthThread;
      }
    }
  }

  return IntermediateResult{std::move(lengthMatrix), longestPathSourceGlobal, longestPathLengthGlobal};
}


CellLocation DoubleDropOmpLfp::execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix, outlet);
  return result.longestPathSource;
}


std::vector<CellLocation> DoubleDropOmpLfp::executeAll(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix, outlet);

  const int height = result.lengthMatrix.height;
  const int width = result.lengthMatrix.width;

  std::vector<CellLocation> sourceVectorGlobal;

  #pragma omp parallel
  {
    std::vector<CellLocation> sourceVectorThread;

    #pragma omp for nowait
    for (int row = 1; row <= height; ++row)
    {
      for (int col = 1; col <= width; ++col)
      {
        if (result.lengthMatrix.value[row][col] == result.longestPathLength)
        {
          sourceVectorThread.push_back({row, col});
        }
      }
    }

    if (!sourceVectorThread.empty())
    {
      #pragma omp critical
      {
        sourceVectorGlobal.insert(sourceVectorGlobal.end(), sourceVectorThread.begin(), sourceVectorThread.end());
      }
    }
  }

  return sourceVectorGlobal;
}
