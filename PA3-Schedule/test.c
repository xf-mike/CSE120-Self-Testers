#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "testbase.h"

void test0() {
	// EMPTY TEST should raise no error

}

void test1() {
	// SchedProc() may called more than once
	// before test inited
	// This is because you explicitl call DoSched()
	// and also, sys will automatically call once
	// That would be twice
	// The different doesn't matter
	// we only need to check the log between -test inited-
	// and -test ended-
	SetSchedPolicy(ARBITRARY);
	InitTest();
	EndTest();
}

void test2() {
	// leave current running process unclosed is OK
	// If there is any other hanging process, that is an issue
	SetSchedPolicy(ARBITRARY);
	InitTest();
	StartingProc(2);
	// EndingProc(1);
	EndTest();
}

void test3() {  // FIFO
	SetSchedPolicy(FIFO);
	InitTest();
	int i;
	for (i = 2; i <= MAXPROCS + 1; i++) {
		StartingProc(i);
	}
	for (i = 1; i <= MAXPROCS + 1; i++) {
		EndingProc(i);
	}
	EndTest();
}

void test4() {  // LIFO
	SetSchedPolicy(LIFO);
	InitTest();
	int i;
	for (i = 2; i <= MAXPROCS + 1; i++) {
		StartingProc(i);
	}
	for (; i > 0; i--) {
		EndingProc(i);
	}
	EndTest();
}

void test5() {  // ROUNDROBIN
	SetSchedPolicy(ROUNDROBIN);
	InitTest();
	int i;
	for (i = 2; i <= MAXPROCS + 1; i++) {
		HandleTimerIntr();
		StartingProc(i);
	}
	for (i = 0; i < MAXPROCS; i++) {
		EndCurrentProc();
		// 6,2,7,4,8,1,9,5,10,3
	}
	EndTest();
}

void test6() {  // MLFQ[LMAX=1]
	if (LMAX != 1) {
		DPrintf("You have to Manually set LMAX to 1 in testbase.h\n");
		DPrintf("Otherwise it is impossible to test the special case!\n");
		Exit(-1);
	}
	SetSchedPolicy(MLFQ);
	InitTest();         // p1.l = 0, p1.q = 1
	HandleTimerIntr();  // p1.l = 1, p1.s = 1, p1.q = 2, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 1, p1.q = 1, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 2, p1.q = 2, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 2, p1.q = 1, p1 run

    // 2进来，重置p1.q为2, p1.q ++
	StartingProc(2);    // p1.l = 1, p1.s = 3, p1.q = 2, p2 run

	HandleTimerIntr();  // p2.l = 1, p2.s = 1, p2.q = 2, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 3, p1.q = 1, p1 run

	DPrintf("↓ p1.L: 1 → 0. Keep Runing\n");
	HandleTimerIntr();  // p1.l = 0, p1.s = 0, p1.q = 1, p1 run
	DPrintf("↑ p1.L: 1 → 0. Keep Runing\n");

	HandleTimerIntr();  // p1.l = 1, p1.s = 1, p1.q = 2, p2 run
	HandleTimerIntr();  // p2.l = 1, p2.s = 1, p2.q = 1, p2 run

	HandleTimerIntr();  // p2.l = 1, p2.s = 2, p2.q = 2, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 1, p1.q = 1, p1 run

	HandleTimerIntr();  // p1.l = 1, p1.s = 2, p1.q = 2, p2 run
	HandleTimerIntr();  // p2.l = 1, p2.s = 2, p2.q = 1, p2 run

	
	HandleTimerIntr();  // p2.l = 1, p2.s = 3, p2.q = 2, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 2, p1.q = 1, p1 run

	HandleTimerIntr();  // p1.l = 1, p1.s = 3, p1.q = 2, p2 run
	HandleTimerIntr();  // p2.l = 1, p2.s = 3, p2.q = 1, p2 run

	DPrintf("↓ p2.L: 1 → 0. Keep Runing\n");
	HandleTimerIntr();  // p2.l = 0, p2.s = 0, p2.q = 1, p2 run
	DPrintf("↑ p2.L: 1 → 0. Keep Runing\n");
	
	HandleTimerIntr();  // p2.l = 1, p2.s = 1, p2.q = 2, p1 run
	HandleTimerIntr();  // p1.l = 1, p1.s = 3, p1.q = 1, p1 run
	
	DPrintf("↓ p1.L: 1 → 0. Keep Runing\n");
	HandleTimerIntr();  // p1.l = 0, p1.s = 0, p1.q = 1, p1 run
	DPrintf("↑ p1.L: 1 → 0. Keep Runing\n");

	HandleTimerIntr();  // p1.l = 1, p1.s = 1, p1.q = 2, p2 run

	EndCurrentProc();   // 2
	EndCurrentProc();   // 1
	EndTest();
}


void test7() {  // PFS L overflow test
	SetSchedPolicy(PROPORTIONAL);
	InitTest();
	MyRequestCPUrate(1, -1);
	MyRequestCPUrate(1, 101);
	MyRequestCPUrate(1, 100);
	MyRequestCPUrate(1, 0);
	int i;
	for (i = 2; i <= MAXPROCS; i++) {
		StartingProc(i);
	}
	for (i = 0; i < 10; i++) {
		HandleTimerIntr();
	}
	for (i = 0; i < MAXPROCS; i++) {
		EndCurrentProc();
		// 1,2,3,4,5,6,7,8,9,10
	}
	EndTest();
}

