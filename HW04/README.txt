Zadání domácího úkolu číslo 4
=============================

V souborech program-x86.list a program-mips.list naleznete
výpis programu z paměti počítače.

Program byl nejdříve zkompilovaný překladačem jazyka C pro cílovou platformu
x86, 32-bitový režim a operační systém GNU/Linux. Poté byl stejný program
zkompilovaný překladačem pro little-endian variantu architektury MIPS32.

Výpis obsahuje funkci toplevel_fnc(), která volá funkci subroutine_fnc().
Dále se v programu vyskytují systémová volání.

Vaším úkolem je analyzovat funkci programu.

V souboru syscalls.lst naleznete jména systémových volání,
která se mohou v programu vyskytovat. Upravte seznam
tak, aby obsahoval jen ta volání, která skutečně program
používá. Dodržte formát jedno jméno volání na jeden řádek.
Nepoužívejte žádné oddělovače.
(správně vyplněný seznam 1 bod)


Na základě analýzy funkce subroutine_fnc() vyplňte seznam
parametrů funkce v hlavičkovém souboru subroutine.h.
Parametry budou obvykle celá čísla (int), znaky (char)
nebo ukazatele na tyto typy.
(správný počet a typ argumentů 2 body)

V souboru subroutine.c implementujte funkci subroutine_fnc()
s ekvivalentním chováním, jako měla původní funkce v assembleru.

V souboru toplevel.c implementujte zbytek funkcionality
programu s tím, že tato funkce bude vracet stejnou číselnou
hodnotu jako původní program.

Odevzdaný program musí být možné zkompilovat kompilátorem.
Ekvivalent volání make v adresáři. Instrukce v Makefile
pro sestavovací program neupravujte.


README.txt - tento soubor
program-mips.list - výpis paměti ze systému s architekturou MIPS,
                    do souboru si můžete dělat poznámky
program-x86.list - výpis paměti ze systému s architekturou x86
                    do souboru si můžete dělat poznámky
Makefile - instrukce pro sestavovací program, neupravujte
main.c - vstupní bod programu, volání funkce toplevel_fnc(), neupravujte
subroutine.h - prototyp funkce subroutine_fnc(), vyplňte typ návratové hodnoty
                    a paremtry
subroutine.c - implementace funkce, vyplňte
toplevel.c - implementace nejvyšší úrovně programu, vyplňte
syscalls.lst - seznam jen těch systémových volání, která jsou
               v programu využitá, upravte do této podoby

Homework 4 Assignment
=====================

Files program-x86.list and program-mips.list are dump/listing
of a program obtained from a computer memory.

Program has been compiled using C compiler for x86 32-bit
computer and OS GNU/Linux first. Then the same code has
been compiled for little-endian variant
of MIPS32 architecture.

Program consists of a function called toplevel_fnc(), which calls
a function subroutine_fnc(). One or both functions calls operating
systems services.

Your assignment is to analyze/decode program function.

The file syscalls.lst includes listing of some system calls.
Some small subset of these is used by the analyzed program.
Edit list such way, that only entries corresponding to actually
used systemcalls for your program variant are present
in the file. Keep formatting, write exactly single systemcall
per single line, use no separators or additional spaces
(the list matching program 1 point)

Fill list of the subroutine_fnc() function arguments with
types in header file subroutine.h.
The parameters are typically integer numbers (int), characters
(char) or pointers to these types.
(correct parameter types and count 2 points)

Copy parameters with types to a file subroutine.c and implement
body of the function subroutine_fnc(). The C implementation
of subroutine_fnc() should process parameters and call systemcalls
same way as original assembly code.

The remaining algorithm parts required to fulfil equivalent
operation as original code should be implemented in toplevel.c
file as body of toplevel_fnc(). A value returned by whole program
should be the same as is returned by original code.

It is required that submitted program(source code) can be compiled
by C language compiler. This requirement is equivalent
to calling make in the directory. Do not modify targets
and rules defined by provided Makefile.

README.txt - this file
program-mips.list - dump of computer/process memory for MIPS
                   architecture variant,
                   you can add notes, comments etc in this file
program-x86.list - dump of computer/process memory for x86
                   architecture variant,
                   you can add notes, comments etc in this file
Makefile - instructions for the program build by make utility
main.c - program entry point and main() function which calls
                   toplevel_fnc(), do not edit the file
subroutine.h - declaration of function subroutine_fnc(),
                   fill in arguments with types and return value type
subroutine.c - definition, implementation of the subroutine_fnc()
                   function, edit/implement body
toplevel.c - program top level logic/algorithm implementation, edit
syscalls.lst - list of the syscalls used in given program sample,
                   edit to not include calls not used in given fragment



