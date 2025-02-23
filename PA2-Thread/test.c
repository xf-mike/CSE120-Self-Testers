#include "mycode2.h"
#include "aux.h"
#include "umix.h"

/*
Example Output:

Queue is empty. Schedule will do nothing
thread 0 called Schedule and get CPU again
Created New Thread ID: 1
Created New Thread ID: 2
Created New Thread ID: 3
Created New Thread ID: 4
Created New Thread ID: 5
Thread 0 is yielding to 5
Thread 5 is activated with parameter 0
Thread 5 is exting 0
Thread 1 is activated with parameter 0
Thread 1 is exting 0
Thread 2 is activated with parameter 0
Thread 2 is yielding to 0
Thread 0 resume from 2
Thread 0 is exting
Thread 3 is activated with parameter 0
Created New Thread ID: 6
Created New Thread ID: 7
Created New Thread ID: 8
Created New Thread ID: 9
Created New Thread ID: 0
Created New Thread ID: 1
Thread 3 is yielding to 0
Thread 0 is activated with parameter 1
Thread 0 is yielding to 1
Thread 1 is activated with parameter 1
Thread 1 is exting 1
Thread 4 is activated with parameter 0
Thread 4 give up CPU. Scheduling...
Thread 2 resume from -1
Thread 2 give up CPU. Scheduling...
Thread 6 is activated with parameter 1
Thread 6 is yielding to 8
Thread 8 is activated with parameter 1
Thread 8 is yielding to itself
Thread 8 resume from 8
Thread 8 give up CPU. Scheduling...
Thread 7 is activated with parameter 1
Thread 7 is yielding to itself
Thread 7 resume from 7
Thread 7 give up CPU. Scheduling...
Thread 9 is activated with parameter 1
Thread 9 is yielding to 3
Thread 3 resume from 9
Created New Thread ID: 5
Created New Thread ID: 1
Created New Thread should fail: -1
Created New Thread should fail again: -1
Thread 3 give up CPU. Scheduling...
Thread 0 resume from -1
Thread 0 is yielding to 3
Thread 3 is resume from schedule, now exting...
Thread 4 is resume from schedule, now exting...
Thread 2 is resume from schedule, now exting...
Thread 6 resume from -1
Thread 6 give up CPU. Scheduling...
Thread 8 is resume from schedule, now exting...
Thread 7 is resume from schedule, now exting...
Thread 9 resume from -1
Thread 9 give up CPU. Scheduling...
Thread 5 is activated with parameter 1
Thread 5 is exting 1
Thread 1 is activated with parameter 1
Thread 1 is exting 1
Thread 0 resume from -1
Thread 0 give up CPU. Scheduling...
Thread 6 is resume from schedule, now exting...
Thread 9 is resume from schedule, now exting...
Thread 0 is resume from schedule, now exting...
*/


void ThreadFunc(int p) {
    DPrintf("Thread %d is activated with parameter %d\n", MyGetThread(), p);

    if (MyGetThread() == 5 || MyGetThread() == 1) {
        DPrintf("Thread %d is exting %d\n", MyGetThread());
        MyExitThread();

    } else if (MyGetThread() == 2){
        DPrintf("Thread %d is yielding to %d\n", MyGetThread(), p);
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(p));

    } else if (MyGetThread() == 3){
        int i;
        for (i = 0; i < 6; i++) {
            DPrintf("Created New Thread ID: %d\n", MyNewThread(ThreadFunc, 1));
        }
        
        DPrintf("Thread %d is yielding to %d\n", MyGetThread(), 0);
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(0));

        for (i = 0; i < 2; i++) {
            DPrintf("Created New Thread ID: %d\n", MyNewThread(ThreadFunc, 1));
        }

        DPrintf("Created New Thread should fail: %d\n", MyNewThread(ThreadFunc, 1));
        DPrintf("Created New Thread should fail again: %d\n", MyNewThread(ThreadFunc, 1));

    } else if (MyGetThread() == 0){
        DPrintf("Thread %d is yielding to %d\n", MyGetThread(), p);
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(p));
        
        DPrintf("Thread %d is yielding to %d\n", MyGetThread(), 3);
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(3));

    } else if (MyGetThread() == 6){
        DPrintf("Thread %d is yielding to %d\n", MyGetThread(), 8);
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(8));
    
    } else if (MyGetThread() == 7 || MyGetThread() == 8){
        DPrintf("Thread %d is yielding to itself\n", MyGetThread());
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(MyGetThread()));

    } else if (MyGetThread() == 9){
        DPrintf("Thread %d is yielding to %d\n", MyGetThread(), 3);
        DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(3));
    }
    
    
    DPrintf("Thread %d give up CPU. Scheduling...\n", MyGetThread());
    MySchedThread();
    DPrintf("Thread %d is resume from schedule, now exting...\n", MyGetThread());
    
}

void Main() {
    MyInitThreads();
    DPrintf("Queue is empty. Schedule will do nothing\n");
    MySchedThread();
    DPrintf("thread %d called Schedule and get CPU again\n", MyGetThread());
    int i;

    for (i = 0; i < 5; i++) {
        DPrintf("Created New Thread ID: %d\n", MyNewThread(ThreadFunc, 0));
    }
    
    DPrintf("Thread %d is yielding to %d\n", MyGetThread(), 5);
    DPrintf("Thread %d resume from %d\n", MyGetThread(), MyYieldThread(5));
    
    DPrintf("Thread %d is exting\n", MyGetThread());
    MyExitThread();

}