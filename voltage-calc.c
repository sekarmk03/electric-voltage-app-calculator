#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COL 6
#define MAX 200

typedef struct
{
    double Arus, R1, R2, RTotal, Tegangan;
} rangkaian;

int lenData[COL][MAX];
char dataStr[MAX][COL][20];

double input_data(int n, rangkaian p[]);
int digits(int number);
int maxW(int arr[], int n);
void batas(FILE *file, int n, char tipe);
void printData(FILE *file, int width[], char content[][20]);
void toArrayStr(int n, rangkaian p[]);
void simpan_data(FILE *file, int n, rangkaian p[], double rata2);

int main()
{
    int jmlData = 0;
    FILE *tegangan;
    printf("Masukkan jumlah rangkaian paralel : ");
    scanf("%d", &jmlData);
    rangkaian paralel[MAX];
    double rata2T = input_data(jmlData, paralel);
    toArrayStr(jmlData, paralel);
    simpan_data(tegangan, jmlData, paralel, rata2T);
    return 0;
}

double input_data(int n, rangkaian p[])
{
    double TRata2 = 0;
    int i = 0;
    for (i = 0; i < n; i++)
    {
        printf("Rangkaian %d\n", i + 1);
        printf("Arus : ");
        scanf(" %lf", &p[i].Arus);
        printf("R1 : ");
        scanf(" %lf", &p[i].R1);
        printf("R2 : ");
        scanf(" %lf", &p[i].R2);
        p[i].RTotal = (double)(p[i].R1 * p[i].R2) / (p[i].R1 + p[i].R2);
        p[i].Tegangan = (double)(p[i].Arus * p[i].RTotal);
        TRata2 += p[i].Tegangan;
        if (i < n - 1)
            printf("--------------------------------\n");
        lenData[0][i] = digits(i + 1) + 1;
        lenData[1][i] = digits(p[i].Arus) + 3;
        lenData[2][i] = digits(p[i].R1) + 3;
        lenData[3][i] = digits(p[i].R2) + 3;
        lenData[4][i] = digits(p[i].RTotal) + 3;
        lenData[5][i] = digits(p[i].Tegangan) + 3;
    }
    lenData[0][i] = strlen("No.");
    lenData[1][i] = strlen("(amp)");
    lenData[2][i] = strlen("(ohm)");
    lenData[3][i] = strlen("(ohm)");
    lenData[4][i] = strlen("R_Total");
    lenData[5][i] = strlen("(I_x_R_Total)_Volt");
    return (double)(TRata2 / n);
}

int digits(int number)
{
    int count = 0;
    while (number != 0)
    {
        number /= 10;
        count++;
    }
    return count;
}

int maxW(int arr[], int n)
{
    int max = 0;
    max = arr[0];
    int i;
    for (i = 1; i < n + 1; i++)
    {
        if (max < arr[i])
            max = arr[i];
    }
    return max;
}

void batas(FILE *file, int n, char tipe)
{
    int i;
    for (i = 0; i < n; i++)
        fprintf(file, "%c", tipe);
    fprintf(file, "\n");
}

void printData(FILE *file, int width[], char content[][20])
{
    int i, j;
    for (j = 0; j < COL; j++)
    {
        for (i = 0; i < (width[j] - strlen(content[j])) / 2; i++)
            fprintf(file, " ");
        fprintf(file, " ");
        fprintf(file, "%s", content[j]);
        fprintf(file, " ");
        for (i = ((width[j] - strlen(content[j])) / 2); i < (width[j] - strlen(content[j])); i++)
            fprintf(file, " ");
    }
    fprintf(file, "\n");
}

void toArrayStr(int n, rangkaian p[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        sprintf(dataStr[i][0], "%d.", i + 1);
        sprintf(dataStr[i][1], "%0.2lf", p[i].Arus);
        sprintf(dataStr[i][2], "%0.2lf", p[i].R1);
        sprintf(dataStr[i][3], "%0.2lf", p[i].R2);
        sprintf(dataStr[i][4], "%0.2lf", p[i].RTotal);
        sprintf(dataStr[i][5], "%0.2lf", p[i].Tegangan);
    }
}

void simpan_data(FILE *file, int n, rangkaian p[], double rata2)
{
    file = fopen("Tegangan.txt", "w");
    int i;
    char judulHeader1[][20] = {"No.", "Arus", "R1", "R2", "R Total", "Tegangan"};
    char judulHeader2[][20] = {" ", "(amp)", "(ohm)", "(ohm)", "(Ohm)", "(I x R Total) Volt"};
    int width[6], sum = 0;
    for (i = 0; i < COL; i++)
    {
        width[i] = maxW(lenData[i], n);
        sum += width[i];
    }
    batas(file, 12 + sum, '-');
    printData(file, width, judulHeader1);
    printData(file, width, judulHeader2);
    batas(file, 12 + sum, '=');
    for (i = 0; i < n; i++)
        printData(file, width, dataStr[i]);
    batas(file, 12 + sum, '-');
    char tRata[100] = {"Tegangan rata-rata adalah"};
    fprintf(file, "%s", tRata);
    for (i = 0; i < (12 + sum) - strlen(tRata) - (digits(rata2) + 8); i++)
        fprintf(file, " ");
    fprintf(file, "%0.2f Volt\n", rata2);
    fclose(file);
}
