// // This file is part of www.nand2tetris.org
// // and the book "The Elements of Computing Systems"
// // by Nisan and Schocken, MIT Press.
// // File name: projects/07/StackArithmetic/StackTest/StackTest.vm
// 
// // Executes a sequence of arithmetic and logical operations
// // on the stack. 
// push constant 17
  @17
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 17
  @17
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// eq
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest0
  D;JEQ
  @SP
  A=M-1
  M=0
(LBL.StackTest0)
// push constant 17
  @17
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 16
  @16
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// eq
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest1
  D;JEQ
  @SP
  A=M-1
  M=0
(LBL.StackTest1)
// push constant 16
  @16
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 17
  @17
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// eq
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest2
  D;JEQ
  @SP
  A=M-1
  M=0
(LBL.StackTest2)
// push constant 892
  @892
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 891
  @891
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// lt
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest3
  D;JLT
  @SP
  A=M-1
  M=0
(LBL.StackTest3)
// push constant 891
  @891
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 892
  @892
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// lt
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest4
  D;JLT
  @SP
  A=M-1
  M=0
(LBL.StackTest4)
// push constant 891
  @891
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 891
  @891
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// lt
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest5
  D;JLT
  @SP
  A=M-1
  M=0
(LBL.StackTest5)
// push constant 32767
  @32767
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 32766
  @32766
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// gt
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest6
  D;JGT
  @SP
  A=M-1
  M=0
(LBL.StackTest6)
// push constant 32766
  @32766
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 32767
  @32767
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// gt
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest7
  D;JGT
  @SP
  A=M-1
  M=0
(LBL.StackTest7)
// push constant 32766
  @32766
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 32766
  @32766
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// gt
  @SP
  M=M-1
  A=M-1
  D=M
  M=-1
  A=A+1
  D=D-M
  @LBL.StackTest8
  D;JGT
  @SP
  A=M-1
  M=0
(LBL.StackTest8)
// push constant 57
  @57
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 31
  @31
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// push constant 53
  @53
  D=A
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
// push constant 112
  @112
  D=A
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
// neg
  @SP
  A=M-1
  M=-M
// and
  @SP
  M=M-1
  A=M
  D=M
  A=A-1
  D=M&D
  M=D
// push constant 82
  @82
  D=A
  @SP
  M=M+1
  A=M-1
  M=D
// or
  @SP
  M=M-1
  A=M
  D=M
  A=A-1
  D=M|D
  M=D
// not
  @SP
  A=M-1
  M=!M
