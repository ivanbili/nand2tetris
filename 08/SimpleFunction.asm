// // This file is part of www.nand2tetris.org
// // and the book "The Elements of Computing Systems"
// // by Nisan and Schocken, MIT Press.
// // File name: projects/08/FunctionCalls/SimpleFunction/SimpleFunction.vm
// 
// // Performs a simple calculation and returns the result.
// function SimpleFunction.test 2
(SimpleFunction.test)
  @SP
  M=M+1
  M=M+1
  A=M-1
  M=0
  A=A-1
  M=0
// push local 0
  @0
  D=A
  @LCL
  A=D+M
  D=M
  @SP
  M=M+1
  A=M-1
  M=D
// push local 1
  @1
  D=A
  @LCL
  A=D+M
  D=M
  @SP
  M=M+1
  A=M-1
  M=D
// add
  @SP
  M=M-1
  A=M
  D=M
  A=A-1
  D=M+D
  M=D
// not
  @SP
  A=M-1
  M=!M
// push argument 0
  @0
  D=A
  @ARG
  A=D+M
  D=M
  @SP
  M=M+1
  A=M-1
  M=D
// add
  @SP
  M=M-1
  A=M
  D=M
  A=A-1
  D=M+D
  M=D
// push argument 1
  @1
  D=A
  @ARG
  A=D+M
  D=M
  @SP
  M=M+1
  A=M-1
  M=D
// sub
  @SP
  M=M-1
  A=M
  D=M
  A=A-1
  D=M-D
  M=D
// return
  @LCL
  D=M
  @R13
  M=D
  @5
  A=D-A
  D=M
  @R14
  M=D
  @SP
  M=M-1
  A=M
  D=M
  @ARG
  A=M
  M=D
  @ARG
  D=M+1
  @SP
  M=D
  @R13
  M=M-1
  A=M
  D=M
  @THAT
  M=D
  @R13
  M=M-1
  A=M
  D=M
  @THIS
  M=D
  @R13
  M=M-1
  A=M
  D=M
  @ARG
  M=D
  @R13
  M=M-1
  A=M
  D=M
  @LCL
  M=D
  @R14
  A=M
  0;JMP
