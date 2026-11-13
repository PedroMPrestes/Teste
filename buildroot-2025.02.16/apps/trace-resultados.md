
## Questão 1 

#### Lazy

> trace-cmd record -e exceptions:page_fault_user -e kmem:mm_page_alloc -F paging-test lazy 32


```
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 91     majflt = 1      VmRSS = 1484 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1484 kB
acesso   minflt = 8192   majflt = 0      VmRSS = 34252 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1560 kB
libtracecmd: No such file or directory
  No compression algorithms are supported
CPU0 data recorded at offset=0x126000
    430080 bytes in size

# trace-cmd report | grep -c 'paging-test.*page_fault_user'
8245

# trace-cmd report | grep -c 'paging-test.*mm_page_alloc'
8240

# trace-cmd report | grep -o 'error_code=0x[0-9a-f]*' | sort | uniq -c
     38 error_code=0x4
   8202 error_code=0x6
      5 error_code=0x7
```

#### Populate

> trace-cmd record -e exceptions:page_fault_user -e kmem:mm_page_alloc -F paging-test populate 32

```
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 88     majflt = 0      VmRSS = 1392 kB
mmap     minflt = 8192   majflt = 0      VmRSS = 34160 kB
acesso   minflt = 0      majflt = 0      VmRSS = 34160 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1464 kB

# trace-cmd report | grep -c 'paging-test.*page_fault_user'
51

# trace-cmd report | grep -c 'paging-test.*mm_page_alloc'
8234

# trace-cmd report | grep -o 'error_code=0x[0-9a-f]*' | sort | uniq -c
     36 error_code=0x4
     10 error_code=0x6
      5 error_code=0x7

```

#### Tabela 
| Modo | getrusage() | page_fault_user | mm_page_alloc | error_code |
|:---:|:---:|:---:|:---:|:---:|
| Lazy | 8192 (Step: acesso) | 8245 | 8240 | 38 (0x4); 8202(0x6); 5(0x7) |
| Populate | 8192 (Step: mmap) | 51 | 8234 | 36(0x4); 10(0x6); 5(0x7)|

- No modo populate, o getrusage() conta 8192 faltas no mmap(). Quantos eventos page_fault_user aparecem? Explique a diferença.
    > 51
    > O getrusage() conta todas as faltas geradas de forma síncrona dentro da chamada mmap (em modo kernel).
    > Já no page_fault_user, se conta apenas as faltas ocorridas enquanto a CPU executava em modo usuário (fora do mmap). O modo Populate pré-carrega tudo no kernel, eliminando quase todas as falhas em usuário.
    


## Questão 2

```
echo 0 > /proc/sys/vm/nr_hugepages
trace-cmd record -e kmem:mm_page_alloc sh -c 'echo 8 > /proc/sys/vm/nr_hugepages'
```

- Quantas alocações aparecem e qual o seu order?

```
# trace-cmd report | grep -c mm_page_alloc
35 
```

```
# trace-cmd report | grep -o 'order=[0-9]*' | sort | uniq -c
     27 order=0
      8 order=10
```

```
# grep -i hugepagesize /proc/meminfo
Hugepagesize:       4096 kB
```
 - Relacione o valor com o Hugepagesize de /proc/meminfo. 
 > 27 Alocações de ordem 0, cada uma sendo uma pagina de 4 KiB; 
 > 8  Alocações de ordem 10, cada uma sendo 2ˆ10 paginas de 4 KiB;
 >
 > hugepages size = 4096 kB | 2ˆ10 * 4  = 4096 KiB

 - Com base nisso, explique por que o paging-test huge 32 quase não gera eventos mm_page_alloc.

```
# trace-cmd record -e kmem:mm_page_alloc -F paging-test huge 32
pagina = 4096 bytes, regiao = 32 MiB (8192 paginas)
inicio   minflt = 83     majflt = 0      VmRSS = 1312 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1312 kB
acesso   minflt = 8      majflt = 0      VmRSS = 1312 kB
munmap   minflt = 0      majflt = 0      VmRSS = 1312 kB
# trace-cmd report | grep -c 'paging-test.*mm_page_alloc'
26
```
 > O huge quase não gera eventos pois as hugepages já estão alocadas pelo kernel,o programa apenas reutiliza as mesmas paginas.

## Questão 3

```
trace-cmd record -e vmscan:mm_vmscan_kswapd_wake -e vmscan:mm_vmscan_direct_reclaim_begin -e oom:mark_victim paging-test lazy 100
```
```
pagina = 4096 bytes, regiao = 100 MiB (25600 paginas)
inicio   minflt = 84     majflt = 0      VmRSS = 1440 kB
mmap     minflt = 0      majflt = 0      VmRSS = 1440 kB
```

```
# trace-cmd report > /tmp/oom.txt
```

```
# grep -n 'paging-test.*direct_reclaim_begin' /tmp/oom.txt | head -1
5:     paging-test-221   [000] .....  3961.220295: mm_vmscan_direct_reclaim_begin: order=0 gfp_flags=GFP_HIGHUSER_MOVABLE|__GFP_COMP|__GFP_ZO

# grep -n 'mark_victim' /tmp/oom.txt | head -1
261:     paging-test-221   [000] .N.1.  3971.916190: mark_victim:          pid=221 comm=paging-test total-vm=105132kB anon-rss=73344kB file-0

# grep -n 'mark_victim' /tmp/oom.txt
261:     paging-test-221   [000] .N.1.  3971.916190: mark_victim:          pid=221 comm=paging-test total-vm=105132kB anon-rss=73344kB file-0

# grep MemTotal /proc/meminfo
MemTotal:         116596 kB

```

- Quanto tempo se passou entre o primeiro mm_vmscan_direct_reclaim_begin do paging-test e o mark_victim;
    > 
- Quanta memória o processo estava usando (anon-rss) ao ser eliminado, comparando com o MemTotal de /proc/meminfo.
    > 