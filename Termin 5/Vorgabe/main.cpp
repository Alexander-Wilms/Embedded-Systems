#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>   // For mode constants
#include <fcntl.h>      // For O_ constants
#include <sys/time.h>
#include <CCommQueue.h>
#include <sys/time.h>
#include <sched.h>
#include "base/CMessage.h"

using namespace std;

#define SHM_NAME        "/estSHM"
#define QUEUE_SIZE      64
#define NUM_MESSAGES    10000

CBinarySemaphore myCBinarySemaphorePtr;
int len_sem = sizeof(CBinarySemaphore);
int len_queue = NUM_MESSAGES*sizeof(CMessage);

int main(void)
{
    int file_descriptor;

    // create child process via fork()
    cout << "Creating a child process ..." << endl;
    pid_t pid = fork();


    const sched_param myparam = {sched_get_priority_max(SCHED_FIFO)};

    sched_setscheduler(pid, SCHED_FIFO, &myparam);

    if (0 == pid)
    {
        // Child process - Reads all Messages from the Queue and outputs them with auxiliary data.
        // i.e. consumer
        cout << "C: this is the child process" << endl;

        // create and open a new shared memory object
        // int shm_open(const char *name, int oflag, mode_t mode);
        if((file_descriptor = shm_open(SHM_NAME, O_RDWR , S_IRWXU | S_IRWXG)) > 0)
            cout << "C: shared memory successfully opened" << endl;
        else
            cout << "C: failed to open shared memory" << endl;

        // map memory
        // i.e. treat that bit of memory as if it was a CMessage
        // create binary semaphore in the shared memory
        cout << "C: create binary semaphore" << endl;
        CBinarySemaphore * myCBinarySemaphoreMemPtr = (CBinarySemaphore *) mmap (NULL, len_sem+len_queue, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
        if(myCBinarySemaphoreMemPtr == NULL)
            cout << "C: error in mmap (sem)" << endl;
        CBinarySemaphore * myCBinarySemaphorePtr = new (myCBinarySemaphoreMemPtr) CBinarySemaphore();

        // create queue in the shared memory
        cout << "C: create queue" << endl;
        CCommQueue * myCCommQueueMemPtr = (CCommQueue *) myCBinarySemaphorePtr + len_sem;
        CCommQueue * myCCommQueuePtr = new (myCCommQueueMemPtr) CCommQueue((Int32)QUEUE_SIZE, *myCBinarySemaphorePtr);

        // read predefined number of CMessages from queue

        // create CMessage object
        cout << "C: create CMessage object" << endl;
        CMessage myCMessage;

        int j = 0;
        timeval tv;
        int32_t sum;
        while(j < NUM_MESSAGES)
        {
            // block child proces while there's no CMessage to be read
            cout << "C: try to take semaphore" << endl;
            myCBinarySemaphorePtr->take();

            // pop CMessage from queue
            cout << "C: pop CMessage from queue" << endl;
            if(myCCommQueuePtr->getMessage(myCMessage))
            {
                gettimeofday(&tv,0);
                int32_t time = (int) tv.tv_usec;
                cout << "C: Message successfully popped from queue" << endl;
                // Performance: Berechnung 1 mal & speichern & lesen oder 2 mal berechnen
                int32_t delta = tv.tv_sec * 1000000L + tv.tv_usec - myCMessage.getParam1();
                cout << "C: current time:"<< time << ", delta: " << delta << endl;
                sum += delta;
                j++;
                cout << "average time to transmit: " << sum/(j+1) << endl;
            }

        }

        shm_unlink(SHM_NAME);

        cout << "child pid: " << getpid() << endl;

    }
    else if (pid > 0)
    {
        // Parent process - Writes all Messages into the Queue
        // i.e. producer
        cout << "P: this is the parent process" << endl;

        // create and open a new shared memory object
        // int shm_open(const char *name, int oflag, mode_t mode);
        if((file_descriptor = shm_open(SHM_NAME, O_CREAT | O_RDWR , S_IRWXU | S_IRWXG)) > 0)
            cout << "P: shared memory successfully created" << endl;
        else
            cout << "P: failed to create shared memory" << endl;

        // The  truncate()  and ftruncate() functions cause the regular file named
        // by path or referenced by fd to be truncated  to  a  size  of  precisely
        // length bytes.
        //
        // If  the  file  previously  was larger than this size, the extra data is
        // lost.  If the file previously was shorter,  it  is  extended,  and  the
        // extended part reads as null bytes ('\0').
        ftruncate(file_descriptor,len_sem+len_queue);

        // map memory
        // i.e. treat that bit of memory as if it was a CMessage
        // create binary semaphore in the shared memory
        cout << "P: create binary semaphore" << endl;
        CBinarySemaphore * myCBinarySemaphoreMemPtr = (CBinarySemaphore *) mmap (NULL, len_sem+len_queue, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
        if(myCBinarySemaphoreMemPtr == NULL)
            cout << "P: error in mmap (sem)" << endl;
        CBinarySemaphore * myCBinarySemaphorePtr = new (myCBinarySemaphoreMemPtr) CBinarySemaphore();

        // create queue in the shared memory
        cout << "P: create queue" << endl;
        CCommQueue * myCCommQueueMemPtr = (CCommQueue *) myCBinarySemaphorePtr + len_sem;
        CCommQueue * myCCommQueuePtr = new (myCCommQueueMemPtr) CCommQueue((Int32)QUEUE_SIZE, *myCBinarySemaphorePtr);

        sleep(1);

        // create CMessage object
        cout << "P: create CMessage object" << endl;
        CMessage myCMessage;

        int i = 0;
        int j = 0;

        timeval tv;

        while(j < NUM_MESSAGES)
        {
            // fill CMessage object with data
            cout << "P: fill CMessage object with data" << endl;
            gettimeofday(&tv,0);
            myCMessage.setParam1((Int32) tv.tv_sec * 1000000 + tv.tv_usec);
            cout << "P: message type: " << myCMessage.getParam1() << endl;

            // push CMessage into queue
            cout << "P: push CMessage into queue" << endl;
            if(myCCommQueuePtr->add(myCMessage, false))
                cout << "P: Message successfully added to queue" << endl;

            // give semaphore to signal child that it may read from the queue
            cout << "P: give semaphore to signal child that it may read from the queue" << endl;
            myCBinarySemaphorePtr->give();

            i++;
            j++;

            //cout << "P: waiting for input" << endl;
                //getchar();
                cout << endl;
        }

        while(myCCommQueuePtr->getNumOfMessages()) { myCBinarySemaphorePtr->give(); }

        sleep(1);

        // delete shared memory
        shm_unlink(SHM_NAME);

        cout << "parent pid: " << getpid() << endl;
        exit(7);
    }
    else
    {
        // Error
        cerr << "Couldn't create a child process. Exiting" << endl;
        abort();
    }

    return 0;
}
