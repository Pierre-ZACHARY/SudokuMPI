//
// Created by tpuser on 26/01/23.
//

#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    cout<<"Hello World from process "<<rank<<" out of "<<size<<endl;
    MPI_Finalize();
    return 0;
}