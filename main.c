#define _GNU_SOURCE
#include <malloc.h> 
#include <pthread.h> 
#include <sched.h> 
#include <signal.h> 
#include <stdint.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> // Inclusão da biblioteca para tipos de dados do sistema
#include <sys/wait.h> // Inclusão da biblioteca para esperar por processos
#include <time.h> // Inclusão da biblioteca para manipulação de tempo
#include <unistd.h> // Inclusão da biblioteca para chamadas ao sistema POSIX
#include <stdbool.h> // Inclusão da biblioteca para tipo bool (true, false)

#define FIBER_STACK 1024 * 64 // Define o tamanho do stack para as fibras

int band = 0; 
struct c {
  int saldo; 
};

typedef struct c conta; 

conta from, to; 
int valor = 10; 
int contador_transferencias = 0; 

pthread_mutex_t mutex_contas; 

void *transferencia(void *arg) { // Função que realiza a transferência de saldo entre contas
  pthread_mutex_lock(&mutex_contas); 

  // Verifica se a transferência é possível e qual a direção da mesma
  bool saldo_from_maior_0_band_igual_0 = (from.saldo >= valor) && (band == 0);
  bool saldo_to_maior_valor_band_igual_1 = (to.saldo >= valor) && (band == 1);

  // Condição para transferência de 'from' para 'to'
  if (saldo_from_maior_0_band_igual_0) {
    from.saldo -= valor; 
    to.saldo += valor; 
    contador_transferencias++; // Incrementa o contador de transferências

    // Se o saldo de 'from' for negativo após a transferência, reverte a operação
    if (from.saldo < 0) {
      to.saldo -= valor; 
      from.saldo += valor; 
      contador_transferencias--; // Decrementa o contador de transferências
    }

    // Se o saldo de 'from' for zero, muda a direção da transferência
    if (from.saldo == 0) {
      band = 1;
    }

  } 
  // Condição para transferência de 'to' para 'from'
  else if (saldo_to_maior_valor_band_igual_1) {
    to.saldo -= valor; 
    from.saldo += valor; 
    contador_transferencias++; 

    // Se o saldo de 'to' for negativo após a transferência, reverte a operação
    if (to.saldo < 0) {
      from.saldo -= valor; 
      to.saldo += valor; 
      contador_transferencias--; 
    }
    // Se o saldo de 'to' for zero, muda a direção da transferência
    if (to.saldo == 0) {
      band = 0;
    }
  }

  // Impressão dos resultados da transferência
  printf("Transferência %d concluída com sucesso!\n", contador_transferencias);
  printf("Saldo de c1: %d\n", from.saldo);
  printf("Saldo de c2: %d\n", to.saldo);

  pthread_mutex_unlock(&mutex_contas); // Desbloqueia o mutex
  return 0; // Retorna nulo (padrão para função de thread)
}

int main() {
  void *stack; 
  int i; 
  pthread_t threads_transferencia[100]; // Array para armazenar os IDs das threads de transferência

  stack = malloc(FIBER_STACK); // Alocação de memória para a pilha
  if (stack == NULL) { // Verifica se a alocação falhou
    perror("malloc: could not allocate stack");
    exit(EXIT_FAILURE);
  }

  pthread_mutex_init(&mutex_contas, NULL);
  from.saldo = 100; // Inicializa o saldo de 'from'
  to.saldo = 100; // Inicializa o saldo de 'to'

  printf("Transferencias\n"); 

  // Cria 100 threads para realizar transferências
  for (i = 0; i < 100; i++) {
    if (pthread_create(&threads_transferencia[i], NULL, transferencia, NULL) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  // Espera todas as 100 threads terminarem
  for (i = 0; i < 100; i++) {
    if (pthread_join(threads_transferencia[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  // Finaliza o programa, imprimindo o total de transferências realizadas
  printf("Transferências concluídas e memória liberada.\n");
  printf("Total de transferências realizadas: %d\n", contador_transferencias);

  pthread_mutex_destroy(&mutex_contas); 
  free(stack); // Liberação da memória alocada para a pilha

  return 0; 
}
