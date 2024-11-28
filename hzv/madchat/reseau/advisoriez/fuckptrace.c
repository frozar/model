/********************************************************\
 * fuckptrace.c                                         *
 * By truff (truff@projet7.org)                         *
 *                                                      *
 * lkm to bypass anti ptrace protections in reverse     *
 * engineering process.                                 *
 *                                                      *
 * Greetz to #root and #!fr people                      *
 *                                                      *
 *   www.projet7.org           - Security Researchs -   *
\********************************************************/


#define MODULE 
#define __KERNEL__ 

#include <linux/module.h> 
#include <linux/kernel.h> 
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <linux/sched.h>

extern void* sys_call_table[]; 
extern struct task_struct *current;

int (*orig_ptrace)(int requete, int pid, int addr, int data);


int hacked_ptrace(int requete, int pid, int addr, int data)
{ 
  int ret = 0;

  if (current->ptrace & PT_PTRACED)
    ret = 0;
  else 
    ret = orig_ptrace (requete, pid, addr, data); 

  return ret;
} 

int init_module(void)  
{ 
  orig_ptrace = sys_call_table[SYS_ptrace]; 
  sys_call_table[SYS_ptrace] = hacked_ptrace; 
  
  return 0; 
} 

void cleanup_module(void)  
{ 
  sys_call_table[SYS_ptrace]=orig_ptrace; 
}

