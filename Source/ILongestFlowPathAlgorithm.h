#ifndef I_LONGEST_FLOW_PATH_ALGORITHM_H
#define I_LONGEST_FLOW_PATH_ALGORITHM_H


#include "FlowDirectionMatrix.h"
#include "CellLocation.h"


class ILongestFlowPathAlgorithm
{
  public:
    virtual ~ILongestFlowPathAlgorithm() = 0;
    virtual CellLocation execute(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet) = 0;
};


#endif
