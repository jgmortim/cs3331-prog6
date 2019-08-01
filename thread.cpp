/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/26/2019                                       */
/* PROGRAM ASSIGNMENT #6                                       */
/* FILE NAME : thread.cpp                                      */
/* PROGRAM PURPOSE :                                           */
/*    Contains all class implementations for my threads.       */
/* ----------------------------------------------------------- */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "thread.h"
#define SIZEINT sizeof(int)
#define SIZEOUTPUT 1024
#define END -1

using namespace std;
int Primes[25]; // 3 <= n <= 100. There are 25 primes less than 100.

/* ----------------------------------------------------------- */
/* FUNCTION : constructor                                      */
/*    Creates a new Master thread. Also creates P2.            */
/* PARAMETER USAGE :                                           */
/*    n {int} - max number to check for primality.             */
/* FUNCTIONS CALLED :                                          */
/*    memset(), P.P(), sizeof(), ThreadName.seekp()            */
/* ----------------------------------------------------------- */
Master::Master(int n) {
	nMax=n;
	UserDefinedThreadID = 0;
	memset(Primes, 0, sizeof(Primes));
	ThreadName.seekp(0, ios::beg);
	ThreadName << "Master" << '\0';
	P2 = new P(1, 2, UserDefinedThreadID);
}

/* ----------------------------------------------------------- */
/* FUNCTION : constructor                                      */
/*    Creates a new P thread.                                  */
/* PARAMETER USAGE :                                           */
/*    index {int} - the index of the thread (P2 at index 1).   */
/*    id {int} - the id of the thread.                         */
/*    left {int} - the id of the left thread (Master is 0).    */
/* FUNCTIONS CALLED :                                          */
/*    sprintf(), SynOneToOneChannel.SynOneToOneChannel(),      */
/*    ThreadName.seekp()                                       */
/* ----------------------------------------------------------- */
P::P(int index, int id, int left) {
	char name[SIZEOUTPUT];
	ID=id;
	Index=index;
	UserDefinedThreadID = id;
	Pnext=NULL;
	sprintf(name, "channel-%d-%d", left, id);
	channel = new SynOneToOneChannel(name, left, id);
	ThreadName.seekp(0, ios::beg);
	ThreadName << "P" << ID << '\0';
}

/* ----------------------------------------------------------- */
/* FUNCTION : ThreadFunc                                       */
/*    Process for the Master thread to perform when begin() is */
/*    called.                                                  */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    Exit(), P.Begin(), PrintResults(), sprintf(), strlen(),  */
/*    SynOneToOneChannel.Send(), Thread.ThreadFunc(), write()  */
/* ----------------------------------------------------------- */
void Master::ThreadFunc() {
	Thread::ThreadFunc();
	char output[SIZEOUTPUT];
	int nCur=3;
	P2->Begin();
	sprintf(output, "Master starts\n");
	write(1, output, strlen(output));
	for(; nCur<=nMax; nCur++) { // Send all numbers to P2 starting with 3.
		sprintf(output, "Master sends %d to P2\n", nCur);
		write(1, output, strlen(output));
		P2->channel->Send(&nCur, SIZEINT);
	}
	nCur=END;
	sprintf(output, "Master sends END\n");
	write(1, output, strlen(output));
	P2->channel->Send(&nCur, SIZEINT); // Send END.
	P2->Join(); // Wait for all P threads to exit.
	PrintResults(); // Print the results.
	Exit(); // And exit.
}

/* ----------------------------------------------------------- */
/* FUNCTION : ThreadFunc                                       */
/*    Process for the P threads to perform when begin() is     */
/*    called.                                                  */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    Exit(), P.Join(), P.NewP(), sprintf(), strlen(),         */
/*    SynOneToOneChannel.Receive(), SynOneToOneChannel.Send(), */
/*    Thread.ThreadFunc(), write()                             */
/* ----------------------------------------------------------- */
void P::ThreadFunc() {
	Thread::ThreadFunc();
	char output[SIZEOUTPUT];
	int number=0;
	sprintf(output, "%*cP%d starts and memorizes %d\n", Index*2, ' ', ID, ID);
	write(1, output, strlen(output));
	Primes[Index-1] = ID;
	while (1) {
		channel->Receive(&number, SIZEINT); // Receive number.
		if (number == END) { // If receive END.
			sprintf(output, "%*cP%d recieves END\n", Index*2, ' ', ID);
			write(1, output, strlen(output));
			if (Pnext != NULL) { //If P is not the last thread
				Pnext->channel->Send(&number, SIZEINT); // Pass END.
				Pnext->Join(); // Wait for right thread to exit.
			}
			Exit(); // Then exit.
		}
		// If not END.
		sprintf(output, "%*cP%d recieves %d\n", Index*2, ' ', ID, number);
		write(1, output, strlen(output));
		if (number % ID != 0) {	// If not a multiple of i. 
			if (Pnext == NULL) NewP(number); // If P is last.      
			else { // If P is not last thread.
				sprintf(output, "%*cP%d sends %d to P%d\n", Index*2, ' ', ID, number, Pnext->GetID());
				write(1, output, strlen(output));
				Pnext->channel->Send(&number, SIZEINT); // Pass number.
			}
		}
	}
}

/* ----------------------------------------------------------- */
/* FUNCTION : NewP                                             */
/*    Creates a new P thread to the right of the calling       */
/*    thead.                                                   */
/* PARAMETER USAGE :                                           */
/*    n {int} - the prime number to remember.                  */
/* FUNCTIONS CALLED :                                          */
/*    P.Begin(), P.P(), sprintf(), strlen(), write()           */
/* ----------------------------------------------------------- */
void P::NewP(int n) {
	char output[SIZEOUTPUT];
	sprintf(output, "%*cP%d creates P%d\n", Index*2, ' ', ID, n);
	write(1, output, strlen(output));
	Pnext = new P(Index+1, n, ID); // Create new P thread.
	Pnext->Begin(); // Start new thread.
}

/* ----------------------------------------------------------- */
/* FUNCTION : PrintResults                                     */
/*    Prints the results of the sieve.                         */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    sprintf(), strlen(), write()                             */
/* ----------------------------------------------------------- */
void Master::PrintResults() {
	char output[SIZEOUTPUT];
	sprintf(output, "Master prints the complete result:\n ");
	write(1, output, strlen(output));
	for (int i=0; Primes[i]!=0 && i<25; i++) {
		sprintf(output, " %d", Primes[i]);
		write(1, output, strlen(output));
	}
	sprintf(output, "\nMaster terminates\n");
	write(1, output, strlen(output));
}

/* ----------------------------------------------------------- */
/* FUNCTION : GetID                                            */
/*    Returns the ID of the thread.                            */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int P::GetID() {
	return ID;
}

/* ----------------------------------------------------------- */
/* FUNCTION : Destructor                                       */
/*    Destructor for P threads.                                */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
P::~P() {
	delete channel;
}

