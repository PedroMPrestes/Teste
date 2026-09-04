#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#include "informacoes_sistema.h"


void get_Version(char *buffer, size_t size){
    FILE *file = fopen("/proc/version", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Versao do Sistema</h3><p>Erro ao abrir /proc/version</p>");
        return;
    }

    char versao[256] = "";
 
    if (fgets(versao, sizeof(versao), file) == NULL) {
        snprintf(buffer, size, "<h3>Versao do Sistema</h3><p>Erro ao ler /proc/version</p>");
        fclose(file);
        return;
    }
 
    versao[strcspn(versao, "\n")] = '\0';
    fclose(file);
    snprintf(buffer, size, "<h3>Versao do Sistema</h3><p>%s</p>", versao);
}


//lembrar de converter
void get_Uptime(char *buffer, size_t size){
    FILE *file = fopen("/proc/uptime", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Uptime</h3><p>Erro ao abrir /proc/uptime</p>");
        return;
    }

    double uptime_seconds;

    if (fscanf(file, "%lf", &uptime_seconds) != 1) {
        snprintf(buffer, size, "<h3>Uptime</h3><p>Erro ao ler /proc/uptime</p>");
        fclose(file);
        return;
    }

    fclose(file);

    unsigned long total_seconds = (unsigned long)uptime_seconds;

    unsigned long days = total_seconds / 86400;
    total_seconds %= 86400;

    unsigned long hours = total_seconds / 3600;
    total_seconds %= 3600;

    unsigned long minutes = total_seconds / 60;
    unsigned long seconds = total_seconds % 60;

    snprintf(buffer, size,"<h3>Uptime</h3><p>%lud %02luh %02lum %02lus</p>",days, hours, minutes, seconds);
}


void get_idleTime(char *buffer, size_t size) {
    FILE *file = fopen("/proc/uptime", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Tempo Ocioso</h3><p>Erro ao abrir /proc/uptime</p>");
        return;
    }

    double uptime_seconds;
    double idle_seconds;

    if (fscanf(file, "%lf %lf", &uptime_seconds, &idle_seconds) != 2) {
        snprintf(buffer, size, "<h3>Tempo Ocioso</h3><p>Erro ao ler /proc/uptime</p>");
        fclose(file);
        return;
    }

    fclose(file);

    unsigned long total_seconds = (unsigned long)idle_seconds;

    unsigned long days = total_seconds / 86400;
    total_seconds %= 86400;

    unsigned long hours = total_seconds / 3600;
    total_seconds %= 3600;

    unsigned long minutes = total_seconds / 60;
    unsigned long seconds = total_seconds % 60;

    snprintf(buffer, size,"<h3>Tempo Ocioso</h3><p>%lud %02luh %02lum %02lus</p>",days, hours, minutes, seconds);
}

void get_sisTIme(char *buffer, size_t size){
    FILE *file = fopen("/proc/driver/rtc", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Data e Hora do Sistema</h3><p>Erro ao abrir /proc/driver/rtc</p>");
        return;
    }

    char line[256];
    char rtc_time[20] = "";
    char rtc_date[20] = "";

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "rtc_time", 8) == 0) {
            sscanf(line, "rtc_time : %19s", rtc_time);
        }

        if (strncmp(line, "rtc_date", 8) == 0) {
            sscanf(line, "rtc_date : %19s", rtc_date);
        }
    }

    fclose(file);

    snprintf(buffer, size,"<h3>Data e Hora do Sistema</h3><p>Data: %s | Hora: %s</p>",rtc_date, rtc_time);
}


void get_CPUModel(char *buffer, size_t size) {
FILE *file = fopen("/proc/cpuinfo", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Modelo do Processador</h3><p>Erro ao abrir /proc/cpuinfo</p>");
        return;
    }

    char line[256];
    char model_name[256] = "";

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "model_name", 10) == 0) {
            sscanf(line,  "model name : %[^\n]", model_name);
        }
    }

    fclose(file);

    snprintf(buffer, size, "<h3>Modelo do Processador</h3><p>%s</p>", model_name);
}


void get_CpuCores(char *buffer, size_t size) {
FILE *file = fopen("/proc/cpuinfo", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Numero de Nucleos</h3><p>Erro ao abrir /proc/cpuinfo</p>");
        return;
    }

    char line[256];
    char cpu_cores[20] = "";

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "cpu cores", 9) == 0) {
            sscanf(line, "cpu cores : %19s", cpu_cores);
            break;
        }
    }

    fclose(file);

    snprintf(buffer, size, "<h3>Numero de Nucleos</h3><p>%s</p>", cpu_cores);
}