void test8() {  // PFS: each process should run at least once in k * 100 Qs
	SetSchedPolicy(PROPORTIONAL);
	InitTest();
	int i, k=9;
	for (i = 2; i <= k + 1; i++) {
		StartingProc(i);
	}
	MyRequestCPUrate(1, 99);

	clearQuantumCount();
	mute();
	for (i = 0; i < k * 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(1, 99);  // repeated request should have no effect
	}
	printQuantumCount();

	DPrintf("Again!");
	clearQuantumCount();
	for (i = 0; i < k * 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(2, 2);  // invalid request should have no effect
		MyRequestCPUrate(11, 2);  // invalid request should have no effect
		MyRequestCPUrate(2, 0);  // this request should have no effect
	}
	printQuantumCount();

	unmute();

	// Test whether CPU will be correctly allocated after requested CPU changed
	MyRequestCPUrate(1, 4);  // Sometime we can request more than current Free CPU
	HandleTimerIntr(); // then 2 will run

	// Test whether CPU will be correctly allocated after non-requesting process exit
	EndCurrentProc(); // end 2
	k--;

	clearQuantumCount();
	mute();
	for (i = 0; i < k * 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(1, 4);  // repeated request should have no effect
	}
	printQuantumCount();

	DPrintf("Again!");
	clearQuantumCount();
	for (i = 0; i < k * 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(2, 1);  // invalid request should have no effect
		MyRequestCPUrate(3, 0);  // this request should have no effect
	}
	printQuantumCount();
	// Exit();
	unmute();
	for (i = 0; i < MAXPROCS-1; i++) {
		EndCurrentProc();
		// 1,3,4,5,6,7,8,9,10
	}
	EndTest();
}

void test9() {  // PFS: 10% tolerance test
	SetSchedPolicy(PROPORTIONAL);
	InitTest();
	mute();
	int i, k=9;
	for (i = 2; i <= k + 1; i++) {
		StartingProc(i);
	}
	unmute();
	MyRequestCPUrate(1, 40);
	MyRequestCPUrate(2, 30);
	MyRequestCPUrate(3, 10);
	MyRequestCPUrate(4, 10);
	mute();

	// 100 quantum of unstable period
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(1, 101);  // invalid request should have no effect
	}
	// 100 quantum of stable period
	DPrintf("100 quantum of stable period!");
	clearQuantumCount();
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(2, 30);  // repeated request should have no effect
	}
	printQuantumCount();
	// another 100 quantum of stable period
	DPrintf("another 100 quantum of stable period!");
	clearQuantumCount();
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(5, 0);  // this request should have no effect
	}
	printQuantumCount();
	
	unmute();
	MyRequestCPUrate(4, 20);  // Sometime we can request more than current Free CPU
	// Test whether CPU will be correctly allocated after non-requesting process exit
	EndCurrentProc(); // end 1
	EndCurrentProc(); // end 2
	mute();

	// 100 quantum of unstable period
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(1, 10);  // invalid request should have no effect
	}
	// 100 quantum of stable period
	DPrintf("100 quantum of stable period!");
	clearQuantumCount();
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(2, 10);  // invalid request should have no effect
		MyRequestCPUrate(3, 10);  // repeated request should have no effect
	}
	printQuantumCount();
	// another 100 quantum of stable period
	DPrintf("another 100 quantum of stable period!");
	clearQuantumCount();
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
		MyRequestCPUrate(10, 0);  // this request should have no effect
	}
	printQuantumCount();
	
	unmute();


	for (i = 0; i < MAXPROCS-2; i++) {
		EndCurrentProc();
		// 1,2,3,4,5,6,7,8,9,10
	}
	EndTest();
}

void test10() {  // MLFQ: long term can increase the gap of quantum each process can get
	if (LMAX != 12) {
		DPrintf("You have to Manually set LMAX to 12 in testbase.h\n");
		Exit(-1);
	}
	SetSchedPolicy(MLFQ);
	InitTest();

	HandleTimerIntr();
	HandleTimerIntr();
	HandleTimerIntr();
	StartingProc(2);
	HandleTimerIntr();
	HandleTimerIntr();
	HandleTimerIntr();
	StartingProc(3);
	HandleTimerIntr();
	StartingProc(4);

	mute();
	int i;
	clearQuantumCount();
	// Q == 100
	for (i = 0; i < 100; i++) {
		HandleTimerIntr();
	}
	printQuantumCount();
	
	// Q == 1000
	for (i = 0; i < 1000; i++) {
		HandleTimerIntr();
	}
	printQuantumCount();

	// Q == 10000
	for (i = 0; i < 10000; i++) {
		HandleTimerIntr();
	}
	printQuantumCount();
	unmute();
	for (i = 0; i < 4; i++) {
		EndCurrentProc();
		// 1,2,3,4,5,6,7,8,9,10
	}
	EndTest();
}

void Main()
{
	test0();

	Exit();
}