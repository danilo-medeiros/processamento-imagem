#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;

const int TAMANHO_MAXIMO_IMAGEM = 512;
const int TAMANHO_MAXIMO_NOME_ARQUIVO = 30;
const int QTD_CANAIS = 3;
const char PASTA_ENTRADA[10] = "imagens/";
const char PASTA_SAIDA[10] = "out/";
const char EXTENSAO[10] = ".ppm";

struct Pixel {
  int red;
  int green;
  int blue;
};

struct Img {
  int qtd_linhas;
  int qtd_colunas;
  Pixel matriz[TAMANHO_MAXIMO_IMAGEM][TAMANHO_MAXIMO_IMAGEM];
};

void abre_img(char nome[], Img& img);

void salva_img(char nome[], Img img);

void menu_principal();

int main() {
  menu_principal();
  /* char nome_arquivo[TAMANHO_MAXIMO_NOME_ARQUIVO];
  Img imagem;
  abre_img(nome_arquivo, imagem);
  salva_img(nome_arquivo, imagem); */
  return 0;
}

void abre_img(char nome[], Img& img) {
  fstream arquivo;
  char nome_arquivo[20];
  strcpy(nome_arquivo, PASTA_ENTRADA);
  cout << "Nome do arquivo (deve estar dentro de imagens/ e com extensao .ppm):\n";
  cin >> nome;
  strcat(nome_arquivo, nome);
  strcat(nome_arquivo, EXTENSAO);
  arquivo.open(nome_arquivo);

  if (!arquivo.is_open()) {
    cout << "Não foi possível abrir o arquivo\n";
    return;
  }

  string tipo_arquivo;
  getline(arquivo, tipo_arquivo);

  if (tipo_arquivo != "P3") {
    cout << "Arquivo invalido. Deve ser um arquivo .ppm\n";
    return;
  }

  arquivo >> img.qtd_colunas >> img.qtd_linhas;
  cout << "Tamanho do arquivo: " << img.qtd_colunas << " x " << img.qtd_linhas << endl;

  int maximo;
  arquivo.ignore();
  arquivo >> maximo;
  cout << maximo << endl;
  
  for (int i = 0; i < img.qtd_linhas; i++) {
    arquivo.ignore();
    for (int j = 0; j < img.qtd_colunas; j++) {
      Pixel pixel;
      arquivo >> pixel.red;
      arquivo >> pixel.green;
      arquivo >> pixel.blue;
      img.matriz[i][j] = pixel;
    }
  }
  arquivo.close();
}

void salva_img(char nome[], Img img) {
  ofstream arquivo_saida;

  char nome_saida[TAMANHO_MAXIMO_NOME_ARQUIVO];
  strcpy(nome_saida, PASTA_SAIDA);
  strcat(nome_saida, nome);
  strcat(nome_saida, EXTENSAO);

  arquivo_saida.open(nome_saida);
  arquivo_saida << "P3\n";
  arquivo_saida << img.qtd_colunas << " " << img.qtd_linhas << "\n";
  arquivo_saida << "255\n";

  for (int i = 0; i < img.qtd_linhas; i++) {
    for (int j = 0; j < img.qtd_colunas; j++) {
      arquivo_saida << img.matriz[i][j].red << " " << img.matriz[i][j].green << " " << img.matriz[i][j].blue << " ";
    }
    arquivo_saida << "\n";
  }
  cout << "Nome do arquivo de saida: " << nome_saida << endl;
  arquivo_saida.close();
}

void menu_principal() {
  bool rodar = true;
  while (rodar) {
    cout << "\nProcessador de imagens ADL\n\nEscolha uma funcao:\n\n";
    cout << "[1] Negativo de imagem\n";
    cout << "[2] Binarizacao de canais\n";
    cout << "[0] Sair\n";
    int escolha;
    cin >> escolha;
    switch(escolha) {
      case 0:
        rodar = false;
        break;
      case 1:
        cout << "\nFuncao nao implementada\n";
        break;
      case 2:
        cout << "\nFuncao nao implementada\n";
        break;
    }
    cout << "\nDeseja continuar? (s, n)\n";
    char escolha_saida;
    cin >> escolha_saida;
    if (escolha_saida != 's') {
      rodar = false;
    }
  }
  cout << "Ate a proxima\n";
}

