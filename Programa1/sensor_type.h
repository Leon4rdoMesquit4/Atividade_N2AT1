#ifndef SENSOR_H
#define SENSOR_H

#define INITIAL_CAPACITY 100
#define MAX_SENSOR_TYPES 10

typedef struct {
    long timestamp;
    char sensor_id[50];
    char value[50];
} SensorData;

typedef enum {
    SENSOR_TEMP,
    SENSOR_PRES,
    SENSOR_VIBR,
    SENSOR_ALRM,
    SENSOR_UMID,
    SENSOR_UNKNOWN
} SensorType;

typedef struct {
    SensorType type;
    SensorData *entries;
    int count;
    int capacity;
} SensorTypeData;

SensorType get_sensor_type(const char *sensor_id);
const char *sensor_type_to_string(SensorType type);
int compare_by_timestamp(const void *a, const void *b);
int find_or_create_sensor_type(SensorTypeData *data_array, int *type_count, SensorType type);

#endif