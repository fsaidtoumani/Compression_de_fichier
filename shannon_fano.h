#include <stdio.h>
typedef struct {
    char character;
    char* code;
    int len_valid_caract;
    int len_code;
    int nb_occurrences;
} shannon_fano_code;

int count_char_occurrences(const char* filename, int* occurrences);
int estimate_char_probabilites(int* occurrences, float* probabilites);
float* estimate_char_information(float* probabilites, float* information) ;
float calculer_entropie(int *occurrences, float *probabilites, float *information);
void trie_selection_decroiss(float *tableau, int taille,shannon_fano_code* codes);
void invers(float *a, float *b,char *c,char *d);
void build_shannon_fano_codes(float* probabilities, shannon_fano_code* codes, int start, int end);
shannon_fano_code* generer_shannon_fano_codes(float* probabilities);
shannon_fano_code* trouver_code_shannon_fano(shannon_fano_code* codes, char c);
void afficher_codes_shannon_fano(shannon_fano_code* codes, int nb_codes);
void liberer_codes_shannon_fano(shannon_fano_code* codes);
