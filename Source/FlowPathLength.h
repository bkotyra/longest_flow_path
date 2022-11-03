#ifndef FLOW_PATH_LENGTH_H
#define FLOW_PATH_LENGTH_H


#include "CommonConstants.h"


struct FlowPathLength
{
  public:
    int straight;
    int diagonal;

  public:
    double length() const;

    bool operator==(const FlowPathLength& second) const;
    bool operator!=(const FlowPathLength& second) const;
    bool operator<(const FlowPathLength& second) const;
    bool operator>(const FlowPathLength& second) const;
};


#endif
