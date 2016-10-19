#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*These variables are associated with the implementation of the VM*/
int fp ; 
int i ; 
int j, k ; 
char input_line [7] ;

/*These are variables representing the VM itself*/

char IR[6] ;
short int PC = 0 ;

short int P0 ; //these are the pointer registers
short int P1 ;
short int P2 ;
short int P3 ;

int R0 ; //GP regs
int R1 ;
int R2 ;
int R3 ;

int ACC ;
char PSW[2] ;
char memory [100][6] ; 		  //this is the program memory for first program
short int opcode ;            //nice to know what we are doing
int program_line = 0 ;


 // HELPER FUNCTIONS

	//This function returns the integer value of operand 1 
	//when this operand is an immediate two-byte integer. 
	
	int ParseOP1(char *IR) 
	    { int op1 = ((int) (IR[2]) - 48)*10 ;
	      op1 += ((int) (IR[3])- 48) ;
	      return op1;
	    }

	// returns the integer value of operand 2 when this operand is a two-byte integer. 
	int ParseOP2(char *IR) 
	    { int op2 = ((int) (IR[4]) - 48)*10 ;
	      op2 += ((int) (IR[5])- 48) ;
	      return op2;
	    }

	//returns the integer value of operands 1 and 2 combined to form a 4-byte integer.
	int ParseOP1andOP2Imm(char *IR) 
	    { int ops = ((int) (IR[2]) - 48)*1000 ;
	      ops += ((int) (IR[3])- 48)*100 ;
	      ops += ((int) (IR[4]) - 48)*10 ;
	      ops += ((int) (IR[5])- 48) ;
	      return ops;
	  	}

	// returns the register number of the register used as operand  1 of an instruction. 
	// Can be either Pointer or General-Purpose register. 
	int ParseOP1Reg(char *IR)  
	    { int regOp1 = ((int) (IR[3]) - 48) ;
	      return regOp1 ;
	    }

	// returns the register number of a register used as operand  2 of an instruction. 
	// Can be either a Pointer or General-Purpose register. 
	int ParseOP2Reg(char *IR)  
	    { int regOp2 = ((int) (IR[5]) - 48) ;
	      return regOp2 ;
	  	}

	// returns the data stored at memory location Memory_Location
	int FetchData(int Memory_Location)  
		{	char contents;
			contents = ((((int) memory[Memory_Location][2])-48) *1000) +
						((((int) memory[Memory_Location][3])-48) *100) +
						((((int) memory[Memory_Location][4])-48) *10) +
						(((int) memory[Memory_Location][5])-48);

			return contents;
		}


	//Prints out the contents of the IR on the same line.
	void printIR(char *IR) 
		{	char contents;
		    for (i = 0; i < 6 ; i++) {
				contents = IR[i] ;
		        printf("%c ", contents);
		    }
		    printf("\n");
		}


	//converts an int NUM to a char to store in memory MEM
	void store(int NUM, int LOC)
		{	memory[LOC][0] = 'Z';
			memory[LOC][1] = 'Z';
			memory[LOC][2] = ((char) (NUM/1000)+48);		//stores the 1000s place of NUM as a char in memory location LOC
			memory[LOC][3] = ((char) ((NUM/100)%10)+48);	//stores the 100s place of NUM as a char in memory location LOC
			memory[LOC][4] = ((char) ((NUM/10)%10)+48);		//stores the 10s place of NUM as a char in memory location LOC
			memory[LOC][5] = ((char) (NUM%10)+48);			//stores the 1s place of NUM as a char in memory location LOC
		}

	//prints out the contents of memory from row 0 to row upto
	//This should print out all instructions and data stored in memory. 
	void printMEM(int upto) 
		{for (i = 0; i < (upto) ; i++){
			printf("\nContents of Row %d: ", i);		//prints row number
				for(j = 0; j < 6; j++){				//loops through all 6 values (js) in memory row i
					char contents = memory[i][j] ;	//gets value j of row i
					printf("%c ", contents);			//prints value j of row i
		    	}
		    }
		}



//OPCODES FOR INTERPRETER 

