#ifndef MPI_QSORT
#define MPI_QSORT

#include <math.h>


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

int sort_recursive(int* array, int arrSize, int currProcRank, int maxRank, int rankIndex) {
	MPI_Status status;

  // Calculate the rank of sharing process
	int shareProc = currProcRank + pow(2, rankIndex);
	rankIndex++;

  // If no process is available, sort sequentially and return
	if (shareProc > maxRank) {
		quicksort(array, 0, arrSize - 1);
		return 0;
	}

  // Divide array in two parts with the pivot in between
	int j = 0;
	int pivotIndex;
	do {
		pivotIndex = partition(array, j, arrSize - 1);
		j++;
	} while (pivotIndex == j - 1);

	// Send partition, sort the remaining partitions, receive sorted partition
	if (pivotIndex <= arrSize - pivotIndex) {
		MPI_Send(array, pivotIndex - 1, MPI_INT, shareProc, pivotIndex, MPI_COMM_WORLD);
		sort_recursive((array + pivotIndex + 1), (arrSize - pivotIndex - 1), currProcRank, maxRank, rankIndex);
		MPI_Recv(array, pivotIndex - 1, MPI_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
	else {
		MPI_Send((array + pivotIndex + 1), arrSize - pivotIndex - 1, MPI_INT, shareProc, pivotIndex + 1, MPI_COMM_WORLD);
		sort_recursive(array, (pivotIndex + 1), currProcRank, maxRank, rankIndex);
		MPI_Recv((array + pivotIndex + 1), arrSize - pivotIndex - 1, MPI_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
}


#endif
