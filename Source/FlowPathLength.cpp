#include "FlowPathLength.h"


double FlowPathLength::length() const
{
  return straight + diagonal * SQRT_2;
}


bool FlowPathLength::operator==(const FlowPathLength& second) const
{
  return (straight == second.straight) && (diagonal == second.diagonal);
}


bool FlowPathLength::operator!=(const FlowPathLength& second) const
{
  return !(*this == second);
}


bool FlowPathLength::operator<(const FlowPathLength& second) const
{
  return straight + diagonal * SQRT_2 < second.straight + second.diagonal * SQRT_2;
}


bool FlowPathLength::operator>(const FlowPathLength& second) const
{
  return second < *this;
}
