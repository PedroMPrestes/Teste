# Configuracao do programa
TARGET       = monitor
INSTALL_PATH = var/www/data/cgi-bin

## Configuracao de diretorios
ROOT_DIR  = ../..
TOOLCHAIN = $(ROOT_DIR)/output/host/bin
OVERLAY   = $(ROOT_DIR)/overlay

## Configuracao de ferramentas
TRIPLET = $(TOOLCHAIN)/i686-buildroot-linux-gnu
CC      = $(TRIPLET)-gcc

default: $(OVERLAY)/$(INSTALL_PATH)/$(TARGET)

$(OVERLAY)/$(INSTALL_PATH)/$(TARGET): $(OVERLAY)/$(INSTALL_PATH) interface_qemu.c informacoes_sistema.c informacoes_sistema.h
	@$(CC) interface_qemu.c informacoes_sistema.c -o $@ -O2

$(OVERLAY)/$(INSTALL_PATH):
	@mkdir -p $@

clean:
	@rm -f $(OVERLAY)/$(INSTALL_PATH)/$(TARGET)