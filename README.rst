Zephyr Firmware Lab
===================

Projeto incremental de Engenharia de Firmware desenvolvido com Zephyr RTOS para estudo de arquitetura de firmware, abstração de hardware e comportamento orientado a eventos em sistemas embarcados.

Visão Geral
===========

Este repositório documenta a evolução de uma aplicação embarcada estruturada para exercitar práticas empregadas em projetos profissionais de firmware.

A aplicação utiliza o Zephyr RTOS para ler um botão por interrupção e controlar um LED por meio de uma thread dedicada. Cada acionamento do botão publica um evento interno que avança o modo de operação do LED em um ciclo: desligado, lento, médio, rápido e novamente desligado. Um sistema de logging baseado em fila FIFO registra os eventos relevantes da aplicação em tempo de execução.

O foco está menos na complexidade funcional e mais na disciplina de projeto: modularização, separação de responsabilidades, comunicação entre threads e uso das APIs do Zephyr RTOS.

Objetivos
=========

* Arquitetura modular para sistemas embarcados.
* Abstração de hardware utilizando Device Tree.
* Configuração do sistema por meio de Kconfig.
* Desenvolvimento utilizando as APIs do Zephyr RTOS.
* Construção de aplicações orientadas a eventos.
* Comunicação entre threads por semáforo e fila FIFO.
* Organização do código visando legibilidade, portabilidade e manutenção.

Competências Exercitadas
========================

* Hardware Abstraction Layer (HAL).
* Device Tree e overlays.
* Aliases de Device Tree.
* Configuração de periféricos utilizando ``gpio_dt_spec``.
* Validação de dispositivos com ``device_is_ready()``.
* Configuração de GPIO de entrada e saída.
* Encapsulamento de drivers em módulos independentes.
* Arquitetura em camadas com baixo acoplamento.
* Princípio da Responsabilidade Única (SRP).
* Tratamento consistente de erros.
* Interrupções (IRQ) e callbacks do subsistema GPIO.
* Programação orientada a eventos.
* Sincronização entre threads utilizando ``k_sem``.
* Threads com ``k_thread_create``.
* Fila FIFO com ``k_fifo`` e alocação por ``k_mem_slab``.
* Sistema de logging estruturado com fonte, evento, valor e timestamp.
* Sistema de configuração Kconfig (``prj.conf``).
* Sistema de build do Zephyr (West + CMake).

Estrutura do Repositório
========================

* ``src/main.c``: coordenação do fluxo principal da aplicação.
* ``src/led.c``: encapsulamento da inicialização e do controle do LED.
* ``src/button.c``: configuração do botão, callback de interrupção e publicação de eventos.
* ``src/app_events.c``: mecanismo interno de eventos baseado em semáforo.
* ``src/led_thread.c``: thread dedicada ao controle do ciclo de modos do LED.
* ``src/logging_thread.c``: thread de logging com fila FIFO e alocação por memory slab.
* ``include/``: cabeçalhos públicos compartilhados entre os módulos.
* ``boards/``: overlays e ajustes específicos da placa.
* ``docs/``: documentação técnica complementar.

Tecnologias
===========

* Linguagem C
* Zephyr RTOS
* ESP32-C3
* Device Tree
* Kconfig
* CMake
* West
* GPIO Driver API

Ambiente de Desenvolvimento
===========================

* Zephyr SDK
* Zephyr RTOS
* West
* CMake
* ESP32-C3 DevKitM-1 (``esp32c3_devkitc``)

Compilação
==========

.. code-block:: bash

   west build -b esp32c3_devkitc

Caso seja necessário recriar completamente o diretório de build:

.. code-block:: bash

   west build -b esp32c3_devkitc -p always

Para gravar o firmware:

.. code-block:: bash

   west flash

Para acompanhar a saída serial:

.. code-block:: bash

   west espressif monitor

Funcionamento
=============

A aplicação implementa um comportamento orientado a eventos com controle de modo por thread.

O botão é configurado para gerar interrupções no GPIO. Quando acionado:

1. o hardware gera uma interrupção;
2. o driver GPIO executa o callback registrado;
3. o callback publica um evento via semáforo e envia uma mensagem de log;
4. a LED thread desperta, avança o modo de operação e registra a mudança;
5. a logging thread consome a fila FIFO e exibe os eventos no console.

Essa arquitetura desacopla a lógica de aplicação da infraestrutura de hardware e distribui responsabilidades entre threads independentes.

Status do Projeto
=================

Concluído
---------

* Organização modular
* Device Tree e overlays
* GPIO e callbacks de interrupção
* Arquitetura orientada a eventos
* Semáforo para comunicação entre ISR e thread
* Thread dedicada ao controle do LED
* Ciclo de modos: desligado, lento, médio, rápido
* Fila FIFO com memory slab
* Thread de logging estruturado
* Timestamp por ``k_uptime_get_32()``

Próximas Etapas
---------------

* Debounce por software
* Work Queues
* Timers
* UART
* SPI
* I2C
* Sensores
* Bluetooth Low Energy (BLE)

Licença
=======

Este projeto foi desenvolvido para fins de estudo, documentação técnica e composição de portfólio em Engenharia de Firmware.

Contato
=======

* LinkedIn: **@thaivalentim* 