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
* @return 1 if the checksum hasn't been validated
* @return 0 if the checksum has been validated
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

/**
* Add an input to the result block
* @param addBlock the result block
* @param mBlock the item to add
*/
void MessageAddition(volatile MSG_BLOCK* addBlock, volatile MSG_BLOCK* mBlock){
	DWORD i;
	printf("\n\tcurrent element %d in addition result is %d\n", followed_element, addBlock->mData[followed_element]);
	printf("\telement %d to add in the addition result is %d\n", followed_element, mBlock->mData[followed_element]);
	for(i = 0; i < DATA_SIZE; i++){
		addBlock->mData[i] += mBlock->mData[i];
	}
	printf("\tnew element %d is %d\n\n", followed_element, addBlock->mData[followed_element]);
}

/**
* Reset result block
* @param addBlock the resulted msg blocked to reset
*/
void MessageReset(volatile MSG_BLOCK* addBlock){
	DWORD i;
	for(i = 0; i < DATA_SIZE; i++){
		addBlock->mData[i] = 0;
	}
}
