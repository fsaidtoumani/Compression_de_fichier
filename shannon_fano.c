#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shannon_fano.h"
#include <math.h>
#define MAX_CHAR 256

// Fonction pour calculer le nombre d'occurrences de chaque caractère
int count_char_occurrences(const char* filename, int* occurrences) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return -1;
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        occurrences[c]++;
    }

    fclose(file);
    return 0;
}

// Fonction pour estimer la probabilite d'apparition de chaque caractère
int estimate_char_probabilites(int* occurrences, float* probabilites) 
{
    int total_chars = 0;
    for (int i = 0; i < MAX_CHAR; i++) 
    {
        total_chars += occurrences[i];
    }

    for (int i = 0; i < MAX_CHAR; i++) 
    {
        probabilites[i] = (float)occurrences[i] / (float)total_chars;
    }

    return 0;
}

// Fonction pour estimer la quantite d'information contenue dans chaque caractère
float* estimate_char_information(float* probabilites, float* information) 
{
    for (int i = 0; i < MAX_CHAR; i++) 
    {
        if (probabilites[i] > 0.0) 
        {
            information[i] = -(log2f(probabilites[i]));
        } else {
            information[i] = 0.0;
        }
    }

    return information;
}

float calculer_entropie(int *occurrences, float *probabilites, float *information) {

    float entropie = 0.0;
    for (int i = 0; i < MAX_CHAR; i++) {
        entropie += probabilites[i] * information[i];
    }

    return entropie;
}

//Fonction pour afficher les codes de Shannon-Fano final

int afficher_codes(shannon_fano_code* codes,const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return -1;
    }
    int c;

    while ((c = fgetc(file)) != EOF) //Affichage des caractères du fichier
    {
        printf("%c",c);
    }
    fseek(file, 0, SEEK_SET);

    printf("\n");

    while ((c = fgetc(file)) != EOF) //Affichage des codes de Shannon-Fano du fichier
    {
        for (int i = 0; i < codes->len_valid_caract; i++) {
            if (codes[i].character == c)
                printf("%s",codes[i].code);
        }
    }
    printf("\n");

    fclose(file);
    return 0;
}

void invers(float *a, float *b,char *c,char *d) {
	float temp = *a;
    char temp2=*c;
	*a = *b;
    *c=*d;
	*b = temp;
    *d=temp2;
}


void trie_selection_decroiss(float *tableau, int taille,shannon_fano_code* codes) {
	for (int i = 0; i < taille - 1; i++) {
		int max_idx = i;
		for (int j = i + 1; j < taille; j++) {
			if (tableau[j] > tableau[max_idx]) {
				max_idx = j;
			}
		}
		invers(&tableau[i], &tableau[max_idx],&codes[i].character,&codes[max_idx].character);
	}
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


void build_shannon_fano_codes(float* probabilities, shannon_fano_code* codes, int start, int end) {
    if (start >= end) {
        return;
    }


    float total_prob = 0.0;
    for (int i = start; i <= end; i++) {
        total_prob += probabilities[i];
    }

    float half_prob = 0 , compt_moitier_b=0;
    int split_index = start;
    while (compt_moitier_b < total_prob / 2) {
        half_prob += probabilities[split_index++];
        compt_moitier_b=half_prob;
        compt_moitier_b+=probabilities[split_index];
    }

    for (int i = start; i < split_index; i++) {
        strcat(codes[i].code, "0");
        codes[i].len_code++;
    }

    for (int i = split_index; i <= end; i++) {
        strcat(codes[i].code, "1");
        codes[i].len_code++;
    }

    build_shannon_fano_codes(probabilities, codes, start, split_index - 1);
    build_shannon_fano_codes(probabilities, codes, split_index, end);
}

shannon_fano_code* generer_shannon_fano_codes(float* probabilities) {
    int valid_char_count = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (probabilities[i] > 0.0) {
            valid_char_count++;
        }
    }

    float valid_probabilities[valid_char_count];
    shannon_fano_code* codes = malloc(valid_char_count * sizeof(shannon_fano_code));
    int index = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (probabilities[i] > 0) {
            valid_probabilities[index] = probabilities[i];
            codes[index].character = i;
            codes[index].code = malloc(1);
            codes[index].code[0] = '\0';
            codes[index].len_code=0;
            index++;
        }
    }
	 codes->len_valid_caract=index;
    trie_selection_decroiss(valid_probabilities, valid_char_count,codes);
    build_shannon_fano_codes(valid_probabilities, codes, 0, codes->len_valid_caract - 1);

    return codes;
}

// Fonction pour trouver le code Shannon-Fano d'un caractère
shannon_fano_code* trouver_code_shannon_fano(shannon_fano_code* codes, char c) {
    for (int i = 0; i < codes->len_valid_caract; i++) {
        if (codes[i].character == c) {
            return &codes[i];
        }
    }
    return NULL;
}

// Fonction pour liberer la memoire allouee pour les codes Shannon-Fano
void liberer_codes_shannon_fano(shannon_fano_code* codes) {
    for (int i = 0; i < codes->len_valid_caract; i++) {
        free(codes[i].code);
    }
    free(codes);
}