void get_CargaSistema(char *buffer, size_t size) {
//cat/proc/loadavg fazer uma tabela com a legenda da saida
    FILE *file = fopen("/proc/loadavg", "r");
    if (file == NULL) {
        snprintf(buffer, size, "<h3>Carga do Sistema</h3><p>Erro ao abrir /proc/loadavg</p>");
        return;
    }
    char line[256];
    //Ex: 0.57 1.02 0.93 2/1178 14459
    char media_1[6];
    char media_5[6];
    char media_15[6];
    char process_momento[10];
    char last_pid[10];

    fscanf(file, "%5s %5s %5s %9s %9s",
        media_1,
        media_5,
        media_15,
        process_momento,
        last_pid);

    snprintf(buffer, size,
    "<h3>Carga do Sistema</h3>"
    "<table border=\"1\">"
    "<tr><td>%s</td><td>carga media do ultimo 1 min</td></tr>"
    "<tr><td>%s</td><td>carga media dos ultimos 5 minutos</td></tr>"
    "<tr><td>%s</td><td>carga media dos ultimos 15 min</td></tr>"
    "<tr><td>%s</td><td>processos executando/total de processos</td></tr>"
    "<tr><td>%s</td><td>PID do ultimo processo criado</td></tr>"
    "</table>",
    media_1,
    media_5,
    media_15,
    process_momento,
    last_pid);

    fclose(file);
}

#define SAMPLE_FILE "/tmp/cpu_usage_sample.txt"
#define STAT_FILE "/proc/stat"

void get_CpuUsage(char *buffer, size_t size) {
    FILE *file = fopen(STAT_FILE, "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Ocupacao da CPU</h3><p>Erro ao abrir /proc/stat</p>");
        return; /* faltava o return original: sem ele o código continuaria
                    usando um FILE* NULL mais abaixo */
    }

    char line[256];
    unsigned long user = 0, nice = 0, system = 0, idle = 0;
    unsigned long iowait = 0, irq = 0, softirq = 0, steal = 0;
    int achou_linha_cpu = 0;

    while (fgets(line, sizeof(line), file)) {
        /* "cpu " (com espaço) identifica a linha agregada de todos os
           núcleos. Sem o espaço, "cpu0", "cpu1" etc. também dão match
           em strncmp(line, "cpu", 3) e sobrescrevem os valores no final
           do loop, corrompendo a leitura. */
        if (strncmp(line, "cpu ", 4) == 0) {
            sscanf(line, "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
                   &user, &nice, &system, &idle,
                   &iowait, &irq, &softirq, &steal);
            achou_linha_cpu = 1;
            break; /* já temos o que precisamos, não há motivo pra continuar lendo */
        }
    }

    fclose(file);

    if (!achou_linha_cpu) {
        snprintf(buffer, size, "<h3>Ocupacao da CPU</h3><p>Erro ao interpretar /proc/stat</p>");
        return;
    }

    unsigned long total_2   = user + nice + system + idle + iowait + irq + softirq + steal;
    unsigned long ocupado_2 = total_2 - idle - iowait;

    /* --- Persistência da amostra anterior --- */
    unsigned long total_1 = 0, ocupado_1 = 0;
    int existe_amostra_anterior = 0;

    FILE *sample = fopen(SAMPLE_FILE, "r");
    if (sample != NULL) {
        if (fscanf(sample, "%lu %lu", &total_1, &ocupado_1) == 2) {
            existe_amostra_anterior = 1;
        }
        fclose(sample);
    }

    /* Grava a amostra atual para ser usada na próxima requisição */
    sample = fopen(SAMPLE_FILE, "w");
    if (sample != NULL) {
        fprintf(sample, "%lu %lu\n", total_2, ocupado_2);
        fclose(sample);
    }

    if (!existe_amostra_anterior) {
        snprintf(buffer, size, "<h3>Ocupacao da CPU</h3><p>Calculando ocupacao do processador...</p>");
        return;
    }

    unsigned long delta_total = total_2 - total_1;

    if (delta_total == 0) {
        /* evita divisão por zero caso as duas leituras tenham
           ocorrido no mesmo instante/tick */
        snprintf(buffer, size, "<h3>Ocupacao da CPU</h3><p>Calculando ocupacao do processador...</p>");
        return;
    }

    double ocupacao = 100.0 * (double)(ocupado_2 - ocupado_1) / (double)delta_total;

    snprintf(buffer, size, "<h3>Ocupacao da CPU</h3><p>%.2f%%</p>", ocupacao);
}


