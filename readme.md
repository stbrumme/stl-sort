# STL sort - C++ implementation of sorting algorithms

The interface is identical to `std::sort`, that means you provide two iterators and an optional less-than functor.
Just replace `std::sort` by one of my algorithms, e.g. `shellSort` - that's it !

Included algorithms:
- Bubble Sort
- Selection Sort
- Insertion Sort
- Shell Sort
- Heap Sort
- Merge Sort
- Merge Sort (in-place)
- Quick Sort
- Intro Sort

Note: unlike the original `std::sort`, my code works with `std::list`, too.

See my website https://create.stephan-brumme.com/stl-sort/ for a live demo, code examples and benchmarks.
