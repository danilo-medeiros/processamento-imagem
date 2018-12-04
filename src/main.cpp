#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

const int TAMANHO_MAXIMO_IMAGEM = 512;
const int TAMANHO_MAXIMO_NOME_ARQUIVO = 30; 

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

int main() {
  char nome_arquivo[TAMANHO_MAXIMO_NOME_ARQUIVO];
  Img imagem;
  abre_img(nome_arquivo, imagem);
  return 0;
}

void abre_img(char nome[], Img& img) {
  fstream arquivo;
  cout << "Digite o nome do arquivo que deseja abrir (.ppm):\n";
  cin >> nome;
  arquivo.open(nome);
  if (!arquivo.is_open()) {
    cout << "Ocorreu um erro ao abrir o arquivo\n";
    return;
  }
  string tipo_arquivo;
  getline(arquivo, tipo_arquivo);
  if (tipo_arquivo != "P3") {
    cout << "Arquivo invalido. Deve ser um arquivo .ppm\n";
    return;
  }
  arquivo >> img.qtd_linhas >> img.qtd_colunas;
  
}
