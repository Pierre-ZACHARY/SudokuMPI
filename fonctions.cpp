//
// Created by sophie on 05/02/2020.
//
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int* gen_sudoku(int k) {
    int* sudoku = new int[k*k*k*k];
    int* ligne = new int[k*k];
    for (int i=0; i<k*k; i++)
        ligne[i] = i+1;
    int perm = 0;
    for (int i=0; i<k*k; i++) {
        for (int j = 0; j < k * k; j++)
            sudoku[i * k * k + j] = ligne[(j + perm) % (k * k)];
        perm+=3;
    }
    return sudoku;
}
int verification(int* tab, int n)
{

    for (int i=0; i<n; i++)
        cout << tab[i] << " ";
    cout << endl;
    int val = 0;
    int* ref = new int[n];
    for (int i=0; i<n; i++)
        ref[i] = 0;
    for (int i=0; i<n; i++)
        ref[tab[i]-1]=1;
    for (int i=0; i<n; i++)
        if (ref[i]==0)
            val=1;
    cout << "verification=" << val << endl;
    delete[] ref;
    return val;
}