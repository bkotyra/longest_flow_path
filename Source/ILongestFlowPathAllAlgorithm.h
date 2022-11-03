#ifndef I_LONGEST_FLOW_PATH_ALL_ALGORITHM_H
#define I_LONGEST_FLOW_PATH_ALL_ALGORITHM_H


#include "FlowDirectionMatrix.h"
#include "CellLocation.h"
#include <vector>


class ILongestFlowPathAllAlgorithm
{
  public:
    virtual ~ILongestFlowPathAllAlgorithm() = 0;
    virtual std::vector<CellLocation> executeAll(const FlowDirectionMatrix& directionMatrix, const CellLocation& outlet) = 0;
};


#endif