// OPCODE 0
	void OP0(char *IR)
	{ int PREG, VAL ;
	  printf("Opcode = 00. Load Pointer Immediate\n") ;
   	  printIR(IR) ; 
         
  	 PREG = ParseOP1Reg(IR) ;
  	 VAL = 	ParseOP2 (IR) ;
	
	switch(PREG)
		{case 0: P0 = VAL ; break;
		  case 1: P1 = VAL ; break;
		  case 2: P2 = VAL ; break;
		  case 3: P3 = VAL ; break;
		} 
	  
	  //Will print out junk until the two helper functions are implemented!
	  printf("Loaded P[%d] with immediate value %d\n", PREG, VAL ) ;
	}

/* Now its your turn! Provide the prototype and implementation for the remaining opcodes. */

//OPCODE 1 
	void OP1(char *IR) {
	int PREG, NUM;		//PREG is operand 1, NUM is number added to the pointer
	printf("Opcode = 01. Add to Pointer Immediate\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);		//grabs pointer number at operand 1 in instruction register
	NUM = ParseOP2(IR);			//grabs two-digit int at operand 2 in instruction register

	switch(PREG) {
		case 0: P0 += NUM; break;		//address contained in P0 increased by NUM
		case 1: P1 += NUM; break;		//address contained in P1 increased by NUM
		case 2: P2 += NUM; break;		//address contained in P2 increased by NUM
		case 3: P3 += NUM; break;		//address contained in P3 increased by NUM
	}

	printf("Added %d to P[%d] address\n", NUM, PREG) ;
}

//OPCODE 2 
	void OP2(char *IR) {
	int PREG, NUM;		//PREG is operand 1, NUM is number subtracted from the pointer
	printf("Opcode = 02. Subtract from Pointer Immediate\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);		//grabs pointer number at operand 1 in instruction register
	NUM = ParseOP2(IR);			//grabs two-digit int at operand 2 in instruction register

	switch(PREG) {
		case 0: P0 -= NUM; break;		//address contained in P0 decremented by NUM
		case 1: P1 -= NUM; break;		//address contained in P1 decremented by NUM
		case 2: P2 -= NUM; break;		//address contained in P2 decremented by NUM
		case 3: P3 -= NUM; break;		//address contained in P3 decremented by NUM
	}

	printf("Subtracted %d from P[%d] address\n", NUM, PREG) ;
}

//OPCODE 3 
	void OP3(char *IR) {
	int NUM;		//NUM is number loaded into ACC
	printf("Opcode = 03. Load Accumulator Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR); 	//grabs operand 1 and operand 2 as a 4-digit int

	//sets ACC to NUM 
	ACC = NUM;

	printf("Loaded %d into Accumulator\n",NUM) ;
}

//OPCODE 4
	void OP4(char *IR) {
	int PREG, ADD, contents;	//PREG is operand 1, ADD will be address, contnets is value stored at ADD
	printf("Opcode = 04. Load Accumulator Register Immediate\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);		//grabs pointer number at operand 1 in instruction register

	switch(PREG) {
		case 0: ADD=P0; break;		//address contained in P0
		case 1: ADD=P1; break;		//address contained in P1
		case 2: ADD=P2; break;		//address contained in P2
		case 3: ADD=P3; break;		//address contained in P3
	}

	contents = FetchData(ADD);

	//sets Accumulator with contents of memory address contained in ADD
	ACC = contents ;

	printf("Set Accumulator to %d at P[%d]\n", contents, PREG) ;
}

//OPCODE 5
	void OP5(char *IR) {
	int ADD, contents;		//ADD will be address, contents will be value stored at ADD
	printf("Opcode = 05. Load Accumulator Direct Addressing\n");
	printIR(IR);

	ADD = ParseOP1(IR);		//grabs operand 1 two-digit integer

	contents = FetchData(ADD);

	//sets Accumulator with contents of memory address contained in ADD
	ACC = contents ;

	printf("Loaded Accumulator with %d at address %d\n", contents, ADD) ;
}

//OPCODE 6
	void OP6(char *IR) {
	int PREG, ADD;				//PREG is operand 1, ADD will be address 
	printf("Opcode = 06. Store Accumulator Register Addressing\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);		//grabs pointer number at operand 1 in instruction register

	switch(PREG) {
		case 0: ADD=P0; break;		//address contained in P0
		case 1: ADD=P1; break;		//address contained in P1
		case 2: ADD=P2; break;		//address contained in P2
		case 3: ADD=P3; break;		//address contained in P3
	}

	//stores contents of ACC to memory address ADD held in pointer PREG
	store(ACC, ADD);

	printf("Stored Accumulator contents, %d, to address held in P[%d]\n", ACC, PREG) ;
}

//OPCODE 7
	void OP7(char *IR) {
	int ADD;				//PREG is operand 1, ADD will be address 
	printf("Opcode = 07. Store Accumulator Direct Addressing\n");
	printIR(IR);

	ADD = ParseOP1(IR);	//grabs operand 1 two-digit integer

	//stores contents of ACC to memory address ADD held in pointer PREG
	store(ACC, ADD);

	printf("Stored Accumulator contents, %d, to address %d\n", ACC, ADD) ;
}

//OPCODE 8
	void OP8(char *IR) {
	int REG, PREG, RADD, PADD, contents;	//REG is operand 1, PREG is operand 2, RADD is register address, PADD is pointer address, contents is data in address RADD
	printf("Opcode = 08. Store Register to memory: Register Addressing\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number
	PREG = ParseOP2Reg(IR);		//grabs operand 2 register number (pointer)

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in P0
		case 1: PADD=P1; break;		//address contained in P1
		case 2: PADD=P2; break;		//address contained in P2
		case 3: PADD=P3; break;		//address contained in P3
	}

	contents = FetchData(RADD);

	//stores contents of memory address RADD in memory address PADD
	store(contents, PADD);

	printf("Stored %d of Register R[%d] in memory address pointed to by P[%d]\n", contents, REG, PREG) ;
}

//OPCODE 9
	void OP9(char *IR) {
	int REG, RADD, ADD, contents;	//REG is operand 1, RADD is register address, ADD is operand 2/memory address, contents is data stored in address RADD
	printf("Opcode = 09. Store Register to memory: Direct Addressing\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number
	ADD = ParseOP2(IR);			//grabs operand 2 two-digit integer (for memory address)

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}

	contents = FetchData(RADD);

	//stores contents of memory address RADD in memory address ADD
	store(contents, ADD);

	printf("Stored %d of Register R[%d] in memory address %d\n", contents, REG, ADD) ;
}

//OPCODE 10
	void OP10(char *IR) {
	int REG, PREG, RADD, PADD, contents;	//REG is operand 1, PREG is operand 2, RADD is register address, PADD is pointer address, contents is data from address PADD
	printf("Opcode = 10. Load Register from memory: Register Addressing\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number
	PREG = ParseOP2Reg(IR);		//grabs operand 2 register number (pointer)

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in P0
		case 1: PADD=P1; break;		//address contained in P1
		case 2: PADD=P2; break;		//address contained in P2
		case 3: PADD=P3; break;		//address contained in P3
	}

	contents = FetchData(PADD);

	//stores contents of pointers memory address PADD in memory address RADD
	store(contents, RADD);

	printf("Loaded Register R[%d] with %d of memory location pointed to by P[%d]\n", REG, contents, PREG) ;
}

//OPCODE 11
	void OP11(char *IR) {
	int REG, RADD, ADD, contents;		//REG is operand 1, RADD is register address, ADD is operand 2/memory address, contents is data stored in address ADD
	printf("Opcode = 11. Load Register from memory: Direct Addressing\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number
	ADD = ParseOP2(IR);			//grabs operand 2 two-digit int

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}
	
	contents = FetchData(ADD);

	//stores contents of memory address ADD in register memory address RADD
	store(contents, RADD);

	printf("Loaded Register R[%d] with %d of memory location %d\n", REG, contents, ADD) ;
}

//OPCODE 12
	void OP12(char *IR) {
	int NUM;					//integer being loaded into R0
	printf("Opcode = 12. Load Register R0 Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR);	//grabs operand 1 and 2 as 4-digit int

	//stores 4-digit NUM in register R0
	store(NUM, R0);

	printf("Loaded Register R0 with %d\n", NUM) ;
}

//OPCODE 13
	void OP13(char *IR) {
	int REG, REG2, RADD, RADD2, contents;	//REG is operand 1, REG2 is operand 2, RADD is register address, RADD2 is second register address, contents is data stored in address RADD2
	printf("Opcode = 13. Register to Register Transfer\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number
	REG2 = ParseOP2Reg(IR);		//grabs operand 2 register number

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}

	switch(REG2) {
		case 0: RADD2=R0; break;		//address contained in R0
		case 1: RADD2=R1; break;		//address contained in R1
		case 2: RADD2=R2; break;		//address contained in R2
		case 3: RADD2=R3; break;		//address contained in R3
	}

	contents = FetchData(RADD2);

	//stores contents of register memory address RADD2 in second register memory address RADD
	store(contents, RADD);

	printf("Assigned Register R[%d] to %d of Register R[%d]\n", REG, contents, REG2) ;
}

//OPCODE 14
	void OP14(char *IR) {
	int REG, RADD, contents;	//REG is operand 1, RADD is register address, contents is data stored in address RADD
	printf("Opcode = 14. Load Accumulator from Register\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number

	switch(REG) {
		case 0: ACC=R0; break;		//address contained in R0
		case 1: ACC=R1; break;		//address contained in R1
		case 2: ACC=R2; break;		//address contained in R2
		case 3: ACC=R3;	break;		//address contained in R3
	}

	printf("Assigned Accumulator to %d of Register R[%d]\n", ACC, REG) ;
}

//OPCODE 15
	void OP15(char *IR) {
	int REG;	//REG is operand 1
	printf("Opcode = 15. Load Register from Accumulator\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number

	switch(REG) {
		case 0: R0 = ACC; break;		//address contained in R0 assigned ACC
		case 1: R1 = ACC; break;		//address contained in R1 assigned ACC
		case 2: R2 = ACC; break;		//address contained in R2 assigned ACC
		case 3: R3 = ACC; break;		//address contained in R3 assigned ACC
	}

	printf("Assigned Register R[%d] to %d of Accumulator\n", REG, ACC) ;
}

//OPCODE 16
	void OP16(char *IR) {
	int NUM;						//4-digit integer to be added to ACC
	printf("Opcode = 16. Add Accumulator Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR);	//grabs operand 1 and 2 as 4-digit int

	//adds NUM to ACC
	ACC += NUM;

	printf("Added %d to Accumulator\n", NUM) ;
}

//OPCODE 17
	void OP17(char *IR) {
	int NUM;						//4-digit integer to be subtracted from ACC
	printf("Opcode = 17. Subtract Accumulator Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR);	//grabs operand 1 and 2 as 4-digit int

	//subtracts NUM from ACC
	ACC -= NUM;

	printf("Subtracted %d from Accumulator\n", NUM) ;
}

//OPCODE 18
	void OP18(char *IR) {
	int REG, RADD, contents;	//REG is operand 1, RADD is register address, contents is data stored in address RADD
	printf("Opcode = 18. Add contents of Register to Accumulator\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}

	contents = FetchData(RADD);

	//adds contents of register memory address RADD to ACC
	ACC += contents;

	printf("Added %d of Register R[%d] to Accumulator\n", contents, REG) ;
}

//OPCODE 19
	void OP19(char *IR) {
	int REG, RADD, contents;	//REG is operand 1, RADD is register address, contents is data stored in address RADD
	printf("Opcode = 19. Subtract contents of Register from Accumulator\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);		//grabs operand 1 register number

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	}

	contents = FetchData(RADD);

	//subtracts contents of register memory address RADD from ACC
	ACC -= contents;

	printf("Subtracted %d of Register R[%d] from Accumulator\n", contents, REG) ;
}

//OPCODE 20
	void OP20(char *IR) {
	int PREG, PADD, contents;	//PREG is operand 1, PADD is register address (pointer), contents is data stored in address PADD
	printf("Opcode = 20. Add Accumulator Register Addressing\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);		//grabs operand 1 register number (pointer)

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in R0
		case 1: PADD=P1; break;		//address contained in R1
		case 2: PADD=P2; break;		//address contained in R2
		case 3: PADD=P3; break;		//address contained in R3
	}

	contents = FetchData(PADD);

	//adds contents of register memory address PADD to ACC
	ACC += contents;

	printf("Added %d of memory address pointed to by Pointer R[%d] to Accumulator\n", contents, PREG) ;
}

//OPCODE 21
	void OP21(char *IR) {
	int ADD, contents;	//ADD is memory address, contents is data stored in address ADD
	printf("Opcode = 21. Add Accumulator Direct Addressing\n");
	printIR(IR);

	ADD = ParseOP1(IR);			//grabs operand 1 memory address 

	contents = FetchData(ADD);

	//adds contents of memory address ADD to ACC
	ACC += contents;

	printf("Added %d of address %d to Accumulator\n", contents, ADD) ;
}

//OPCODE 22
	void OP22(char *IR) {
	int PREG, PADD, contents;	//PREG is operand 1, PADD is register address (pointer), contents is data stored in address PADD
	printf("Opcode = 22. Subtract from Accumulator Register Addressing\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);		//grabs operand 1 register number (pointer)

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in R0
		case 1: PADD=P1; break;		//address contained in R1
		case 2: PADD=P2; break;		//address contained in R2
		case 3: PADD=P3; break;		//address contained in R3
	}

	contents = FetchData(PADD);

	//subtracts contents of register memory address PADD from ACC
	ACC -= contents;

	printf("Subtracted %d of memory address pointed to by Pointer R[%d] from Accumulator\n", contents, PREG) ;
}

//OPCODE 23
	void OP23(char *IR) {
	int ADD, contents;	//ADD is memory address, contents is data stored in address ADD
	printf("Opcode = 23. Subtract from Accumulator Direct Addressing\n");
	printIR(IR);

	ADD = ParseOP1(IR);			//grabs operand 1 memory address 

	contents = FetchData(ADD);

	//subtracts contents of memory address ADD from ACC
	ACC -= contents;

	printf("Subtracted %d of address %d from Accumulator\n", contents, ADD) ;
}

//OPCODE 24
	void OP24(char *IR, char *PSW) {
	int PREG, PADD, contents;	//PREG is operand 1, PADD is register address (pointer), contents is data stored in address PADD
	printf("Opcode = 24. Compare Equal Register Addressing\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);			//grabs operand 1 memory address

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in P0
		case 1: PADD=P1; break;		//address contained in P1
		case 2: PADD=P2; break;		//address contained in P2
		case 3: PADD=P3; break;		//address contained in P3
	} 

	contents = FetchData(PADD);

	//checks if ACC == contents pointed to by pointer
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC == contents){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("Compared contents of memory address pointed to by Pointer P[%d] (%d) to Accumulator (%d), set value of PSW[0]\n", PREG, contents, ACC) ;
}

//OPCODE 25
	void OP25(char *IR, char *PSW) {
	int PREG, PADD, contents;	//PREG is operand 1, PADD is register address (pointer), contents is data stored in address PADD
	printf("Opcode = 25. Compare Less Register Addressing\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);			//grabs operand 1 memory address

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in P0
		case 1: PADD=P1; break;		//address contained in P1
		case 2: PADD=P2; break;		//address contained in P2
		case 3: PADD=P3; break;		//address contained in P3
	} 

	contents = FetchData(PADD);

	//checks if ACC < contents pointed to by pointer
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC < contents){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was less than contents of memory address pointed to by Pointer P[%d] (%d), then value of PSW[0] was set to true\n", ACC, PREG, contents) ;
}

//OPCODE 26
	void OP26(char *IR, char *PSW) {
	int PREG, PADD, contents;	//PREG is operand 1, PADD is register address (pointer), contents is data stored in address PADD
	printf("Opcode = 26. Compare Greater Register Addressing\n");
	printIR(IR);

	PREG = ParseOP1Reg(IR);			//grabs operand 1 memory address

	switch(PREG) {
		case 0: PADD=P0; break;		//address contained in P0
		case 1: PADD=P1; break;		//address contained in P1
		case 2: PADD=P2; break;		//address contained in P2
		case 3: PADD=P3; break;		//address contained in P3
	} 

	contents = FetchData(PADD);

	//checks if ACC > contents pointed to by pointer
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC > contents){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was greater than contents of memory address pointed to by Pointer P[%d] (%d), then value of PSW[0] was set to true\n", ACC, PREG, contents) ;
}

//OPCODE 27
	void OP27(char *IR, char *PSW) {
	int NUM;	//NUM is operand 1 and 2 as a 4-digit int that will be compared to ACC
	printf("Opcode = 27. Compare Greater Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR);			//grabs operand 1 and operand 2 as 4-digit int

	//checks if ACC > NUM
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC > NUM){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was greater than %d, then value of PSW[0] was set to true\n", ACC, NUM) ;
}

//OPCODE 28
	void OP28(char *IR, char *PSW) {
	int NUM;	//NUM is operand 1 and 2 as a 4-digit int that will be compared to ACC
	printf("Opcode = 28. Compare Equal Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR);			//grabs operand 1 and operand 2 as 4-digit int

	//checks if ACC == NUM
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC == NUM){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was equal to %d, then value of PSW[0] was set to true\n", ACC, NUM) ;
}

//OPCODE 29
	void OP29(char *IR, char *PSW) {
	int NUM;	//NUM is operand 1 and 2 as a 4-digit int that will be compared to ACC
	printf("Opcode = 29. Compare Less Immediate\n");
	printIR(IR);

	NUM = ParseOP1andOP2Imm(IR);			//grabs operand 1 and operand 2 as 4-digit int

	//checks if ACC < NUM
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC < NUM){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was less than %d, then value of PSW[0] was set to true\n", ACC, NUM) ;
}

//OPCODE 30
	void OP30(char *IR, char *PSW) {
	int REG, RADD, contents;	//REG is operand 1, RADD is register address, contents is data stored in address RADD
	printf("Opcode = 30. Compare Register Equal\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);			//grabs operand 1 memory address

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;	 	//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	} 

	contents = FetchData(RADD);

	//checks if ACC == contents pointed to by register address
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC == contents){
		PSW[0] = '1';
	} 
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was equal to contents of memory address pointed to by Register R[%d] (%d), then value of PSW[0] was set to true\n", ACC, REG, contents) ;
}

//OPCODE 31
	void OP31(char *IR, char *PSW) {
	int REG, RADD, contents;	//REG is operand 1, RADD is register address, contents is data stored in address RADD
	printf("Opcode = 31. Compare Register Less\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);			//grabs operand 1 memory address

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	} 

	contents = FetchData(RADD);

	//checks if ACC < contents pointed to by register address
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC < contents){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was less than contents of memory address pointed to by Register R[%d] (%d), then value of PSW[0] was set to true\n", ACC, REG, contents) ;
}

//OPCODE 32
	void OP32(char *IR, char *PSW) {
	int REG, RADD, contents;	//REG is operand 1, RADD is register address, contents is data stored in address RADD
	printf("Opcode = 32. Compare Register Greater\n");
	printIR(IR);

	REG = ParseOP1Reg(IR);			//grabs operand 1 memory address

	switch(REG) {
		case 0: RADD=R0; break;		//address contained in R0
		case 1: RADD=R1; break;		//address contained in R1
		case 2: RADD=R2; break;		//address contained in R2
		case 3: RADD=R3; break;		//address contained in R3
	} 

	contents = FetchData(RADD);

	//checks if ACC > contents pointed to by register address
	//if true set PSW[0] to true, if not, sets it to false
	if(ACC > contents){
		PSW[0] = '1';
	}
	else{
		PSW[0] = '0';
	}

	printf("If Accumulator (%d) was greater than contents of memory address pointed to by Register R[%d] (%d), then value of PSW[0] was set to true\n", ACC, REG, contents) ;
}

//OPCODE 33
	void OP33(char *IR, char *PSW, short int *PC) {
	int ADD;	//ADD is new PC address
	printf("Opcode = 33. Branch Conditional True\n");
	printIR(IR);

	ADD = ParseOP1(IR);			//grabs operand 1 two-digit int

	//if PSW[0] is true then PC set to ADD
	if (PSW[0] == '1'){
		*PC = ADD;
	}
	else{
		*PC += 1;
	}

	printf("If PSW[0] was true, PC was set to %d\n", ADD) ;
}

//OPCODE 34
	void OP34(char *IR, char *PSW, short int *PC) {
	int ADD;	//ADD is new PC address
	printf("Opcode = 34. Branch Conditional False\n");
	printIR(IR);

	ADD = ParseOP1(IR);			//grabs operand 1 two-digit int

	//if PSW[0] is false then PC set to ADD
	if (PSW[0] == '0'){
		*PC = ADD;
	}
	else{
		*PC += 1;
	}

	printf("If PSW[0] was false, PC was set to %d\n", ADD) ;
}

//OPCODE 35
	void OP35(char *IR, short int *PC) {
	int ADD;	//ADD is new PC address
	printf("Opcode = 35. Branch Unconditional\n");
	printIR(IR);

	ADD = ParseOP1(IR);			//grabs operand 1 two-digit int

	//set PC to ADD
	*PC = ADD;

	printf("PC was set to %d\n", ADD) ;
}


//MAIN FUNCTION
main(int argc, char *argv[])
{

 //Step 1 Read file into VM memory. Assume the file name is program2.

   fp = open("program2", O_RDONLY) ; //always check the return value. 
   printf("Open is %d\n", fp) ;

 if (fp < 0) //error in read 
        {printf("Could not open file\n");
         exit(0) ;
        }


 //read in the first line of the program

 int ret = read (fp, input_line, 7 ) ; //returns number of characters read`

  while (1)
        {
         if (ret <= 0) //indicates end of file or error
                break ; //breaks out of infinite loop
      	
	//copy from input line into program memory

	printf("Copying Program line %d into memory\n", program_line) ;
        for (i = 0; i < 6 ; i++)
		{
                memory[program_line][i] = input_line[i] ;
		printf("%c ", memory[program_line][i]) ;
		}
	printf("\n") ;

        //read in next line of code

        ret = read (fp, input_line, 7 ) ;

	//if the firat character is a 'Z' then you are reading data. 
	//No more program code so break out of loop


	if(input_line[0] == 'Z')
		break ;

        program_line++ ; //now at a new line in the prog
     }

printf("PROGRAM COPIED INTO VM MEMORY!!\n") ;

	int Done = 0 ;
	PC = 0;
	while (!Done)
		{for (i = 0; i < 6 ; i++)
			IR[i] = memory[PC][i] ;

		 opcode = ((int) (IR[0])- 48) * 10 ;
		 opcode += ((int) (IR[1])- 48) ;
	         printf("\nIn Program Execution Loop: New PC is %d OPCODE IS %d\n\n", PC, opcode) ;

		/* You need to put in the case statements for the remaining opcodes */
		switch(opcode) {
		case 0: OP0(IR) ; PC++; break ;
		case 1: OP1(IR) ; PC++; break ;
		case 2: OP2(IR) ; PC++; break ;
		case 3: OP3(IR) ; PC++ ; break ; 
		case 4: OP4(IR) ; PC++ ; break ; 
		case 5: OP5(IR) ; PC++ ; break ;
		case 6: OP6(IR) ; PC++ ; break ;  
		case 7: OP7(IR) ; PC++ ; break ; 
		case 8: OP8(IR) ; PC++ ; break ; 
		case 9: OP9(IR) ; PC++ ; break ; 
		case 10: OP10(IR) ; PC++ ; break ; 
		case 11: OP11(IR) ; PC++ ; break ; 
		case 12: OP12(IR) ; PC++ ; break ; 
		case 13: OP13(IR) ; PC++ ; break ; 
		case 14: OP14(IR) ; PC++; break ;
		case 15: OP15(IR) ; PC++; break ;
		case 16: OP16(IR) ; PC++; break ;
		case 17: OP17(IR) ; PC++; break ;
		case 18: OP18(IR) ; PC++; break ;
		case 19: OP19(IR) ; PC++; break ;
		case 20: OP20(IR) ; PC++; break ;
		case 21: OP21(IR) ; PC++; break ;
		case 22: OP22(IR) ; PC++; break ;
		case 23: OP23(IR) ; PC++; break ;
		case 24: OP24(IR, PSW) ; PC++ ; break; 
		case 25: OP25(IR, PSW) ; PC++ ; break; 
		case 26: OP26(IR, PSW) ; PC++ ; break; 
		case 27: OP27(IR, PSW) ; PC++ ; break; 
		case 28: OP28(IR, PSW) ; PC++ ; break; 
		case 29: OP29(IR, PSW); PC++ ; break ;
		case 30: OP30(IR, PSW); PC++ ; break ;
		case 31: OP31(IR, PSW); PC++ ; break ;
		case 32: OP32(IR, PSW); PC++ ; break ;
		case 33: OP33(IR, PSW, &PC) ; break ; 
		case 34: OP34(IR, PSW, &PC) ; break ; 
		case 35: OP35(IR, &PC) ; break ;
		case 99: printf("ALL DONE!!!\n") ; Done = 1 ; break ;
		default: printf("Instruction %d not found!!~\n", opcode) ;
			 exit(0) ;
		}
	}
	
	//get count of number of lines of memory
	int ct = 0;		//counter for lines of memory used
	char op0;		//first operand value  of memory line
	//loops through and looks at very first value in opcode, if not 0, 1, 2, 3, or Z then breaks, else increase count
	for(i = 0; i < 100; i++){
		op0 = memory[i][0];
		if((op0 != '0') && (op0 != '1') && (op0 != '2') && (op0 != '3') && (op0 != 'Z')&&(op0 != '9')){
			break;
		}
		else{
			ct++;
		}
	}
	printMEM(ct); 	//ct is upto parameter 
}