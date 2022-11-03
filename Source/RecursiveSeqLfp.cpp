#include "RecursiveSeqLfp.h"


RecursiveSeqLfp::PathData RecursiveSeqLfp::findLongestPath(const FlowDirectionMatrix& directionMatrix, int row, int col)
{
  PathData longestPath{{row, col}, {0, 0}};

  if (directionMatrix.value[row][col + 1] & DIRECTION_LEFT)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row, col + 1);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row + 1][col + 1] & DIRECTION_UP_LEFT)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row + 1, col + 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row + 1][col] & DIRECTION_UP)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row + 1, col);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row + 1][col - 1] & DIRECTION_UP_RIGHT)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row + 1, col - 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row][col - 1] & DIRECTION_RIGHT)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row, col - 1);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row - 1][col - 1] & DIRECTION_DOWN_RIGHT)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row - 1, col - 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row - 1][col] & DIRECTION_DOWN)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row - 1, col);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row - 1][col + 1] & DIRECTION_DOWN_LEFT)
  {
    PathData candidatePath = findLongestPath(directionMatrix, row - 1, col + 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  return longestPath;
}


CellLocation RecursiveSeqLfp::execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  PathData longestPath = findLongestPath(directionMatrix, outlet.row, outlet.col);
  return longestPath.source;
}
