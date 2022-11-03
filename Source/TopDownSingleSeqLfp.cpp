#include "TopDownSingleSeqLfp.h"


const char TopDownSingleSeqLfp::SOURCE = -1;
const char TopDownSingleSeqLfp::LINK = -2;


CellLocation TopDownSingleSeqLfp::sourceForLocation(int row, int col, const FramedMatrix<PathData>& pathMatrix, const InletNumberMatrix& inletNumberMatrix)
{
  return (inletNumberMatrix.value[row][col] != SOURCE) ? pathMatrix.value[row][col].source : CellLocation{row, col};
}


void TopDownSingleSeqLfp::compareAndUpdate(const CellLocation& source, const FramedMatrix<PathData>& pathMatrix, FlowPathLength& maxLength, CellLocation& maxSource)
{
  if (pathMatrix.value[source.row][source.col].length > maxLength)
  {
    maxLength = pathMatrix.value[source.row][source.col].length;
    maxSource = source;
  }
}


TopDownSingleSeqLfp::IntermediateResult TopDownSingleSeqLfp::mainAlgorithm(const FlowDirectionMatrix& directionMatrix)
{
  const int height = directionMatrix.height;
  const int width = directionMatrix.width;

  InletNumberMatrix inletNumberMatrix = InletNumberCalculator::calculate(directionMatrix, SOURCE, LINK);
  FramedMatrix<PathData> pathMatrix(height, width);

  for (int row = 1; row <= height; ++row)
  {
    for (int col = 1; col <= width; ++col)
    {
      if ((inletNumberMatrix.value[row][col] == SOURCE) && (directionMatrix.value[row][col] != DIRECTION_NONE))
      {
        pathMatrix.value[row][col].length = {0, 0};

        CellLocation pathSource{row, col};
        int pathRow = row;
        int pathCol = col;
        char pathInletNumber = 0;

        do
        {
          FlowPathLength& pathLength = pathMatrix.value[pathSource.row][pathSource.col].length;

          switch (directionMatrix.value[pathRow][pathCol])
          {
            case DIRECTION_RIGHT:                 ++pathCol; ++pathLength.straight; break;
            case DIRECTION_DOWN_RIGHT: ++pathRow; ++pathCol; ++pathLength.diagonal; break;
            case DIRECTION_DOWN:       ++pathRow;            ++pathLength.straight; break;
            case DIRECTION_DOWN_LEFT:  ++pathRow; --pathCol; ++pathLength.diagonal; break;
            case DIRECTION_LEFT:                  --pathCol; ++pathLength.straight; break;
            case DIRECTION_UP_LEFT:    --pathRow; --pathCol; ++pathLength.diagonal; break;
            case DIRECTION_UP:         --pathRow;            ++pathLength.straight; break;
            case DIRECTION_UP_RIGHT:   --pathRow; ++pathCol; ++pathLength.diagonal; break;
          }

          if (inletNumberMatrix.value[pathRow][pathCol] < 0)
          {
            pathMatrix.value[pathRow][pathCol].source = pathSource;
          }

          else
          {
            pathInletNumber = --inletNumberMatrix.value[pathRow][pathCol];

            if (pathInletNumber == 0)
            {
              FlowPathLength maxLength = pathLength;

              if (directionMatrix.value[pathRow][pathCol + 1] & DIRECTION_LEFT)
              {
                CellLocation source = sourceForLocation(pathRow, pathCol + 1, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow + 1][pathCol + 1] & DIRECTION_UP_LEFT)
              {
                CellLocation source = sourceForLocation(pathRow + 1, pathCol + 1, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow + 1][pathCol] & DIRECTION_UP)
              {
                CellLocation source = sourceForLocation(pathRow + 1, pathCol, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow + 1][pathCol - 1] & DIRECTION_UP_RIGHT)
              {
                CellLocation source = sourceForLocation(pathRow + 1, pathCol - 1, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow][pathCol - 1] & DIRECTION_RIGHT)
              {
                CellLocation source = sourceForLocation(pathRow, pathCol - 1, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow - 1][pathCol - 1] & DIRECTION_DOWN_RIGHT)
              {
                CellLocation source = sourceForLocation(pathRow - 1, pathCol - 1, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow - 1][pathCol] & DIRECTION_DOWN)
              {
                CellLocation source = sourceForLocation(pathRow - 1, pathCol, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              if (directionMatrix.value[pathRow - 1][pathCol + 1] & DIRECTION_DOWN_LEFT)
              {
                CellLocation source = sourceForLocation(pathRow - 1, pathCol + 1, pathMatrix, inletNumberMatrix);
                compareAndUpdate(source, pathMatrix, maxLength, pathSource);
              }

              pathMatrix.value[pathRow][pathCol].source = pathSource;
            }
          }
        }
        while ((pathInletNumber == 0) && (directionMatrix.value[pathRow][pathCol] != DIRECTION_NONE));
      }
    }
  }

  return IntermediateResult{std::move(inletNumberMatrix), std::move(pathMatrix)};
}


CellLocation TopDownSingleSeqLfp::execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix);
  return sourceForLocation(outlet.row, outlet.col, result.pathMatrix, result.inletNumberMatrix);
}


std::vector<CellLocation> TopDownSingleSeqLfp::executeMultiple(const FlowDirectionMatrix& directionMatrix, const std::vector<CellLocation>& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix);

  const int outletCells = outlet.size();
  std::vector<CellLocation> sourceVector(outletCells);

  for (int i = 0; i < outletCells; ++i)
  {
    sourceVector[i] = sourceForLocation(outlet[i].row, outlet[i].col, result.pathMatrix, result.inletNumberMatrix);
  }

  return sourceVector;
}
