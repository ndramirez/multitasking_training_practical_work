#include <stdio.h>
#include <pthread.h>

//Message data size
#define DATA_SIZE 256
#define DWORD unsigned int

//producer and consumer count storage
volatile DWORD consumeCount, produceCount;
extern unsigned int followed_element;
pthread_t idP, idC;

//message type definition
typedef struct MSG_BLOCK_TAG
{
	pthread_t threadID;
	time_t timestamp;
	DWORD checksum;
	DWORD mData[DATA_SIZE];
} MSG_BLOCK;

/**
* Set the message content. The mBlock timestamp is not set by that function, user has to set it.
* @param mBlock the message pointer
* @param threadId the thread id that produce the message
*/
void MessageFill(volatile MSG_BLOCK* mBlock, pthread_t threadId);

/**
* Display the message content
* @param mBlock the message pointer
* @return 1 if the checksum hasn't been validated
* @return 0 if the checksum has been validated
*/
int MessageDisplay(volatile MSG_BLOCK* mBlock);

/**
* Add an input to the result block
* @param addBlock the result block
* @param mBlock the item to add
*/
void MessageAddition(volatile MSG_BLOCK* addBlock, volatile MSG_BLOCK* mBlock);

/**
* Reset result block
* @param addBlock the resulted msg blocked to reset
*/
void MessageReset(volatile MSG_BLOCK* addBlock);
