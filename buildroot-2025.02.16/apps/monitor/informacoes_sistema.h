#ifndef informacoes_sistema_H
#define informacoes_sistema_H

#include <stddef.h>

//funcoes gerais
void get_Version(char *buffer, size_t size);
void get_Uptime(char *buffer, size_t size);
void get_idleTime(char *buffer, size_t size);
void get_sisTIme(char *buffer, size_t size);
void get_CPUModel(char *buffer, size_t size);
void get_CpuCores(char *buffer, size_t size);
void get_CargaSistema(char *buffer, size_t size);
void get_CpuUsage(char *buffer, size_t size);
void get_Memory(char *buffer, size_t size);
void get_IO(char *buffer, size_t size);
void get_filessytem(char *buffer, size_t size);
void get_devices(char *buffer, size_t size);
void get_networkDevices(char *buffer, size_t size);
void get_RunningProcesses(char *buffer, size_t size);

//funcoes para os processos
void get_process_status(long pid, char *buffer, size_t size);
void get_process_cmdline(long pid, char *buffer, size_t size);
void get_process_stat(long pid, char *buffer, size_t size);
#endif