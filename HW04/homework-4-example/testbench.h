#include <unistd.h>
#include <sys/syscall.h>

#define FUNC_ATTR __attribute__ ((section("my_text")))
#define DATA_ATTR __attribute__ ((section("my_data"))) __attribute__ ((nocommon))

int toplevel_fnc(void);

/*********************************************************************/
#if defined(mips)

#define INTERNAL_SYSCALL(name, err, nr, args...) \
        internal_syscall##nr (, "li\t$2, %2\t\t\t# " #name "\n\t",      \
                              "i" (__NR_##name), err, args)

#define __SYSCALL_CLOBBERS "$1", "$3", "$8", "$9", "$10", "$11", "$12", \
	"$13", "$14", "$15", "$24", "$25", "memory"

#define internal_syscall0(ncs_init, cs_init, input, err, dummy...)	\
({									\
	long _sys_result;						\
									\
	{								\
	register long __v0 asm("$2") ncs_init;				\
	register long __a3 asm("$7");					\
	__asm__ volatile (						\
	".set\tnoreorder\n\t"						\
	cs_init								\
	"syscall\n\t"							\
	".set reorder"							\
	: "=r" (__v0), "=r" (__a3)					\
	: input								\
	: __SYSCALL_CLOBBERS);						\
	err = __a3;							\
	_sys_result = __v0;						\
	}								\
	err? -1: _sys_result;							\
})

#define internal_syscall1(ncs_init, cs_init, input, err, arg1)		\
({									\
	long _sys_result;						\
									\
	{								\
	register long __v0 asm("$2") ncs_init;				\
	register long __a0 asm("$4") = (long) arg1;			\
	register long __a3 asm("$7");					\
	__asm__ volatile (						\
	".set\tnoreorder\n\t"						\
	cs_init								\
	"syscall\n\t"							\
	".set reorder"							\
	: "=r" (__v0), "=r" (__a3)					\
	: input, "r" (__a0)						\
	: __SYSCALL_CLOBBERS);						\
	err = __a3;							\
	_sys_result = __v0;						\
	}								\
	err? -1: _sys_result;							\
})

#define internal_syscall2(ncs_init, cs_init, input, err, arg1, arg2)	\
({									\
	long _sys_result;						\
									\
	{								\
	register long __v0 asm("$2") ncs_init;				\
	register long __a0 asm("$4") = (long) arg1;			\
	register long __a1 asm("$5") = (long) arg2;			\
	register long __a3 asm("$7");					\
	__asm__ volatile (						\
	".set\tnoreorder\n\t"						\
	cs_init								\
	"syscall\n\t"							\
	".set\treorder"						\
	: "=r" (__v0), "=r" (__a3)					\
	: input, "r" (__a0), "r" (__a1)					\
	: __SYSCALL_CLOBBERS);						\
	err = __a3;							\
	_sys_result = __v0;						\
	}								\
	err? -1: _sys_result;							\
})

#define internal_syscall3(ncs_init, cs_init, input, err, arg1, arg2, arg3)\
({									\
	long _sys_result;						\
									\
	{								\
	register long __v0 asm("$2") ncs_init;				\
	register long __a0 asm("$4") = (long) arg1;			\
	register long __a1 asm("$5") = (long) arg2;			\
	register long __a2 asm("$6") = (long) arg3;			\
	register long __a3 asm("$7");					\
	__asm__ volatile (						\
	".set\tnoreorder\n\t"						\
	cs_init								\
	"syscall\n\t"							\
	".set\treorder"						\
	: "=r" (__v0), "=r" (__a3)					\
	: input, "r" (__a0), "r" (__a1), "r" (__a2)			\
	: __SYSCALL_CLOBBERS);						\
	err = __a3;							\
	_sys_result = __v0;						\
	}								\
	err? -1: _sys_result;							\
})

/*********************************************************************/
#elif defined(__i386__)
# define ASMFMT_1(arg1) \
        , "b" (arg1)
# define ASMFMT_2(arg1, arg2) \
        , "b" (arg1), "c" (arg2)
# define ASMFMT_3(arg1, arg2, arg3) \
        , "b" (arg1), "c" (arg2), "d" (arg3)
# define ASMFMT_4(arg1, arg2, arg3, arg4) \
        , "b" (arg1), "c" (arg2), "d" (arg3), "S" (arg4)
# define ASMFMT_5(arg1, arg2, arg3, arg4, arg5) \
        , "b" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5)

#define EXTRAVAR_0
#define EXTRAVAR_1
#define EXTRAVAR_2
#define EXTRAVAR_3
#define EXTRAVAR_4
#define EXTRAVAR_5

#define LOADARGS_1
#define LOADARGS_2
#define LOADARGS_3
#define LOADARGS_4
#define LOADARGS_5

#define RESTOREARGS_1
#define RESTOREARGS_2
#define RESTOREARGS_3
#define RESTOREARGS_4
#define RESTOREARGS_5

#define INTERNAL_SYSCALL(name, err, nr, args...) \
  ({                                                                          \
    register unsigned int resultvar;                                          \
    EXTRAVAR_##nr                                                             \
    asm volatile (                                                            \
    LOADARGS_##nr                                                             \
    "movl %1, %%eax\n\t"                                                      \
    "int $0x80\n\t"                                                           \
    RESTOREARGS_##nr                                                          \
    : "=a" (resultvar)                                                        \
    : "i" (__NR_##name) ASMFMT_##nr(args) : "memory", "cc");                  \
    (int) resultvar; })

/*********************************************************************/
#else
/*
    syscall number      rax
    arg 1               rdi
    arg 2               rsi
    arg 3               rdx
    arg 4               r10
    arg 5               r8
    arg 6               r9
*/

#define SYS_ify(syscall_name)   __NR_##syscall_name

# undef DO_CALL
# define DO_CALL(syscall_name, args)            \
    DOARGS_##args                               \
    movl $SYS_ify (syscall_name), %eax;         \
    syscall;

#endif
