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

- Em qual etapa ocorrem as faltas nos modos populate e mlock? Por que uma aplicação de tempo real prefere esse comportamento?
    R: populate -> mmap | Mlock -> mlock; Para evitar falta de paginas durante a excução, reduzindo atrasos.

- Por que o modo huge gera 8 faltas, e quantas faltas ocorreriam com 32 MiB em páginas grandes de 2 MB (x86_64)?
    R: Pois foi alocado 8 paginas; ocorreriam 16 faltas ( 2 MiB por falta)

- Por que o modo file com cache frio gera faltas maiores, e por que o total de faltas é bem menor que o número de páginas?
    R: Pois não estão em cache, precisam ser lidas do disco; Por conta da leitura antecipada (read-ahead) do OS do linux

- Por que o VmRSS não aumenta no modo huge? Em qual campo de /proc/<pid>/status essa memória aparece? (consulte man 5 proc)
    R:Porque não são contabilizadas da mesma forma que paginas no RSS; A memória aparece no "HugetlbPages"