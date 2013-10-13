// //////////////////////////////////////////////////////////
// sort.cpp
// Copyright (c) 2011, 2013 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

// g++ -O3 sort.cpp -o sort

#include <cstdio>
#include <cstdlib>
#include <cmath> // fabs

#include <vector>
#include <list>
#include <algorithm>

#include "sort.h"


// datatype to be sorted
typedef int T;


// if set then results will be checked whether they are properly sorted
//#define CHECKRESULT


// protect server from overload:
// disable a few very slow benchmark when more than RestrictedSort elements
const int RestrictedSort =   25000;
// upper limit, no sorts beyond this number of elements
const int MaxSort        = 1000000;

//#define USE_WINDOWS_TIMER
#ifdef USE_WINDOWS_TIMER
#include <windows.h>
#else
#include <sys/time.h>
#endif


// OS-specific timing
static double seconds()
{
#ifdef USE_WINDOWS_TIMER
  LARGE_INTEGER frequency, now;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter  (&now);
  return now.QuadPart / double(frequency.QuadPart);
#else
  timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec + now.tv_usec/1000000.0;
#endif
}


int main(int argc, char** argv)
{
  // number of elements to be sorted
  int numElements = 10000;
  if (argc == 2)
    numElements = atoi(argv[1]);

  // only positive numbers
  if (numElements == 0)
    numElements = 10000;
  if (numElements < 0)
    numElements = -numElements;
  // avoid server overload
  if (numElements > MaxSort)
    numElements = MaxSort;

  printf("%d element%s\n", numElements, numElements == 1 ? "":"s");

  // initialize containers
  // 0,1,2,3,4,...
  std::vector<T> ascending(numElements);
  for (int i = 0; i < numElements; i++)
    ascending[i] = i;

  // ...,4,3,2,1,0
  std::vector<T> descending = ascending;
  std::reverse(descending.begin(), descending.end());

  // just random
  std::vector<T> random(numElements);
  srand(time(NULL));//0); // keep randomness reproducible ;-)
  for (int i = 0; i < numElements; i++)
    random[i] = rand();

#ifdef CHECKRESULT
  std::vector<T> sorted = ascending;
  std::vector<T> sortedRandom = random;
  std::sort(sortedRandom.begin(), sortedRandom.end());
#endif // CHECKRESULT

  // use this container for input data
  std::vector<T> data;

  double timeInverted = 0;
  double timeSorted   = 0;
  double timeRandom   = 0;

  // BubbleSort
  // sorted data
  data = ascending;
  timeSorted = seconds();
  bubbleSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    bubbleSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    bubbleSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Bubble Sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf("Bubble Sort\t%8.3f ms\tn/a\tn/a\tn/a\n", 1000*timeSorted);

  // SelectionSort
  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    selectionSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // sorted data
    timeSorted = seconds();
    selectionSort(data.begin(), data.end());
    timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    selectionSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Selection Sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf("Selection Sort\tn/a\tn/a\tn/a\tn/a\n");

  // InsertionSort
  // sorted data
  data = ascending;
  timeSorted = seconds();
  insertionSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  if (numElements < RestrictedSort)
  {
    // inverted data
    data = descending;
    timeInverted = seconds();
    insertionSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
    if (data != sorted)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    // random data
    data = random;
    timeRandom = seconds();
    insertionSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
    if (data != sortedRandom)
      printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

    printf("Insertion Sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf("Insertion Sort\t%8.3f ms\tn/a\tn/a\tn/a\n", 1000*timeSorted);

  // ShellSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  shellSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  shellSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  shellSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Shell Sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // QuickSort
  // inverted data
  data = descending;
  timeInverted = seconds();
  quickSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // sorted data
  timeSorted = seconds();
  quickSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

#ifdef CHECKRESULT
  if (data != sorted)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  // random data
  data = random;
  timeRandom = seconds();
  quickSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

#ifdef CHECKRESULT
  if (data != sortedRandom)
    printf("Sorting problem @ %d ", __LINE__);
#endif // CHECKRESULT

  printf("Quick Sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // std::sort
  // inverted data
  data = descending;
  timeInverted = seconds();
  std::sort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

  // sorted data
  timeSorted = seconds();
  std::sort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  // random data
  data = random;
  timeRandom = seconds();
  std::sort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

  printf("std::sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // std::stable_sort
  // inverted data
  data = descending;
  timeInverted = seconds();
  std::stable_sort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

  // sorted data
  timeSorted = seconds();
  std::stable_sort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  // random data
  data = random;
  timeRandom = seconds();
  std::stable_sort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

  printf("std::stable_sort\t%8.3f ms\t%8.3f ms\t%8.3f ms\t%8.3f ms\n",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  return 0;
}
