#include "msg.h"

/**
* Set the message content
* @param mBlock the message pointer
* @param threadId the thread id that produce the message
*/
void MessageFill(volatile MSG_BLOCK* mBlock, pthread_t threadId){
	DWORD i;
	mBlock->checksum = 0;
	int max_number, minimum_number;
	max_number = 100;
	minimum_number = 1;
	for(i=0;i < DATA_SIZE;i++){
		mBlock->mData[i] = rand() % (max_number + 1 - minimum_number) + minimum_number;
		mBlock->checksum ^= mBlock->mData[i];
	}
	mBlock->threadID = threadId;
	return;
}

/**
* Display the message content
* @param mBlock the message pointer
*/
int MessageDisplay(volatile MSG_BLOCK* mBlock){
	DWORD i, tcheck=0;
	for(i=0;i < DATA_SIZE;i++)
		tcheck ^= mBlock->mData[i];
	if(tcheck == mBlock->checksum){
		printf("\n[Control Message] Checksum validated\n");
		return(0);
	}
	else {
		printf("\n[Control Message] Checksum failed, message corrupted\n");
		return(1);
	}
}

void MessageAddition(volatile MSG_BLOCK* addBlock, volatile MSG_BLOCK* mBlock){
	DWORD i;
	printf("\n\tcurrent element %d in addition result is %d\n", followed_element, addBlock->mData[4]);
	printf("\telement %d to add in the addition result is %d\n", followed_element, mBlock->mData[4]);
	for(i = 0; i < DATA_SIZE; i++){
		addBlock->mData[i] += mBlock->mData[i];
	}
	printf("\tnew element %d is %d\n\n", followed_element, addBlock->mData[4]);
}

void MessageReset(volatile MSG_BLOCK* addBlock){
	DWORD i;
	for(i = 0; i < DATA_SIZE; i++){
		addBlock->mData[i] = 0;
	}
}
