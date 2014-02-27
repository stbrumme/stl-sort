// //////////////////////////////////////////////////////////
// count.cpp
// Copyright (c) 2014 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

// g++ -O3 count.cpp -o count
// and -std=c++11 if possible

#include <cstdio>
#include <cstdlib>

#include <vector>
#include <algorithm> // std::sort, std::reverse

#include "sort.h"


/// count assignments and comparisions
class Number
{
public:
  /// set value
  Number(int x = 0) : value(x) { }
  /// copy constructor is counted as an assignment
  Number(const Number& other) : value(other.value) { ++numAssignments; }

  static int numLessThan;
  /// count comparisons
  bool operator< (const Number& other) const { ++numLessThan; return value <  other.value; }

  static int numAssignments;
  /// count assignments
  void operator= (const Number& other)       { ++numAssignments;     value =  other.value; }

  bool operator==(const Number& other) const { return                value == other.value; }

  /// reset counters
  static void reset() { numLessThan = numAssignments = 0; }

private:
  /// actually just a simple integer
  int value;
};

int Number::numLessThan    = 0;
int Number::numAssignments = 0;


typedef std::vector<Number> Container;


// protect server from overload:
const int MaxSort = 100000;


int main(int argc, char** argv)
{
  // number of elements to be sorted
  int numElements = 100;
  if (argc == 2)
    numElements = atoi(argv[1]);

  // only positive numbers
  if (numElements == 0)
    numElements = 100;
  if (numElements < 0)
    numElements = -numElements;
  // avoid server overload
  if (numElements > MaxSort)
    numElements = MaxSort;

  printf("%d element%s", numElements, numElements == 1 ? "":"s");

  // initialize containers
  // 0,1,2,3,4,...
  Container ascending(numElements);
  for (int i = 0; i < numElements; i++)
    ascending[i] = i;

  // ...,4,3,2,1,0
  Container descending = ascending;
  std::reverse(descending.begin(), descending.end());

  // just random
  Container random = ascending;
  srand(time(NULL));//0);
  std::random_shuffle(random.begin(), random.end());

  // use this container for input data
  Container data;

  // bubble sort
  printf("\nBubble Sort");
  data = ascending;
  Number::reset();
  bubbleSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  bubbleSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  bubbleSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // selection sort
  printf("\nSelection Sort");
  data = ascending;
  Number::reset();
  selectionSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  selectionSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  selectionSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // insertion sort
  printf("\nInsertion Sort");
  data = ascending;
  Number::reset();
  insertionSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  insertionSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  insertionSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // shell sort
  printf("\nShell Sort");
  data = ascending;
  Number::reset();
  shellSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  shellSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  shellSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // heap sort
  printf("\nHeap Sort");
  data = ascending;
  Number::reset();
  heapSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  heapSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  heapSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // n-ary heap sort
  printf("\n8-ary Heap Sort");
  data = ascending;
  Number::reset();
  naryHeapSort<8>(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  naryHeapSort<8>(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  naryHeapSort<8>(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // merge sort
  printf("\nMerge Sort");
  data = ascending;
  Number::reset();
  mergeSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  mergeSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  mergeSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // in-place merge sort
  printf("\nMerge Sort in-place");
  data = ascending;
  Number::reset();
  mergeSortInPlace(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  mergeSortInPlace(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  mergeSortInPlace(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // quick sort
  printf("\nQuick Sort");
  data = ascending;
  Number::reset();
  quickSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  quickSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  quickSort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // std::sort
  printf("\nstd::sort");
  data = ascending;
  Number::reset();
  std::sort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  std::sort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  std::sort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  // std::stable_sort
  printf("\nstd::stable_sort");
  data = ascending;
  Number::reset();
  std::stable_sort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = descending;
  Number::reset();
  std::stable_sort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  data = random;
  Number::reset();
  std::stable_sort(data.begin(), data.end());
  printf("\t%d\t%d", Number::numLessThan, Number::numAssignments);

  printf("\n");
  return 0;
}
