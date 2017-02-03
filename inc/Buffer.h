#ifndef BUFFER_H_
#define BUFFER_H_

#include<string.h>
#include <stdlib.h>
#include "stm32f0xx.h"

#define MAX_BUFFER_DATA (8)



#define BUFFER_TOTAL_SIZE (4)
#define BUFFER_MAX_DATA_SIZE (5)


typedef struct Buffer{

	char memmory[BUFFER_TOTAL_SIZE][BUFFER_MAX_DATA_SIZE];
	char popped[BUFFER_MAX_DATA_SIZE];
	uint8_t size; //uint8_t
	uint8_t index_to_pop;
	int index_to_load;
	int full;
} Buffer;

//Public functions ------------------------------

extern void Buffer_add(Buffer* b, char* str);
extern void Buffer_pop(Buffer* b);
extern Buffer* Buffer_init();


//-----------------------------------------------

#endif
 
