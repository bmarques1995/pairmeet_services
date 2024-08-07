# Instalando o projeto

para instalar o projeto, são necessárias duas etapas:

- Instalar a biblioteca UUID com, em modo admin:

  - `sudo apt install uuid-dev`: para derivados de debian
  - `sudo dnf install libuuid-devel`: para derivados de debian
  - `sudo emerge ossp-uuid`: para derivados de portage

- Rodar o comando `cmake --preset linux-debug`, para modo de depuração e `cmake --preset linux-release` para modo de lançamento