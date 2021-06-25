// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

(WHITE)
  @KBD
  D=M
  @BLACKEN
  D;JNE
  @WHITE
  0;JMP
(BLACK)
  @KBD
  D=M
  @WHITEN
  D;JEQ
  @BLACK
  0;JMP
(BLACKEN)
  @8192
  D=A
(LOOPBL)
  D=D-1
  @SCREEN
  A=A+D
  M=-1
  @LOOPBL
  D;JNE
  @BLACK
  0;JMP
(WHITEN)
  @8192
  D=A
(LOOPWH)
  D=D-1
  @SCREEN
  A=A+D
  M=0
  @LOOPWH
  D;JNE
  @WHITE
  0;JMP