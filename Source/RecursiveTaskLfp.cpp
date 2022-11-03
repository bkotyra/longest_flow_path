#include "RecursiveTaskLfp.h"


RecursiveTaskLfp::PathData RecursiveTaskLfp::findLongestPathSequential(const FlowDirectionMatrix& directionMatrix, int row, int col)
{
  PathData longestPath{{row, col}, {0, 0}};

  if (directionMatrix.value[row][col + 1] & DIRECTION_LEFT)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row, col + 1);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row + 1][col + 1] & DIRECTION_UP_LEFT)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row + 1, col + 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row + 1][col] & DIRECTION_UP)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row + 1, col);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row + 1][col - 1] & DIRECTION_UP_RIGHT)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row + 1, col - 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row][col - 1] & DIRECTION_RIGHT)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row, col - 1);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row - 1][col - 1] & DIRECTION_DOWN_RIGHT)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row - 1, col - 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row - 1][col] & DIRECTION_DOWN)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row - 1, col);
    candidatePath.length.straight++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  if (directionMatrix.value[row - 1][col + 1] & DIRECTION_DOWN_LEFT)
  {
    PathData candidatePath = findLongestPathSequential(directionMatrix, row - 1, col + 1);
    candidatePath.length.diagonal++;

    if (candidatePath.length > longestPath.length)
    {
      longestPath = candidatePath;
    }
  }

  return longestPath;
}


RecursiveTaskLfp::PathData RecursiveTaskLfp::findLongestPathParallel(const FlowDirectionMatrix& directionMatrix, int row, int col, int recursionLevel)
{
  if (recursionLevel < parallelRecursionLimit)
  {
    PathData candidatePath[8];
    char candidatePaths = 0;

    if (directionMatrix.value[row][col + 1] & DIRECTION_LEFT)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row, col + 1, recursionLevel + 1);
        candidatePath[candidateIndex].length.straight++;
      }
    }

    if (directionMatrix.value[row + 1][col + 1] & DIRECTION_UP_LEFT)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row + 1, col + 1, recursionLevel + 1);
        candidatePath[candidateIndex].length.diagonal++;
      }
    }

    if (directionMatrix.value[row + 1][col] & DIRECTION_UP)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row + 1, col, recursionLevel + 1);
        candidatePath[candidateIndex].length.straight++;
      }
    }

    if (directionMatrix.value[row + 1][col - 1] & DIRECTION_UP_RIGHT)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row + 1, col - 1, recursionLevel + 1);
        candidatePath[candidateIndex].length.diagonal++;
      }
    }

    if (directionMatrix.value[row][col - 1] & DIRECTION_RIGHT)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row, col - 1, recursionLevel + 1);
        candidatePath[candidateIndex].length.straight++;
      }
    }

    if (directionMatrix.value[row - 1][col - 1] & DIRECTION_DOWN_RIGHT)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row - 1, col - 1, recursionLevel + 1);
        candidatePath[candidateIndex].length.diagonal++;
      }
    }

    if (directionMatrix.value[row - 1][col] & DIRECTION_DOWN)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row - 1, col, recursionLevel + 1);
        candidatePath[candidateIndex].length.straight++;
      }
    }

    if (directionMatrix.value[row - 1][col + 1] & DIRECTION_DOWN_LEFT)
    {
      char candidateIndex = candidatePaths++;

      #pragma omp task shared(directionMatrix, candidatePath)
      {
        candidatePath[candidateIndex] = findLongestPathParallel(directionMatrix, row - 1, col + 1, recursionLevel + 1);
        candidatePath[candidateIndex].length.diagonal++;
      }
    }

    #pragma omp taskwait

    PathData longestPath{{row, col}, {0, 0}};

    for (int i = 0; i < candidatePaths; ++i)
    {
      if (candidatePath[i].length > longestPath.length)
      {
        longestPath = candidatePath[i];
      }
    }

    return longestPath;
  }

  else
  {
    return findLongestPathSequential(directionMatrix, row, col);
  }
}


RecursiveTaskLfp::RecursiveTaskLfp(int parallelRecursionLimit):
  parallelRecursionLimit(parallelRecursionLimit)
{
}


CellLocation RecursiveTaskLfp::execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet)
{
  PathData longestPath;

  #pragma omp parallel
  {
    #pragma omp single nowait
    {
      longestPath = findLongestPathParallel(directionMatrix, outlet.row, outlet.col, 0);
    }
  }

  return longestPath.source;
}
