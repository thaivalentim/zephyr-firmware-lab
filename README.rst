Zephyr Firmware Lab
===================

Projeto de Engenharia de Firmware desenvolvido com Zephyr RTOS que demonstra arquitetura embarcada profissional através de uma aplicação orientada a eventos, com modularização rigorosa, abstração de hardware, e comunicação thread-safe entre componentes.

Visão Geral
===========

Uma aplicação que lê um botão por interrupção (com debounce por software) e controla um LED através de uma thread dedicada. Cada pressão do botão avança o LED através de um ciclo de modos: desligado → lento → médio → rápido → desligado. Todos os eventos são capturados em tempo real por um sistema de logging estruturado baseado em fila FIFO com alocação by memory slab.

**Objetivo educacional**: demonstrar padrões profissionais de firmware—separação de camadas, responsabilidades bem definidas, comunicação entre threads, e tratamento robusto de erros.

Compilação e Execução
=====================

Compilar
--------

.. code-block:: bash

   west build -b esp32c3_devkitc

Recriar completamente (se necessário):

.. code-block:: bash

   west build -b esp32c3_devkitc -p always

Gravar no dispositivo
---------------------

.. code-block:: bash

   west flash

Monitor serial
--------------

.. code-block:: bash

   west espressif monitor

Saída esperada:

.. code-block:: text

   [MAIN] Application initialized successfully
   [LED THREAD] Running
   [LOGGING THREAD] Started
   [LOG] src=0 evt=0 val=0 ts=15 ms
   [LOG] src=1 evt=1 val=0 ts=120 ms
   [LOG] src=2 evt=2 val=1 ts=122 ms

Status do Projeto
=================

Completo
-----------

* Organização modular com SRP
* Device Tree + overlays
* GPIO e interrupções
* Debounce por software (50ms)
* State machine orientada a eventos
* Semáforo para ISR-safe communication
* Fila FIFO com memory slab
* Logging estruturado com timestamp
* Error handling em camadas
* Threads com prioridades bem definidas
* Portalfólio pronto

Futuras Expansões
--------------------

* Work Queues para operações assíncronas
* Timers periódicos
* UART para comunicação externa
* SPI/I2C para sensores
* Bluetooth Low Energy (BLE)
* Testes unitários automatizados

Tecnologias
===========

* **Linguagem**: C (C11)
* **RTOS**: Zephyr
* **Hardware**: ESP32-C3 DevKitM-1
* **Build**: CMake + West
* **Hardware Abstraction**: Device Tree
* **Configuration**: Kconfig

Ambiente de Desenvolvimento
===========================

* Zephyr SDK 0.16.x+
* Python 3.8+
* West (Zephyr package manager)
* CMake 3.20+
* Compilador GCC ARM Embedded

Licença
=======

Apache License 2.0 — Desenvolvido para fins educacionais e de portfólio em Engenharia de Firmware.

Contato
=======

LinkedIn: @thaivalentim