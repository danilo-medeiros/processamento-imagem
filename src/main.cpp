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

void salva_img(char nome[], Img& img);

int ler_escolha_menu();

// Funcoes:

void corta_rgb(Img& imagem, Img& img_saida, int li, int ci, int lf, int cf);
void binariza_canal(Img& img_in, Img& img_out, int limiar);
void diminui_tamanho(Img& img_in, Img& img_out);
void reflete(Img& img_in, Img& img_out);
void borra_imagem(Img& img_in, Img& img_out);
void equaliza_histograma(Img& img_in, Img& img_out);


int main() {
  cout << "\nProcessador de imagens ADL\n\nEscolha uma funcao:\n\n";
  bool rodar = true;

  while (rodar) {
    int escolha = ler_escolha_menu();
    if (escolha == 0) {
      break;
    }
    char nome_arquivo[TAMANHO_MAXIMO_NOME_ARQUIVO];
    Img imagem;
    Img img_saida;

    switch (escolha) {
      case 0: // Sair
        rodar = false;
        break;
      case 1: // Negativo de imagem
        cout << "\nFuncao nao implementada\n\n=================\n";
        /* char nome_arquivo[TAMANHO_MAXIMO_NOME_ARQUIVO];
        Img imagem;
        abre_img(nome_arquivo, imagem);
        salva_img(nome_arquivo, imagem); */
        break;
      case 2: // Binarizacao de canais
        cout << "\nFuncao nao implementada\n\n=================\n";
        break;
      case 3:
        abre_img(nome_arquivo, imagem);
        int li, ci, lf, cf;
        cout << "Coluna inicial: ";
        cin >> ci;
        cout << "Coluna final: ";
        cin >> cf;
        cout << "Linha inicial: ";
        cin >> li;
        cout << "Linha final: ";
        cin >> lf;
        if (li < 0 || li >= imagem.qtd_linhas || lf <= 0 || lf > imagem.qtd_linhas
          || ci < 0 || ci >= imagem.qtd_colunas || cf <= 0 || cf > imagem.qtd_colunas) {
          cout << "\nERRO:\n\n===== Intervalo selecionado invalido =====\n\n";
          break;
        }
        corta_rgb(imagem, img_saida, li, ci, lf, cf);
        salva_img(nome_arquivo, img_saida);
        break;
      case 4:
        abre_img(nome_arquivo, imagem);
        int limiar;
        cout << "Limiar: ";
        cin >> limiar;
        binariza_canal(imagem, img_saida, limiar);
        salva_img(nome_arquivo, img_saida);
        break;
      case 5:
        abre_img(nome_arquivo, imagem);
        diminui_tamanho(imagem, img_saida);
        salva_img(nome_arquivo, img_saida);
        break;
      case 6:
        abre_img(nome_arquivo, imagem);
        reflete(imagem, img_saida);
        salva_img(nome_arquivo, img_saida);
        break;
      case 7:
        abre_img(nome_arquivo, imagem);
        borra_imagem(imagem, img_saida);
        salva_img(nome_arquivo, img_saida);
        break;
    }
  }
  cout << "Ate a proxima\n";
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

void salva_img(char nome[], Img& img) {
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

int ler_escolha_menu() {
  cout << "[1] Negativo de imagem\n";
  cout << "[2] Binarizacao de canais\n";
  cout << "[3] Recorte de subimagem (crop)\n";
  cout << "[4] Binarização de imagem (por canal)\n";
  cout << "[5] Redução de tamanho da imagem\n";
  cout << "[6] Reflexão horizontal/vertical na imagem (mirror)\n";
  cout << "[7] Convolução para filtragem passa baixas\n";
  cout << "[0] Sair\n";
  int escolha;
  cin >> escolha;
  return escolha;
}

void corta_rgb(Img& imagem, Img& img_saida, int li, int ci, int lf, int cf) {
  img_saida.qtd_linhas = lf - li;
  img_saida.qtd_colunas = cf - ci;
  
  for (int i = li; i < lf; i++) {
    for (int j = ci; j < cf; j++) {
      img_saida.matriz[i - li][j - ci] = imagem.matriz[i][j];
    }
  }
}

void binariza_canal(Img& img_in, Img& img_out, int limiar) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      Pixel novo_pixel;
      if (img_in.matriz[i][j].red < limiar) {
        novo_pixel.red = 0;
      } else {
        novo_pixel.red = 255;
      }

      if (img_in.matriz[i][j].green < limiar) {
        novo_pixel.green = 0;
      } else {
        novo_pixel.green = 255;
      }

      if (img_in.matriz[i][j].blue < limiar) {
        novo_pixel.blue = 0;
      } else {
        novo_pixel.blue = 255;
      }

      img_out.matriz[i][j] = novo_pixel;
    }
  }
}

void diminui_tamanho(Img& img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas / 2;
  img_out.qtd_colunas = img_in.qtd_colunas / 2;

  for (int i = 0; i < img_in.qtd_linhas; i++) {
    if (i % 2 != 0) {
      continue;
    }
    for (int j = 0; j < img_in.qtd_colunas; j++) {
      if (j % 2 != 0) {
        continue;
      }
      img_out.matriz[i / 2][j / 2] = img_in.matriz[i][j];
    }
  }
}

// Implementando reflexao horizontal
void reflete(Img& img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    int metade = img_out.qtd_colunas / 2;
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      if (j < metade) {
        img_out.matriz[i][j] = img_in.matriz[i][img_out.qtd_colunas - j - 1]; 
      } else {
        img_out.matriz[i][j] = img_in.matriz[i][j]; 
      }
    }
  }
}

void borra_imagem(Img& img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;

  for (int i = 0; i < img_out.qtd_linhas; i++) {
    int pos_y = i;
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      int pos_x = j;

      float somatorio_green = 0;
      float somatorio_red = 0;
      float somatorio_blue = 0;
      for (int k = pos_y - 1; k <= pos_y + 1; k++) {
        for (int l = pos_x - 1; l <= pos_x + 1; l++) {
          Pixel pixel;
          if (k < 0 || k >= img_out.qtd_linhas || l < 0 || l >= img_out.qtd_colunas) {
            pixel.red = 0;
            pixel.green = 0;
            pixel.blue = 0;
          } else {
            pixel = img_in.matriz[k][l];
          }
          int pos_x_filtro = k - pos_x; 
          int pos_y_filtro = k - pos_y;

          somatorio_green += pixel.green / 9;
          somatorio_red += pixel.red / 9;
          somatorio_blue += pixel.blue / 9;
        }
      }
      img_out.matriz[i][j].green = somatorio_green;
      img_out.matriz[i][j].red = somatorio_red;
      img_out.matriz[i][j].blue = somatorio_blue;
    }
  }
}

void equaliza_histograma(Img& img_in, Img& img_out) {
  
}