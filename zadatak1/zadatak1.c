#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int prank, csize;
    int received_numbers[csize];
    int sent_numbers[csize];
    
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    srand(time(NULL) + prank);
   
    //Inicijalizacija niza za primljene i poslate brojeve
    for (int i = 0; i < csize; i++) {
        received_numbers[i] = -1;
        sent_numbers[i] = -1;
    }

    //Slanje različitih brojeva svakom drugom procesu
    for (int dest = 0; dest < csize; dest++) {
        if (dest != prank && sent_numbers[dest] == -1) {
            int send_number;
            do {
                send_number = (prank * 10) + (rand() % 10);  // Prva cifra odgovara broju procesa
            } while (send_number % 10 == prank || received_numbers[dest] == send_number);  // Provera da li se ponavlja druga cifra ili broj već primljen od istog procesa
           
            MPI_Send(&send_number, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            
            sent_numbers[dest] = send_number; 
        }
    }

    // Prijem poruka od drugih procesa
    for (int source = 0; source < csize; source++) {
        if (source != prank) {
            MPI_Recv(&received_numbers[source], 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    printf("Process %d received:", prank);
    for (int i = 0; i < csize; i++) {
        if (received_numbers[i] != -1) {
        
            printf(" %d", received_numbers[i]);
        
        }
    }
    printf("\n");

    MPI_Finalize();

    return 0;
}

