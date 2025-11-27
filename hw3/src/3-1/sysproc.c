#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_ssusbrk(void)
{
  int addr;
  int n, t;

  // 인자가 제대로 들어왔는지 체크
  if (argint(0, &n) < 0 || argint(1, &t) < 0)
    return -1;
  if (n % PGSIZE != 0 || n == 0 || (n < 0 && t < 0))
    return -1;

  if (n > 0) // allocation
  {
    addr = myproc()->sz;
    myproc()->sz += n;
    myproc()->na += n / PGSIZE;
  } 
  else // deallocation
  {
    struct rtcdate r;
    cmostime(&r);
    cprintf("Memory deallocation request(%d): %d-%d-%d %d:%d:%d\n", t, r.year, r.month, r.day, r.hour, r.minute, r.second);
    addr = myproc()->sz;
    myproc()->pending_dealloc_ticks = t; // 새로운 호출을 기준으로 초기화
    myproc()->pending_dealloc_size += n; // 누적
  }
  
  return addr;
}

int sys_memstat(void) {
  return memstat();
}