void get_Memory(char *buffer, size_t size) {
    FILE *file = fopen("/proc/meminfo", "r");

    if (file == NULL) {
        snprintf(buffer, size, "<h3>Memoria RAM</h3><p>Erro ao abrir /proc/meminfo</p>");
        return;
    }

    char line[256];

    long mem_total = 0;
    long mem_available = 0;

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %ld kB", &mem_total);
        }

        if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %ld kB", &mem_available);
        }
    }

    fclose(file);

    // Calcula a memória usada
    long mem_used = mem_total - mem_available;

    // Converte de KB para MB
    mem_total /= 1024;
    mem_used /= 1024;

    snprintf(buffer, size,"<h3>Memoria RAM</h3><p>Total: %ld MB | Usada: %ld MB</p>",mem_total, mem_used);
}

void get_IO(char *buffer, size_t size) {
    FILE *file = fopen("/proc/diskstats","r");
    if (file==NULL){
        snprintf(buffer, size, "<h3>Operacoes de E/S</h3><p>Erro ao abrir /proc/diskstats</p>");
        return;
    }

    char line[256];
    unsigned int major, minor;
    char nome[32];
    unsigned long leituras = 0, escritas = 0;
    int achou = 0;
 
    while (fgets(line, sizeof(line), file) != NULL) {
        /* campo 4 = leituras completadas, campo 8 = escritas completadas
           (os %*u no meio pulam os campos que nao usamos) */
        int lidos = sscanf(line, "%u %u %31s %lu %*u %*u %*u %lu",
                            &major, &minor, nome, &leituras, &escritas);
 
        if (lidos != 5)
            continue;
 
        if (strcmp(nome, "sda") != 0)
            continue;
 
        achou = 1;
        break;
    }
 
    fclose(file);
 
    if (!achou) {
        snprintf(buffer, size, "<h3>Operacoes de E/S</h3><p>Dispositivo sda nao encontrado</p>");
        return;
    }
 
    snprintf(buffer, size,
             "<h3>Operacoes de E/S</h3>"
             "<table border=\"1\">"
             "<tr><th>Dispositivo</th><th>Leituras</th><th>Escritas</th></tr>"
             "<tr><td>%s</td><td>%lu</td><td>%lu</td></tr>"
             "</table>",
             nome, leituras, escritas);
    
}


void get_filessytem(char *buffer, size_t size) {
    //ignorar o nodev
    FILE *file = fopen("/proc/filesystems", "r");
    if (file == NULL) {
        snprintf(buffer, size, "<h3>Sistemas de Arquivos</h3><p>Erro ao abrir /proc/filesystems</p>");
        return;
    }

    char line[256];
    char lista[2048] = "";

    buffer[0] = '\0';

    while (fgets(line, sizeof(line), file) != NULL) {
        char tipo[100];

        if (sscanf(line, "%99s", tipo) != 1)
            continue;

        if (strcmp(tipo, "nodev") == 0)
            continue;

        strncat(lista, "<li>", sizeof(lista) - strlen(lista) - 1);
        strncat(lista, tipo, sizeof(lista) - strlen(lista) - 1);
        strncat(lista, "</li>", sizeof(lista) - strlen(lista) - 1);
    }

    fclose(file);
    snprintf(buffer, size, "<h3>Sistemas de Arquivos</h3><ul>%s</ul>", lista);
}


void get_devices(char *buffer, size_t size) {
    FILE *file = fopen("/proc/devices", "r");

    if (file == NULL){
        snprintf(buffer, size, "<h3>Dispositivos</h3><p>Erro ao abrir /proc/devices</p>");
        return;
    }
    char line[256];
    buffer[0] = '\0';
    char conteudo[2048] = "";

    while (fgets(line, sizeof(line), file) != NULL) {
        //pula a linha em branco que separa o Character e o Block
        if (line[0] == '\n')
            continue;
 
        strncat(conteudo, line, sizeof(conteudo) - strlen(conteudo) - 1);
    }
 
    fclose(file);
 
    snprintf(buffer, size, "<h3>Dispositivos</h3><pre>%s</pre>", conteudo);
}


