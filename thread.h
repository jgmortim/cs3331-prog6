/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/26/2019                                       */
/* PROGRAM ASSIGNMENT #6                                       */
/* FILE NAME : thread.h                                        */
/* PROGRAM PURPOSE :                                           */
/*    Contains all class definitions for my threads.           */
/* ----------------------------------------------------------- */

#include "ThreadClass.h"

class P : public Thread {
	public:
		P(int index, int id, int left); // Constructor.
		~P(); // Destructor.
		void NewP(int n); // Create a new P thread.
		int GetID(); // gets thread ID.
		SynOneToOneChannel *channel; // Channel to communicate with previous thread.
	private:
		void ThreadFunc();
		P *Pnext; // Pointer to next P thread. Null if last thread.
		int ID;
		int Index;
		
};

class Master : public Thread {
	public:
		Master(int n);
	private:
		void ThreadFunc();
		void PrintResults();
		P *P2; // pointer to P2.
		int nMax;
		
};
