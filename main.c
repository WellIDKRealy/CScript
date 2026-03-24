#define _GNU_SOURCE
#include <stdio.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifndef __NR_memfd_create
#  if defined(__x86_64__)
#    define __NR_memfd_create 319
#  elif defined(__i386__)
#    define __NR_memfd_create 356
#  elif defined(__arm__)
#    define __NR_memfd_create 385
#  elif defined(__aarch64__)
#    define __NR_memfd_create 279
#  elif defined(__riscv)
#    define __NR_memfd_create 279
#  else
#    warning "Unknown architecture for memfd_create syscall number"
#  endif
#endif

#ifndef __NR_memfd_create
#  if defined(__x86_64__)
#    define __NR_memfd_create 319
#  elif defined(__i386__)
#    define __NR_memfd_create 356
#  elif defined(__arm__)
#    define __NR_memfd_create 385
#  elif defined(__aarch64__)
#    define __NR_memfd_create 279
#  elif defined(__riscv)
#    define __NR_memfd_create 279
#  else
#    warning "Unknown architecture for memfd_create syscall number"
#  endif
#endif

#define MFD_CLOEXEC         0x0001U
#define MFD_ALLOW_SEALING   0x0002U

static inline int memfd_create(const char *name, unsigned int flags) {
  return syscall(__NR_memfd_create, name, flags);
}

int main(int argc, char* argv[]) {
  assert(argc >= 2);
  int fd = memfd_create("exec", MFD_CLOEXEC | MFD_ALLOW_SEALING);
  assert(fd != -1);

  pid_t pid = getpid();
  
  char path[64];
  snprintf(path, sizeof(path), "/proc/%d/fd/%d", pid, fd); 

  // Compile
  int sep;
  for(sep = 0; sep < argc; sep++) {
    if(strcmp(argv[sep], "--") == 0) {
      break;
    }
  }

  // Separator needs to be present
  assert(sep != argc);

  pid_t other_pid = fork();
  assert(other_pid >= 0);
  if(getpid() != pid) {
    char* tcc_argv[sep + 4];
    tcc_argv[0] = "tcc";
    for(int i = 1; i < sep; i++) {
      tcc_argv[i] = argv[i];
    }
    
    tcc_argv[sep] = argv[sep + 1];
    tcc_argv[sep + 1] = "-o";
    tcc_argv[sep + 2] = path;
    tcc_argv[sep + 3] = NULL;
 
    execvpe("tcc", tcc_argv, environ);
  }

  int comp_stat;
  waitpid(other_pid, &comp_stat, 0);
  if(comp_stat != 0) {
    return comp_stat;
  }

  lseek(fd, 0, SEEK_SET);
  fexecve(fd, argv + sep + 1, environ);
  
}
