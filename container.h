// //////////////////////////////////////////////////////////
// container.h
// Copyright (c) 2014,2020 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#pragma once

#include <iterator>
#include <vector>


// stored data type
typedef int Value;


// you can choose between
// - forward iterator,
// - bidirectional iterator and
// - random access iterator

// uncomment at most one line to switch between those three iterators
//#define BIDIRECTIONALITERATOR
#define RANDOMACCESSITERATOR
// fallback if neither BIDIRECTIONALITERATOR nor RANDOMACCESSITERATOR is defined
#if !defined(BIDIRECTIONALITERATOR) && !defined(RANDOMACCESSITERATOR)
  #define FORWARDITERATOR
#endif

// forward declaration
class Container;

/// can be forward iterator, bidirectional or random access (see #define above)
class Iterator :
#if !defined(BIDIRECTIONALITERATOR) && !defined(RANDOMACCESSITERATOR)
                 public std::iterator<std::forward_iterator_tag,       int>
#elseif defined(BIDIRECTIONALITERATOR) && !defined(RANDOMACCESSITERATOR)
                 public std::iterator<std::bidirectional_iterator_tag, int>
#else
                 public std::iterator<std::random_access_iterator_tag, int>
#endif
{
public:
  typedef Value value_type;

  /// construct empty iterator
  Iterator() : iterator() {}
  /// construct new iterator
  Iterator(const std::vector<Value>::iterator& ite) : iterator(ite) {}

  /// only copy position, skip container
  void operator=(const Iterator& other) { iterator = other.iterator; }

  /// step forward (prefix)
  Iterator& operator++()    { ++iterator; return *this; }
  /// step forward (postfix)
  Iterator  operator++(int) { Iterator result(*this); ++iterator; return result; }

#if defined(BIDIRECTIONALITERATOR) || defined(RANDOMACCESSITERATOR)
  /// step backward (prefix)
  Iterator& operator--()    { --iterator; return *this; }
  /// step backward (postfix)
  Iterator  operator--(int) { Iterator result(*this); ++iterator; return result; }
#endif

#if defined(RANDOMACCESSITERATOR)
  /// leap forward
  Iterator operator+ (std::iterator_traits<Iterator>::difference_type howFar) const
                     { return iterator + howFar; }
  /// leap backward
  Iterator operator- (std::iterator_traits<Iterator>::difference_type howFar) const
                     { return iterator - howFar; }

  /// leap forward
  Iterator operator+=(std::iterator_traits<Iterator>::difference_type howFar)
                     { iterator += howFar; return *this; }
  /// leap backward
  Iterator operator-=(std::iterator_traits<Iterator>::difference_type howFar)
                     { iterator -= howFar; return *this; }

  /// ordered iterators
  bool operator<(const Iterator& other) const { return iterator < other.iterator; }
  /// ordered iterators
  bool operator>(const Iterator& other) const { return iterator > other.iterator; }

  /// measure distance
  std::iterator_traits<Iterator>::difference_type operator-(const Iterator& other) const
                     { return iterator - other.iterator; }
#endif

  /// pointing to same element ?
  bool operator==(const Iterator& other) const { return iterator == other.iterator; }
  /// pointing to different element ?
  bool operator!=(const Iterator& other) const { return iterator != other.iterator; }

  /// dereference
  int& operator*()       { return *iterator; }
  /// dereference
  int  operator*() const { return *iterator; }

private:
  /// has random-access iterator
  std::vector<Value>::iterator iterator;
};


/// only implements iterators and element access
class Container
{
public:
  /// construct vector
  explicit Container(size_t initialSize = 0) : container(initialSize) {}

  /// return iterator pointing to first element
  Iterator begin() { return container.begin(); }
  /// return iterator pointing beyond last element
  Iterator end()   { return container.end();   }

  /// read/write access to an element
  int& operator[](size_t pos)                   { return container[pos]; }
  /// read/write access to an element
  bool operator!=(const Container& other) const { return container != other.container; }

private:
  /// actually just a simple vector
  std::vector<Value> container;
};
