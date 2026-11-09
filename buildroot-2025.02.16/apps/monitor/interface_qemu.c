#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>

#include "informacoes_sistema.h"

int valida(const char *pid);//const pra nao mudar
void get_menu(void);
void get_processo(long pid);

int main(void) {
    
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><head>");
    printf("<meta http-equiv=\"refresh\" content=\"5\">");
    printf("</head><body>");
    const char *query = getenv("QUERY_STRING");
    long pid = -1;
    if (query != NULL && strncmp(query, "pid=", 4) == 0) { //strncmp verifica se a string comeca com 'pid=', da pra mudar isso
        const char *valor = query + 4;
        if (valida(valor) == 1) {
            pid = atol(valor); //converte a string pra long
        }
    }

    if (pid > 0) {
        get_processo(pid);
    } else {
        get_menu();
    }
    printf("</body></html>");
    return 0;
}

int valida(const char *pid){
    if (pid == NULL || *pid == '\0') {
        return 0; 
    }

    int i = 0;
    while (pid[i] != '\0') {
        if (!isdigit(pid[i])) {
            return 0; // nao eh digito
        }
        i++;
    }

    return 1; // tudo eh digito
}

void get_menu(void) {
    /* 
    - Versão do sistema e kernel = model name
    
    - Uptime = cat /proc/uptime '>3804.45< 14062.67'
    
    - Tempo ocioso = cat/proc/uptime '3804.45 >4062.67<'

    - Data e hora do sistema = cat /proc/driver/rtc => rtc_time e rtc_date
    
    - Modelo do processador e núcleos = cat /proc/cpuinfo => model_name e cat /proc/cpuinfo => cpu_cores (número de núcleos físicos)
    
    - Carga do sistema = cat/proc/loadavg (perguntar pro sor o que ele quer dizer com carga do sistema e qual pegar)
    
    - Ocupação da CPU (percentual) = fazer o calculo (total = user + nice + system + idle + iowait + irq + softirq + steal
                                                      ocupado = total - idle - iowait
                                                      ocupacao(%) = 100 * (ocupado_2 - ocupado_1) / (total_2 - total_1))
    - Memória RAM total/usada = cat /proc/meminfo => MemTotal/1024 / cat /proc/meminfo => MemFree/1024 (total - free)
    
    - Operações de E/S = cat /proc/diskstats
    
    - Sistemas de arquivos = /proc/filesystems
    
    - Dispositivos e grupos = cat /proc/devices
    
    - Dispositivos de rede = cat /proc/net/dev
    
    - Lista de processos
    */
   char buffer[4096];

    get_Version(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_Uptime(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_idleTime(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_sisTIme(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_CPUModel(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_CpuCores(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_CargaSistema(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_CpuUsage(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_Memory(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_IO(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_filessytem(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_devices(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_networkDevices(buffer, sizeof(buffer));
    printf("%s", buffer);
    
    get_RunningProcesses(buffer, sizeof(buffer));
    printf("<div>%s</div>", buffer);
}

int somente_digitos(const char *s) {
    if (s == NULL || *s == '\0') {
        return 0;
    }
    for (const char *p = s; *p; p++) {
        if (!isdigit((unsigned char)*p)) {
            return 0;
        }
    }
    return 1;
}

int str_para_pid(const char *s, long *out) {
    char *endptr;
    long val = strtol(s, &endptr, 10);

    if (val <= 0 || val > INT_MAX || *endptr != '\0') {
        return 0;
    }
    *out = val;
    return 1;
}

int pid_existe(long pid) {
    char path[64];
    struct stat st;

    snprintf(path, sizeof(path), "/proc/%ld", pid);
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

void get_processo(long pid) {
    /*
    - PID, PID do processo pai (PPID) e nome do processo = /proc cat PID/status => Pid & /proc cat PID/status => PPid & /proc cat PID/status => Name
    
    - Estado do processo (executando, dormindo, zumbi, etc.), apresentado por extenso e não apenas pela letra = /proc cat PID/status => State
    
    - Identificador do usuário (UID) proprietário do processo = /proc cat PID/status => Uid
    
    - Linha de comando completa, com os argumentos separados corretamente = /proc cat cmdline
    
    - Número de threads = /proc cat PID/status => Threads
    
    - Memória virtual total e memória residente (em kilobytes) = /proc cat PID/status => VmSize & /proc cat PID/status => VmRSS
    
    - Tempo de processador consumido em modo usuário e em modo sistema, convertido para segundos;
    
    - Prioridade e valor de _nice_ = /proc cat PID/stat => 18ª Prioridade & /proc cat PID/stat => 19ª Nice:
    
    - Um link de retorno para a página geral.
    */
    char buffer[4096];

   if (!pid_existe(pid)) {
        printf("<p>Processo %ld não encontrado (já encerrado ou inexistente).</p>", pid);
        printf("<a href=\"monitor\">Voltar</a>");
        return;
    }

    get_process_status(pid, buffer, sizeof(buffer));
    printf("%s", buffer);

    if (strstr(buffer, "nao encontrado") != NULL) {
        printf("<a href=\"monitor\">Voltar</a>");
        return;
    }

    get_process_cmdline(pid, buffer, sizeof(buffer));
    printf("%s", buffer);

    get_process_stat(pid, buffer, sizeof(buffer));
    printf("%s", buffer);

    printf("<a href=\"monitor\">Voltar</a>");
}

