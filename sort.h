// //////////////////////////////////////////////////////////
// sort.h
// Copyright (c) 2013 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

// g++ -O3 sort.cpp -o sort

// All sort algorithm follow the same syntax as std::sort
// i.e.: quickSort(container.begin(), container.end());
// They sort the container in-place
// You can provide your own less-than operator, too
// i.e.: quickSort(container.begin(), container.end(), myless());

#pragma once

#include <algorithm>  // std::iter_swap
#include <iterator>   // std::advance
#include <functional> // std::less


// use std::move if C++11 is enabled
#if    (__cplusplus__ >= 201103L) || __GXX_EXPERIMENTAL_CXX0X__
  #undef  __std_move__
  #define __std_move__(x) std::move(x)
#else
  #define __std_move__(x) (x)
#endif


/// Bubble Sort, allow user-defined less-than operator
template <typename iterator, typename LessThan>
void bubbleSort(iterator first, iterator last, LessThan lessThan)
{
  if (first == last)
    return;

  // usually "last" points beyond the last element
  // now it points directly to that last element
  --last;
  // only one element => it's sorted
  if (first == last)
    return;

  bool swapped;
  do
  {
    // reset swapped flag
    swapped = false;

    iterator current = first;
    while (current != last)
    {
      // bubble up
      iterator next = current;
      ++next;

      // two neighbors in wrong order ? swap them !
      if (lessThan(*next, *current))
      {
        std::iter_swap(current, next);
        swapped = true;
      }

      ++current;
    }

    // last element is already sorted now
    --last; // remove this line if you only have a forward iterator

    // last will move closer towards first
  } while (swapped && first != last);
}


/// Bubble Sort with default less-than operator
template <typename iterator>
void bubbleSort(iterator first, iterator last)
{
  bubbleSort(first, last, std::less<typename iterator::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Selection Sort, allow user-defined less-than operator
template <typename iterator, typename LessThan>
void selectionSort(iterator first, iterator last, LessThan lessThan)
{
  for (iterator current = first; current != last; ++current)
  {
    // find smallest element in the unsorted part and remember its iterator in "minimum"
    iterator minimum = current;
    iterator compare = current;
    ++compare;

    // walk through all still unsorted elements
    while (compare != last)
    {
      // new minimum found ? save its iterator
      if (lessThan(*compare, *minimum))
        minimum = compare;

      // next element
      ++compare;
    }

    // add minimum to the end of the already sorted part
    if (current != minimum)
      std::iter_swap(current, minimum);
  }
}


/// Selection Sort with default less-than operator
template <typename iterator>
void selectionSort(iterator first, iterator last)
{
  selectionSort(first, last, std::less<typename iterator::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Insertion Sort, allow user-defined less-than operator
template <typename iterator, typename LessThan>
void insertionSort(iterator first, iterator last, LessThan lessThan)
{
  if (first == last)
    return;

  // skip first element, consider it as sorted
  iterator current = first;
  ++current;

  // insert all remaining unsorted elements into the sorted elements
  while (current != last)
  {
    // insert "compare" into the already sorted elements
    const typename iterator::value_type compare = __std_move__(*current);

    // find location inside sorted range, beginning from the right end
    iterator pos = current;
    while (pos != first)
    {
      // stop if left neighbor is not smaller
      iterator left = pos;
      --left;
      if (!lessThan(compare, *left))
        break;

      // shift that left neighbor one position to the right
      *pos = __std_move__(*left);
      pos  = __std_move__( left); // same as --pos
    }

    // found final position
    *pos = __std_move__(compare);

    // sort next element
    ++current;
  }
}


/// Insertion Sort with default less-than operator
template <typename iterator>
void insertionSort(iterator first, iterator last)
{
  insertionSort(first, last, std::less<typename iterator::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Shell Sort, allow user-defined less-than operator
template <typename iterator, typename LessThan>
void shellSort(iterator first, iterator last, LessThan lessThan)
{
  const size_t numElements = std::distance(first, last);
  if (numElements <= 1)
    return;

  // sequence taken from Wikipedia (Marcin Ciura)
  static const size_t OptimalIncrements[] =
  { 68491, 27396, 10958, 4383, 1750, 701, 301, 132, 57, 23, 10, 4, 1, 0 };
  size_t incrementIndex = 0;
  size_t increment = OptimalIncrements[incrementIndex++];
  // increment must not be bigger than the number of elements to be sorted
  while (increment >= numElements)
    increment = OptimalIncrements[incrementIndex++];

  // stumble through all increments in descending order
  while (increment > 0)
  {
    iterator stripe = first;
    size_t offset   = increment;
    std::advance(stripe, offset);
    while (stripe != last)
    {
      // these iterators are always "increment" apart
      iterator right = stripe;
      iterator left  = stripe;
      std::advance(left, -int(increment));

      // value to be sorted
      typename iterator::value_type compare = *right;

      // note: stripe is simply the same as first + offset
      // but operator+() is expensive for non-random access iterators
      size_t pos = offset;
      // only look at values between "first" and "last"
      while (pos >= increment)
      {
        // found right spot ?
        if (!lessThan(compare, *left))
          break;

        // no, still not correct order: shift bigger element to the right
        *right = __std_move__(*left);

        // go one step to the left
        right  = left;
        std::advance(left, -int(increment));

        pos -= increment;
      }

      // found sorted position
      *right = __std_move__(compare);

      // next stripe
      ++stripe;
      ++offset;
    }

    // smaller increment
    increment = OptimalIncrements[incrementIndex++];
  }
}


/// Shell Sort with default less-than operator
template <typename iterator>
void shellSort(iterator first, iterator last)
{
  shellSort(first, last, std::less<typename iterator::value_type>());
}


// /////////////////////////////////////////////////////////////////////


/// Quick Sort, allow user-defined less-than operator
template <typename iterator, typename LessThan>
void quickSort(iterator first, iterator last, LessThan lessThan)
{
  size_t numElements = std::distance(first, last);
  // already sorted ?
  if (numElements <= 1)
    return;

  iterator pivot = last;
  --pivot;

  // choose middle element as pivot (good choice for partially sorted data)
  if (numElements > 2)
  {
    iterator middle = first;
    std::advance(middle, numElements/2);
    std::iter_swap(middle, pivot);
  }

  // scan beginning from left and right end and swap misplaced elements
  iterator left  = first;
  iterator right = pivot;
  while (left != right)
  {
    // look for mismatches
    while (!lessThan(*pivot, *left)  && left != right)
      ++left;
    while (!lessThan(*right, *pivot) && left != right)
      --right;
    // swap two values which are both on the wrong side of the pivot element
    if (left != right)
      std::iter_swap(left, right);
  }

  // pivot is on its final position
  if (lessThan(*pivot, *left))
    std::iter_swap(pivot, left);

  // subdivide
  quickSort(first,  left, lessThan);
  quickSort(++left, last, lessThan); // *left itself is already sorted
}


/// Quick Sort with default less-than operator
template <typename iterator>
void quickSort(iterator first, iterator last)
{
  quickSort(first, last, std::less<typename iterator::value_type>());
}

#undef __std_move__
