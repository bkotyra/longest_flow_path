#ifndef RECURSIVE_SEQ_LFP_H
#define RECURSIVE_SEQ_LFP_H


#include "ILongestFlowPathAlgorithm.h"
#include "FlowPathLength.h"


class RecursiveSeqLfp: public ILongestFlowPathAlgorithm
{
  private:
    struct PathData
    {
      CellLocation source;
      FlowPathLength length;
    };

    PathData findLongestPath(const FlowDirectionMatrix& directionMatrix, int row, int col);

  public:
    CellLocation execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);
};


#endif
