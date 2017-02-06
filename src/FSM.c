
#include "FSM.h"
#include <string.h>
#include "Si7006.h"

#define MIN_COMMAND_LENGTH (4)
#define MAX_COMMAND_LENGTH (5)
/*	FSM, TASK HANDLER
 * 	Takes commands from input buffer
 *	String Compare Capable
 *	Check for more commands in buffer
 *	If Buffers are empty, sleep task
 *
 *	Dependent on UART; pushes commands onto Buffer and wakeup and sleep cycle
 */
//Local Global Variables
Buffer* input_buff; //Commands in input_buff come from the UART
Buffer* output_buff; //Commands handled here in FSM go to output_buff
/*
 * Use this to check is commands are the same
 */
static int is_same(char* str0, char* str1){
	if(strcpy(str0,str1) == 0){
		return 1;
	}
	else;
		return 0;
}
/*
 * For executing commands
 * Filled this with some example code
 */
static void comm_exec(char* comm, unsigned int arg){

}
/*
 * If something is in the Buffer, wakes up and preforms task
 *
 */
extern void FSM_go(){
	if(check_UART_busy() == 0){
		while(Buffer_size(input_buff) > 0){
			FSM_do();
		}
	}
}

extern void FSM_do(){
	if(Buffer_size(input_buff) > 0){ //Can be used to handle any in coming commands
		/*
		char data[MAX_COMMAND_LENGTH];
		Buffer_pop(input_buff, data);
		char comm[4] = {data[0], data[1], data[2], '\0'};
		int arg = (unsigned int) data[3];
		comm_exec(comm, arg);
		*/
	}

	if(Buffer_size(output_buff) > 0){ //Can be used to handle any outgoing commands
		/*
		if(check_UART_busy() == 0){
			char mesg[MAX_COMMAND_LENGTH];
			Buffer_pop(output_buff, mesg);
			UART_push_out(mesg);
			*/
		}

}

/*
 *Takes command from UART
 *Wakes up FSM
 */
void command_recv(char *comm, int comm_length){
	Buffer_add(input_buff, comm);
	FSM_go();
}

//Message currently must be under five characters counting \0
//Check to see if we can send directly at this point
extern void Send_to_Odroid(char* mesg){
	Buffer_add(output_buff, mesg);
}


extern void init_FSM(void){
	Buffer_init(input_buff);
	Buffer_init(output_buff);
}
