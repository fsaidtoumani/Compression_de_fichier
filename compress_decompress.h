#include <stdio.h>
#include "codage_decodage_char.h"
void compresser_fichier(const char* input_filename, const char* output_filename, shannon_fano_code* codes);
int decompresser_fichier(const char* input_filename, const char* output_filename);
bool fichiers_identiques(const char* filename1, const char* filename2);