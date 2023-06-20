/*
Calculo Numerico: Tabalho 2

Lucas A. Pataluch
Murilo M. Grosso - 13672043
Octavio X. Furio - 13681862
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char* itemDaTupla(char* linha, int item);
void dadosParaVetor(FILE* stream, int coluna, double* vetor);
void MMQ(int n, double* t, double* y, double* a);
double g1(double t);
double g2(double t);
double g(double t, double* a);
double SQres(int n, double* t, double* y, double* a);
double SQtot(int n, double* t, double* a);
double R2(int n, double* t, double* y, double* a);

int main()
{
    //Base de dados
    char arquivo[50] = "dados.txt";
    FILE* stream = fopen(arquivo, "r");

    const int n = 101; //Numero de pontos
    double t[n + 1]; //Vetor t_i
    double y[n + 1]; //Vetor y_i
    double a[2 + 1]; //Vetor dos coeficientes

    dadosParaVetor(stream, 2, t);
    dadosParaVetor(stream, 3, y);
    
    MMQ(n, t, y, a);
}

/*
<Resumo>: Retorna o item da tupla de uma
determinada linha do arquivo
*/
const char* itemDaTupla(char* linha, int item)
{
    const char* substr;

    for (substr = strtok(linha, ","); substr && *substr; substr = strtok(NULL, ",\n"))
        if (!--item)
            return substr;
    
    return 0;
}

/*
<Resumo>: Converte os dados de uma coluna 
do arquivo para vetores.
*/
void dadosParaVetor(FILE* stream, int coluna, double* vetor)
{
    char linha[1024];
    int i = 0;

    rewind(stream);
    while (fgets(linha, 1024, stream))
    {
        char* temp = strdup(linha);
        vetor[i] = atof(itemDaTupla(temp, coluna));
        free(temp);

        i++;
    }
}

/*
<Resumo>: Metodo dos Minimos Quadrados com
n pontos de y(t) para chegar a uma aroximacao na forma
g(t) = a * g1(t) + b * g2(t).
*/
void MMQ(int n, double* t, double* y, double* a)
{
    double soma_g1_g1 = 0;
    double soma_g1_g2 = 0;
    double soma_g2_g2 = 0;
    double soma_g1_y = 0;
    double soma_g2_y = 0;

    for(int i = 1; i <= n; i++)
    {
        soma_g1_g1 += g1(t[i]) * g1(t[i]);
        soma_g1_g2 += g1(t[i]) * g2(t[i]);
        soma_g2_g2 += g2(t[i]) * g2(t[i]);
        soma_g1_y += g1(t[i]) * y[i];
        soma_g2_y += g2(t[i]) * y[i];
    }

    double denom = soma_g1_g1 * soma_g2_g2 - soma_g1_g2 * soma_g1_g2;
    a[1] = (soma_g1_y * soma_g2_g2 - soma_g1_g2 * soma_g2_y)/denom;
    a[2] = (soma_g1_g1 * soma_g2_y - soma_g1_g2 * soma_g1_y)/denom;

    printf("\n\tFuncao de ajuste:");
    printf("\n\tg(t) = %f + %ft²\n\n", a[1], a[2]);

    printf("\tSQres = %f\n", SQres(n,t,y,a));
    printf("\tSQtot = %f\n", SQtot(n,t,a));
    printf("\tR2 = %f \n\n", R2(n,t,y,a));
}

//Funcao g1(t) = 1
double g1(double t)
{
    return 1;
}

//Funcao g2(t) = t²
double g2(double t)
{
    return t*t;
}

//Funcao g(t) = a1g1(t) + a2g2(t)
double g(double t, double* a)
{
    return a[1]*g1(t) + a[2]*g2(t);
}

//Soma dos residuos ao quadrados
double SQres(int n, double* t, double *y, double *a)
{
    double res = 0;
    double r;

    for(int i = 1; i <= n; i++)
    {
        r = y[i] - g(t[i],a);
        res += r*r;
    }

    return res;
}

//Erro total
double SQtot(int n, double* t, double* a)
{
    double soma = 0;
    for(int i = 1; i <= n; i++){
        soma += g(t[i], a);    
    }
    double y = soma / n;
    
    double sqtot = 0;
    for(int i = 1; i <= n; i++){
       sqtot += (g(t[i], a) - y) * (g(t[i], a) - y);    
    }

    return sqtot;
}

//Coeficiente da quantidade do ajuste
double R2(int n, double* t, double* y, double* a)
{
    return 1.0 - (SQres(n,t,y,a) / SQtot(n,t,a));
}
