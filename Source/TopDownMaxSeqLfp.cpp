#include "TopDownMaxSeqLfp.h"


const char TopDownMaxSeqLfp::SOURCE = -1;
const int TopDownMaxSeqLfp::UNDETERMINED = -1;


TopDownMaxSeqLfp::IntermediateResult TopDownMaxSeqLfp::mainAlgorithm(const FlowDirectionMatrix& directionMatrix)
{
  const int height = directionMatrix.height;
  const int width = directionMatrix.width;

  InletNumberMatrix inletNumberMatrix = InletNumberCalculator::calculate(directionMatrix, SOURCE);
  FramedMatrix<PathData> pathMatrix(height, width, {UNDETERMINED, UNDETERMINED}, {UNDETERMINED, UNDETERMINED});

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

          CellLocation& source = pathMatrix.value[pathRow][pathCol].source;

          if ((source.row == UNDETERMINED) || (pathLength > pathMatrix.value[source.row][source.col].length))
          {
            source = pathSource;
          }
          else
          {
            pathSource = source;
          }

          --inletNumberMatrix.value[pathRow][pathCol];
        }
        while ((inletNumberMatrix.value[pathRow][pathCol] == 0) && (directionMatrix.value[pathRow][pathCol] != DIRECTION_NONE));
      }
    }
  }

  return IntermediateResult{std::move(inletNumberMatrix), std::move(pathMatrix)};
}


CellLocation TopDownMaxSeqLfp::execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix);

  const PathData& longestPath = result.pathMatrix.value[outlet.row][outlet.col];
  return (result.inletNumberMatrix.value[outlet.row][outlet.col] != SOURCE) ? longestPath.source : outlet;
}


std::vector<CellLocation> TopDownMaxSeqLfp::executeMultiple(const FlowDirectionMatrix& directionMatrix, const std::vector<CellLocation>& outlet)
{
  IntermediateResult result = mainAlgorithm(directionMatrix);

  const int outletCells = outlet.size();
  std::vector<CellLocation> sourceVector(outletCells);

  for (int i = 0; i < outletCells; ++i)
  {
    const PathData& longestPath = result.pathMatrix.value[outlet[i].row][outlet[i].col];
    sourceVector[i] = (result.inletNumberMatrix.value[outlet[i].row][outlet[i].col] != SOURCE) ? longestPath.source : outlet[i];
  }

  return sourceVector;
}
