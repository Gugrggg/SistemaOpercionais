Gustavo Galhardo Rodrigues - 10403091
Pedro Nogueira Ribeiro - 10324924

Como compilar: GCC
gcc main.c -o main.out

***1. A conta to pode receber mais de uma transferência simultânea;***

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

***2. A conta from pode enviar mais de uma transferência simultânea;***
***3. A conta from não pode enviar dinheiro se não tiver mais saldo;***

if (from.saldo >= valor) { // Verifica se há saldo suficiente para transferência
    from.saldo -= valor; // Reduz o saldo de 'from'
    to.saldo += valor; // Aumenta o saldo de 'to'
    contador_transferencias++; // Incrementa o contador de transferências
} else {
    printf("Saldo insuficiente para transferência.\n");
}

***4. A conta to pode trocar de ordem com a conta from, ou seja, a conta que enviava pode***
receber e a conta que recebia pode enviar;

if (saldo_from_maior_0_band_igual_0) {
    from.saldo -= valor;
    to.saldo += valor;
    contador_transferencias++;
    if (from.saldo == 0) {
        band = 1; // Troca a direção da transferência
    }
} else if (saldo_to_maior_valor_band_igual_1) {
    to.saldo -= valor;
    from.saldo += valor;
    contador_transferencias++;
    if (to.saldo == 0) {
        band = 0; // Troca a direção da transferência
    }
}

***5.Poderão ser realizadas até 100 transações simultâneas de transferência***

int main() {
  pthread_t threads_transferencia[100]; // Declara um array de threads para as transferências

  // Inicializa as contas e o mutex
  pthread_mutex_init(&mutex_contas, NULL);
  from.saldo = 100;
  to.saldo = 100;

  // Cria 100 threads para realizar as transferências
  for (int i = 0; i < 100; i++) {
    if (pthread_create(&threads_transferencia[i], NULL, transferencia, NULL) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  // Espera todas as 100 threads terminarem
  for (int i = 0; i < 100; i++) {
    if (pthread_join(threads_transferencia[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  pthread_mutex_destroy(&mutex_contas); // Limpeza final

  return 0;
}
