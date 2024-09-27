/*******************************************************************************************
 * File Name	: Lab1.c
 * Description	: This file is the skeleton for Lab 1.

 * Instructions	: Complete the state machine as indicated in the comments inside each state.
 * Implement the functions declared below and use them to complete the state machine.
 * Do not modify any function declaration/signature.
 * Do not modify the main function nor any global declaration or definition.
 *******************************************************************************************/

 /******************************************************************************************
 * Student Name: Amar Singh
 * Student Number: 8935727
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define STACK_SIZE	 7
#define BUFFER_SIZE	 4

 /**
 * Brief: Structure to hold stack index and stack data
 */
typedef struct
{
	int8_t index;
	char stackData[STACK_SIZE];

} stack_t;

/**
* Brief: Enum to represent type of command.
*/
typedef enum
{
	COMMAND_NONE = 0,
	COMMAND_EXIT,
	COMMAND_PUSH,
	COMMAND_POP,
	COMMAND_PRINT,

} command_t;

/**
* Brief: Structure to hold the parameters passed to the state machine
*/
struct StateMachineParameters
{
	command_t command;
	char input[BUFFER_SIZE];

} parameters[] =
{
	{.command = COMMAND_NONE  },				// Run START_STATE
	{.command = COMMAND_NONE  },				// Run INITIALIZATION_STATE
	{.command = COMMAND_PRINT },				// Expect an error message
	{.command = COMMAND_PUSH, .input = "abc" },
	{.command = COMMAND_PRINT },				// Expect "abc"
	{.command = COMMAND_PUSH, .input = "def" },
	{.command = COMMAND_PRINT },				// Expect "abcdef"
	{.command = COMMAND_PUSH, .input = "123" }, // Expect stack overflow message
	{.command = COMMAND_PRINT },				// Expect "abcdef1"
	{.command = 100, .input = "xyz" },		// Parameters should be neglected
	{.command = COMMAND_POP   },				// Pop out one item: '1'
	{.command = COMMAND_POP   },				// Pop out one item: 'f'
	{.command = COMMAND_PRINT },				// Expect "abcde"
	{.command = COMMAND_PUSH, .input = "xyz" }, // Expect stack overflow message
	{.command = COMMAND_PRINT },				// Expect "abcdexy"
	{.command = COMMAND_POP   },                
	{.command = COMMAND_POP   },
	{.command = COMMAND_POP   },
	{.command = COMMAND_POP   },
	{.command = COMMAND_PUSH, .input = "1234" }, // In this case it will reflect an error as exceeding buffer limit(i.e 4)
	{.command = COMMAND_POP   },
	{.command = COMMAND_POP   },
	{.command = COMMAND_POP   },
	{.command = COMMAND_PUSH, .input = "-1" },   // we can also push a string of length less than buffer limit
	{.command = COMMAND_POP   },
	{.command = COMMAND_POP   },
	{.command = COMMAND_PUSH, .input = "abc" },
	{.command = COMMAND_PRINT },                  // Expect "abc"
	{.command = COMMAND_EXIT },					  // Expect "abc"
};

void runStateMachine(struct StateMachineParameters parameters);
bool pushIntoStack(char* pStack, int8_t* pStackIndex, char inputData);
bool popFromStack(char* pStack, int8_t* pStackIndex, char* pOutputData);
bool printOutStackContents(char* pStack, int8_t stackIndex);
bool isStackEmpty(int8_t stackIndex);
bool isStackFull(int8_t stackIndex);
bool resetStack(char* pStack, int8_t* pStackIndex);


int main()
{
	int i;
	for (i = 0; i < sizeof(parameters) / sizeof(parameters[0]); i++)
	{
		runStateMachine(parameters[i]);
		runStateMachine(parameters[0]);	// Switch state back to WAIT_FOR_COMMAND_STATE
	}

	return 0;
}


