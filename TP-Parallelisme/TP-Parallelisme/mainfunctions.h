#include "datadef.h"
#include <unistd.h>
#define NITERS      4           /* number of items produced/consumed */


void *Producer(void *arg);

void *Consumer(void *arg);