#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./lib/include/json.h"
#include "./lib/include/hash.h"
#include "./lib/include/avl.h"
#include "./structs.h"
#include "./utils.h"

int isEqual(void *reg, const char *key) {
    return strcmp(get_cod(reg), key) == 0;
}

int utf8_strlen(const char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80)
            j++;
        i++;
    }
    return j;
}

void insere(void *municipio, char *key, char *value) {
    tmunicipio *m = (tmunicipio *) municipio;
    if (isEqual(key, "codigo_ibge")) {
        strcpy(m->codigo_ibge, value);
    } else if (isEqual(key, "nome")) {
        strcpy(m->nome, value);
    } else if (isEqual(key, "latitude")) {
        m->latitude = atof(value);
    } else if (isEqual(key, "longitude")) {
        m->longitude = atof(value);
    } else if (isEqual(key, "capital")) {
        m->capital = atoi(value);
    } else if (isEqual(key, "codigo_uf")) {
        m->codigo_uf = atoi(value);
    } else if (isEqual(key, "siafi_id")) {
        m->siafi_id = atoi(value);
    } else if (isEqual(key, "ddd")) {
        m->ddd = atoi(value);
    } else if (isEqual(key, "fuso_horario")) {
        strcpy(m->fuso_horario, value);
    }
}

