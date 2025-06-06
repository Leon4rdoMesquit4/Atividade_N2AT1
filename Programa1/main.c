/*
    Programa1 - main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  
#include <sys/types.h>  
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include "sensor_type.c"

#define MAX_LINE 256

void remove_dir(const char *path);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    struct stat st = {0};
    if (stat("Organizacao", &st) == 0) {
        remove_dir("Organizacao");
    }
    if (mkdir("Organizacao", 0755) != 0) {
        perror("Erro ao criar pasta Organizacao");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    SensorTypeData sensor_data[MAX_SENSOR_TYPES];
    int type_count = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp)) {
        SensorData entry;
        if (sscanf(line, "%ld %s %s", &entry.timestamp, entry.sensor_id, entry.value) != 3)
            continue;

        SensorType type = get_sensor_type(entry.sensor_id);
        if (type == SENSOR_UNKNOWN)
            continue;

        int index = find_or_create_sensor_type(sensor_data, &type_count, type);

        SensorTypeData *data = &sensor_data[index];

        if (data->count >= data->capacity) {
            data->capacity *= 2;
            data->entries = realloc(data->entries, data->capacity * sizeof(SensorData));
            if (!data->entries) {
                perror("Erro ao realocar memória");
                exit(1);
            }
        }

        data->entries[data->count++] = entry;
    }

    fclose(fp);

    for (int i = 0; i < type_count; i++) {
        SensorTypeData *data = &sensor_data[i];
        qsort(data->entries, data->count, sizeof(SensorData), compare_by_timestamp);

        char filename[100];
        snprintf(filename, sizeof(filename), "Organizacao/%s.txt", sensor_type_to_string(data->type));
        FILE *out = fopen(filename, "w");
        if (!out) {
            perror("Erro ao criar arquivo");
            continue;
        }

        for (int j = 0; j < data->count; j++) {
            fprintf(out, "%ld %s %s\n", data->entries[j].timestamp,
                    data->entries[j].sensor_id, data->entries[j].value);
        }

        fclose(out);
        free(data->entries);
    }

    return 0;
}

void remove_dir(const char *path) {
    DIR *d = opendir(path);
    if (!d) return;
    struct dirent *dir;
    char filepath[512];
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        snprintf(filepath, sizeof(filepath), "%s/%s", path, dir->d_name);
        remove(filepath);
    }
    closedir(d);
    rmdir(path);
}