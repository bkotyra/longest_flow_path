#ifndef TOP_DOWN_MAX_SEQ_LFP_H
#define TOP_DOWN_MAX_SEQ_LFP_H


#include "ILongestFlowPathAlgorithm.h"
#include "ILongestFlowPathMultipleAlgorithm.h"
#include "InletNumberCalculator.h"
#include "FlowPathLength.h"


class TopDownMaxSeqLfp: public ILongestFlowPathAlgorithm, public ILongestFlowPathMultipleAlgorithm
{
  private:
    static const char SOURCE;
    static const int UNDETERMINED;

    union PathData
    {
      CellLocation source;
      FlowPathLength length;
    };

    struct IntermediateResult
    {
      InletNumberMatrix inletNumberMatrix;
      FramedMatrix<PathData> pathMatrix;
    };

    IntermediateResult mainAlgorithm(const FlowDirectionMatrix& directionMatrix);

  public:
    CellLocation execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);
    std::vector<CellLocation> executeMultiple(const FlowDirectionMatrix& directionMatrix, const std::vector<CellLocation>& outlet);
};


#endif