/**
* Brief: This function runs a state machine that accepts commands as arguments
* in order to push/pop data into/from a stack structure defined locally and displays its contents.
*/
void runStateMachine(struct StateMachineParameters parameters)
{
	static stack_t stack;
	static char inputString[BUFFER_SIZE];	// Stores the user's input string

	static enum
	{
		START_STATE,
		INITIALIZATION_STATE,
		WAIT_FOR_COMMAND_STATE,
		PUSH_TO_STACK_STATE,
		POP_FROM_STACK_STATE,
		PRINT_STACK_CONTENTS_STATE,
		FINISH_STATE

	} state = START_STATE;
	
	


	switch (state)
	{
	case START_STATE:
	{
		printf("Program started!\n");
		state = INITIALIZATION_STATE;
		break;
	}

	case INITIALIZATION_STATE:
	{
		/**
		 * 1) Call a function to reset stack
		 * 2) Switch state to WAIT_FOR_COMMAND_STATE
		*/
		if(resetStack(stack.stackData, &stack.index))
		{
		    printf("Successfully initialized\n");
		    state = WAIT_FOR_COMMAND_STATE;
		    break;
		}
		/*
		* If somehow stack is not initialized then it will
		 Switch back to Start state instead of Command state
		 as without stack we cannot do any operations.		
		*/
		else
		{
		    printf("Initialization failed\n");
		    state = START_STATE;
		    break;
		}
		
	}

	case WAIT_FOR_COMMAND_STATE:
	{	
		/**
		 * Extract the command embedded in the parameter passed and switch to the
		 * corresponding state as shown below:
		 * 
		 * COMMAND_PUSH -> go to PUSH_TO_STACK_STATE
		 * COMMAND_POP -> go to POP_FROM_STACK_STATE
		 * COMMAND_PRINT -> go to PRINT_STACK_CONTENTS_STATE
		 * COMMAND_EXIT -> go to FINISH_STATE
		 * 
		 * Program should neglect any other input
        */
       	switch (parameters.command) 
       	{
       	    
       	case COMMAND_NONE:
 		                  break;
 		                  
		case COMMAND_PUSH:
		                  strcpy(inputString, parameters.input);
			              state = PUSH_TO_STACK_STATE;
			              break;
		case COMMAND_POP:
			              state = POP_FROM_STACK_STATE;
			              break;
		case COMMAND_PRINT:
			              state = PRINT_STACK_CONTENTS_STATE;
			              break;
		case COMMAND_EXIT:
			              state = FINISH_STATE;
			              break;
 		
		default:
			              printf("Invalid command %d\n", parameters.command);
			              break;
		}
		break;
	}

	case PUSH_TO_STACK_STATE:
	{
	    /**
	     * Call a function to push the data available in the input string into the stack
	     * The function should check for overflow condition.
	     *
	     * After all data is pushed, switch state to WAIT_FOR_COMMAND_STATE
	     * 
	     */
	     
	    if(strlen(inputString) <= (BUFFER_SIZE - 1))
	    {
	         for(int i=0; i < strlen(inputString); i++)
	         {
                 char ch = inputString[i];
	             if(pushIntoStack(stack.stackData, &stack.index, ch));
	             else
	             break;
	         }
	         state = WAIT_FOR_COMMAND_STATE;
		     break;
	    }
	    else
	    printf("WARNING: You are exceeding the buffer limit!\n");
		state = WAIT_FOR_COMMAND_STATE;
		break;
		     
	}

	case POP_FROM_STACK_STATE:
	{
	    /**
	     * 1) Call a function to pop one element from the stack.
	     * 2) Display the popped out data,
	     * 3) Switch state to WAIT_FOR_COMMAND_STATE 
	     */
	     if(isStackEmpty(stack.index))
	     {
	           printf("WARNING: Stack is empty!\n");
	           state = WAIT_FOR_COMMAND_STATE;
		       break;
	     }
	     else
	     {
	         char ch;
	           if(popFromStack(stack.stackData, &stack.index, &ch))
	           printf("Popped element is %c\n", ch);
	           state = WAIT_FOR_COMMAND_STATE;
		       break;
	     }
	     break;
	}

	case PRINT_STACK_CONTENTS_STATE:
	{
	    /**
	     * 1) Display the stack contents without popping out the data
	     * 2) In case the stack is empty, print out a suitable message.
	     * 3) Switch state to WAIT_FOR_COMMAND_STATE.
	     */
	     if(isStackEmpty(stack.index))
	     {
	          printf("WARNING: Nothing to print\n");
	          state = WAIT_FOR_COMMAND_STATE;
		      break;
	     }
	     else
	     {
	         
	         if(printOutStackContents(stack.stackData, stack.index));
	         printf("\n");
	          state = WAIT_FOR_COMMAND_STATE;
		      break;
	     }
	     
	}

	case FINISH_STATE:
	{
		/**
		 * Perform the same steps done in PRINT_STACK_CONTENTS_STATE except exit the state machine instead of
		 * switching state to WAIT_FOR_COMMAND_STATE.
		 */
		 if(isStackEmpty(stack.index))
	     {
	          printf("Leaving stack empty! you can reuse it later\n");
	          exit(0);
		      break;
	     }
	     else
	     {
	         if(printOutStackContents(stack.stackData, stack.index));
	         printf("\n");
	         exit(0);
		     break;
	     }
	     
		 
	}

	default:
		state = START_STATE;
		break;
	}
}


bool pushIntoStack(char* pStack, int8_t* pStackIndex, char inputData)
{
    if(isStackFull(*pStackIndex))
    {
        printf("WARNING: Stack is already full\n");
        return false;
    }
       (*pStackIndex)++;
       pStack[(*pStackIndex)] = inputData;
       return true;
}


bool popFromStack(char* pStack, int8_t* pStackIndex, char* pOutputData)
{
        *pOutputData = pStack[*pStackIndex];
        (*pStackIndex)--;
        return true;
}


bool printOutStackContents(char* pStack, int8_t stackIndex)
{    
     for(int i=0; i <= stackIndex; i++)
     printf("%c", pStack[i]);
     return true;
}


bool isStackEmpty(int8_t stackIndex)
{
     if(stackIndex < 0)
     return true;
     else
     return false;
}


bool isStackFull(int8_t stackIndex)
{
     if(stackIndex == (STACK_SIZE - 1))
     return true;
     else
     return false;
}


bool resetStack(char* pStack, int8_t* pStackIndex)
{
       *pStackIndex = -1;
       for(int i=0; i <= STACK_SIZE; i++)
       pStack[i] = '\0';
       return true;
}
