/*
TEST INJECTOR
*/


int quantumCount[MAXPROCS] = {0};
int totalQuantum = 0;

void clearQuantumCount() {
    totalQuantum = 0;
    int i;
    for (i = 0; i < MAXPROCS; i++) {
		quantumCount[i] = 0;
	}
}

int userMode = 0;  // Start with kernel mode

void InitTest() {
    userMode = 1;
    clearQuantumCount();
    InitSched();
	StartingProc(1);
    DPrintf("------test inited-------\n");
}

int currentP = 0;

int needScheduleTimes = 1;

void EndTest() {
    DPrintf("------test ended--------\n");
    if (currentP != 0) { 
        EndCurrentProc();  // Leave current process unclosed is OK
    }
    // Exit();
    // currentP = _SchedProc();
    while (currentP != 0) {
        DPrintf("warn: hanging process %d\n", currentP);  // Leave hanging process unclosed is BAD!
        EndCurrentProc();
    }
    userMode = 0;
    clearQuantumCount();
}


void updatedQuantumCount(int p) {
    // DPrintf("updatedQuantumCount(%d)\n", p);
    quantumCount[p-1]++;
    totalQuantum++;
}

void printQuantumCount() {
    DPrintf("\n---------QuantumCount----------\n");
    int i;
    for (i = 0; i < MAXPROCS; i++) {
        if (quantumCount[i] > 0) {
            DPrintf("Process[%d]: %d / %d\n", i + 1, quantumCount[i], totalQuantum);
        }
    }
    
    DPrintf("---------------------------------\n");
}

int SchedProc()
{
    if (currentP!=0) {
        updatedQuantumCount(currentP);
    }
    currentP = _SchedProc();
    if (userMode) {
        DPrintf("SchedProc() → %d\n", currentP);
    }
    if (currentP == 0) {
        return 0;
    } else {
        return 1;
    }
}

void _DoSched() {
    needScheduleTimes++;
}

void ExecuteSched() {
    while (needScheduleTimes > 0) {
        SchedProc();
        needScheduleTimes--;
    }
}

int StartingProc(int p) {
    int s = _StartingProc(p);

    if (userMode) {
        if (s == 1) {
            DPrintf("StartingProc(%d): Success\n", p);
        } else {
            DPrintf("StartingProc(%d): Fail\n", p);
        }
    }
    ExecuteSched();
    return s;
}

int EndingProc(int p) {
    int s = _EndingProc(p);

    if (userMode) {
        if (s == 1) {
            DPrintf("EndingProc(%d): Success\n", p);
        } else {
            DPrintf("EndingProc(%d): Fail\n", p);
        }

        needScheduleTimes++;
        ExecuteSched();
    }
    return s;
}

int EndCurrentProc(int p) {
    return EndingProc(currentP);
}

void HandleTimerIntr()
{
    _HandleTimerIntr();
    if (userMode) {
        DPrintf("Interrupt Occur\n");
    }
    ExecuteSched();
}

int MyRequestCPUrate(int p, int n)
{
    if (userMode) {
        int s = _MyRequestCPUrate(p, n);
        if (s == 0) {
            DPrintf("MyRequestCPUrate(p=%d, n=%d): Success\n", p, n);
        } else {
            DPrintf("MyRequestCPUrate(p=%d, n=%d): Fail\n", p, n);
        }
        ExecuteSched();
    }
}

void mute() {
    userMode = 0;
}

void unmute() {
    userMode = 1;
}

