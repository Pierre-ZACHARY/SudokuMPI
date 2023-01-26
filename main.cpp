//
// Created by tpuser on 26/01/23.
//

#include <mpi.h>
#include <iostream>
#include "fonctions.h"
using namespace std;

int main(int argc, char** argv) {
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    cout << "Hello from process " << pid << " out of " << nprocs << endl;
    int k = atoi(argv[1]);
    if(k!=nprocs){
        cout<<"k must be equal to the number of processes"<<endl;
        MPI_Finalize();
        return 0;
    }
    int root = 0;
    int* sudoku;
    if(pid==root){
        cout<<"(root) k="<<k<<endl;
        sudoku = gen_sudoku(k);

        for (int i=0; i<k*k; i++) {
            for (int j=0; j<k*k; j++)
                cout << sudoku[i*k*k+j] << " ";
            cout << endl;
        }
    }
    else{
        sudoku = new int[k*k*k];
    }
    MPI_Win win;
    if(pid==root) MPI_Win_create(nullptr, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    else MPI_Win_create(sudoku, k*k*k*sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    // distribute the sudoku
    if(pid==root){
        for(int i =0; i<nprocs; i++){
            if(pid!=i){
                MPI_Win_lock(MPI_LOCK_EXCLUSIVE, i, 0, win);
                MPI_Put(sudoku+i*(k*k*k), k*k*k, MPI_INT, i, 0, k*k*k, MPI_INT, win);
                MPI_Win_unlock(i, win);
            }
        }
    }

    MPI_Win_fence(0, win);

    // print the sudoku on each process
    cout << "pid=" << pid << endl;
    for(int i =0; i<k; i++){
        for(int j =0; j<k*k; j++){
            cout<<sudoku[i*k*k+j]<<" ";
        }
        cout<<endl;
    }
    MPI_Win_fence(0, win);

    bool* should_stop = new bool(false);
    MPI_Win stop;
    MPI_Win_create(should_stop, sizeof(bool), sizeof(bool), MPI_INFO_NULL, MPI_COMM_WORLD, &stop);

    // verify the sudoku
    int val = 0;

    for(int i =0; i<k; i++){ // verifie les lignes
        // le début de la ligne est à l'indice i*k*k
        if(should_stop) break;
        val += verification(sudoku+i*k*k, k*k);
    }
#pragma omp for collapse(3)
    for(int i = 0; i<k; i++){ // pour chaque bloc
//        if(*should_stop) break;
        int* bloc = new int[k*k];
        for(int g = 0; g<k; g++){// pour chaque ligne du bloc
            for(int j = 0; j<k; j++) // pour chaque colonne du bloc
                bloc[g*k+j] = sudoku[i*k+j+g*k];
        }
        // verification du bloc
        val += verification(bloc, k*k);
    }
    MPI_Win_fence(0, win);


    // print val for each process
    cout << "pid=" << pid << " val=" << val << endl;



    MPI_Finalize();
    return 0;
}