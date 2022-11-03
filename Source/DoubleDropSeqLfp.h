#ifndef DOUBLE_DROP_SEQ_LFP_H
#define DOUBLE_DROP_SEQ_LFP_H


#include "ILongestFlowPathAlgorithm.h"
#include "ILongestFlowPathAllAlgorithm.h"
#include "FlowPathLength.h"
#include <utility>


class DoubleDropSeqLfp: public ILongestFlowPathAlgorithm, public ILongestFlowPathAllAlgorithm
{
  private:
    static const int UNDETERMINED;
    static const int OUT_OF_BASIN;

    struct IntermediateResult
    {
      FramedMatrix<FlowPathLength> lengthMatrix;
      CellLocation longestPathSource;
      FlowPathLength longestPathLength;
    };

    IntermediateResult mainAlgorithm(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);

  public:
    CellLocation execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);
    std::vector<CellLocation> executeAll(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);
};


#endif
