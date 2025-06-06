#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    long timestamp;
    char data[1024];
} SensorData;

int find_file(const char *name, long target_timestamp);
int binary_search_closest(SensorData *data, int left, int right, long target);
long convert_to_timestamp(int dia, int mes, int ano, int hora, int minuto, int segundo);

int main(int argc, char *argv[]) {
    if (argc != 9) {
        printf("Uso: %s arg1 ABCD dia mes ano hora minuto segundo\n", argv[0]);
        printf("Exemplo: %s arg1 ABCD 15 3 2024 14 30 0\n", argv[0]);
        return 1;
    }

    if (strlen(argv[2]) != 4) {
        printf("Argumento inválido: %s (deve ter 4 caracteres)\n", argv[2]);
        return 1;
    }

    int dia = atoi(argv[3]);
    int mes = atoi(argv[4]);
    int ano = atoi(argv[5]);
    int hora = atoi(argv[6]);
    int minuto = atoi(argv[7]);
    int segundo = atoi(argv[8]);

    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || 
        hora < 0 || hora > 23 || minuto < 0 || minuto > 59 || segundo < 0 || segundo > 59) {
        printf("Valores de data/hora inválidos!\n");
        return 1;
    }

    long target_timestamp = convert_to_timestamp(dia, mes, ano, hora, minuto, segundo);
    if (target_timestamp <= 0) {
        printf("Erro ao converter data/hora para timestamp\n");
        return 1;
    }

    find_file(argv[2], target_timestamp);
    
    return 0;
}

long convert_to_timestamp(int dia, int mes, int ano, int hora, int minuto, int segundo) {
    struct tm timeinfo;
    timeinfo.tm_year = ano - 1900;  
    timeinfo.tm_mon = mes - 1;      
    timeinfo.tm_mday = dia;         
    timeinfo.tm_hour = hora;        
    timeinfo.tm_min = minuto;       
    timeinfo.tm_sec = segundo;      
    timeinfo.tm_isdst = -1;         

    time_t timestamp = mktime(&timeinfo);
    if (timestamp == -1) {
        return -1;
    }
    return (long)timestamp;
}

int find_file(const char *name, long target_timestamp) {
    char file_name[256];
    snprintf(file_name, sizeof(file_name), "../Programa1/Organizacao/%s.txt", name);
    
    FILE *fp = fopen(file_name, "r"); 
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo '%s': %s\n", file_name, strerror(errno));
        return 1;
    }

    int count = 0;
    char linha[1024];
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        count++;
    }
    rewind(fp);

    SensorData *data = (SensorData *)malloc(count * sizeof(SensorData));
    if (data == NULL) {
        printf("Erro ao alocar memória\n");
        fclose(fp);
        return 1;
    }

    int i = 0;
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        char *token = strtok(linha, ",");
        if (token != NULL) {
            data[i].timestamp = atol(token);
            strcpy(data[i].data, linha);
            i++;
        }
    }

    int closest_index = binary_search_closest(data, 0, count - 1, target_timestamp);
    
    if (closest_index != -1) {
        printf("Timestamp mais próximo encontrado: ");
        printf("%s", data[closest_index].data);
    } else {
        printf("Nenhum timestamp encontrado\n");
    }

    free(data);
    fclose(fp);
    return 0;
}

int binary_search_closest(SensorData *data, int left, int right, long target) {
    if (left > right) {
        return -1;
    }

    if (left == right) {
        return left;
    }

    int mid = left + (right - left) / 2;
    
    if (data[mid].timestamp == target) {
        return mid;
    }

    if (data[mid].timestamp > target) {
        if (mid == left) {
            return mid;
        }
        if (data[mid-1].timestamp < target) {
            return (target - data[mid-1].timestamp < data[mid].timestamp - target) ? mid-1 : mid;
        }
        return binary_search_closest(data, left, mid - 1, target);
    }

    if (data[mid].timestamp < target) {
        if (mid == right) {
            return mid;
        }
        if (data[mid+1].timestamp > target) {
            return (target - data[mid].timestamp < data[mid+1].timestamp - target) ? mid : mid+1;
        }
        return binary_search_closest(data, mid + 1, right, target);
    }

    return mid;
}
