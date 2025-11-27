#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"

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
sys_lseek(void) {
    int fd;
    int offset, whence;
    struct file *f;

    if (argint(0, &fd) < 0 || argint(1, &offset) < 0 || argint(2, &whence) < 0) return -1;
    
	if ((f = myproc()->ofile[fd]) == 0) return -1;

    switch (whence) {
        case SEEK_SET:
            if (offset < 0 || offset > f->ip->size) return -1;
            f->off = offset;
            break;
        case SEEK_CUR:
            if (f->off + offset < 0 || f->off + offset > f->ip->size) return -1;
            f->off += offset;
            break;
        case SEEK_END:
            if (f->ip->size + offset < 0 || f->ip->size + offset > f->ip->size) return -1;
            f->off = f->ip->size + offset;
            break;
        default:
            return -1;
    }

    return f->off;
}
