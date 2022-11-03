#ifndef RECURSIVE_TASK_LFP_H
#define RECURSIVE_TASK_LFP_H


#include "ILongestFlowPathAlgorithm.h"
#include "FlowPathLength.h"
#include <omp.h>


class RecursiveTaskLfp: public ILongestFlowPathAlgorithm
{
  private:
    struct PathData
    {
      CellLocation source;
      FlowPathLength length;
    };

    const int parallelRecursionLimit;

    PathData findLongestPathSequential(const FlowDirectionMatrix& directionMatrix, int row, int col);
    PathData findLongestPathParallel(const FlowDirectionMatrix& directionMatrix, int row, int col, int recursionLevel);

  public:
    RecursiveTaskLfp(int parallelRecursionLimit);

    CellLocation execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);
};


#endif
