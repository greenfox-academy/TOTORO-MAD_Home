#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "functions.h"

#define NUMBER_OF_FILES         16
#define MULTIPLIER              1000
#define EDGE_CORRECTION         40
#define NUMBERS_IN_A_ROW        36
#define VOLT_DIFF_THRESHOLD     1.5
#define BUFFER_SIZE             50
#define NUMBER_OF_DATA          268
#deifne MIN_TEMPERATURE         16


int main()
{
    FILE* source_files[NUMBER_OF_FILES];
    FILE* text_files[NUMBER_OF_FILES];
    FILE* bin_codes;

    char source[NUMBER_OF_FILES][10] = {"16.csv", "17.csv", "18.csv", "19.csv", "20.csv",
                                        "21.csv", "22.csv", "23.csv", "24.csv", "25.csv",
                                        "26.csv", "27.csv", "28.csv", "29.csv", "30.csv",
                                        "onoff.csv"};

    char texts[NUMBER_OF_FILES][10] = {"16.txt", "17.txt", "18.txt", "19.txt", "20.txt",
                                       "21.txt", "22.txt", "23.txt", "24.txt", "25.txt",
                                       "26.txt", "27.txt", "28.txt", "29.txt", "30.txt",
                                       "onoff.txt"};

    char string_buffer[BUFFER_SIZE];
    float time_1;
    float time_2;
    float voltage_1;
    float voltage_2;
    uint16_t difference;
    uint8_t new_line = 0;
    uint16_t data_counter;
    uint8_t high_or_low = 0;

// open the files
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        if ((source_files[i] = fopen(source[i], "r")) != NULL)
            printf("file %s opened\n", source[i]);
        if ((text_files[i] = fopen(texts[i], "w")) != NULL)
            printf("file %s opened\n", texts[i]);
    }

    bin_codes = fopen("binary_codes.txt", "w");

// text filling algorithm
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        data_counter = 0;
        if (i < 15)
            fprintf(text_files[i], "uint16_t degree_%d[] = {", i + MIN_TEMPERATURE);
        else
            fprintf(text_files[i], "uint16_t onoff[] = {");

        fprintf(bin_codes, "\n%d\t", i + NUMBER_OF_FILES);

        while (fgets(string_buffer, BUFFER_SIZE, source_files[i])) {

            time_1 = atof(strtok (string_buffer, ","));
            voltage_1 = atof(strtok (NULL, "\n"));

            if (time_1 > 0 ) {
                while (1) {
                    fgets(string_buffer, BUFFER_SIZE, source_files[i]);
                    time_2 = atof(strtok (string_buffer, ","));
                    voltage_2 = atof(strtok (NULL, "\n"));

                    if (abs(voltage_2 - voltage_1) > VOLT_DIFF_THRESHOLD) {
                        difference = (time_2 - time_1) * MULTIPLIER + EDGE_CORRECTION;

                        if (high_or_low == 1)
                            fprintf(bin_codes, "%d", filling_binary(difference));

                        if (data_counter != NUMBER_OF_DATA - 1 && high_or_low == 1)
                            fprintf(text_files[i], "%d, ", filling_binary(difference));
                        else if (high_or_low == 1)
                            fprintf(text_files[i], "%d};", filling_binary(difference));

                        new_line++;
                        data_counter++;
                        high_or_low = !high_or_low;

                        if (new_line == NUMBERS_IN_A_ROW) {
                            fprintf(text_files[i], "\n");
                            new_line = 0;
                        }

                        break;
                    }//if
                }//while
            }//if
        }//while
    }//for

// close the files
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        fclose(source_files[i]);
        fclose(text_files[i]);
    }
    fclose(bin_codes);

    return 0;
}
