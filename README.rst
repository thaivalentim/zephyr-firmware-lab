# Zephyr Firmware Lab

Projeto incremental de Engenharia de Firmware desenvolvido com **Zephyr RTOS**, estruturado para explorar e documentar conceitos fundamentais de sistemas embarcados modernos por meio de implementações progressivas.

O objetivo deste repositório não é apenas implementar funcionalidades utilizando o Zephyr, mas compreender as decisões de arquitetura do framework, exercitando práticas de desenvolvimento utilizadas em projetos profissionais de firmware.

Ao longo da evolução do projeto são explorados conceitos como abstração de hardware, organização modular, Device Tree, Kconfig, interrupções, callbacks e mecanismos de sincronização do kernel, sempre priorizando baixo acoplamento, encapsulamento e clareza arquitetural.

---

# Objetivos

Este projeto busca desenvolver e demonstrar competências em Engenharia de Firmware, com foco em:

* arquitetura modular para sistemas embarcados;
* abstração de hardware utilizando Device Tree;
* configuração do sistema por meio de Kconfig;
* desenvolvimento utilizando as APIs do Zephyr RTOS;
* construção de aplicações orientadas a eventos;
* organização do código visando legibilidade, portabilidade e manutenção.

---

# Competências Exercitadas

Durante o desenvolvimento deste projeto foram aplicados conceitos importantes de Engenharia de Firmware e Sistemas Embarcados, incluindo:

* Hardware Abstraction Layer (HAL);
* Device Tree e overlays;
* aliases de Device Tree;
* configuração de periféricos utilizando `gpio_dt_spec`;
* validação de dispositivos com `device_is_ready()`;
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
* sincronização utilizando `k_event`;
* sistema de configuração Kconfig (`prj.conf`);
* sistema de build do Zephyr (West + CMake).

---

# Tecnologias

* Linguagem C
* Zephyr RTOS
* ESP32-C3
* Device Tree
* Kconfig
* CMake
* West
* GPIO Driver API

---

# Ambiente de Desenvolvimento

O projeto foi desenvolvido utilizando:

* Zephyr SDK;
* Zephyr RTOS;
* West;
* CMake;
* ESP32-C3 DevKitM-1 (`esp32c3_devkitc`).

---

# Compilação

```bash
west build -b esp32c3_devkitc
```

Caso seja necessário recriar completamente o diretório de build:

```bash
west build -b esp32c3_devkitc -p always
```

Para gravar o firmware:

```bash
west flash
```

Para acompanhar a saída serial:

```bash
west espressif monitor
```

---

# Funcionamento

A aplicação implementa um comportamento orientado a eventos.

O botão é configurado para gerar interrupções no GPIO. Quando um evento ocorre:

1. o hardware gera uma interrupção;
2. o driver GPIO executa o callback registrado;
3. o callback publica um evento utilizando `k_event`;
4. a aplicação desperta e processa o evento;
5. o LED é atualizado conforme a lógica definida.

Essa arquitetura desacopla completamente a lógica da aplicação da infraestrutura de hardware, aproximando o projeto das práticas empregadas em sistemas embarcados profissionais.

---

# Roadmap

## Concluído

* [x] Organização modular
* [x] Device Tree
* [x] GPIO
* [x] Device Tree Overlays
* [x] Polling
* [x] Interrupções
* [x] GPIO Callbacks
* [x] `k_event`
* [x] Arquitetura orientada a eventos

## Próximas etapas

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

---

# Licença

Este projeto foi desenvolvido para fins de estudo, documentação técnica e composição de portfólio em Engenharia de Firmware.

---

# Contato

* LinkedIn: **@thaivalentim**