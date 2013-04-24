// //////////////////////////////////////////////////////////
// sort.cpp
// Copyright (c) 2011 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

// g++ -O3 sort.cpp -o sort

#include <cstdio>
#include <cstdlib>
#include <cmath> // fabs

#include <vector>
#include <list>
#include <algorithm>

// protect server from overload:
// disable a few very slow benchmark when more than RestrictedSort elements
const int RestrictedSort =   30000;
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


template <typename iterator>
void bubbleSort(iterator begin, iterator end)
{
  if (begin == end)
    return;

  // usually end points beyond the last element, now it points to the last element
  --end;

  bool swapped;
  do
  {
    swapped = false;
    iterator current = begin;
    while (current != end)
    {
      // bubble up
      iterator next = current;
      ++next;

      if (*current > *next)
      {
        std::iter_swap(current, next);
        swapped = true;
      }
      ++current;
    }

    // last element is already sorted now
    --end;
  } while (swapped && begin != end);
}


template <typename iterator>
void selectionSort(iterator begin, iterator end)
{
//#pragma omp parallel for
  for (iterator current = begin; current != end; ++current)
  {
    // find smallest element in the unsorted part
    iterator minimum = current;
    iterator compare = current;
    ++compare;
    while (compare != end)
    {
      if (*compare < *minimum)
        minimum = compare;
      ++compare;
    }

    // move it to the end of the sorted part (unless current is already the minimum)
    if (current != minimum)
      std::iter_swap(current, minimum);
  }
}


template <typename iterator>
void insertSort(iterator begin, iterator end)
{
  iterator current = begin;
  ++current;
  while (current != end)
  {
    typename iterator::value_type compare = *current;
    // shift bigger value to the right
    iterator pos = current;
    --pos;

    while (pos != begin && compare < *pos)
    {
      iterator next = pos;
      ++next;
      *next = *pos;
      pos--;
    }
    // special case: first element
    if (pos != begin || compare >= *begin)
      ++pos;
    *pos = compare;

    ++current;
  }
}


template <typename iterator>
void shellSort(iterator begin, iterator end)
{
  const size_t numElements = std::distance(begin, end);

  // sequence taken from Wikipedia
  const size_t NumOptimal = 14;
  static const size_t OptimalIncrements[NumOptimal] =
  { 0, 1, 4, 10, 23, 57, 132, 301, 701, 1750, 4383, 10958, 27396, 68491 };
  size_t incrementIndex = NumOptimal-1;
  size_t increment = OptimalIncrements[incrementIndex--];
  // increment must not be bigger than the number of elements to be sorted
  while (increment >= numElements)
    increment = OptimalIncrements[incrementIndex--];

  // stumble through all increments in descending order
  while (increment > 0)
  {
    // interleaved sort
    iterator first = begin;
    std::advance(first, increment);
    if (first == end)
      continue;

    // place element "compare" at correct position (for current increment)
    size_t compareOffset = increment;
    for (iterator iteCompare = first; iteCompare != end; ++iteCompare, ++compareOffset)
    {
      const typename iterator::value_type compare = *iteCompare;

      iterator current = iteCompare;
      iterator next    = current;
      std::advance(next, -int(increment));
      size_t nextOffset = compareOffset - increment;

      while (compare < *next)
      {
        *current = *next;
        current  =  next;

        // done ?
        if (nextOffset < increment)
          break;
        // keep going ...
        std::advance(next, -int(increment));
        nextOffset -= increment;
      }
      *current = compare;
    }
    increment = OptimalIncrements[incrementIndex--];
  }
}


template <typename iterator>
void quickSort(iterator begin, iterator end)
{
  // already sorted ?
  if (begin == end)
    return;
  iterator pivot = end;
  --pivot;
  if (begin == pivot)
    return;

  // choose middle element as pivot (good choice for partially sorted data)
  size_t numElements = std::distance(begin, end);
  if (numElements > 2)
  {
    iterator middle = begin;
    std::advance(middle, numElements/2);
    std::iter_swap(middle, pivot);
  }

  iterator left  = begin;
  iterator right = pivot;
  //--right;
  while (left != right)
  {
    // look for mismatches
    //while (*left  <= *pivot && left  != pivot)
    while (*left  <= *pivot && left  != right)
      left++;
    //while (*right >= *pivot && right != begin)
    while (*right >= *pivot && right != left)
      right--;
    // swap two values which are both on the wrong side of the pivot element
    if (left != right)
      std::iter_swap(left, right);
  }

  // pivot is on its final position
  if (*left > *pivot)
    std::iter_swap(left, pivot);

  // subdivide
  quickSort(begin, left);
  quickSort(++left, end); // *lft itself is already sorted
}


