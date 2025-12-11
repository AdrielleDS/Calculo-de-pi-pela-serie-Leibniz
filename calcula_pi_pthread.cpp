#include <iostream>
#include <iomanip>
#include <cmath>     //Usado para a função pow
#include <pthread.h> //Usado para API de threads
#include <cstdlib>   //Usado para conversão de argumento
using namespace std;

const long long TOTAL_TERMS = 1000000000LL; // Constante total de termos da série (1 bilhão)

// POO em C++ e cada objeto ThreadPi representa uma tarefa de cálculo paralela
class ThreadPi
{
private:
long long inicio;
long long fim;
double soma_parcial;

public:
ThreadPi()
{
inicio = 0;
fim = 0;
soma_parcial = 0.0;
}

// Método para definir o bloco de trabalho da thread
void setIntervalo(long long i, long long f)
{
inicio = i;
fim = f;
soma_parcial = 0.0;
}

// Método de acesso getter para o programa principal obter o resultado parcial
double getSomaParcial()
{
return soma_parcial;
}

// Método principal que executa o cálculo da serie de Leibniz
// O resultado é armazenado em soma_parcial
void calcular()
{
for (long long n = inicio; n < fim; n++)
{
soma_parcial += pow(-1.0, n) / (2.0 * n + 1.0);
}
}

// Função estatica que permite calcular o objeto correto
static void *executar(void *arg)
{
ThreadPi *obj = static_cast<ThreadPi *>(arg);
obj->calcular();
return nullptr;
}
};

// Função principal que configura o paralelismo.
int main(int argc, char *argv[])
{

if (argc != 2)
{
cout << "Uso: " << argv[0] << " <numero_de_threads>" << endl;
return 1;
}

int num_threads = atoi(argv[1]); // converte string para inteiro

if (num_threads != 1 && num_threads != 2 &&
num_threads != 4 && num_threads != 8)
{
cout << "\nNumero invalido. Use apenas 1, 2, 4 ou 8 threads." << endl;
return 1;
}

// Vetores para manipular pthreads
pthread_t threads[8];
ThreadPi threads_obj[8];

// Divisão das tarefas
long long termos_por_thread = TOTAL_TERMS / num_threads;
long long resto = TOTAL_TERMS % num_threads;
long long atual = 0;

// Criação da threads
for (int i = 0; i < num_threads; i++)
{
long long fim_intervalo = atual + termos_por_thread +
(i == num_threads - 1 ? resto : 0);

// Configura o objeto com o intervalo de calculo
threads_obj[i].setIntervalo(atual, fim_intervalo);
pthread_create(&threads[i], nullptr, ThreadPi::executar, &threads_obj[i]); // Cria a thread passando o objeto Thread como argumento

atual = fim_intervalo; // Ponto de inicio para proxima thread
}

// Espera os resultados
double soma_total = 0.0;
for (int i = 0; i < num_threads; i++)
{
pthread_join(threads[i], nullptr);             // Espera o termino da thread
soma_total += threads_obj[i].getSomaParcial(); // Soma os resultados parciais de cada thread
}

double pi_aproximado = 4.0 * soma_total; // Resultado final

// Saída formatada para o resultado e medição do comando time
cout << fixed << setprecision(15);
cout << "Resultado de pi aproximado com " << num_threads
<< " threads = " << pi_aproximado << endl;

return 0;
}
