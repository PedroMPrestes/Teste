## Overcommit mem 1 Swapon

```
#echo 1 > /proc/sys/vm/overcommit_memory 
#cat  /proc/sys/vm/overcommit_memory 
1
```

#### mem-test-write

```
....
got 134 MiB
got 135 MiB
got 136 MiB
Killed
```
#### mem-test-read

```
...
118
119
120
121
Killed
```

## Overcommit_mem 2 


```
#echo 2 > /proc/sys/vm/overcommit_memory 
#cat  /proc/sys/vm/overcommit_memory 
2
```

#### mem-test-write 

```
...
got 82 MiB
got 83 MiB
malloc failure after 83 MiB
```

#### mem-test-read 
```
81
82
83
```
**(sem Killed)**

### Obs: 
> Nos casos onde foi utilizado o overcommit 1 os dois testes encerram o processo devido ao esgotamento de recursos. 
> 
>Para o overcommit 2: o primeiro teste provocou erro no malloc, (não precisou de killed) e o segundo teste foi encerrado antes do (killed)  