# TP2 de SOPE: etapa 1

Este trabalho foi realizado por Eduardo Campos (201604920), Miguel Gomes (201605908) e Pedro Azevedo (201603816) no âmbito da disciplina de SOPE.

## Compilação e Execução

Na pasta onde se encontra o makefile principal faça:
```
make clean && make
```

Executar os dois programas , em terminais separados,  primeiro o Q1 e de seguida o U1:
```
./Q1 <-t nsecs> <fifoname>
./U1 <-t nsecs> <fifoname>
```

## Descrição do Programa

### Utentes

Ao iniciar o programa U1 este vai preencher uma struct do tipo UserParser.
```c
typedef struct
{
    int nsecs;
    char fifoname[256];
} User;
```
Após esta inicialização o programa vai criar multiplas threads ,durante o período especificado, e a tempos aleatórios. Estas threads iram criar uma struct do tipo message que por sua vez irá ser utilizada para escreverem para o FIFO público ,definido pelo utilizador.
```c
typedef struct Message
{
    int i;
    int pid;
    long tid;
    int dur;
    int pl;
} Message;
```
Depois do envio da mensagem a thread espera que pela resposta do servidor que será enviada para um FIFO privado criado pela thread, terminando após recebido.
A seguir ao tempo de execução terminar, o programa espera por todas as threads criadas terminem e liberta a memória das structs utilizadas.

```c
for (int i = 0; i < n_threads; i++){
   pthread_join(threads[i],NULL);
}

close(fd);
destroyUser(user);
```

### Quartos de Banho

Ao inicializar o Q1 este vai preencher uma struct do tipo BathroomParser, tendo em conta que alguns dos elementos da struct não são utilizados para a Etapa 1.
```c
typedef struct
{
    int secs_f;
    int nsecs;
    int place_f;
    int nplaces;
    int thread_f;
    int nthreads;
    char fifoname[MAX_FIFONAME];

} BathroomParser;
```

Após a inicialização, o programa espera por pedidos vindos do FIFO público indicado e trata-os criando uma thread nova que será responsável pela comunicação da resposta de sucesso ou insucesso ,pelo o FIFO privado correspondente. Depois de enviar a resposta espera o tempo que foi indicado, terminando após esse mesmo.
Passado o tempo de execução definido, igualmente ao programa dos utentes, este espera que todas as suas threads terminem e liberta a memória de todas as structs utilizadas.

### Notas:

1. Ambos os programas têm um array dinâmico de threads onde são guardadas todas as threads, este array é inicializado com 1 000 000 de posições e só após o nº de pedidos ou threads exceder esse mesmo começamos a realocar mais espaço para esse array.

2. Caso a criação de threads dê erro, o progama sai do ciclo de criação de threads e termina normalmente.

3. O único sistema de sincronização presente no código é mutexes, e utilizado no servidor para aumentar o nº de pedidos tratados.

4. No caso de o U1 ser executado primeiro, o programa cria na mesma as threads ,no entanto o registo do log a fase de operação será sempre CLOSD, dado que tenta abrir o FIFO apenas uma vez no início do programa e se não conseguir considera que o Quarto de Banho está fechado.



