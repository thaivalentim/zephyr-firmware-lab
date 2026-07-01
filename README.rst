Zephyr Firmware Lab
===================

Projeto incremental de Engenharia de Firmware desenvolvido com **Zephyr RTOS** para estudo de arquitetura de firmware, abstração de hardware e comportamento orientado a eventos em sistemas embarcados.

Visão Geral
===========

Este repositório documenta a evolução de uma aplicação embarcada simples, porém arquiteturalmente estruturada, construída para exercitar práticas usadas em projetos profissionais de firmware.

A aplicação utiliza o Zephyr para ler um botão por interrupção e alternar um LED por meio de eventos do kernel. O foco está menos na complexidade funcional e mais na disciplina de projeto: modularização, separação de responsabilidades, uso de Device Tree e comunicação entre contexto de interrupção e contexto de aplicação.

Objetivos
=========

Este projeto busca desenvolver e demonstrar competências em Engenharia de Firmware, com foco em:

* arquitetura modular para sistemas embarcados;
* abstração de hardware utilizando Device Tree;
* configuração do sistema por meio de Kconfig;
* desenvolvimento utilizando as APIs do Zephyr RTOS;
* construção de aplicações orientadas a eventos;
* organização do código visando legibilidade, portabilidade e manutenção.

Competências Exercitadas
========================

Durante o desenvolvimento deste projeto foram aplicados conceitos importantes de Engenharia de Firmware e Sistemas Embarcados, incluindo:

* Hardware Abstraction Layer (HAL);
* Device Tree e overlays;
* aliases de Device Tree;
* configuração de periféricos utilizando ``gpio_dt_spec``;
* validação de dispositivos com ``device_is_ready()``;
* configuração de GPIO de entrada e saída;
* encapsulamento de drivers em módulos independentes;
* arquitetura em camadas;
* baixo acoplamento entre módulos;
* princípio da Responsabilidade Única (SRP);
* tratamento consistente de erros;
* polling para aquisição de entradas digitais;
* interrupções (IRQ);
* callbacks do subsistema GPIO;
* programação orientada a eventos;
* sincronização utilizando ``k_event``;
* sistema de configuração Kconfig (``prj.conf``);
* sistema de build do Zephyr (West + CMake).

Estrutura do Repositório
========================

* ``src/main.c``: coordenação do fluxo principal da aplicação.
* ``src/led.c``: encapsulamento da inicialização e do controle do LED.
* ``src/button.c``: configuração do botão, callback de interrupção e publicação de eventos.
* ``src/app_events.c``: mecanismo interno de eventos da aplicação.
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

O projeto foi desenvolvido utilizando:

* Zephyr SDK;
* Zephyr RTOS;
* West;
* CMake;
* ESP32-C3 DevKitM-1 (``esp32c3_devkitc``).

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

Status do Projeto
=================

O projeto está em evolução contínua. A base atual já cobre:

* inicialização de periféricos via Device Tree;
* leitura de botão com interrupção;
* publicação e consumo de eventos com ``k_event``;
* controle de LED em arquitetura modular;
* documentação técnica para portfólio.

Funcionamento
=============

A aplicação implementa um comportamento orientado a eventos.

O botão é configurado para gerar interrupções no GPIO. Quando um evento ocorre:

1. o hardware gera uma interrupção;
2. o driver GPIO executa o callback registrado;
3. o callback publica um evento utilizando ``k_event``;
4. a aplicação desperta e processa o evento;
5. o LED é atualizado conforme a lógica definida.

Essa arquitetura desacopla completamente a lógica da aplicação da infraestrutura de hardware, aproximando o projeto das práticas empregadas em sistemas embarcados profissionais.

Roadmap
=======

Concluído
---------

* [x] Organização modular
* [x] Device Tree
* [x] GPIO
* [x] Device Tree Overlays
* [x] Polling
* [x] Interrupções
* [x] GPIO Callbacks
* [x] ``k_event``
* [x] Arquitetura orientada a eventos

Próximas etapas
---------------

* [ ] Debounce por software
* [ ] Threads
* [ ] Semáforos
* [ ] Filas de mensagens
* [ ] Work Queues
* [ ] Timers
* [ ] UART
* [ ] SPI
* [ ] I²C
* [ ] Sensores
* [ ] Bluetooth Low Energy (BLE)

Licença
=======

Este projeto foi desenvolvido para fins de estudo, documentação técnica e composição de portfólio em Engenharia de Firmware.

Contato
=======

* LinkedIn: **@thaivalentim**