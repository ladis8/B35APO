#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12
#define s0 $16
#define s1 $17
#define s2 $18
#define s3 $19
#define s4 $20
.globl    pole
.data
.align    2
pole:
.word    17, 5, 20, 9, 17, 3, 16, 10, 8, 10, 7, 11, 1, 13, 9, 14, 8, 13, 19, 18, 17, 6
.text
.globl start
.set noat
.set noreorder
.ent start
start:
      // s0 - bazova adresa zacatku vstupniho pole
      // s1 - pocet prvku vstupniho pole
      // s2 - bazova adresa zacatku vystupniho pole
      // s3 - velikost okna
  
      la   s0, pole     // Nastaveni adresy pocatku vstupniho pole do registru s0
      addi s1, $0, 13   // Nastaveni poctu prvku vstupniho pole do s1
      sll  s1, s1, 2    // Vypocteni velikosti vstupniho pole (v Bajtech)
  
      add  s2, s0, s1   // Nastaveni adresy pocatku vystupniho za konec vstupniho pole
      addi s3, $0, 5    // Nastaveni poctu prvku okna
      sll  s3, s3, 2    // Vypocteni velikosti vystupniho okna
      addi t1, $0, 0     // i = 0;
      sub  s1, s1, s3    // Nastaveni horni meze vnejsiho cyklu na velikost...
      addi s1, s1, 0x4   // ...vystupniho pole
L1:
      addi t2, $0, 0     // j = 0; - offset v okne (plati pro vnitrni smycku)
      addi t4, $0, 0     // t4 = 0;  - nulovani souctu v okne
L2:
      add  s4, s0, t2    // s4 - adresa aktualniho prkvu v okne
      lw   t0, 0(s4)     // t0 <- Mem[s4]
      add  t4, t4, t0    // Pripocetni dalsiho prvku
      addi t2, t2, 0x4   // j += 4; - posun na dalsi prvek v okne
      slt  t3, t2, s3    // t3 = (t2 < s3) ? 1 : 0;  
      bne  t3, $0, L2    // Test na ukonceni cyklu
  
      srl  t4, t4, 2     //
      sw   t4, 0(s2)     // Ulozeni vysledku do vystupniho pole: Mem[s2] <- t4
      addi s2, s2, 0x4   // Posun na dalsi prvek ve vystupnim poli
      addi s0, s0, 0x4   // Posun okna ve vstupnim poli
  
      addi t1, t1, 0x4   // i += 4; - inkrementace pocitadla
      slt  t3, t1, s1    // t3 = (t1 < s1) ? 1 : 0;
      bne  t3, $0, L1
  
      nop
inf_loop:
      beq $0,$0, inf_loop
	.end start
