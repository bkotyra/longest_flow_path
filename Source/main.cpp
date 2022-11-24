#include "FlowDirectionLoader.h"
#include "RecursiveSeqLfp.h"
#include "RecursiveTaskLfp.h"
#include "TopDownMaxSeqLfp.h"
#include "TopDownSingleSeqLfp.h"
#include "TopDownSingleOmpLfp.h"
#include "DoubleDropSeqLfp.h"
#include "DoubleDropOmpLfp.h"
#include <chrono>
#include <iostream>


std::string algorithmLabel(int algorithmIndex)
{
  switch (algorithmIndex)
  {
    case 1: return "recursive (sequential)";
    case 2: return "recursive (task-based parallel)";
    case 3: return "top-down: maximum length (sequential)";
    case 4: return "top-down: single update (sequential)";
    case 5: return "top-down: single update (parallel)";
    case 6: return "double drop (sequential)";
    case 7: return "double drop (parallel)";
    default: return "";
  }
}


ILongestFlowPathAlgorithm* createAlgorithm(int algorithmIndex, int algorithmParameter)
{
  switch (algorithmIndex)
  {
    case 1: return new RecursiveSeqLfp();
    case 2: return new RecursiveTaskLfp(algorithmParameter);
    case 3: return new TopDownMaxSeqLfp();
    case 4: return new TopDownSingleSeqLfp();
    case 5: return new TopDownSingleOmpLfp();
    case 6: return new DoubleDropSeqLfp();
    case 7: return new DoubleDropOmpLfp();
    default: return nullptr;
  }
}


void printUsage()
{
  std::cout << "required arguments:" << std::endl
            << " 1.  flow direction filename" << std::endl
            << " 2.  outlet location filename (containing row and column coordinates, one-based indexing)" << std::endl
            << " 3.  algorithm index" << std::endl
            << "(4.) algorithm parameter (only for task-based recursive implementation: task creation limit)" << std::endl
            << std::endl
            << "available algorithms:" << std::endl;

  for (int i = 1; i <= 7; ++i)
  {
    std::cout << ' ' << i << ".  " << algorithmLabel(i) << std::endl;
  }
}


CellLocation loadOutletLocation(std::string filename)
{
  CellLocation outlet;

  std::fstream file(filename, std::ios::in);
  file >> outlet.row;
  file >> outlet.col;
  file.close();

  return outlet;
}


void executeMeasurement(std::string directionFilename, std::string outletFilename, int algorithmIndex, int algorithmParameter)
{
  std::cout << "loading flow direction file (" << directionFilename << ")..." << std::endl;
  FlowDirectionMatrix directionMatrix = FlowDirectionLoader::loadGdal(directionFilename);
  std::cout << "flow direction data: " << directionMatrix.height << " rows, " << directionMatrix.width << " columns" << std::endl;

  std::cout << "loading outlet file (" << outletFilename << ")..." << std::endl;
  CellLocation outletLocation = loadOutletLocation(outletFilename);
  std::cout << "outlet location: row " << outletLocation.row << ", column " << outletLocation.col << std::endl;

  std::cout << "executing " << algorithmLabel(algorithmIndex) << " algorithm..." << std::endl;
  ILongestFlowPathAlgorithm* algorithm = createAlgorithm(algorithmIndex, algorithmParameter);

  auto stamp_begin = std::chrono::high_resolution_clock::now();
  CellLocation sourceLocation = algorithm->execute(directionMatrix, outletLocation);
  auto stamp_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> stamp_diff = stamp_end - stamp_begin;

  std::cout << "source location: row " << sourceLocation.row << ", column " << sourceLocation.col << std::endl;
  std::cout << "execution time (ms): " << lround(stamp_diff.count()) << std::endl;
}


int main(int argc, char** argv)
{
  if (argc < 4)
  {
    printUsage();
  }

  else
  {
    const std::string directionFilename = argv[1];
    const std::string outletFilename = argv[2];
    const int algorithmIndex = atoi(argv[3]);
    const int algorithmParameter = (argc > 4) ? atoi(argv[4]) : 0;

    executeMeasurement(directionFilename, outletFilename, algorithmIndex, algorithmParameter);
  }
}
