#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <time.h>
using namespace std;
#define NoOfLoops 100000

/*const struct timespec {
time_t tv_sec = 0;
long tv_nsec = 1;
}res, rem;*/

// the function being executed as thread 2 writing x
void* main_thread(void* parameterPtr) {

    // explicit cast of parameterPtr into a pointer to int
    char *xPtr = (char *) parameterPtr;

    // increment x up to 10 and write on screen
    while ( 1 ) {
        cout << *xPtr << flush;
        sleep(1);
        //nanosleep(&req,&rem); // wait a second
    }

    cout << endl << "reached end of incrementing x" << endl;

    // return NULL as function demands for a return value
    return NULL;
}

// main runs thread 1 writing y
// and creates 2nd thread to write x in parallel
int main() {
    sched_setaffinity(getpid(), (size_t) cpusetsize, (cpu_set_t *) mask);

    // variables we modify in two threads
    char x = 'A', y = 'B';

    // show the initial values of x and y
    //cout << "start count x=" << x << ", y=" << y << endl;

    // thread ID for second thread
    pthread_t thread2_id;
    pthread_attr_t attr2;
    // thread ID for second thread
    pthread_t thread3_id;
    pthread_attr_t attr3;

    struct sched_param schedparam2,schedparam3;

    // Priority
    schedparam2.sched_priority = 20;
    schedparam3.sched_priority = 20;

    // FIFO 1 - 1   AABBAABBAABB
    // FIFO 1 - 99  ABABABABABA
    // RR 1-1       AABBAABBAABB
    // RR 1 - 99    AABBAABBAABB

    // The pthread_attr_init() function initializes the thread attributes object pointed to by attr with default attribute values
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    /*
    Various "real-time" policies are also supported, for special time-
    critical applications that need precise control over the way in which
    runnable threads are selected for execution.  For the rules governing
    when a process may use these policies, see sched(7).  The real-time
    policies that may be specified in policy are:

    SCHED_FIFO    a first-in, first-out policy; and

    SCHED_RR      a round-robin policy.
    */

    pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
    pthread_attr_setschedparam(&attr2, &schedparam2);

    pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr3, SCHED_FIFO);
    pthread_attr_setschedparam(&attr3, &schedparam3);

    // create second thread executing function thread2_main */
    if(pthread_create(&thread2_id, &attr2, main_thread, &x)) {
        //cerr << "Error: thread not created" << endl;
        return 1;
    }

    // create second thread executing function thread2_main */
    if(pthread_create(&thread3_id, &attr3, main_thread, &y)) {
        //cerr << "Error: thread not created" << endl;
        return 1;
    }
    //sleep 2 seconds, than start incrementing y up to 5
    //sleep(2);

    // increment y up to 5 and write on screen
    //while ( ++y < NoOfLoops ) {
        //cout << "A" << flush;
        //sleep(1); // wait a second
    //}

    //cout << endl << "reached end of incrementing y" << endl;

    // wait for the second thread till it finishes
    if ( pthread_join(thread2_id, NULL) ) {
        //cerr << "Error: thread not joined" << endl;
        return 2;
    }

    // wait for the second thread till it finishes
    if ( pthread_join(thread3_id, NULL) ) {
        //cerr << "Error: thread not joined" << endl;
        return 3;
    }

    // the results - x is now 10, thank you thread 2
    //cout << endl << "end count x=" << x << ", y=" << y << endl;
    return 0;
}
