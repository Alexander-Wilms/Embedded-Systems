#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>   // For mode constants
#include <fcntl.h>      // For O_ constants
#include <sys/time.h>
#include <CCommQueue.h>

#include <new>   // for placement

#include "base/CMessage.h"

using namespace std;

#define SHM_NAME        "/estSHM"
#define QUEUE_SIZE      100
#define NUM_MESSAGES    10000

CBinarySemaphore myCBinarySemaphore;
int len = NUM_MESSAGES*sizeof(CBinarySemaphore);

int main()
{
    int fd;

    // create child process via fork()
    cout << "Creating a child process ..." << endl;
    pid_t pid = fork();

    if (0 == pid)
    {
        // Child process - Reads all Messages from the Queue and outputs them with auxiliary data.
        // i.e. consumer
        cout << "C: this is the child process" << endl;

        // create and open a new shared memory object
        // int shm_open(const char *name, int oflag, mode_t mode);
        if((fd = shm_open(SHM_NAME, O_RDWR , S_IRWXU | S_IRWXG)) > 0)
            cout << "C: shared memory successfully opened" << endl;

        // map memory
        cout << "C: map memory" << endl;
        // i.e. treat that bit of memory as if it was a CBinarySemaphore
        CBinarySemaphore * shared_msg = (CBinarySemaphore *) mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        // create binary semaphore
        CBinarySemaphore myCBinarySemaphore = new (shared_msg) CBinarySemaphore();

        // create queue
        cout << "C: create queue" << endl;
        Int32 sizeNeeded = CCommQueue::getNumOfBytesNeeded(QUEUE_SIZE);
        // get the RAM for it
        Int8* ptr = new Int8[sizeNeeded];
        // create the  queue
        CCommQueue* myCCommQueuePtr = new(ptr) CCommQueue(QUEUE_SIZE, myCBinarySemaphore);

        // read predefined number of CMessages from queue

        // create CMessage object
        cout << "P: create CMessage object" << endl;
        CMessage myCMessage;

        while(1)
        {
            // block child proces while there's no CMessage to be read
            if(myCBinarySemaphore.take())
            {
                // pop CMessage from queue
                cout << "C: pop CMessage from queue" << endl;
                if(myCCommQueuePtr->getMessage(myCMessage))
                    cout << "C: Message successfully popped from queue" << endl;
                cout << "C: message type: " << myCMessage.getMessageType() << endl;
            }
        }

    }
    else if (pid > 0)
    {
        // Parent process - Writes all Messages into the Queue
        // i.e. producer
        cout << "P: this is the parent process" << endl;

        // create and open a new shared memory object
        // int shm_open(const char *name, int oflag, mode_t mode);
        if((fd = shm_open(SHM_NAME, O_CREAT | O_RDWR , S_IRWXU | S_IRWXG)) > 0)
            cout << "P: shared memory successfully created" << endl;

        // The  truncate()  and ftruncate() functions cause the regular file named
        // by path or referenced by fd to be truncated  to  a  size  of  precisely
        // length bytes.
        //
        // If  the  file  previously  was larger than this size, the extra data is
        // lost.  If the file previously was shorter,  it  is  extended,  and  the
        // extended part reads as null bytes ('\0').
        ftruncate(fd,len);

        // map memory
        // i.e. treat that bit of memory as if it was a CMessage
        cout << "P: map memory" << endl;
        CMessage * shared_msg = (CMessage *) mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        // create binary semaphore
        cout << "P: create binary semaphore" << endl;
        CBinarySemaphore myCBinarySemaphore = new (shared_msg) CBinarySemaphore();

        // create queue
        cout << "P: create queue" << endl;
        Int32 sizeNeeded = CCommQueue::getNumOfBytesNeeded(QUEUE_SIZE);
        // get the RAM for it
        Int8* ptr = new Int8[sizeNeeded];
        // create the  queue
        CCommQueue* myCCommQueuePtr = new(ptr) CCommQueue(QUEUE_SIZE, myCBinarySemaphore);

        // create CMessage object
        cout << "P: create CMessage object" << endl;
        CMessage myCMessage;

        // fill CMessage object with data
        cout << "P: fill CMessage object with data" << endl;
        myCMessage.setMessageType((CMessage::MessageType) 0);
        cout << "P: message type: " << myCMessage.getMessageType() << endl;

        // push CMessage into queue
        cout << "P: push CMessage into queue" << endl;
        if(myCCommQueuePtr->add(myCMessage))
            cout << "P: Message successfully added to queue" << endl;

        // give semaphore to signal child that it may read from the queue
        cout << "P: give semaphore to signal child that it may read from the queue" << endl;
        myCBinarySemaphore.give();

        // delete shared memory
    }
    else
    {
        // Error
        cerr << "Couldn't create a child process. Exiting" << endl;
        abort();
    }

    return 0;
}
