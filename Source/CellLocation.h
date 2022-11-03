#ifndef CELL_LOCATION_H
#define CELL_LOCATION_H


#include "FlowDirectionConstants.h"
#include <cmath>


struct CellLocation
{
  public:
    int row;
    int col;

  public:
    void move(unsigned char direction);
    CellLocation neighbor(unsigned char direction) const;
    double distance(const CellLocation& second) const;

    bool operator==(const CellLocation& second) const;
    bool operator!=(const CellLocation& second) const;
};


#endif
