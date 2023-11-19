#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shannon_fano.h"
#include <stdbool.h>
#include "compress_decompress.h"
#include "codage_decodage_char.h"

// Fonction pour compresser le fichier
void compresser_fichier(const char* input_filename, const char* output_filename, shannon_fano_code* codes) {
    FILE* input_file = fopen(input_filename, "r");
    FILE* output_file = fopen(output_filename, "w");

    char current_char;
    char buffer[8] = {0};
    int buffer_index = 0;

    // ecrire le nombre de caractères valides
    fwrite(&codes->len_valid_caract, sizeof(int), 1, output_file);
    
    // ecrire le nombre de caractères du fichier
    fwrite(&codes->nb_occurrences, sizeof(int), 1, output_file);

    // ecrire les caractères et leurs codes Shannon-Fano correspondants
    for (int i = 0; i < codes->len_valid_caract; i++) {
        fwrite(&codes[i].character, sizeof(char), 1, output_file);
        fwrite(&codes[i].len_code, sizeof(int), 1, output_file);
        fwrite(codes[i].code, sizeof(char), codes[i].len_code, output_file);
    }
    
    while ((current_char = fgetc(input_file)) != EOF) {
        shannon_fano_code* code = trouver_code_shannon_fano(codes, current_char);
        if (code != NULL) {
            for (int i = 0; i < strlen(code->code); i++) {
                buffer[buffer_index++] = code->code[i];
                if (buffer_index == 8) {
                    char output_char = code_binaire_en_char(buffer);
                    fwrite(&output_char, 1, 1, output_file);
                    buffer_index = 0;
                }
            }
        }
    }

    // ecrire les bits restants dans le tampon
    if (buffer_index > 0) {
        while (buffer_index < 8) {
            buffer[buffer_index++] = '0';
        }
        char output_char = code_binaire_en_char(buffer);
        fwrite(&output_char, 1, 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}




int decompresser_fichier(const char* input_filename, const char* output_filename) {
    FILE* output_file = fopen(output_filename, "w");
    FILE* input_file = fopen(input_filename, "rb");
    if (!input_file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", input_filename);
        return -1;
    }

    int valid_char_count, nb_occurrences;
    fread(&valid_char_count, sizeof(int), 1, input_file);
    fread(&nb_occurrences, sizeof(int), 1, input_file);

    shannon_fano_code* codes = malloc(valid_char_count * sizeof(shannon_fano_code));
    codes->len_valid_caract = valid_char_count;
    codes->nb_occurrences = nb_occurrences;
    for (int i = 0; i < valid_char_count; i++) {
        fread(&codes[i].character, sizeof(char), 1, input_file);
        fread(&codes[i].len_code, sizeof(int), 1, input_file);
        codes[i].code = malloc(codes[i].len_code + 1);
        fread(codes[i].code, sizeof(char), codes[i].len_code, input_file);
        codes[i].code[codes[i].len_code] = '\0';
    }

    int max_code_length = 0;
    for (int i = 0; i < codes->len_valid_caract; i++) {
        if (codes[i].len_code > max_code_length) {
            max_code_length = codes[i].len_code;
        }
    }
    
    char buffer[max_code_length];
    for (int i = 0; i < max_code_length; i++) buffer[i] = '\0';

    int cpt_carract = 0, buffer_index = 0;
    
    char current_char;

    while ( cpt_carract < nb_occurrences) {
    char binary_code[9],current_char = fgetc(input_file);
    char *temp_code_binaire = char_en_code_binaire(current_char);
    strcpy(binary_code, temp_code_binaire);

    for (int i = 0; i < 8; i++) {
        // Ajout du nouveau bit a la fin du buffer
        buffer[buffer_index++] = binary_code[i];

        // Recherche du code correspondant dans le tableau de codes Shannon-Fano
        for (int j = 0; j < codes->len_valid_caract; j++) {
            if (strncmp(buffer, codes[j].code, codes[j].len_code) == 0 && cpt_carract < nb_occurrences) {
                fputc(codes[j].character, output_file);
                cpt_carract++;

                // Suppression du code trouve en decalant les bits restants vers la gauche
                int remaining_bits = buffer_index - codes[j].len_code;
                memmove(buffer, buffer + codes[j].len_code, remaining_bits);
                buffer_index = remaining_bits;

                // S'assurer que le reste du buffer contient des antislash zeros
                for (int k = buffer_index; k < max_code_length; k++) {
                    buffer[k] = '\0';
                }
                break;
            }
        }
    }

    free(temp_code_binaire);
    }


    liberer_codes_shannon_fano(codes);
    fclose(input_file);
    fclose(output_file);
}

//fonction pour verifier si deux fichiers sont identiques
bool fichiers_identiques(const char* filename1, const char* filename2) {
    FILE* file1 = fopen(filename1, "r");
    FILE* file2 = fopen(filename2, "r");

    if (!file1 || !file2) {
        printf("Erreur lors de l'ouverture des fichiers %s et/ou %s\n", filename1, filename2);
        return false;
    }

    int c1, c2;
    while (c1 != EOF && c2 != EOF) {
        c1 = fgetc(file1);
        c2 = fgetc(file2);

        if (c1 != c2) {
            fclose(file1);
            fclose(file2);
            return false;
        }
    }

    fclose(file1);
    fclose(file2);
    return true;
}