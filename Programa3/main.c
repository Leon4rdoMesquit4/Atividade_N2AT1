#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define NUM_SAMPLES 2000
#define MAX_SENSORS 50
#define MAX_NAME 32
#define MAX_LINE_LENGTH 1024

typedef struct {
    char name[MAX_NAME];
    char type[MAX_NAME];
} Sensor;

time_t parse_datetime(const char *datetime_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(datetime_str, "%Y-%m-%d %H:%M:%S", &tm);
    return mktime(&tm);
}

time_t random_time(time_t start, time_t end) {
    return start + rand() % (end - start + 1);
}

void generate_value(char *type, char *buffer) {
    if (strcmp(type, "int") == 0) {
        sprintf(buffer, "%d", rand() % 1000);
    } else if (strcmp(type, "float") == 0) {
        sprintf(buffer, "%.2f", ((float)rand() / RAND_MAX) * 100.0);
    } else if (strcmp(type, "double") == 0) {
        sprintf(buffer, "%.5lf", ((double)rand() / RAND_MAX) * 100.0);
    } else if (strcmp(type, "bool") == 0) {
        sprintf(buffer, "%s", (rand() % 2) ? "true" : "false");
    } else {
        fprintf(stderr, "Unknown data type: %s\n", type);
        exit(EXIT_FAILURE);
    }
}

void shuffle_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file for shuffling");
        return;
    }

    char **lines = NULL;
    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lines = realloc(lines, (count + 1) * sizeof(char *));
        lines[count] = strdup(line);
        count++;
    }
    fclose(fp);

    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = lines[i];
        lines[i] = lines[j];
        lines[j] = temp;
    }

    fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening file for writing");
        for (int i = 0; i < count; i++) {
            free(lines[i]);
        }
        free(lines);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s", lines[i]);
        free(lines[i]);
    }
    free(lines);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <start> <end> <SENSOR:TYPE> ...\n", argv[0]);
        return 1;
    }

    time_t start = parse_datetime(argv[1]);
    time_t end = parse_datetime(argv[2]);

    if (end <= start) {
        fprintf(stderr, "Error: End must be after start.\n");
        return 1;
    }

    int num_sensors = argc - 3;
    Sensor sensors[MAX_SENSORS];

    for (int i = 0; i < num_sensors; i++) {
        char *token = strtok(argv[i + 3], ":");
        if (token == NULL) {
            fprintf(stderr, "Error in sensor format: %s\n", argv[i + 3]);
            return 1;
        }
        strncpy(sensors[i].name, token, MAX_NAME);

        token = strtok(NULL, ":");
        if (token == NULL) {
            fprintf(stderr, "Error in sensor type: %s\n", argv[i + 3]);
            return 1;
        }
        strncpy(sensors[i].type, token, MAX_NAME);
    }

    srand(time(NULL));

    FILE *fp = fopen("../Arquivos_teste/amostras.txt", "w");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < num_sensors; i++) {
        for (int j = 0; j < NUM_SAMPLES; j++) {
            time_t ts = random_time(start, end);
            char value[64];
            generate_value(sensors[i].type, value);
            fprintf(fp, "%ld %s %s\n", ts, sensors[i].name, value);
        }
    }

    fclose(fp);
    shuffle_file("../Arquivos_teste/amostras.txt");
    printf("File 'Arquivos_teste/amostras.txt' generated and shuffled successfully.\n");
    return 0;
}