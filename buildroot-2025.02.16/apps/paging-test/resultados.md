## Questão 2
# paging-test lazy 32
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 81     majflt = 1      VmRSS = 1480 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1480 kB
acesso   minflt = 8192   majflt = 0      VmRSS = 34248 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1556 kB
# paging-test populate 32 
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 83     majflt = 0      VmRSS = 1488 kB
mmap     minflt = 8192   majflt = 0      VmRSS = 34256 kB
acesso   minflt = 0      majflt = 0      VmRSS = 34256 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1564 kB
# paging-test mlock 32
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 74     majflt = 1      VmRSS = 1312 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1440 kB
mlock    minflt = 8192   majflt = 0      VmRSS = 34208 kB
acesso   minflt = 0      majflt = 0      VmRSS = 34208 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1512 kB

-sync 
-echo 3 > /proc/sys/vm/drop_caches 

# paging-test file 16 /root/dados (Frio)
pagina = 4096 bytes, regiao = 16 MiB (4096 paginas)
inicio   minflt = 74     majflt = 4      VmRSS = 1452 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1452 kB
acesso   minflt = 256    majflt = 129    VmRSS = 17728 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1452 kB

# paging-test file 16 /root/dados
pagina = 4096 bytes, regiao = 16 MiB (4096 paginas)
inicio   minflt = 76     majflt = 0      VmRSS = 1468 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1468 kB
acesso   minflt = 257    majflt = 0      VmRSS = 17780 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1468 kB

# paging-test huge 32
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 75     majflt = 1      VmRSS = 1476 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1476 kB
acesso   minflt = 8      majflt = 0      VmRSS = 1476 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1476 kB

## Questão 3

-