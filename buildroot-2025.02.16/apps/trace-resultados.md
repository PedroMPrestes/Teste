
## Questão 1 

#### Lazy

```
trace-cmd record -e exceptions:page_fault_user -e kmem:mm_page_alloc -F paging-test lazy 32
```


#### Populate

```
trace-cmd record -e exceptions:page_fault_user -e kmem:mm_page_alloc -F paging-test lazy 32
```


- No modo populate, o getrusage() conta 8192 faltas no mmap(). Quantos eventos page_fault_user aparecem? Explique a diferença.
    > 



## Questão 2

```
echo 0 > /proc/sys/vm/nr_hugepages
trace-cmd record -e kmem:mm_page_alloc sh -c 'echo 8 > /proc/sys/vm/nr_hugepages'
```

- Quantas alocações aparecem e qual o seu order? Relacione o valor com o Hugepagesize de /proc/meminfo. Com base nisso, explique por que o paging-test huge 32 quase não gera eventos mm_page_alloc.
    >


## Questão 3
- Quanto tempo se passou entre o primeiro mm_vmscan_direct_reclaim_begin do paging-test e o mark_victim;
    > 
- Quanta memória o processo estava usando (anon-rss) ao ser eliminado, comparando com o MemTotal de /proc/meminfo.
    > 