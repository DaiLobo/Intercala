#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

int compara(const void *e1, const void *e2)
{
    return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

void criarArq(){

    FILE *f;
	FILE *new_file;
	Endereco e;
	char dir[15] = "cep_arq";
	char ext[5] = ".dat";
	char nome_arq[15];
	f = fopen("cep.dat", "r");

	int i;
	for(i = 0 ; i < 8; i++){ /*vai criar 8 arquivos*/
		snprintf(nome_arq, 15, "%s%d%s", dir, i, ext); /*para dar o nome ao arquivo*/
		new_file = fopen(nome_arq, "w");

		int j;
		for(j = 0; j < 10; j++){ /*contem 10 registros cada arquivo*/
			fread(&e, sizeof(Endereco), 1, f);
			fwrite(&e, sizeof(Endereco), 1, new_file);
		}
		fclose(new_file);
	}
	printf("\nArquivos criados!!\n");
	fclose(f);
}


void ordena(){

    FILE *f;
    Endereco *e;
    long posicao, qtd;


    char dir[15] = "cep_arq";
	char ext[5] = ".dat";
	char nome_arq[15];
	char nome_arq2[15];
	char arq_intercalado[15];

    int i;

	for( i = 0; i < 8; i++ ){ /*vai ORDENAR 8 arquivos*/

		snprintf(nome_arq, 15, "%s%d%s", dir, i, ext); /*para dar o nome ao arquivo*/
		f = fopen(nome_arq, "rb");
		fseek(f,0,SEEK_END);
        posicao = ftell(f);
        qtd = posicao/sizeof(Endereco);
        e = (Endereco*) malloc(posicao);
        rewind(f);

        if(fread(e,sizeof(Endereco),qtd,f) == qtd)
        {
            printf("Lido = OK\n");
        }
        fclose(f);

        qsort(e,qtd,sizeof(Endereco),compara);
        printf("Ordenado = OK\n");
        f = fopen(nome_arq,"w");
        fwrite(e,sizeof(Endereco),qtd,f);
        fclose(f);
        printf("Escrito = OK\n");
        free(e);
	}
}


void intercala(){

    char dir[15] = "cep_arq";
	char ext[5] = ".dat";
	char nome_arq[15];
	char nome_arq2[15];
	char arq_intercalado[15];

    int prox = 0;
	int n = 8;

	FILE *a, *b, *saida;
    Endereco ea, eb;

	while(prox+1 < n) {

        snprintf(nome_arq, 15, "%s%d%s", dir, prox, ext);
        snprintf(nome_arq2, 15, "%s%d%s", dir, prox+1, ext);
        snprintf(arq_intercalado, 15, "%s%d%s", dir, n, ext);

        a = fopen(nome_arq,"rb");
        b = fopen(nome_arq2,"rb");
        saida = fopen(arq_intercalado,"wb");

        fread(&ea,sizeof(Endereco),1,a);
        fread(&eb,sizeof(Endereco),1,b);

        while(!feof(a) && !feof(b))
        {
            if(compara(&ea,&eb)<0)
            {
                fwrite(&ea,sizeof(Endereco),1,saida);
                fread(&ea,sizeof(Endereco),1,a);
            }
            else
            {
                fwrite(&eb,sizeof(Endereco),1,saida);
                fread(&eb,sizeof(Endereco),1,b);
            }
        }

        while(!feof(a))
        {
            fwrite(&ea,sizeof(Endereco),1,saida);
            fread(&ea,sizeof(Endereco),1,a);
        }
        while(!feof(b))
        {
            fwrite(&eb,sizeof(Endereco),1,saida);
            fread(&eb,sizeof(Endereco),1,b);
        }

        prox = prox + 2;
        n++;


        fclose(a);
        fclose(b);
        fclose(saida);

	}

	printf("Intercalado!\n");

	rename("cep_arq14.dat", "intercalado.dat");

}

void remocao(){

	char dir[15] = "cep_arq";
	char ext[5] = ".dat";
	char nome_arq[15];

    int i;
    for (i = 0; i <= 14; i++){

       snprintf(nome_arq, 15, "%s%d%s", dir, i, ext);
       remove(nome_arq);

    }

    printf("Removido excesso de arquivos!\n");

}


int main(int argc, char**argv)
{
    criarArq();
    ordena();
    intercala();
    remocao();

    return 0;
}
