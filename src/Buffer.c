/*Buffer - Circular Queue Implementation -STATIC VERSION*/

#include <stdlib.h>
#include "stm32f0xx.h"
#include "Buffer.h"

extern void Buffer_add(Buffer* buff, char* str){



			strcpy(buff->memmory[buff->index_to_load],str);
			buff->index_to_load = ((buff->index_to_load)+1)%(BUFFER_TOTAL_SIZE);
			buff->size++;




	if(Buffer_full(buff) == 1){
		while(1){
			//BUFFER GOT FULL
		}
	}
}

extern int Buffer_full(Buffer* buff){ //checks if buffer is full
	int check = buff->full;
	if(buff== BUFFER_TOTAL_SIZE){
		check = 1;
	}
	else check = 0;
	return check;
}

extern void Buffer_pop(Buffer* buff) {



		strcpy(buff->popped,buff->memmory[buff->index_to_pop]);


		buff->size--;
		buff->index_to_pop = (buff->index_to_pop + 1)%(BUFFER_TOTAL_SIZE);



	if(Buffer_full(buff) == 1){
			while(1){
				//BUFFER GOT FULL, doing nothing
			}
		}


}

extern Buffer* Buffer_init(Buffer* buff){
	buff->popped;
	buff->size = 0;
	buff->index_to_pop = 0;
	buff->index_to_load = 0;
	for(int j = 0; j<BUFFER_TOTAL_SIZE;j++){
		for(int i = 0; i<BUFFER_MAX_DATA_SIZE;i++)
			buff->memmory[j][i] = NULL;

	}
return buff;
}