void salva(void *reg, void *dest[]) {
    hash_insere((thash *) dest[0], reg);
    avl_insere((tarv *) dest[1], aloca_string(((tmunicipio *) reg)->nome, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[2], aloca_double(((tmunicipio *) reg)->latitude, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[3], aloca_double(((tmunicipio *) reg)->longitude, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[4], aloca_int(((tmunicipio *) reg)->codigo_uf, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[5], aloca_int(((tmunicipio *) reg)->ddd, ((tmunicipio *) reg)->codigo_ibge));    
}


void imprime_cabecalho(int nome_len, int fuso_len) {
    Coluna("%s Cód. IBGE ", COR_TITULO);
    Coluna("%s %*s ", COR_TITULO, nome_len, "Nome");
    Coluna("%s   Latitude ", COR_TITULO);
    Coluna("%s  Longitude ", COR_TITULO);
    Coluna("%s Capital ", COR_TITULO);
    Coluna("%s Cód. UF ", COR_TITULO);
    Coluna("%s Siafi ID ", COR_TITULO);
    Coluna("%s DDD ", COR_TITULO);
    Coluna("%s  %*s ", COR_TITULO, fuso_len, "Fuso Horário");
    Coluna("\n");
}

void imprime_municipio(thash hash,char *chave, int nome_len, int fuso_len) {
    tmunicipio *m = (tmunicipio *) hash_busca(hash, chave);
    Coluna(" %*s ", 9, m->codigo_ibge);
    Coluna(" %*s ", nome_len, m->nome);
    Coluna(" %*f ", 10, m->latitude);
    Coluna(" %*f ", 10, m->longitude);
    Coluna(" %*d ", 7, m->capital);
    Coluna(" %*d ", 7, m->codigo_uf);
    Coluna(" %*d ", 8, m->siafi_id);
    Coluna(" %*d ", 3, m->ddd);
    Coluna(" %*s ", fuso_len, m->fuso_horario);
    Coluna("\n");
}

void imprime_condicoes(int n_queries[5], tintervalo queries[5])
{
    Warning("Condições ativas:\n");
    for (int i = 0; i < 5; i++)
    {
        if (n_queries[i] != 0)
        {
            if (n_queries[i] == -1) {
                Error("(Desativado) ");
            }
            if (i == 0) {
                char min[200], max[200];
                string_ou_null(queries[i].reg_min, min);
                string_ou_null(queries[i].reg_max, max);
                Success("Nome: ");
                printf("%s a %s\n", min, max);
            } else if (i == 1) {
                char min[50], max[50];
                double_ou_null(queries[i].reg_min, min);
                double_ou_null(queries[i].reg_max, max);
                Success("Latitude: ");
                printf("%s a %s\n", min, max);
            } else if (i == 2) {
                char min[50], max[50];
                double_ou_null(queries[i].reg_min, min);
                double_ou_null(queries[i].reg_max, max);
                Success("Longitude: ");
                printf("%s a %s\n", min, max);
            } else if (i == 3) {
                char min[10], max[10];
                int_ou_null((tint *)queries[i].reg_min, min);
                int_ou_null((tint *)queries[i].reg_max, max);
                Success("UF: ");
                printf("%s a %s\n", min, max);
            } else if (i == 4) {
                char min[10], max[10];
                int_ou_null((tint *)queries[i].reg_min, min);
                int_ou_null((tint *)queries[i].reg_max, max);
                Success("DDD: ");
                printf("%s a %s\n", min, max);
            }
        }
    }
}

void opcoes() {
    printf("(1) Nome | ");
    printf("(2) Latitude | ");
    printf("(3) Longitude | ");
    printf("(4) UF | ");
    printf("(5) DDD\n");
}

int main(int argc, char *argv[]) {
    char *filename = "./municipios.json";
    if (argc != 1) {
        filename = argv[1];
    }

    thash hash_cod;
    tarv arv_nome, arv_latitude, arv_longitude, arv_uf, arv_ddd;

    tintervalo queries[5];
    int n_queries[5] = {0, 0, 0, 0, 0};
    /*  0 - Desativado e sem resultados
     * -1 - Desativado e com resultados
     *  1 - Ativado e com resultados
     * -2 - Ativado e sem resultados
    */

    hash_constroi(&hash_cod, 10006, get_cod);

    avl_constroi(&arv_nome, cmp_nome);
    avl_constroi(&arv_latitude, cmp_double);
    avl_constroi(&arv_longitude, cmp_double);
    avl_constroi(&arv_uf, cmp_int);
    avl_constroi(&arv_ddd, cmp_int);

    FILE *fmunicipios;

    fmunicipios = fopen(filename, "r");

    if (fmunicipios == NULL) {
        printf("%sErro! Não foi possível abrir o arquivo %s\n", COR_ERRO, filename);
        return EXIT_FAILURE;
    }

    void *dest[] = {&hash_cod, &arv_nome, &arv_latitude, &arv_longitude, &arv_uf, &arv_ddd};
    char *keys[] = {"codigo_ibge", "nome", "latitude", "longitude", "capital", "codigo_uf", "siafi_id", "ddd", "fuso_horario"};
    int r = json_parse(fmunicipios, keys, 9, aloca_municipio, insere, salva, dest);

    fclose(fmunicipios);
    if (r == EXIT_FAILURE) {
        printf("Erro ao fazer o parse do arquivo %s\n", filename);
        return EXIT_FAILURE;
    }
    
    while(1) {
        printf("--------------------------------\n");
        imprime_condicoes(n_queries, queries);
        printf("--------------------------------\n");
        printf("1 - Imprimir resultados\n");
        printf("2 - Alterar condição\n");
        printf("3 - Ativar/Desativar condição\n");
        printf("0 - Sair\n");
        printf("--------------------------------\n");
        
        int i;
        printf("Digite a opção: ");
        scanf("%d", &i);

        if (i == 0)
            break;

        switch (i) {
            case 1: {
                tlista *listas[5];
                int n = 0;
                for (int i = 0; i < 5; i++) {
                    if (n_queries[i] == 1) {
                        listas[n] = queries[i].lista;
                        n++;
                    }
                }

                if (n == 0) {
                    Error("Nenhuma condição ativa\n");
                    break;
                }

                tlista *listaR = lista_interseccao(cmp_codigo_ibge, listas, n);

                if (listaR == NULL) {
                    Error(NAO_ENCONTRADO);
                } else {
                    int nome_len = 0;
                    int fuso_len = 0;

                    tlista *aux2 = listaR;
                    while (aux2 != NULL) {
                        tmunicipio *m = (tmunicipio *) hash_busca(hash_cod, ((tint *) aux2->reg)->codigo_ibge);
                        if (strlen(m->nome) > nome_len) {
                            nome_len = strlen(m->nome);
                        }
                        if (strlen(m->fuso_horario) > fuso_len) {
                            fuso_len = strlen(m->fuso_horario);
                        }
                        aux2 = aux2->prox;
                    }

                    imprime_cabecalho(nome_len, fuso_len);

                    tlista *aux = listaR;
                    while (aux != NULL) {
                        tmunicipio *m = (tmunicipio *) hash_busca(hash_cod, ((tint *) aux->reg)->codigo_ibge);
                        int nome_diff = strlen(m->nome) - utf8_strlen(m->nome);
                        int fuso_diff = strlen(m->fuso_horario) - utf8_strlen(m->fuso_horario);

                        imprime_municipio(hash_cod, ((tstring *) aux->reg)->codigo_ibge, nome_len + nome_diff, fuso_len + fuso_diff);
                        aux = aux->prox;
                    }

                    lista_destroi(listaR);
                }

                break;
            }
            case 2:
                opcoes();
                int j;
                scanf("%d", &j);

                if (j < 1 || j > 5) {
                    Error("Opção inválida\n");
                    break;
                }

                if (j == 1) {
                    tstring *s1, *s2;
                    le_string(&s1, &s2, &queries[0], "Nome", &arv_nome);
                    n_queries[0] = queries[0].lista == NULL ? 0 : 1;
                } else if (j == 2) {
                    tdouble *d1, *d2;
                    le_double(&d1, &d2, &queries[1], "Latitude", &arv_latitude);
                    n_queries[1] = queries[1].lista == NULL ? 0 : 1;
                } else if (j == 3) {
                    tdouble *d1, *d2;
                    le_double(&d1, &d2, &queries[2], "Longitude", &arv_longitude);
                    n_queries[2] = queries[2].lista == NULL ? 0 : 1;
                } else if (j == 4) {
                    tint *i1, *i2;
                    le_int(&i1, &i2, &queries[3], "UF", &arv_uf);
                    n_queries[3] = queries[3].lista == NULL ? 0 : 1;
                } else if (j == 5) {
                    tint *i1, *i2;
                    le_int(&i1, &i2, &queries[4], "DDD", &arv_ddd);
                    n_queries[4] = queries[4].lista == NULL ? 0 : 1;
                }
                break;

            case 3:
                opcoes();
                int k;
                scanf("%d", &k);
                if (k < 1 || k > 5) {
                    Error("Opção inválida\n");
                    break;
                }

                if (n_queries[k - 1] == 1) {
                    n_queries[k - 1] = -1;
                } else if (n_queries[k - 1] == -1) {
                    n_queries[k - 1] = 1;
                }
                break;
        }
    }

    hash_apaga(&hash_cod);

    return EXIT_SUCCESS;
}