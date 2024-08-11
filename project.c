/*
Computer Logic & Organization Project
Code Written by Adam Betinsky, Kenneth Valladares and Sebastian Chacon from 4/7/2024-4/17/2024
*/

//START OF PROGRAM

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

  if(ALUControl == 0){ 

    *ALUresult = A + B;

  } else if(ALUControl == 1){ 
    *ALUresult = A - B;

  } else if(ALUControl == 2){  

    if((signed)A < (signed)B){
      *ALUresult = 1;
    } else{
      *ALUresult = 0;
    }

  } else if(ALUControl == 3){  

    if(A < B){
      *ALUresult = 1;
    } else{
      *ALUresult = 0;
    }
    
  } else if(ALUControl == 4){ 
    
    *ALUresult = A & B;
    
  } else if(ALUControl == 5){  
    *ALUresult = A | B;
    
  } else if(ALUControl == 6){  
    *ALUresult = B << 16;
    
  } else{                    
    *ALUresult = !A;
  }
  
  if(*ALUresult == 0){
    *Zero = 1;
  } else {
    
    *Zero = 0;
  }

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

  unsigned i = PC >> 2;

  //Test for word alignment
  if(PC % 4 != 0)  
     return 1;
  
  *instruction = Mem[i];
    return 0;

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

  *op = instruction >> 26;  //op = instruction[31 - 26]
  
  *r1 = (instruction & (0b11111 << 20)) >> 21;  //r1 = instruction[25 - 21]
    
  *r2 = (instruction & (0b11111 << 15)) >> 16;  //r2 = instruction[20 - 16]
    
  *r3 = (instruction & (0b11111 << 10)) >> 11;  //r3 = instruction[15 - 11]
    
  *funct = instruction & (0b111111); // 6 bits address so 0b111111
    
  *offset = instruction & (0b1111111111111111); // 16 bits address so 0b1111111111111111
    
  *jsec = instruction & (0b11111111111111111111111111); // 26 bits address, so would it be 0b11111111111111111111111111


  /*r1,	// instruction [25-21]
	r2,	// instruction [20-16]
	r3,	// instruction [15-11]
	funct,	// instruction [5-0]
	offset,	// instruction [15-0]
	jsec;	// instruction [25-0]*/
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

// if statements

  //load work updqte the control
  //ALU source

  //R type instructions.
  if (op == 0b000000) {
    controls->RegDst = 1;
    controls->ALUSrc = 0;
    controls->MemtoReg = 0;
    controls->RegWrite = 1;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 7;
    controls->Jump = 0;

    //Add I type instructions.
  } else if (op == 0b001000) {
    controls->RegDst = 0;
    controls->ALUSrc = 1;
    controls->MemtoReg = 0;
    controls->RegWrite = 1;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 0;
    controls->Jump = 0;

    //Loading unsigned I type instructions.
  } else if (op == 0b001111) {
    controls->RegDst = 0;
    controls->ALUSrc = 1;
    controls->MemtoReg = 0;
    controls->RegWrite = 1;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 6;
    controls->Jump = 0;

    //Load a word.
  } else if (op == 0b100011) {
    controls->RegDst = 0;
    controls->ALUSrc = 1;
    controls->MemtoReg = 1;
    controls->RegWrite = 1;
    controls->MemRead = 1;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 0;
    controls->Jump = 0;

    //Store a word.
  } else if (op == 0b101011) {  //43
    controls->RegDst = 2;
    controls->ALUSrc = 1;
    controls->MemtoReg = 2;
    controls->RegWrite = 0;
    controls->MemRead = 0;
    controls->MemWrite = 1;
    controls->Branch = 0;
    controls->ALUOp = 0;
    controls->Jump = 0;

    //Beq aka branch equals.
  } else if (op == 0b000100) {
    controls->RegDst = 2;
    controls->ALUSrc = 0;
    controls->MemtoReg = 2;
    controls->RegWrite = 0;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 1;
    controls->ALUOp = 1;
    controls->Jump = 0;

    //Slt aka set on less than.
  } else if (op == 0b001010) {
    controls->RegDst = 1;
    controls->ALUSrc = 0;
    controls->MemtoReg = 0;
    controls->RegWrite = 1;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 2;
    controls->Jump = 0;

    //Slt aka set on less than for unsigned.
  } else if (op == 0b001011) {
    controls->RegDst = 1;
    controls->ALUSrc = 0;
    controls->MemtoReg = 0;
    controls->RegWrite = 1;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 3;
    controls->Jump = 0;

    //Jump register. 
  } else if (op == 0b000010) {
    controls->RegDst = 0;
    controls->ALUSrc = 0;
    controls->MemtoReg = 0;
    controls->RegWrite = 0;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->Branch = 0;
    controls->ALUOp = 0;
    controls->Jump = 1;

    //Halt operation
  } else {
    return 1;
  }
  return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

  *data1 = Reg[r1];
  
  *data2 = Reg[r2];

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

