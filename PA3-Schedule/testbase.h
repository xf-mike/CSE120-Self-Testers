/* same as sys.h
 */

#include <setjmp.h>

/*	Processes
 */
#define MAXPROCS	10	// max processes: DO NOT CHANGE

/*	Scheduling
 */
#define NOSCHEDPOLICY	0
#define ARBITRARY	1	// choose arbitrarily (any process is OK)
#define FIFO		2
#define LIFO		3
#define ROUNDROBIN	4
#define MLFQ		5
#define PROPORTIONAL	6

#define LMAX	12 // orignal is 12

/*	Shared Memory
 */
#define MAXSHM		1024	// max shared memory size (bytes): DO NOT CHANGE

/*	Semaphores
 */
#define MAXSEMS		100	// max semaphores: DO NOT CHANGE

/*	Kernel procedures: can only be called from within the kernel
 */

/*	PA1: Context switching
 */

typedef struct {
        int pid;		// process ID
        jmp_buf env;		// jmp_buf
} CONTEXT;

int SwitchContext(int p);	// switch context to process p
int GetCurProc();		// returns current process's ID

void SetCurProc(int);		// DO NOT USE: HERE ONLY FOR RestoreContext
void SetCurThread(int);		// DO NOT USE: HERE ONLY FOR RestoreContext

				// keep SaveContext/RestoreContext as MACROs
#define SaveContext(C)		(C)->pid = GetCurProc (); \
				setjmp ((C)->env)

#define RestoreContext(C)	SetCurThread ((C)->pid); \
 				SetCurProc ((C)->pid); \
 				longjmp ((C)->env, 1)

/*	PA2: Thread System
 */

/*	PA3: Scheduling
 */
void SetTimer(int t);		// timer to interrupt after t ticks
int GetTimer();			// returns the current value of timer
int SetSchedPolicy(int p);	// set the scheduling policy to p
int GetSchedPolicy();		// return the scheduling policy
void _DoSched();			// make sched decision at next opportune time

/*	PA4: Synchronization
 */
int Block();			// make curr process ineligible for scheduling
int Unblock(int p);		// make process p eligible for scheduling

/*	Other
 */
void Panic(char *s);		// causes abnormal exit from kernel, print s

/* same as mycode3.h
 */
void InitSched();
int StartingProc(int pid);
int EndingProc(int pid);
int SchedProc();
void HandleTimerIntr();
int MyRequestCPUrate(int pid, int n);

// New
void InitTest();
void EndTest();
int EndCurrentProc();
void mute();
void unmute();
void clearQuantumCount();
void printQuantumCount();