int main(int, char**)
{
  printf("Content-Type: text/plain\n\n");

  // number of elements to be sorted is passed via QUERY_STRING environment variable
  const char *query = getenv("QUERY_STRING");
  if (!query)
    query = "10000";
  int numElements = atoi(query);

  // only positive numbers
  if (numElements == 0)
    numElements = 10000;
  if (numElements < 0)
    numElements = -numElements;
  // avoid server overload
  if (numElements > MaxSort)
    numElements = MaxSort;

  printf("<table class=\"index\">"
         " <tr>"
         "  <th>%d element%s</th>"
         "  <th>Already Sorted</th>"
         "  <th>Already Sorted (descending)</th>"
         "  <th>Random Data</th>"
         "  <th>Total Time</th>"
         " </tr>",
         numElements, numElements == 1 ? "":"s");

  // initialize containers
  std::vector<int> data(numElements);
  for (int i = 0; i < numElements; i++)
    data[i] = i;
  std::vector<int> random(numElements);
  srand(0); // keep randomness reproducible ;-)
  for (int i = 0; i < numElements; i++)
    random[i] = rand();

  double timeInverted = 0;
  double timeSorted   = 0;
  double timeRandom   = 0;

  // BubbleSort
  // sorted data
  timeSorted = seconds();
  bubbleSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  if (numElements < RestrictedSort)
  {
    // inverted data
    for (int i = 0; i < numElements; i++)
      data[i] = numElements - (i+1);
    timeInverted = seconds();
    bubbleSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

    // random data
    data = random;
    timeRandom = seconds();
    bubbleSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

    printf(" <tr>"
           "  <td>Bubble Sort</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           " </tr>",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf(" <tr>"
           "  <td>Bubble Sort</td>"
           "  <td>%.3fms</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           " </tr>",
           1000*timeSorted);

  // SelectionSort
  if (numElements < RestrictedSort)
  {
    // inverted data
    for (int i = 0; i < numElements; i++)
      data[i] = numElements - (i+1);
    timeInverted = seconds();
    selectionSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

    // sorted data
    timeSorted = seconds();
    selectionSort(data.begin(), data.end());
    timeSorted = fabs(seconds() - timeSorted);

    // random data
    data = random;
    timeRandom = seconds();
    selectionSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

    printf(" <tr>"
           "  <td>Selection Sort</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           " </tr>",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf(" <tr>"
           "  <td>Selection Sort</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           " </tr>");

  // InsertSort
  // sorted data
  timeSorted = seconds();
  insertSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  if (numElements < RestrictedSort)
  {
    // inverted data
    for (int i = 0; i < numElements; i++)
      data[i] = numElements - (i+1);
    timeInverted = seconds();
    insertSort(data.begin(), data.end());
    timeInverted = fabs(seconds() - timeInverted);

    // random data
    data = random;
    timeRandom = seconds();
    insertSort(data.begin(), data.end());
    timeRandom = fabs(seconds() - timeRandom);

    printf(" <tr>"
           "  <td>Insertion Sort</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           "  <td>%.3fms</td>"
           " </tr>",
           1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));
  }
  else
    printf(" <tr>"
           "  <td>Insertion Sort</td>"
           "  <td>%.3fms</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           "  <td>n/a</td>"
           " </tr>",
           1000*timeSorted);

  // ShellSort
  // inverted data
  for (int i = 0; i < numElements; i++)
    data[i] = numElements - (i+1);
  timeInverted = seconds();
  shellSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

  // sorted data
  timeSorted = seconds();
  shellSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  // random data
  data = random;
  timeRandom = seconds();
  shellSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

  printf(" <tr>"
         "  <td>Shell Sort</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         " </tr>",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // QuickSort
  // inverted data
  for (int i = 0; i < numElements; i++)
    data[i] = numElements - (i+1);
  timeInverted = seconds();
  quickSort(data.begin(), data.end());
  timeInverted = fabs(seconds() - timeInverted);

  // sorted data
  timeSorted = seconds();
  quickSort(data.begin(), data.end());
  timeSorted = fabs(seconds() - timeSorted);

  // random data
  data = random;
  timeRandom = seconds();
  quickSort(data.begin(), data.end());
  timeRandom = fabs(seconds() - timeRandom);

  printf(" <tr>"
         "  <td>Quick Sort</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         " </tr>",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // std::sort
  // inverted data
  for (int i = 0; i < numElements; i++)
    data[i] = numElements - (i+1);
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

  printf(" <tr>"
         "  <td>std::sort</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         " </tr>",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  // std::stable_sort
  // inverted data
  for (int i = 0; i < numElements; i++)
    data[i] = numElements - (i+1);
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

  printf(" <tr>"
         "  <td>std::stable_sort</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         "  <td>%.3fms</td>"
         " </tr>",
         1000*timeSorted, 1000*timeInverted, 1000*timeRandom, 1000*(timeSorted+timeInverted+timeRandom));

  printf("</table>\n");
  return 0;
}
