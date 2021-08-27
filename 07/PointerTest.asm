// // This file is part of www.nand2tetris.org
// // and the book "The Elements of Computing Systems"
// // by Nisan and Schocken, MIT Press.
// // File name: projects/07/MemoryAccess/PointerTest/PointerTest.vm
// 
// // Executes pop and push commands using the 
// // pointer, this, and that segments.
// push constant 3030
  @3030
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// pop pointer 0
  @THIS
  D=A
  @R13
  M=D
  @SP
  M=M-1
  A=M
  D=M
  @R13
  A=M
  M=D
// push constant 3040
  @3040
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// pop pointer 1
  @THAT
  D=A
  @R13
  M=D
  @SP
  M=M-1
  A=M
  D=M
  @R13
  A=M
  M=D
// push constant 32
  @32
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// pop this 2
  @2
  D=A
  @THIS
  D=D+M
  @R13
  M=D
  @SP
  M=M-1
  A=M
  D=M
  @R13
  A=M
  M=D
// push constant 46
  @46
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// pop that 6
  @6
  D=A
  @THAT
  D=D+M
  @R13
  M=D
  @SP
  M=M-1
  A=M
  D=M
  @R13
  A=M
  M=D
// push pointer 0
  @THIS
  D=M
  @SP
  M=M+1
  A=M-1
  M=D
// push pointer 1
  @THAT
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
// push this 2
  @2
  D=A
  @THIS
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
// push that 6
  @6
  D=A
  @THAT
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
