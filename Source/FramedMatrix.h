#ifndef FRAMED_MATRIX_H
#define FRAMED_MATRIX_H


#include "ProcessingMode.h"
#include <omp.h>


template <typename T>
struct FramedMatrix
{
  public:
    const int height;
    const int width;

    T** value;

  public:
    FramedMatrix(int height, int width);
    FramedMatrix(int height, int width, T frameValue, ProcessingMode mode = ProcessingMode::SEQUENTIAL);
    FramedMatrix(int height, int width, T frameValue, T fillValue, ProcessingMode mode = ProcessingMode::SEQUENTIAL);
    FramedMatrix(FramedMatrix&& source);
    ~FramedMatrix();

    void fillFrame(T frameValue);
    void fillContent(T fillValue);

    void fillFrameParallel(T frameValue);
    void fillContentParallel(T fillValue);

    bool operator==(const FramedMatrix<T>& second) const;
    bool operator!=(const FramedMatrix<T>& second) const;
};


template <typename T>
FramedMatrix<T>::FramedMatrix(int height, int width):
  height(height),
  width(width)
{
  value = new T*[height + 2];

  for (int row = 0; row < height + 2; ++row)
  {
    value[row] = new T[width + 2];
  }
}


template <typename T>
FramedMatrix<T>::FramedMatrix(int height, int width, T frameValue, ProcessingMode mode):
  FramedMatrix<T>(height, width)
{
  if (mode == ProcessingMode::PARALLEL)
  {
    fillFrameParallel(frameValue);
  }
  else
  {
    fillFrame(frameValue);
  }
}


template <typename T>
FramedMatrix<T>::FramedMatrix(int height, int width, T frameValue, T fillValue, ProcessingMode mode):
  FramedMatrix<T>(height, width)
{
  if (mode == ProcessingMode::PARALLEL)
  {
    fillFrameParallel(frameValue);
    fillContentParallel(fillValue);
  }
  else
  {
    fillFrame(frameValue);
    fillContent(fillValue);
  }
}


template <typename T>
FramedMatrix<T>::FramedMatrix(FramedMatrix<T>&& source):
  height(source.height),
  width(source.width),
  value(source.value)
{
  source.value = nullptr;
}


template <typename T>
FramedMatrix<T>::~FramedMatrix()
{
  if (value != nullptr)
  {
    for (int row = 0; row < height + 2; ++row)
    {
      delete[] value[row];
    }

    delete[] value;
  }
}


template <typename T>
void FramedMatrix<T>::fillFrame(T frameValue)
{
  for (int col = 0; col <= width + 1; ++col)
  {
    value[0][col] = frameValue;
    value[height + 1][col] = frameValue;
  }

  for (int row = 1; row <= height; ++row)
  {
    value[row][0] = frameValue;
    value[row][width + 1] = frameValue;
  }
}


template <typename T>
void FramedMatrix<T>::fillContent(T fillValue)
{
  for (int row = 1; row <= height; ++row)
  {
    for (int col = 1; col <= width; ++col)
    {
      value[row][col] = fillValue;
    }
  }
}


template <typename T>
void FramedMatrix<T>::fillFrameParallel(T frameValue)
{
  #pragma omp parallel
  {
    #pragma omp for nowait
    for (int col = 0; col <= width + 1; ++col)
    {
      value[0][col] = frameValue;
      value[height + 1][col] = frameValue;
    }

    #pragma omp for nowait
    for (int row = 1; row <= height; ++row)
    {
      value[row][0] = frameValue;
      value[row][width + 1] = frameValue;
    }
  }
}


template <typename T>
void FramedMatrix<T>::fillContentParallel(T fillValue)
{
  #pragma omp parallel
  {
    #pragma omp for
    for (int row = 1; row <= height; ++row)
    {
      for (int col = 1; col <= width; ++col)
      {
        value[row][col] = fillValue;
      }
    }
  }
}


template <typename T>
bool FramedMatrix<T>::operator==(const FramedMatrix<T>& second) const
{
  if ((height != second.height) || (width != second.width))
  {
    return false;
  }

  for (int row = 1; row <= height; ++row)
  {
    for (int col = 1; col <= width; ++col)
    {
      if (value[row][col] != second.value[row][col])
      {
        return false;
      }
    }
  }

  return true;
}


template <typename T>
bool FramedMatrix<T>::operator!=(const FramedMatrix<T>& second) const
{
  return !(*this == second);
}


#endif
