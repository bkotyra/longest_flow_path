#ifndef I_LONGEST_FLOW_PATH_MULTIPLE_ALGORITHM_H
#define I_LONGEST_FLOW_PATH_MULTIPLE_ALGORITHM_H


#include "FlowDirectionMatrix.h"
#include "CellLocation.h"
#include <vector>


class ILongestFlowPathMultipleAlgorithm
{
  public:
    virtual ~ILongestFlowPathMultipleAlgorithm() = 0;
    virtual std::vector<CellLocation> executeMultiple(const FlowDirectionMatrix& directionMatrix, const std::vector<CellLocation>& outlet) = 0;
};


#endif
