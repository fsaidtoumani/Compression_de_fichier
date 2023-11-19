#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shannon_fano.h"
#include "compress_decompress.h"
#define MAX_CHAR 256

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++ MAIN +++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char *argv[]) {
    if (argc < 3)
    {
        printf("Erreur : Utilisation: [-c] input_filename.txt | [-d] input_compressed_filename.fiano | [-v] file1.txt file2.txt\n");
        return -1;
    }

    const char *input_filename;
    const char *output_filename;
    const char *file1, *file2;

    switch (argv[1][0]) {
        case '-':
            if (argv[1][1] == 'c') {
                // Code de compression
                input_filename = argv[2];
                int occurrences[MAX_CHAR] = {0}, nbcar = 0;
                float probabilites[MAX_CHAR] = {0}, information[MAX_CHAR] = {0};
                
                if (count_char_occurrences(input_filename, occurrences) == -1) {
                    printf("Erreur de l'ouverture du fichier \n");
                    return -1;
                }
                estimate_char_probabilites(occurrences, probabilites);
                estimate_char_information(probabilites, information);
                printf("Entropie: %f\n", calculer_entropie(occurrences, probabilites, information));

                for (int i = 0; i < MAX_CHAR; i++) {
                    if (occurrences[i] > 0) {
                        nbcar += occurrences[i];
                    }
                }

                shannon_fano_code *codes = generer_shannon_fano_codes(probabilites);
                codes->nb_occurrences = nbcar;

                
                output_filename = "compressed.fiano";
                printf("Compression du fichier %s vers le fichier %s\n", input_filename, output_filename);
                compresser_fichier(input_filename, output_filename, codes);
                liberer_codes_shannon_fano(codes);
                
            } else if (argv[1][1] == 'd') {
                // Code de décompression
                input_filename = argv[2];
                output_filename = "decompressed_file.txt";
                
                printf("Decompression du fichier %s vers le fichier %s\n", input_filename, output_filename);
                decompresser_fichier(input_filename, output_filename);
                
            } else if (argv[1][1] == 'v') {
                if (argc < 4) {
                    printf("Erreur : Utilisation incorrecte pour l'option [-v]. Utilisation: [-v] file1.txt file2.txt\n");
                    return -1;
                }
                // Code de vérification
                file1 = argv[2];
                file2 = argv[3];

                if (fichiers_identiques(file1, file2)) {
                    printf("Les fichiers %s et %s sont identiques.\n", file1, file2);
                } else {
                    printf("Les fichiers %s et %s ne sont pas identiques.\n", file1, file2);
                }
            } else {
                printf("Erreur : Option inconnue.\n");
                return -1;
            }
            break;
        default:
            printf("Erreur : Option invalide. Utilisation: [-c] input_filename.txt | [-d] input_compressed_filename.fiano | [-v] file1.txt file2.txt\n");
            return -1;
    }

    return 0;
}