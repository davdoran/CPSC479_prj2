#include "mpi.h"
#include "prj2_header.h"

//Function Definitions
int partition(int *array, int low, int high);
void quicksort(int *array, int low, int high);
int sort_recursive(int *array, int arrSize, int currProcRank, int maxRank, int rankIndex);

int main(int argc, char *argv[]) {

	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

  //Calculate Rankindex for the future shareRank calculation
  int rankPower = 0;
  while (pow(2, rankPower) <= rank)
    rankPower++;

  if (rank == 0) {
    int arraySize = 25000; //Variable to change the size of array to be sorted
    srand((unsigned)time(NULL) + rank*size); //Seed rand rum generator
    int randNums[arraySize];
    for(int i=0;i<arraySize;i++) {
      randNums[i]=rand()%1000;   //Generate number between 0 and 1000
    }

		std::cout << "Before Sorting:" << std::endl;

    for(int i=0;i<arraySize;i+=1000) {
			//List numbers
      std::cout << "Index: " << i << "="<< randNums[i] << " " << std::endl;
    }

    double start = MPI_Wtime(); //Start timer

    sort_recursive(randNums, arraySize, rank, size - 1, rankPower);

    double end = MPI_Wtime(); //End timer

		std::cout << "After Sorting:" << std::endl;

		for(int i=0;i<arraySize;i+=1000) {
			//List numbers
			std::cout << "Index:" << i << "="<< randNums[i] << " " << std::endl;
		}
		std::cout << "Execution time:" << end - start << std::endl;
	}
  else {
		//Get the size of the array to receive
		MPI_Status status;
		int subarraySize;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &subarraySize);
    int sourceProcess = status.MPI_SOURCE;
		int *subarray = (int*)malloc(subarraySize * sizeof(int));

		MPI_Recv(subarray, subarraySize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sort_recursive(subarray, subarraySize, rank, size - 1, rankPower);
		MPI_Send(subarray, subarraySize, MPI_INT, sourceProcess, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
