# Perguntas e Respostas sobre a Arquitetura do Zephyr RTOS

## Tema 1: Kconfig

### 1. Qual é a diferença entre Device Tree e Kconfig?

O Device Tree é um mecanismo declarativo utilizado para descrever o hardware disponível na plataforma, fornecendo informações como periféricos, endereços, GPIOs e propriedades físicas para drivers e aplicações. O Kconfig, por sua vez, é o sistema de configuração do software, responsável por selecionar e parametrizar, em tempo de compilação, quais componentes do kernel, drivers e subsistemas do Zephyr farão parte da imagem final do firmware.

### 2. Qual é a função do prj.conf?

O `prj.conf` é o arquivo de configuração da aplicação que atribui valores aos símbolos Kconfig (`CONFIG_*`), permitindo habilitar, desabilitar ou parametrizar funcionalidades do Zephyr. Durante o processo de build, essas configurações são combinadas com os valores padrão da placa e com as dependências definidas pelo sistema Kconfig para produzir a configuração final do firmware.

### 3. O que é um símbolo CONFIG_*?

Um símbolo `CONFIG_*` é uma opção configurável do sistema Kconfig que representa uma funcionalidade ou parâmetro do Zephyr. Esses símbolos podem habilitar recursos, definir valores de configuração e controlar o comportamento de drivers, subsistemas e componentes do kernel, sendo avaliados durante a compilação para determinar quais trechos de código serão incluídos no firmware.

### 4. Por que CONFIG_EVENTS=y resolveu o erro de linkedição?

Ao definir `CONFIG_EVENTS=y`, o sistema Kconfig passou a incluir na compilação a implementação do subsistema de eventos do kernel. Dessa forma, as funções `k_event_post()` e `k_event_wait()` deixaram de ser apenas declarações visíveis ao compilador e passaram a possuir implementações efetivamente vinculadas pelo linker, eliminando os erros de referência indefinida.

### 5. Qual é a diferença entre prj.conf, .config e autoconf.h?

O `prj.conf` contém as configurações solicitadas pela aplicação. O arquivo `.config` representa a configuração final produzida pelo sistema Kconfig após combinar `prj.conf`, `defconfig` e as dependências entre símbolos. O `autoconf.h` é um cabeçalho gerado automaticamente a partir do `.config`, contendo diretivas `#define CONFIG_*` utilizadas pelo compilador para incluir ou excluir partes do código durante a compilação.

### 6. Quem define as configurações padrão de uma placa?

As configurações padrão são definidas pelo arquivo `defconfig` da placa, que estabelece um conjunto inicial de símbolos Kconfig compatíveis com aquele hardware. O `prj.conf` atua sobre essa configuração base, sobrescrevendo ou complementando os valores conforme as necessidades específicas da aplicação.

---

## Tema 2: Threads

### 1. O que é uma thread no Zephyr?

Uma thread é uma unidade de execução independente gerenciada pelo kernel do Zephyr. Cada thread possui sua própria pilha de execução, prioridade e estado. O kernel é responsável por escalonar as threads de acordo com suas prioridades e estados de bloqueio.

### 2. Como uma thread é criada no Zephyr?

Uma thread é criada com `k_thread_create()`, que recebe como argumentos a estrutura de controle da thread, o stack definido com `K_THREAD_STACK_DEFINE`, o tamanho do stack, a função de entrada, argumentos opcionais, a prioridade e o delay de início. O stack deve ser declarado com `K_THREAD_STACK_DEFINE` para garantir o alinhamento correto exigido pela arquitetura.

### 3. O que acontece se o stack de uma thread for muito pequeno?

Se o stack for insuficiente para acomodar o frame de chamada das funções executadas pela thread, ocorre um stack overflow. No Zephyr, isso pode causar corrupção de memória ou uma exceção de hardware, dependendo da configuração de proteção de memória da plataforma.

### 4. O que significa a prioridade de uma thread no Zephyr?

No Zephyr, threads com menor valor numérico de prioridade têm maior precedência de execução. O escalonador preempta threads de menor prioridade quando uma thread de maior prioridade se torna executável. Threads com a mesma prioridade são escalonadas de forma cooperativa ou por time slice, dependendo da configuração do kernel.

---

## Tema 3: Semáforo

### 1. Para que serve o k_sem nesta aplicação?

O semáforo `k_sem` é utilizado como mecanismo de sinalização entre o contexto de interrupção e a LED thread. O callback do botão chama `k_sem_give()` para sinalizar que um evento ocorreu, e a LED thread bloqueia em `k_sem_take()` aguardando esse sinal. Isso permite que a thread durma eficientemente sem consumir ciclos de CPU enquanto não há eventos.

### 2. Por que não executar a lógica diretamente no callback do botão?

Callbacks de interrupção GPIO executam em contexto de ISR, onde o conjunto de operações permitidas é restrito. Operações bloqueantes, alocação de memória e chamadas a subsistemas que não são ISR-safe não podem ser realizadas nesse contexto. Por isso, o callback apenas sinaliza um semáforo e delega o processamento para uma thread de aplicação.

### 3. Qual é a diferença entre k_sem_give() e k_sem_take()?

`k_sem_give()` incrementa o contador do semáforo e, se houver uma thread bloqueada aguardando, a desbloqueia. `k_sem_take()` decrementa o contador do semáforo; se o contador for zero, a thread que chamou a função bloqueia até que outro contexto chame `k_sem_give()`.

---

## Tema 4: Fila FIFO e Memory Slab

### 1. O que é uma fila FIFO no Zephyr?

Uma fila FIFO (`k_fifo`) é uma estrutura de dados do kernel que permite a comunicação assíncrona entre contextos de execução. Os itens são inseridos com `k_fifo_put()` e retirados com `k_fifo_get()`, sempre na ordem em que foram inseridos. A fila pode ser usada entre threads e entre ISR e threads.

### 2. Por que a struct de mensagem precisa do campo fifo_reserved?

O Zephyr utiliza o primeiro campo da struct enfileirada para encadear os nós internamente. Esse campo, convencionalmente chamado `fifo_reserved` e do tipo `void *`, é preenchido pelo kernel durante `k_fifo_put()` e não deve ser manipulado pela aplicação.

### 3. Qual é a diferença entre K_FIFO_DEFINE e k_fifo_init()?

`K_FIFO_DEFINE` declara e inicializa a fila em tempo de compilação, sem necessidade de chamada explícita em runtime. `k_fifo_init()` inicializa uma fila declarada manualmente com `struct k_fifo`, sendo necessário chamá-la antes de qualquer uso da fila. Ambas as abordagens são válidas; `K_FIFO_DEFINE` é mais concisa e elimina o risco de uso da fila antes da inicialização.

### 4. Por que usar k_mem_slab em vez de alocação dinâmica?

Em sistemas embarcados, o uso de `malloc()` ou heap dinâmico é geralmente evitado por introduzir fragmentação de memória e tempo de alocação não determinístico. O `k_mem_slab` fornece blocos de tamanho fixo com alocação e liberação em tempo constante, o que é mais adequado para sistemas de tempo real. Após o consumo da mensagem, a memória deve ser liberada com `k_mem_slab_free()` para que o bloco retorne ao pool.

### 5. O que acontece se o memory slab estiver cheio?

Se todos os blocos do slab estiverem em uso e `k_mem_slab_alloc()` for chamado com `K_NO_WAIT`, a função retorna um código de erro e a mensagem é descartada. Isso é preferível a bloquear o produtor, especialmente quando ele opera em contexto de ISR. O tamanho do slab deve ser dimensionado de acordo com a taxa de produção e consumo de mensagens esperada pela aplicação.
