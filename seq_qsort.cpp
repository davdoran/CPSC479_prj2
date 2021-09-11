//recursively sorts array and times it

#include <chrono>
#include <stdio.h>
#include <iostream>

int partition(int *array, int low, int high) {
  /* Function to sort/partition the array
  IN: array of ints, lowest index to be included, and highest index to be included
  OUT: index of the pivot */
	int pivot = array[low];
	int i = low - 1;
	int j = high + 1;
	while (true) {
		do
			i = i + 1;
		while (array[i] < pivot);

		do
			j = j - 1;
		while (array[j] > pivot);

		if (i >= j)
			return j;

		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

void quicksort(int *array, int low, int high) {
  /* Function to recursively sort the array.
  IN: array of ints, lowest index to be included, and highest index to be included
  OUT: VOID*/
	if (low < high) {
		int p = partition(array, low, high);
		quicksort(array, low, p);
		quicksort(array, p + 1, high);
	}
}

int main(int argc, char const *argv[]) {

  int partition(int *array, int low, int high);
  void quicksort(int *array, int low, int high);

  int arraySize = 25000; //Variable to change the size of array to be sorted
  srand((unsigned)time(NULL)); //Seed rand rum generator
  int randNums[arraySize];
  for(int i=0;i<arraySize;i++) {
    randNums[i]=rand()%1000;   //Generate number between 0 and 1000
  }

  std::cout << "Before Sorting:" << std::endl;

  for(int i=0;i<arraySize;i+=1000) {
    //List numbers
    std::cout << "Index: " << i << "="<< randNums[i] << " " << std::endl;
  }

  auto begin = std::chrono::high_resolution_clock::now();
  quicksort(randNums, 0, arraySize - 1);
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  std::cout << "After Sorting:" << std::endl;

  for(int i=0;i<arraySize;i+=1000) {
    //List numbers
    std::cout << "Index:" << i << "="<< randNums[i] << " " << std::endl;
  }

  //std::cout << "Execution time:" << end - start << std::endl;

  printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
  return 0;
}
