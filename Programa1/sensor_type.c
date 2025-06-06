#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sensor_type.h"

SensorType get_sensor_type(const char *sensor_id) {
    if (strncmp(sensor_id, "TEMP", 4) == 0) return SENSOR_TEMP;
    if (strncmp(sensor_id, "PRES", 4) == 0) return SENSOR_PRES;
    if (strncmp(sensor_id, "VIBR", 4) == 0) return SENSOR_VIBR;
    if (strncmp(sensor_id, "UMID", 4) == 0) return SENSOR_UMID;
    if (strncmp(sensor_id, "ALRM", 4) == 0) return SENSOR_ALRM;
    return SENSOR_UNKNOWN;
}

const char *sensor_type_to_string(SensorType type) {
    switch (type) {
        case SENSOR_TEMP: return "TEMP";
        case SENSOR_PRES: return "PRES";
        case SENSOR_VIBR: return "VIBR";
        case SENSOR_UMID: return "UMID";
        case SENSOR_ALRM: return "ALRM";
        default: return "UNKNOWN";
    }
}

int compare_by_timestamp(const void *a, const void *b) {
    const SensorData *sa = (const SensorData *)a;
    const SensorData *sb = (const SensorData *)b;
    return (sa->timestamp > sb->timestamp) - (sa->timestamp < sb->timestamp);
}

int find_or_create_sensor_type(SensorTypeData *data_array, int *type_count, SensorType type) {
    for (int i = 0; i < *type_count; i++) {
        if (data_array[i].type == type)
            return i;
    }

    if (*type_count >= MAX_SENSOR_TYPES) {
        fprintf(stderr, "Erro: número máximo de tipos de sensor excedido.\n");
        exit(1);
    }

    data_array[*type_count].type = type;
    data_array[*type_count].count = 0;
    data_array[*type_count].capacity = INITIAL_CAPACITY;
    data_array[*type_count].entries = malloc(INITIAL_CAPACITY * sizeof(SensorData));

    if (!data_array[*type_count].entries) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    return (*type_count)++;
}