void get_networkDevices(char *buffer, size_t size) {
    FILE *file = fopen ("/proc/net/dev", "r");
    
    if (file == NULL) {
        snprintf(buffer, size, "<h3>Dispositivos de Rede</h3><p>Erro ao abrir /proc/net/dev</p>");
        return;
    }

    char line[512];
    char linhas_tabela[2048] = "";
    int num_linha = 0;
 
    while (fgets(line, sizeof(line), file)) {
        num_linha++;
        if (num_linha <= 2) //pula linhas brancas
            continue; 
 
        char *dp = strchr(line, ':');
        if (dp == NULL)
            continue;
        *dp = '\0';
 
        char iface[64];
        //remove espacos
        sscanf(line, "%63s", iface); 
 
        unsigned long long rx_bytes = 0, tx_bytes = 0;
        /* apos o ':': rx_bytes e' o 1o campo, tx_bytes e' o 9o campo */
        sscanf(dp + 1, "%llu %*u %*u %*u %*u %*u %*u %*u %llu",
               &rx_bytes, &tx_bytes);
 
        char linha_html[256];
        snprintf(linha_html, sizeof(linha_html),
                 "<tr><td>%s</td><td>%llu</td><td>%llu</td></tr>",
                 iface, rx_bytes, tx_bytes);
 
        strncat(linhas_tabela, linha_html,
                sizeof(linhas_tabela) - strlen(linhas_tabela) - 1);
    }
 
    fclose(file);
 
    snprintf(buffer, size,
             "<h3>Dispositivos de Rede</h3>"
             "<table border=\"1\">"
             "<tr><th>Interface</th><th>RX bytes</th><th>TX bytes</th></tr>"
             "%s"
             "</table>",
             linhas_tabela);
}


void get_RunningProcesses(char *buffer, size_t size) {
    DIR *dir = opendir("/proc");

    if (dir == NULL) {
        snprintf(buffer, size, "<h3>Processos em Execucao</h3><p>Erro ao abrir /proc</p>");
        return;
    }

    struct dirent *entry;
    char itens[4096] = "";
 
    while ((entry = readdir(dir)) != NULL) {
 
        if (!isdigit(entry->d_name[0]))
            continue;
 
        int pid = atoi(entry->d_name);
 
        char path[256];
        char name[256];
 
        snprintf(path, sizeof(path),
                 "/proc/%d/comm", pid);
 
        FILE *file = fopen(path, "r");
 
        if (file == NULL)
            continue;
 
        if (fgets(name, sizeof(name), file)) {
 
            name[strcspn(name, "\n")] = '\0';
 
            char item[512];
 
            snprintf(item, sizeof(item),
                     "<li><a href=\"monitor?pid=%d\">%d - %s</a></li>",
                     pid, pid, name);
 
            strncat(itens, item, sizeof(itens) - strlen(itens) - 1);
        }
 
        fclose(file);
    }
 
    closedir(dir);
 
    snprintf(buffer, size, "<h3>Processos em Execucao</h3><ul>%s</ul>", itens);
}

//letra pra palavra
static const char *estado_por_extenso(char letra)
{
    switch (letra) {
        case 'R': return "Executando";
        case 'S': return "Dormindo (interrompivel)";
        case 'D': return "Dormindo (nao interrompivel)";
        case 'Z': return "Zumbi";
        case 'T': return "Parado";
        case 't': return "Parado (rastreamento)";
        case 'X': return "Encerrado";
        case 'I': return "Ocioso";
        default:  return "Desconhecido";
    }
}

