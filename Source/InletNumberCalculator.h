#ifndef INLET_NUMBER_CALCULATOR_H
#define INLET_NUMBER_CALCULATOR_H


#include "InletNumberMatrix.h"
#include "FlowDirectionMatrix.h"
#include <omp.h>


class InletNumberCalculator
{
  public:
    static InletNumberMatrix calculate(const FlowDirectionMatrix& directionMatrix);
    static InletNumberMatrix calculate(const FlowDirectionMatrix& directionMatrix, char sourceValue);
    static InletNumberMatrix calculate(const FlowDirectionMatrix& directionMatrix, char sourceValue, char linkValue);

    static InletNumberMatrix calculateParallel(const FlowDirectionMatrix& directionMatrix);
    static InletNumberMatrix calculateParallel(const FlowDirectionMatrix& directionMatrix, char sourceValue);
    static InletNumberMatrix calculateParallel(const FlowDirectionMatrix& directionMatrix, char sourceValue, char linkValue);
};


#endif
