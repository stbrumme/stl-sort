// //////////////////////////////////////////////////////////
// lessthan.h
// Copyright (c) 2014 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#pragma once


/// provide only operator< and ==
class Number
{
public:
  /// set value
  Number(int x = 0) : value(x) { }

  /// needed for sorting algorithms
  bool operator< (const Number& other) const { return value <  other.value; }

#ifdef CHECKRESULT
  bool operator==(const Number& other) const { return value == other.value; }
#endif // CHECKRESULT

private:
  /// actually just a simple integer
  int value;
};
