#ifndef TOP_DOWN_SINGLE_OMP_LFP_H
#define TOP_DOWN_SINGLE_OMP_LFP_H


#include "ILongestFlowPathAlgorithm.h"
#include "ILongestFlowPathMultipleAlgorithm.h"
#include "InletNumberCalculator.h"
#include "FlowPathLength.h"
#include <omp.h>


class TopDownSingleOmpLfp: public ILongestFlowPathAlgorithm, public ILongestFlowPathMultipleAlgorithm
{
  private:
    static const char SOURCE;
    static const char LINK;

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

    static CellLocation sourceForLocation(int row, int col, const FramedMatrix<PathData>& pathMatrix, const InletNumberMatrix& inletNumberMatrix);
    static void compareAndUpdate(const CellLocation& source, const FramedMatrix<PathData>& pathMatrix, FlowPathLength& maxLength, CellLocation& maxSource);
    IntermediateResult mainAlgorithm(const FlowDirectionMatrix& directionMatrix);

  public:
    CellLocation execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet);
    std::vector<CellLocation> executeMultiple(const FlowDirectionMatrix& directionMatrix, const std::vector<CellLocation>& outlet);
};


#endif
