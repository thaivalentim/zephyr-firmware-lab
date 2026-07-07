# Descrição Técnica da Aplicação

## Visão Geral

Esta aplicação foi desenvolvida como estudo de arquitetura de firmware sobre o Zephyr RTOS. O sistema implementa um fluxo reativo orientado a eventos para controlar um LED a partir da atuação de um botão físico, com separação explícita entre inicialização de periféricos, tratamento de interrupções, controle de modo por thread e registro de eventos por fila FIFO.

Do ponto de vista funcional, o firmware realiza as seguintes etapas:

1. inicializa os módulos de LED e botão;
2. configura o botão para gerar interrupção em borda de ativação;
3. publica um evento interno quando a interrupção ocorre;
4. a LED thread consome o evento e avança o modo de operação do LED;
5. os módulos produtores enviam mensagens estruturadas para a fila de logging;
6. a logging thread consome a fila e exibe os eventos no console.

## Arquitetura da Aplicação

A aplicação está organizada em módulos C independentes, cada um com responsabilidade bem definida:

* `main.c` coordena a inicialização e o fluxo principal da aplicação;
* `led.c` encapsula a configuração e o controle lógico do LED;
* `button.c` configura o botão, registra o callback de interrupção e publica eventos;
* `app_events.c` implementa o mecanismo interno de eventos baseado em semáforo;
* `led_thread.c` implementa a thread dedicada ao controle do ciclo de modos do LED;
* `logging_thread.c` implementa a fila FIFO, o produtor e a thread consumidora de logs.

Essa estrutura reduz o acoplamento entre a lógica de negócio e o acesso ao hardware, facilitando manutenção, leitura e evolução do firmware.

## Fluxo de Execução

### Inicialização

Na inicialização, `main()` chama `led_init()` e `button_init()`. Essas rotinas validam se os dispositivos descritos na Device Tree estão prontos para uso e configuram os respectivos pinos GPIO.

Se qualquer etapa falhar, a aplicação registra a condição de erro e entra em espera indefinida, evitando a execução parcial do firmware em estado inconsistente.

Em seguida, `main()` inicializa a fila FIFO com `fifo_init()`, cria as threads com `led_thread_init()` e `log_thread_init()`, e envia a primeira mensagem de log sinalizando que a inicialização foi concluída.

### Configuração do LED

O LED é descrito via `gpio_dt_spec`, obtido a partir de um alias do Device Tree. Essa abordagem elimina dependência de números fixos de porta e pino no código-fonte e torna o firmware mais portável entre placas compatíveis.

Após a validação do dispositivo, o pino é configurado como saída e inicializado em estado inativo. O controle é feito por chamadas de alto nível: `led_on()`, `led_off()` e `led_toggle()`.

### Configuração do Botão

O botão é descrito pelo Device Tree e configurado como entrada GPIO. A aplicação registra um `gpio_callback` associado ao pino do botão.

Quando ocorre uma transição de nível configurada para ativação, o driver GPIO dispara o callback `button_pressed()`. Essa função publica um evento interno via semáforo e envia uma mensagem de log com `fifo_producer()`.

### LED Thread

A LED thread é criada com `k_thread_create()` e opera em loop contínuo. Ela mantém o estado atual do modo de operação do LED e aguarda eventos com `app_event_take()`.

O ciclo de modos segue a sequência: `LED_OFF -> LED_SLOW -> LED_MEDIUM -> LED_FAST -> LED_OFF`. A cada avanço de modo, a thread registra a mudança via `fifo_producer()` com o evento `LOG_EVT_MODE_CHANGE` e o novo modo como valor.

Nos modos ativos, a thread alterna o LED com `led_toggle()` e aguarda o próximo evento com timeout definido pelo modo atual. Se o evento chegar antes do timeout, o modo avança; caso contrário, o LED continua piscando no mesmo modo.

### Sistema de Logging

O sistema de logging é composto por três elementos:

* `fifo_producer()`: função chamada pelos módulos produtores para enfileirar mensagens;
* `k_fifo`: fila FIFO do kernel que armazena ponteiros para as mensagens;
* `fifo_consumer()`: thread que consome a fila e exibe as mensagens no console.

A alocação de memória para as mensagens é feita por `k_mem_slab`, que fornece blocos de tamanho fixo sem uso de heap dinâmico. Após o consumo, a memória é liberada com `k_mem_slab_free()`.

Cada mensagem carrega quatro campos: `source` (módulo de origem), `event` (tipo de evento), `value` (dado contextual) e `timestamp` (tempo em ms desde o boot, obtido por `k_uptime_get_32()`).

### Modelo Orientado a Evento

O núcleo do comportamento reativo está em `app_events.c`, que utiliza um semáforo do Zephyr para sinalização entre o contexto de interrupção e a LED thread.

O fluxo é o seguinte:

* o callback do botão chama `app_event_publish()`, que incrementa o semáforo;
* a LED thread bloqueia em `app_event_take()`, que aguarda o semáforo;
* quando o evento é recebido, a thread avança o modo de operação.

Esse desenho separa a rotina de interrupção da lógica de aplicação, mantendo o tratamento de IRQ curto e previsível.

## Papel de Cada Subsistema

### Device Tree

O Device Tree descreve os periféricos utilizados pela aplicação e fornece a base para geração das estruturas de acesso ao hardware em tempo de compilação. O firmware não depende de valores fixos embutidos no código; ele consome a descrição declarativa da placa.

### GPIO

A interface GPIO do Zephyr é responsável por configurar os pinos como entrada ou saída, registrar callbacks e habilitar interrupções. Toda interação física com botão e LED passa por essa abstração, sem acesso direto a registradores do microcontrolador.

### Semáforo

O semáforo `k_sem` é utilizado como mecanismo de sinalização entre o contexto de interrupção e a LED thread. Ele permite que a thread bloqueie eficientemente enquanto aguarda eventos, sem consumir ciclos de CPU.

### Fila FIFO

A fila `k_fifo` é utilizada para comunicação assíncrona entre os módulos produtores e a logging thread. O uso de `k_mem_slab` para alocação garante tempo de alocação determinístico e ausência de fragmentação de memória.

## Comportamento Esperado

Em operação normal, a aplicação permanece aguardando eventos do botão. A cada acionamento, o LED avança para o próximo modo de operação. A logging thread exibe no console cada evento relevante com sua origem, tipo, valor e timestamp.

Exemplo de saída:

```
[LOGGING THREAD] src=0, evt=0, val=0, ts=0 ms       // MAIN, INIT
[LOGGING THREAD] src=1, evt=1, val=0, ts=3236 ms    // BUTTON, BUTTON_PRESS
[LOGGING THREAD] src=2, evt=2, val=1, ts=3236 ms    // LED, MODE_CHANGE -> LED_SLOW
[LOGGING THREAD] src=1, evt=1, val=0, ts=7367 ms    // BUTTON, BUTTON_PRESS
[LOGGING THREAD] src=2, evt=2, val=2, ts=7367 ms    // LED, MODE_CHANGE -> LED_MEDIUM
```

## Objetivo Didático

Este projeto foi estruturado para consolidar fundamentos de desenvolvimento embarcado com Zephyr RTOS e para servir como material técnico público em portfólio. A aplicação demonstra, em um exemplo reduzido, como organizar firmware de forma profissional, com foco em portabilidade, clareza arquitetural e previsibilidade de execução.
