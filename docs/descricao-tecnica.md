# Descrição Técnica da Aplicação

## Visão Geral

Esta aplicação foi desenvolvida como estudo de arquitetura de firmware sobre o Zephyr RTOS. O sistema implementa um fluxo reativo orientado a evento para controlar um LED a partir da atuação de um botão físico, com separação explícita entre inicialização de periféricos, tratamento de interrupções e coordenação do comportamento principal.

Do ponto de vista funcional, o firmware realiza quatro etapas centrais:

1. inicializa os módulos de LED e botão;
2. configura o botão para gerar interrupção em borda de ativação;
3. publica um evento interno quando a interrupção ocorre;
4. consome o evento no loop principal e alterna o estado do LED.

## Arquitetura da Aplicação

A aplicação está organizada em módulos C independentes, cada um com responsabilidade bem definida:

* `main.c` coordena o fluxo principal da aplicação;
* `led.c` encapsula a configuração e o controle lógico do LED;
* `button.c` configura o botão, registra o callback de interrupção e publica eventos;
* `app_events.c` implementa o mecanismo interno de eventos usado pela aplicação.

Essa estrutura reduz o acoplamento entre a lógica de negócio e o acesso ao hardware, facilitando manutenção, leitura e evolução do firmware.

## Fluxo de Execução

### Inicialização

Na inicialização, a função principal chama `led_init()` e `button_init()`. Essas rotinas validam se os dispositivos descritos na Device Tree estão prontos para uso e configuram os respectivos pinos GPIO.

Se qualquer etapa falhar, a aplicação registra a condição de erro e entra em espera indefinida, evitando a execução parcial do firmware em estado inconsistente.

### Configuração do LED

O LED é descrito via `gpio_dt_spec`, obtido a partir de um alias do Device Tree. Essa abordagem elimina dependência de números fixos de porta e pino no código-fonte e torna o firmware mais portável entre placas compatíveis.

Após a validação do dispositivo, o pino é configurado como saída e inicializado em estado inativo. A partir daí, o LED pode ser controlado por chamadas de alto nível como:

* `led_on()`;
* `led_off()`;
* `led_toggle()`.

### Configuração do Botão

O botão também é descrito pelo Device Tree e configurado como entrada GPIO. Além disso, a aplicação registra um `gpio_callback` associado ao pino do botão.

Quando ocorre uma transição de nível configurada para ativação, o driver GPIO dispara o callback `button_pressed()`. Essa função não executa a ação final diretamente; ela apenas publica um evento interno da aplicação.

### Modelo Orientado a Evento

O núcleo do comportamento reativo está em `app_events.c`, que utiliza o subsistema `k_event` do Zephyr para sinalização entre interrupção e thread de aplicação.

O fluxo é o seguinte:

* o callback do botão publica `APP_EVENT_BUTTON`;
* o loop principal bloqueia em `app_event_wait(APP_EVENT_BUTTON)`;
* quando o evento é recebido, a aplicação executa `led_toggle()`.

Esse desenho separa a rotina de interrupção da lógica de aplicação, o que é uma boa prática em firmware embarcado, pois mantém o tratamento de IRQ curto e previsível.

## Papel de Cada Subsistema

### Device Tree

O Device Tree descreve os periféricos utilizados pela aplicação e fornece a base para geração das estruturas de acesso ao hardware em tempo de compilação. O firmware não depende de valores fixos embutidos no código; ele consome a descrição declarativa da placa.

### GPIO

A interface GPIO do Zephyr é responsável por configurar os pinos como entrada ou saída, registrar callbacks e habilitar interrupções. Toda interação física com botão e LED passa por essa abstração, sem acesso direto a registradores do microcontrolador.

### Eventos do Kernel

Os eventos do kernel funcionam como mecanismo de comunicação entre o contexto de interrupção e o contexto da aplicação. Isso evita que a interrupção contenha lógica extensa e mantém o firmware organizado em camadas.

## Comportamento Esperado

Em operação normal, a aplicação permanece aguardando eventos do botão. A cada acionamento válido, o LED muda de estado. O comportamento final é simples, mas a implementação demonstra conceitos importantes de firmware:

* inicialização segura de periféricos;
* uso de abstração de hardware;
* tratamento reativo baseado em interrupção;
* comunicação entre contextos por evento;
* modularização do código.

## Objetivo Didático

Este projeto foi estruturado para consolidar fundamentos de desenvolvimento embarcado com Zephyr RTOS e para servir como material técnico público em portfólio. A aplicação mostra, em um exemplo reduzido, como organizar firmware de forma profissional, com foco em portabilidade, clareza arquitetural e previsibilidade de execução.
