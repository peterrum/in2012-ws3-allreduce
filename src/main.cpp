#include <iostream>
#include <mpi.h>
#include <cmath>

using namespace std;

template <class T>
int MPI_Allreduce_My(T *sendbuf, T *recvbuf, int count,
                     MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {

  // save local value
  T temp = *sendbuf;

  // my rank, number of processors, destination rank
  int rank, nproc, to;
  // size of subblock
  int distance = 1;

  // determin my rank and number of processors
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // number of communication levels log(n) with base 2
  int levels = log(nproc) / log(2);

  // go through all communication levels
  for (int i = 0; i < levels; i++) {
    // increase size of block
    distance *= 2;
    // determine destination rank
    to = (rank + distance / 2) % distance + (rank / distance) * distance;

    // send and resv values from neigbouring rank
    MPI_Sendrecv(sendbuf, count, datatype, to, 0, recvbuf, count,
                 datatype, to, 0, comm, MPI_STATUSES_IGNORE);

    // perform operations
    if (op == MPI_SUM) {
      *sendbuf += *recvbuf;
    } else if (op == MPI_MIN) {
      *sendbuf = min(*sendbuf, *recvbuf);
    } else if (op == MPI_PROD) {
      *sendbuf *= *recvbuf;
    }
  }

  // put final result into
  *recvbuf = *sendbuf;

  // reconstruct local value
  *sendbuf = temp;
}

template <class T>
int MPI_Reduce_My(T *sendbuf, T *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {

  // save local value
  T temp1 = *sendbuf;
  T temp2 = *recvbuf;

  // my rank, number of processors, destination rank
  int rank, nproc, to;
  // size of subblock
  int distance = 1;

  // determin my rank and number of processors
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // number of communication levels log(n) with base 2
  int levels = log(nproc) / log(2);

  // go through all communication levels
  for (int i = 0; i < levels; i++) {
    // increase size of block
    distance *= 2;
    if ((rank / distance) * distance == rank) {
      // determine if rank has to recv
      MPI_Recv(recvbuf, count, datatype,
               rank + distance / 2, 0, comm, MPI_STATUSES_IGNORE);

      // perform operations
      if (op == MPI_SUM) {
        *sendbuf += *recvbuf;
      } else if (op == MPI_MIN) {
        *sendbuf = min(*sendbuf, *recvbuf);
      } else if (op == MPI_PROD) {
        *sendbuf *= *recvbuf;
      }
      // determine if rank has to send
    } else if ((rank / distance) * distance + (distance / 2) == rank) {
      MPI_Send(sendbuf, count, datatype,
               (rank / distance) * distance, 0, comm);
    }

  }

  // put final result into
  if (rank) {
    *recvbuf = temp2;
  } else {
    *recvbuf = *sendbuf;
  }

  // reconstruct local value
  *sendbuf = temp1;
}

int main(int argc, char** argv) {

  // initialisation stuff
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  // initialise buffers
  int local = rank + 1;
  int global1, global2, global3;

  // perform reduction
  MPI_Reduce_My<int>(&local, &global1, 1,
                     MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD);
  MPI_Reduce_My<int>(&local, &global2, 1,
                     MPI_INTEGER, MPI_MIN, MPI_COMM_WORLD);
  MPI_Reduce_My<int>(&local, &global3, 1,
                     MPI_INTEGER, MPI_PROD, MPI_COMM_WORLD);
  //  MPI_Allreduce_My<int>(&local, &global1, 1,
  //                        MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD);
  //  MPI_Allreduce_My<int>(&local, &global2, 1,
  //                        MPI_INTEGER, MPI_MIN, MPI_COMM_WORLD);
  //  MPI_Allreduce_My<int>(&local, &global3, 1,
  //                        MPI_INTEGER, MPI_PROD, MPI_COMM_WORLD);

  // output results
  if (rank == 0) {
    std::cout << "sum on rank " << rank << ": " << global1 << std::endl;
    std::cout << "min on rank " << rank << ": " << global2 << std::endl;
    std::cout << "pro on rank " << rank << ": " << global3 << std::endl;

  }

  // finalise
  MPI_Finalize();

  return 0;
}

