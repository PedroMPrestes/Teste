#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>


void get_ProcessStat(long pid, char *buffer, size_t size) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%ld/stat", pid);
 
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        snprintf(buffer, size, "Erro ao abrir stat do PID %ld", pid);
        return;
    }
 
    char line[1024];
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        snprintf(buffer, size, "Erro ao ler stat do PID %ld", pid);
        return;
    }
    fclose(file);
 
    char *fecha_parenteses = strrchr(line, ')');
    if (fecha_parenteses == NULL) {
        snprintf(buffer, size, "Formato inesperado em stat do PID %ld", pid);
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
        snprintf(buffer, size, "Erro ao interpretar stat do PID %ld", pid);
        return;
    }
 
    /* nunca usar constante fixa (ex: 100) -- varia por plataforma */
    long ticks_por_seg = sysconf(_SC_CLK_TCK);
    double utime_seg = (double) utime_ticks / (double) ticks_por_seg;
    double stime_seg = (double) stime_ticks / (double) ticks_por_seg;
 
    snprintf(buffer, size,
             "Tempo em modo usuario: %.2fs | Tempo em modo sistema: %.2fs | "
             "Prioridade: %ld | Nice: %ld",
             utime_seg, stime_seg, priority, nice_val);
}


int main(void)
{
    char version[256];

    //get_Version()
    get_ProcessStat( 1, version, sizeof(version));

    printf("Teste:\n %s", version);

    return 0;
}