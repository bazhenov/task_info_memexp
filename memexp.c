#include <assert.h>
#include <libproc.h>
#include <mach/task_info.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/proc_info.h>
#include <unistd.h>

const char *MEMORY_UNITS[] = {"B", "KB", "MB", "GB", "TB"};

typedef struct {
  float memory_usage;
  const char *units;
} formatted_memory;

typedef struct {
  uint64_t virtual_memory;
  uint64_t memory;
  uint64_t real_memory;
} memory_usage;

uint64_t measure_memory_usage(memory_usage *usage) {
  pid_t pid = getpid();
  struct proc_taskinfo tinfo;
  int ret = proc_pidinfo(pid, PROC_PIDTASKINFO, 0, (task_info_t)&tinfo,
                         PROC_PIDTASKINFO_SIZE);
  if (ret != PROC_PIDTASKINFO_SIZE) {
    return 1;
  }

  struct rusage_info_v6 rusage;
  if (proc_pid_rusage(pid, RUSAGE_INFO_V6, (rusage_info_t)&rusage) !=
      KERN_SUCCESS) {
    return 1;
  }
  usage->virtual_memory = tinfo.pti_virtual_size;
  usage->memory = rusage.ri_phys_footprint;
  usage->real_memory = rusage.ri_resident_size;
  return 0;
}

static void usage(const char *program_name) {
  fprintf(stderr, "Usage: %s -t <type> -s <size> [-z] [-f] [-r]\n",
          program_name);
  fprintf(stderr, "       -t <type>\n");
  fprintf(stderr, "           malloc            malloc()\n");
  fprintf(stderr, "           alloca            alloca()\n");
  fprintf(stderr, "           mmap-anon         mmap(MAP_ANON)\n");
  fprintf(stderr, "           mmap-file-private mmap(MAP_PRIVATE)\n");
  fprintf(stderr, "           mmap-file-shared  mmap(MAP_SHARED)\n");
  fprintf(stderr, "       -s <size>   the size of allocation in megabytes\n");
  fprintf(stderr, "       -z          write zeros to an allocated buffer\n");
  fprintf(stderr, "       -f          fill an allocation with random data\n");
  fprintf(stderr, "       -r          read the whole buffer before measuring "
                  "memory impact\n");
}

static void *mmap_file(size_t size, int file_options, int protection,
                       int options) {
  const char *filename = "tmp.data";
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Failed to create file\n");
    exit(1);
  }
  for (size_t i = 0; i < size; i++) {
    char c = rand() & 255;
    fwrite(&c, 1, 1, file);
  }
  fclose(file);
  int fd = open(filename, file_options);
  if (fd == -1) {
    perror("Failed to open temprary file\n");
    exit(1);
  }
  void *buffer = mmap(NULL, size, protection, options, fd, 0);
  close(fd);
  return buffer;
}

/// Formats a memory size into a human-readable string.
void format_memory(int64_t bytes, formatted_memory *result) {
  float mem_size = (float)bytes;
  size_t order = 0;
  size_t max_order = sizeof(MEMORY_UNITS) / sizeof(MEMORY_UNITS[0]);
  while (fabsf(mem_size) >= 1024.0 && order < max_order - 1) {
    mem_size /= 1024.0;
    order++;
  }
  result->memory_usage = mem_size;
  result->units = MEMORY_UNITS[order];
}

int main(int argc, char **argv) {
  int opt;
  const char *type = NULL;
  bool zero = false;
  bool fill = false;
  bool read = false;
  size_t size = 0;

  while ((opt = getopt(argc, argv, "t:s:zfrh")) != -1) {
    switch (opt) {
    case 't':
      type = optarg;
      break;
    case 'z':
      zero = true;
      break;
    case 'f':
      fill = true;
      break;
    case 'r':
      read = true;
      break;
    case 's':
      size = atoi(optarg);
      break;
    case 'h':
    default:
      usage(argv[0]);
      exit(1);
    }
  }
  if (type == NULL) {
    fprintf(stderr, "Type should be given\n");
    exit(1);
  }
  if (size < 1) {
    fprintf(stderr, "Size should be at least 1\n");
    exit(1);
  }

  size *= 1024 * 1024;

  // Measuring memory usage before
  memory_usage before;
  if (measure_memory_usage(&before) != 0) {
    perror("Failed to get memory usage\n");
    exit(1);
  }

  void *buffer = NULL;
  if (strcmp(type, "malloc") == 0) {
    buffer = malloc(size);
    if (buffer == NULL) {
      perror("malloc() failed\n");
      exit(1);
    }
  } else if (strcmp(type, "alloca") == 0) {
    buffer = alloca(size);
    if (buffer == NULL) {
      perror("alloca() failed\n");
      exit(1);
    }
  } else if (strcmp(type, "mmap-anon") == 0) {
    buffer =
        mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (buffer == MAP_FAILED) {
      perror("mmap() failed");
      exit(1);
    }
  } else if (strcmp(type, "mmap-file-private") == 0) {
    buffer = mmap_file(size, O_RDONLY, PROT_READ | PROT_WRITE, MAP_PRIVATE);
    if (buffer == MAP_FAILED) {
      perror("mmap() failed");
      exit(1);
    }
  } else if (strcmp(type, "mmap-file-shared") == 0) {
    buffer = mmap_file(size, O_RDWR, PROT_READ | PROT_WRITE, MAP_SHARED);
    if (buffer == MAP_FAILED) {
      perror("mmap() failed");
      exit(1);
    }
  } else {
    fprintf(stderr, "Unknown allocation type: %s\n", type);
    exit(1);
  }

  assert(buffer != NULL);

  // Maniipulating memory block
  if (read) {
    for (size_t i = 0; i < size; i++) {
      volatile char r = ((char *)buffer)[i];
    }
  }
  if (zero) {
    memset(buffer, 0, size);
  }
  if (fill) {
    for (size_t i = 0; i < size; i++) {
      ((char *)buffer)[i] = rand() & 255;
    }
  }

  // Measuring memory usage after
  memory_usage after;
  if (measure_memory_usage(&after) != 0) {
    perror("Failed to get memory usage\n");
    exit(1);
  }

  formatted_memory mem;

  printf("%6s%15s   %15s   %15s\n", "", "Virtual size", "Real memory",
         "Memory");

  printf("%6s", "Before");
  format_memory(before.virtual_memory, &mem);
  printf("%15.2f %2s", mem.memory_usage, mem.units);
  format_memory(before.real_memory, &mem);
  printf("%15.2f %2s", mem.memory_usage, mem.units);
  format_memory(before.memory, &mem);
  printf("%15.2f %2s", mem.memory_usage, mem.units);
  printf("\n");

  printf("%6s", "After");
  format_memory(after.virtual_memory, &mem);
  printf("%15.2f %2s", mem.memory_usage, mem.units);
  format_memory(after.real_memory, &mem);
  printf("%15.2f %2s", mem.memory_usage, mem.units);
  format_memory(after.memory, &mem);
  printf("%15.2f %2s", mem.memory_usage, mem.units);
  printf("\n");

  printf("%6s", "Diff");
  format_memory((int64_t)after.virtual_memory - (int64_t)before.virtual_memory,
                &mem);
  printf("%+15.2f %2s", mem.memory_usage, mem.units);
  format_memory((int64_t)after.real_memory - (int64_t)before.real_memory, &mem);
  printf("%+15.2f %2s", mem.memory_usage, mem.units);
  format_memory((int64_t)after.memory - (int64_t)before.memory, &mem);
  printf("%+15.2f %2s", mem.memory_usage, mem.units);
  printf("\n");

  getchar();

  return 0;
}
