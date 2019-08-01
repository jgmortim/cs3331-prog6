/* ----------------------------------------------------------- */
/* NAME : John Mortimore                     User ID: jgmortim */
/* DUE DATE : 04/26/2019                                       */
/* PROGRAM ASSIGNMENT #6                                       */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE :                                           */
/*    Sieve with Message Passing. 1,445,000 automated tests w/ */
/*    random n between 3 and 100 performed. No deadlock.       */
/* ----------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "thread.h"

using namespace std;
void PrintUsage();

/* ----------------------------------------------------------- */
/* FUNCTION : main                                             */
/*    The main function for the program.                       */
/* PARAMETER USAGE :                                           */
/*    argc {int} - number of argument.                         */
/*    argv {char**} - list of arguments.                       */
/*        argv[1] - the max number to check for primality.     */
/* FUNCTIONS CALLED :                                          */
/*    atoi(), Master.Begin(), Master.Join(), Master.Master()   */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	int n=0;
	if (argc == 1) n=30;
	else if (argc == 2) n = atoi(argv[1]);
	else {PrintUsage(); return 1;}
	if (n<3 || n>100) {PrintUsage(); return 1;}
	Master* master = new Master(n);
	master->Begin();
	master->Join();
	return 0;
}

/* ----------------------------------------------------------- */
/* FUNCTION : PrintUsage                                       */
/*    Print usage if input is incorrect. "You may assume that  */
/*    the largest value for n satisfies 3 <= n <= 100"         */
/*    http://www.csl.mtu.edu/cs3331.ck/www/PROG/PG6/prog6.html */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTIONS CALLED :                                          */
/*    strlen(), write()                                        */
/* ----------------------------------------------------------- */
void PrintUsage() {
	char *usage="Usage:\n prog6 <n>\n where 3<=n<=100\n";
	write(1, usage, strlen(usage));
}

