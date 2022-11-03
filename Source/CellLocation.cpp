#include "CellLocation.h"


void CellLocation::move(unsigned char direction)
{
  switch (direction)
  {
    case DIRECTION_RIGHT:             ++col; break;
    case DIRECTION_DOWN_RIGHT: ++row; ++col; break;
    case DIRECTION_DOWN:       ++row;        break;
    case DIRECTION_DOWN_LEFT:  ++row; --col; break;
    case DIRECTION_LEFT:              --col; break;
    case DIRECTION_UP_LEFT:    --row; --col; break;
    case DIRECTION_UP:         --row;        break;
    case DIRECTION_UP_RIGHT:   --row; ++col; break;
  }
}


CellLocation CellLocation::neighbor(unsigned char direction) const
{
  switch (direction)
  {
    case DIRECTION_RIGHT:      return {row,     col + 1};
    case DIRECTION_DOWN_RIGHT: return {row + 1, col + 1};
    case DIRECTION_DOWN:       return {row + 1, col    };
    case DIRECTION_DOWN_LEFT:  return {row + 1, col - 1};
    case DIRECTION_LEFT:       return {row    , col - 1};
    case DIRECTION_UP_LEFT:    return {row - 1, col - 1};
    case DIRECTION_UP:         return {row - 1, col    };
    case DIRECTION_UP_RIGHT:   return {row - 1, col + 1};
    default:                   return {row    , col    };
  }
}


double CellLocation::distance(const CellLocation& second) const
{
  return sqrt(pow(row - second.row, 2) + pow(col - second.col, 2));
}


bool CellLocation::operator==(const CellLocation& second) const
{
  return (row == second.row) && (col == second.col);
}


bool CellLocation::operator!=(const CellLocation& second) const
{
  return !(*this == second);
}
