// Author: Edward Chan
#include "mpi.h"

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	static const int MASTER = 0, START = 10, PRINT = 20;
	int token, currRank, numNodes, len;
	MPI_Comm_rank(MPI_COMM_WORLD, &currRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numNodes);
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Get_processor_name(processor_name, &len);
	
	if (currRank == MASTER) {
		token = START;
		// Start the ball rolling
		MPI_Send(&token, 1, MPI_INT, (currRank + 1) % numNodes, 0, MPI_COMM_WORLD);

		// Rank of the expected processor
		int expected = MASTER + 1;

		//Receive token in order
		while (numNodes - expected) {
			MPI_Recv(&token, 1, MPI_INT, expected, 0, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			printf("Hello world from Processor %d\n", expected);
			expected++;
		} 
	}
	else {
		// Receive token from previous node
		MPI_Recv(&token, 1, MPI_INT, currRank - 1, 0, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// Send reply to master node
		MPI_Send(&PRINT, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);

		// Send token to next node
		MPI_Send(&token, 1, MPI_INT, (currRank + 1) % numNodes, 0, MPI_COMM_WORLD);

	}
	MPI_Finalize();	
}
