
# Questão 1 

- Comando:
    trace-cmd record -e exceptions:page_fault_user -e kmem:mm_page_alloc -F paging-test lazy 32