int numOnes = 0xffff << 16;

*extended_value = (offset & 1 << 15 ? offset | numOnes : offset);

  /*  Same thing as the code in line 139
  if(offset & 1 << 15){
    *extended_value = offset | numOnes;
  } else {
    *extended_value = offset;
  }
  */
  
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

  if (ALUSrc == 1) {
    data2 = extended_value;
  }

  if (ALUOp == 7){  //check if ALUOp is 7, meaning that is R type
    
    switch (funct) {

      case 0b100000: // add transform 32 into hexadecimal 0b100000
        ALUOp = 0;
        break;

      case 0b100010: // sub transform 34 into hexadecimal 0b100010
        ALUOp = 1;
        break;

      case 0b101010: // slt, transform 42 into hexadecimal 0b101010
        ALUOp = 2;
        break;

      case 0b101011: // sltu, transform 43 into hexadecimal 0b101011
        ALUOp = 3;
        break;

      case 0b100100: // and, transform 36 into hexadecimal 0b100100
        ALUOp = 4;
        break;

      case 0b100101: // or, transform 37 into hexadecimal 0b100101
        ALUOp = 5;
        break;

      case 0b000000: // sll, transform 0 into hexadecimal 0b000000
        ALUOp = 6;
        break;

      case 0b000010: // srl, transform 1 into hexadecimal 0b000010
        ALUOp = 7;
        break;

  
  }

    ALU(data1, data2, ALUOp, ALUresult, Zero);
  
  } else {
    ALU(data1, data2, ALUOp, ALUresult, Zero);
  }

  if(ALUOp < 0 || ALUOp > 7){
    return 1;
  }

  return 0;


/*

ALU_operations(…)
1. Apply ALU operations on data1, and data2 or extended_value (determined by
ALUSrc).
2. The operation performed is based on ALUOp and funct.
3. Apply the function ALU(…).
4. Output the result to ALUresult.
5. Return 1 if a halt condition occurs; otherwise, return 0. 


+ Set parameters for A, B, and ALUControl
+ If R-type instruction, look at funct
+ In the end, call ALU function
*/

  
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

  if(MemWrite == 1){    //writing to memory
    if(ALUresult % 4 == 0){
      Mem[ALUresult >> 2] = data2;
    } else {
      return 1;
    }
  }

  if(MemRead == 1){    //reading from memory
    if(ALUresult % 4 == 0){
      *memdata = Mem[ALUresult >> 2];
    } else {
      return 1;
    }
  }


  return 0;
  
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  

  if(RegWrite == 1 && MemtoReg == 1){
    Reg[r2] = memdata;
  } else if(RegWrite == 1 && MemtoReg == 0 && RegDst == 1){

    Reg[r3] = ALUresult;

  
    
  } else if(RegWrite == 1 && MemtoReg == 0 && RegDst == 0){

    Reg[r2] = ALUresult;
    
  }


  
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

  *PC += 4;

  if(Jump == 1){
    *PC = (jsec << 2) | (*PC & 0xf0000000);
  }

  if(Branch == 1 && Zero == 1){
    *PC += extended_value << 2;
  }


}


//END OF PROGRAM