void get_process_status(long pid, char *buffer, size_t size) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/status", pid);
 
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        snprintf(buffer, size,"<p>Processo %ld nao encontrado (ja encerrado).</p>", pid);
        return;
    }
 
    char line[256];
    char nome[256] = "";
    char estado_letra = '?';
    long ppid = -1, threads = -1, uid = -1;
    long vmsize_kb = -1, vmrss_kb = -1;
 
    while (fgets(line, sizeof(line), file)) {
 
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name:\t%255[^\n]", nome);
 
        } else if (strncmp(line, "State:", 6) == 0) {
            /* formato: "State:\tR (running)" -- so nos interessa a letra */
            sscanf(line, "State:\t%c", &estado_letra);
 
        } else if (strncmp(line, "PPid:", 5) == 0) {
            sscanf(line, "PPid:\t%ld", &ppid);
 
        } else if (strncmp(line, "Threads:", 8) == 0) {
            sscanf(line, "Threads:\t%ld", &threads);
 
        } else if (strncmp(line, "Uid:", 4) == 0) {
            /* formato: "Uid:\treal efetivo salvo fs" -- pegamos o real */
            sscanf(line, "Uid:\t%ld", &uid);
 
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize: %ld kB", &vmsize_kb);
 
        } else if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS: %ld kB", &vmrss_kb);
        }
    }
 
    fclose(file);
 
    snprintf(buffer, size,
             "<p>PID: %ld | PPID: %ld | Nome: %s | Estado: %s | UID: %ld | "
             "Threads: %ld | VmSize: %ld kB | VmRSS: %ld kB</p>",
             pid, ppid, nome, estado_por_extenso(estado_letra), uid,
             threads, vmsize_kb, vmrss_kb);
}

void get_process_cmdline(long pid, char *buffer, size_t size){
    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/cmdline", pid);
 
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        snprintf(buffer, size, "<p>Erro ao abrir cmdline do PID %ld</p>", pid);
        return;
    }
 
    //aux pra ler antes de formatar
    char cru[400];
    size_t max_leitura = sizeof(cru) - 1;
    if (max_leitura > size - 1) max_leitura = size - 1;
 
    size_t lidos = fread(cru, 1, max_leitura, file);
    fclose(file);
 
    if (lidos == 0) {
        /* cmdline vazio -- e' uma thread de kernel */
        snprintf(buffer, size, "<p>Comando: [thread de kernel]</p>");
        return;
    }
 
    cru[lidos] = '\0';
 
    //se deixar \0 buga
    for (size_t i = 0; i + 1 < lidos; i++) {
        if (cru[i] == '\0') {
            cru[i] = ' ';
        }
    }
 
    snprintf(buffer, size, "<p>Comando: %s</p>", cru);
}

void get_process_stat(long pid, char *buffer, size_t size) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/stat", pid);
 
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        snprintf(buffer, size, "<p>Erro ao abrir stat do PID %ld</p>", pid);
        return;
    }
 
    char line[1024];
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        snprintf(buffer, size, "<p>Erro ao ler stat do PID %ld</p>", pid);
        return;
    }
    fclose(file);
 
    char *fecha_parenteses = strrchr(line, ')');
    if (fecha_parenteses == NULL) {
        snprintf(buffer, size, "<p>Formato inesperado em stat do PID %ld</p>", pid);
        return;
    }
 
    char *resto = fecha_parenteses + 1; /* a partir daqui comeca o campo 3 */
 
    char estado;
    long ppid, pgrp, sessao, tty, tpgid;
    unsigned long flags, minflt, cminflt, majflt, cmajflt;
    unsigned long utime_ticks, stime_ticks;
    long cutime, cstime, priority, nice_val;
 
    int lidos = sscanf(resto,
        " %c %ld %ld %ld %ld %ld"   /* campos  3-8  */
        " %lu %lu %lu %lu %lu"       /* campos  9-13 */
        " %lu %lu"                    /* campos 14-15: utime, stime */
        " %ld %ld"                    /* campos 16-17: cutime, cstime */
        " %ld %ld",                   /* campos 18-19: priority, nice */
        &estado, &ppid, &pgrp, &sessao, &tty, &tpgid,
        &flags, &minflt, &cminflt, &majflt, &cmajflt,
        &utime_ticks, &stime_ticks,
        &cutime, &cstime,
        &priority, &nice_val);
 
    if (lidos < 17) {
        snprintf(buffer, size, "<p>Erro ao interpretar stat do PID %ld</p>", pid);
        return;
    }
 
    /* nunca usar constante fixa (ex: 100) -- varia por plataforma */
    long ticks_por_seg = sysconf(_SC_CLK_TCK);
    double utime_seg = (double) utime_ticks / (double) ticks_por_seg;
    double stime_seg = (double) stime_ticks / (double) ticks_por_seg;
 
    snprintf(buffer, size,
             "<p>Tempo em modo usuario: %.2fs | Tempo em modo sistema: %.2fs | "
             "Prioridade: %ld | Nice: %ld</p>",
             utime_seg, stime_seg, priority, nice_val);
}