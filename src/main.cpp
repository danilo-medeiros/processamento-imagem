#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
int ler_escolha_menu();

// Funcoes:

void converte_para_cinza(Img img_in, Img& img_out);
void computa_negativo(Img img_in, Img& img_out);
void binariza(Img img_in, Img& img_out, int limiar);
void corta_rgb(Img imagem, Img& img_saida, int li, int ci, int lf, int cf);
void binariza_canal(Img img_in, Img& img_out, int limiar);
void solariza(Img img_in, Img& img_out, int limiar);
void rotaciona(Img img_in, Img& img_out);
void inverte(Img img_in, Img& img_out);
void diminui_tamanho(Img img_in, Img& img_out);
void reflete(Img img_in, Img& img_out, int indice);
void borra_imagem(Img img_in, Img& img_out);
void equaliza_histograma(Img img_in, Img& img_out);
void separa_canais(Img img_in, Img& img_r, Img& img_g, Img& img_b);
void aumenta_tamanho(Img img_in, Img& img_out);

// Funcoes com mascaras:

void computa_negativo_com_mascara(Img img_in, Img mascara, Img& img_out);
void binariza_com_mascara(Img img_in, Img& img_out, int limiar, Img mascara);

// Mascaras:

void ler_escolha_mascara(Img& mascara);
void gerar_mascara_metade_direita(Img& img);
void gerar_mascara_xadrez(Img& img);
void gerar_mascara_primeiro_quadrante(Img& img);
void gerar_mascara_barras(Img& img);
void aplicar_mascara(Img img_in, Img& img_out, Img mascara);
void gerar_mascara(char nome_mascara[], Img& mascara, char nome_arquivo[], Img imagem);

// Helpers:
int ler_limiar();
bool deseja_aplicar_mascara();

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
    Img mascara;

    switch (escolha) {
      case 1:
        abre_img(nome_arquivo, imagem);
        computa_negativo(imagem, img_saida);
        if (deseja_aplicar_mascara()) {
          char nome_mascara[TAMANHO_MAXIMO_NOME_ARQUIVO];
          gerar_mascara(nome_mascara, mascara, nome_arquivo, imagem);
          aplicar_mascara(imagem, img_saida, mascara);
          salva_img(nome_mascara, mascara);
        }
        strcat(nome_arquivo, "_negativo");
        salva_img(nome_arquivo, img_saida);
        break;
      case 2:
        Img img_r;
        Img img_g;
        Img img_b;
        abre_img(nome_arquivo, imagem);
        separa_canais(imagem, img_r, img_g, img_b);
        char nome_r[TAMANHO_MAXIMO_NOME_ARQUIVO];
        strcpy(nome_r, nome_arquivo);
        strcat(nome_r, "_red");

        char nome_g[TAMANHO_MAXIMO_NOME_ARQUIVO];
        strcpy(nome_g, nome_arquivo);
        strcat(nome_g, "_green");

        char nome_b[TAMANHO_MAXIMO_NOME_ARQUIVO];
        strcpy(nome_b, nome_arquivo);
        strcat(nome_b, "_blue");

        salva_img(nome_r, img_r);
        salva_img(nome_g, img_g);
        salva_img(nome_b, img_b);
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
          || ci < 0 || ci >= imagem.qtd_colunas || cf <= 0 || cf > imagem.qtd_colunas || ci >= cf || li >= lf) {
          cout << "Intervalo selecionado invalido\n";
          exit(EXIT_FAILURE);
        }
        corta_rgb(imagem, img_saida, li, ci, lf, cf);
        strcat(nome_arquivo, "_recorte");
        salva_img(nome_arquivo, img_saida);
        break;
      case 4:
        abre_img(nome_arquivo, imagem);
        int limiar;
        cout << "Limiar: ";
        cin >> limiar;
        binariza_canal(imagem, img_saida, limiar);
        if (deseja_aplicar_mascara()) {
          char nome_mascara[TAMANHO_MAXIMO_NOME_ARQUIVO];
          gerar_mascara(nome_mascara, mascara, nome_arquivo, imagem);
          aplicar_mascara(imagem, img_saida, mascara);
          salva_img(nome_mascara, mascara);
        }
        strcat(nome_arquivo, "_binarizado_canal");
        salva_img(nome_arquivo, img_saida);
        break;
      case 5:
        abre_img(nome_arquivo, imagem);
        diminui_tamanho(imagem, img_saida);
        strcat(nome_arquivo, "_reduzido");
        salva_img(nome_arquivo, img_saida);
        break;
      case 6:
        abre_img(nome_arquivo, imagem);
        cout << "Coluna de reflexao: \n";
        int indice;
        cin >> indice;
        if (indice <= 0 || indice > imagem.qtd_colunas - 1) {
          cout << "Coluna selecionada invalida\n";
          exit(EXIT_FAILURE);
        }
        reflete(imagem, img_saida, indice);
        strcat(nome_arquivo, "_refletido");
        salva_img(nome_arquivo, img_saida);
        break;
      case 7:
        abre_img(nome_arquivo, imagem);
        borra_imagem(imagem, img_saida);
        if (deseja_aplicar_mascara()) {
          char nome_mascara[TAMANHO_MAXIMO_NOME_ARQUIVO];
          gerar_mascara(nome_mascara, mascara, nome_arquivo, imagem);
          aplicar_mascara(imagem, img_saida, mascara);
          salva_img(nome_mascara, mascara);
        }
        strcat(nome_arquivo, "_borrado");
        salva_img(nome_arquivo, img_saida);
        break;
      case 8:
        abre_img(nome_arquivo, imagem);
        equaliza_histograma(imagem, img_saida);
        strcat(nome_arquivo, "_equalizado");
        salva_img(nome_arquivo, img_saida);
        break;
      case 9:
        abre_img(nome_arquivo, imagem);
        converte_para_cinza(imagem, img_saida);
        if (deseja_aplicar_mascara()) {
          char nome_mascara[TAMANHO_MAXIMO_NOME_ARQUIVO];
          gerar_mascara(nome_mascara, mascara, nome_arquivo, imagem);
          aplicar_mascara(imagem, img_saida, mascara);
          salva_img(nome_mascara, mascara);
        }
        strcat(nome_arquivo, "_escala_cinza");
        salva_img(nome_arquivo, img_saida);
        break;
      case 10:
        abre_img(nome_arquivo, imagem);
        binariza(imagem, img_saida, ler_limiar());

        if (deseja_aplicar_mascara()) {
          char nome_mascara[TAMANHO_MAXIMO_NOME_ARQUIVO];
          gerar_mascara(nome_mascara, mascara, nome_arquivo, imagem);
          aplicar_mascara(imagem, img_saida, mascara);
          salva_img(nome_mascara, mascara);
        }
        strcat(nome_arquivo, "_binarizado");
        salva_img(nome_arquivo, img_saida);
        break;
      case 11:
        abre_img(nome_arquivo, imagem);
        cout << "Limiar: ";
        int limiar_solariza;
        cin >> limiar_solariza;
        solariza(imagem, img_saida, limiar_solariza);

        if (deseja_aplicar_mascara()) {
          char nome_mascara[TAMANHO_MAXIMO_NOME_ARQUIVO];
          gerar_mascara(nome_mascara, mascara, nome_arquivo, imagem);
          aplicar_mascara(imagem, img_saida, mascara);
          salva_img(nome_mascara, mascara);
        }
        strcat(nome_arquivo, "_solarizado");
        salva_img(nome_arquivo, img_saida);
        break;
      case 12:
        abre_img(nome_arquivo, imagem);
        rotaciona(imagem, img_saida);
        strcat(nome_arquivo, "_rotacionado_horario");
        salva_img(nome_arquivo, img_saida);
        break;
      case 13:
        abre_img(nome_arquivo, imagem);
        inverte(imagem, img_saida);
        strcat(nome_arquivo, "_invertido_x");
        salva_img(nome_arquivo, img_saida);
        break;
      case 14:
        abre_img(nome_arquivo, imagem);
        aumenta_tamanho(imagem, img_saida);
        strcat(nome_arquivo, "_aumentado");
        salva_img(nome_arquivo, img_saida);
        break;
      default:
        cout << "Opção inválida\n";
        break;
    }
    char escolha_final;
    cout << "\nNovamente? (s/n): ";
    cin >> escolha_final;
    if (escolha_final != 's') {
      break;
    }
  }
  cout << "Ate a proxima\n";
  return 0;
}

// Abre arquivo salvo no computador na pasta imagens relativa ao projeto
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
    cout << "Não foi possível abrir o arquivo.\n";
    exit(EXIT_FAILURE);
  }

  char tipo_arquivo[3];
  arquivo.getline(tipo_arquivo, 3);

  cout << tipo_arquivo << endl;
  if (strcmp(tipo_arquivo, "P3") != 0) {
    cout << "Arquivo invalido. Deve ser um arquivo .ppm\n";
    exit(EXIT_FAILURE);
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
  cout << "ARQUIVO SALVO EM: " << nome_saida << endl;
  arquivo_saida.close();
}

int ler_escolha_menu() {
  cout << "[1] Negativo de imagem\n";
  cout << "[2] Separação de canais\n";
  cout << "[3] Recorte de subimagem (crop)\n";
  cout << "[4] Binarização de imagem (por canal)\n";
  cout << "[5] Redução de tamanho da imagem\n";
  cout << "[6] Reflexão horizontal/vertical na imagem (mirror)\n";
  cout << "[7] Convolução para filtragem passa baixas\n";
  cout << "[8] Equalização de histograma\n";
  cout << "[9] Conversão para níveis de cinza\n";
  cout << "[10] Binarização de imagem (preto e branco)\n";
  cout << "[11] Solarização de imagem\n";
  cout << "[12] Rotaciona\n";
  cout << "[13] Inversão vertical na imagem (flip)\n";
  cout << "[14] Aumento de tamanho da imagem\n";
  cout << "[0] Sair\nSua opcao: ";
  int escolha;
  cin >> escolha;
  return escolha;
}

int ler_limiar() {
  int limiar;
  cout << "Limiar: ";
  cin >> limiar;
  if (limiar <= 0 || limiar >= 255) {
    cout << "Limiar deve ser entre 0 e 255\n";
    exit(EXIT_FAILURE);
  }
  return limiar;
}

bool deseja_aplicar_mascara() {
  char opcao_mascara;
  cout << "Deseja aplicar mascara ? (s/n): ";
  cin >> opcao_mascara;
  return opcao_mascara == 's';
}

void converte_para_cinza(Img img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      int media = (img_in.matriz[i][j].red + img_in.matriz[i][j].green + img_in.matriz[i][j].blue) / 3;
      img_out.matriz[i][j] = { media, media, media };
    }
  }
}

void computa_negativo(Img img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      img_out.matriz[i][j] = {
        255 - img_in.matriz[i][j].red,
        255 - img_in.matriz[i][j].green,
        255 - img_in.matriz[i][j].blue
      };
    }
  }
}

void binariza(Img img_in, Img& img_out, int limiar) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      int media = (img_in.matriz[i][j].red + img_in.matriz[i][j].green + img_in.matriz[i][j].blue) / 3;
      int valor_pixel = media < limiar ? 0 : 255;
      img_out.matriz[i][j] = { valor_pixel, valor_pixel, valor_pixel };
    }
  }
}

void corta_rgb(Img imagem, Img& img_saida, int li, int ci, int lf, int cf) {
  img_saida.qtd_linhas = lf - li;
  img_saida.qtd_colunas = cf - ci;
  
  for (int i = li; i < lf; i++) {
    for (int j = ci; j < cf; j++) {
      img_saida.matriz[i - li][j - ci] = imagem.matriz[i][j];
    }
  }
}

void binariza_canal(Img img_in, Img& img_out, int limiar) {
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

void solariza(Img img_in, Img& img_out, int limiar) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      img_out.matriz[i][j] = {
        img_in.matriz[i][j].red < limiar ? (255 - img_in.matriz[i][j].red) : img_in.matriz[i][j].red,
        img_in.matriz[i][j].green < limiar ? (255 - img_in.matriz[i][j].green) : img_in.matriz[i][j].green,
        img_in.matriz[i][j].blue < limiar ? (255 - img_in.matriz[i][j].blue) : img_in.matriz[i][j].blue
      };
    }
  }
}

void rotaciona(Img img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_colunas;
  img_out.qtd_colunas = img_in.qtd_linhas;
  for (int i = 0; i < img_in.qtd_linhas; i++) {
    for (int j = 0; j < img_in.qtd_colunas; j++) {
      img_out.matriz[j][img_in.qtd_linhas - i - 1] = img_in.matriz[i][j];
    }
  }
}

void inverte(Img img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_colunas;
  img_out.qtd_colunas = img_in.qtd_linhas;
  for (int i = 0; i < img_in.qtd_linhas; i++) {
    for (int j = 0; j < img_in.qtd_colunas; j++) {
      img_out.matriz[img_in.qtd_linhas - i - 1][j] = img_in.matriz[i][j];
    }
  }
}

void diminui_tamanho(Img img_in, Img& img_out) {
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
void reflete(Img img_in, Img& img_out, int indice) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      if (j < indice) {
        img_out.matriz[i][j] = img_in.matriz[i][img_out.qtd_colunas - j - 1]; 
      } else {
        img_out.matriz[i][j] = img_in.matriz[i][j]; 
      }
    }
  }
}

void borra_imagem(Img img_in, Img& img_out) {
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
            pixel = { 0, 0, 0 };
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
      img_out.matriz[i][j] = {
        (int) somatorio_red,
        (int) somatorio_green,
        (int) somatorio_blue
      };
    }
  }
}

void equaliza_histograma(Img img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas;
  img_out.qtd_colunas = img_in.qtd_colunas;

  int histograma_red[256] = {0};
  int histograma_green[256] = {0};
  int histograma_blue[256] = {0};

  for (int i = 0; i < img_in.qtd_linhas; i++) {
    for (int j = 0; j < img_in.qtd_colunas; j++) {
      histograma_red[img_in.matriz[i][j].red] += 1;
      histograma_green[img_in.matriz[i][j].green] += 1;
      histograma_blue[img_in.matriz[i][j].blue] += 1;
    }
  }
  int total_pixels = img_out.qtd_colunas * img_out.qtd_linhas;

  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      int soma_histograma_red = 0;
      int soma_histograma_blue = 0;
      int soma_histograma_green = 0;

      for (int k = 0; k <= img_in.matriz[i][j].red; k++) {
        soma_histograma_red += histograma_red[k];
      }

      for (int k = 0; k <= img_in.matriz[i][j].green; k++) {
        soma_histograma_green += histograma_green[k];
      }

      for (int k = 0; k <= img_in.matriz[i][j].blue; k++) {
        soma_histograma_blue += histograma_blue[k];
      }

      Pixel pixel;
      pixel.red = (soma_histograma_red * 255) / total_pixels;
      pixel.green = (soma_histograma_green * 255) / total_pixels;
      pixel.blue = (soma_histograma_blue * 255) / total_pixels;
      img_out.matriz[i][j] = pixel;
    }
  }
}

void separa_canais(Img img_in, Img& img_r, Img& img_g, Img& img_b) {
  img_r.qtd_linhas = img_in.qtd_linhas;
  img_r.qtd_colunas = img_in.qtd_colunas;

  img_g.qtd_linhas = img_in.qtd_linhas;
  img_g.qtd_colunas = img_in.qtd_colunas;

  img_b.qtd_linhas = img_in.qtd_linhas;
  img_b.qtd_colunas = img_in.qtd_colunas;

  for (int i = 0; i < img_in.qtd_linhas; i++) {
    for (int j = 0; j < img_in.qtd_colunas; j++) {
      Pixel pixel_r;
      Pixel pixel_g;
      Pixel pixel_b;

      pixel_r.red = img_in.matriz[i][j].red;
      pixel_r.green = 0;
      pixel_r.blue = 0;

      pixel_g.red = 0;
      pixel_g.green = img_in.matriz[i][j].green;
      pixel_g.blue = 0;

      pixel_b.red = 0;
      pixel_b.green = 0;
      pixel_b.blue = img_in.matriz[i][j].blue;

      img_r.matriz[i][j] = pixel_r;
      img_g.matriz[i][j] = pixel_g;
      img_b.matriz[i][j] = pixel_b;
    }
  }
}

void aumenta_tamanho(Img img_in, Img& img_out) {
  img_out.qtd_linhas = img_in.qtd_linhas * 2 -1;
  img_out.qtd_colunas = img_in.qtd_colunas * 2 - 1;
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      img_out.matriz[i * 2][j * 2] = img_in.matriz[i][j];
    }
  }

  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      // Linha
      if ((i == 0 || i == img_out.qtd_linhas - 1) && j % 2 != 0) {
        Pixel anterior, proximo;
        if (j - 1 >= 0) {
          anterior = img_out.matriz[i][j - 1];
        } else {
          anterior = {0, 0, 0};
        }
        
        if (j + 1 <= img_out.qtd_colunas - 1) {
          proximo = img_out.matriz[i][j + 1];
        } else {
          proximo = {0, 0, 0};
        }
        img_out.matriz[i][j] = {
          anterior.red + proximo.red,
          anterior.green + proximo.green,
          anterior.blue + proximo.blue,
        };
      }

      // Coluna
      if ((j == 0 || j == img_out.qtd_colunas - 1) && i % 2 != 0) {
        Pixel anterior, proximo;
        if (i - 1 >= 0) {
          anterior = img_out.matriz[i - 1][j];
        } else {
          anterior = {0, 0, 0};
        }
        
        if (i + 1 <= img_out.qtd_linhas - 1) {
          proximo = img_out.matriz[i + 1][j];
        } else {
          proximo = {0, 0, 0};
        }
        img_out.matriz[i][j] = {
          anterior.red + proximo.red / 2,
          anterior.green + proximo.green / 2,
          anterior.blue + proximo.blue / 2,
        };
      }
    }
  }

  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      if (i > 0 && i < img_out.qtd_linhas - 1 && i % 2 != 0
        && j > 0 && j < img_out.qtd_colunas - 1 && j % 2 != 0) {
        Pixel p1 = img_out.matriz[i - 1][j - 1];
        Pixel p2 = img_out.matriz[i - 1][j + 1];
        Pixel p3 = img_out.matriz[i + 1][j - 1];
        Pixel p4 = img_out.matriz[i + 1][j + 1];
        img_out.matriz[i][j] = {
          (p1.red + p2.red + p3.red + p4.red) / 4,
          (p1.green + p2.green + p3.green + p4.green) / 4,
          (p1.blue + p2.blue + p3.blue + p4.blue) / 4
        };
      }
    }
  }

  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      int soma_indices = i + j + 1;
      if (soma_indices % 2 == 0 && (i > 0 && i < img_out.qtd_linhas - 1
        && j > 0 && j < img_out.qtd_colunas - 1)) {
        Pixel p1 = img_out.matriz[i - 1][j];
        Pixel p2 = img_out.matriz[i][j - 1];
        Pixel p3 = img_out.matriz[i][j + 1];
        Pixel p4 = img_out.matriz[i + 1][j];
        img_out.matriz[i][j] = {
          (p1.red + p2.red + p3.red + p4.red) / 4,
          (p1.green + p2.green + p3.green + p4.green) / 4,
          (p1.blue + p2.blue + p3.blue + p4.blue) / 4
        };
      }
    }
  }
}

void ler_escolha_mascara(Img& mascara) {
  int escolha;
  cout << "Escolha a mascara que deseja aplicar: \n";
  cout << "[1] Metade direita\n[2] Xadrez\n[3] Primeiro quadrante\n[4] Barras\nSua opcao: ";
  cin >> escolha;
  switch(escolha) {
    case 1:
      gerar_mascara_metade_direita(mascara);
      break;
    case 2:
      gerar_mascara_xadrez(mascara);
      break;
    case 3:
      gerar_mascara_primeiro_quadrante(mascara);
      break;
    case 4:
      gerar_mascara_barras(mascara);
      break;
    default:
      cout << "Opcao invalida\n";
      break;
  }
}

void gerar_mascara_metade_direita(Img& img) {
  int metade = img.qtd_colunas / 2;
  for (int i = 0; i < img.qtd_linhas; i++) {
    for (int j = 0; j < img.qtd_colunas; j++) {
      if (j < metade) {
        img.matriz[i][j] = { 0, 0, 0 };
      } else {
        img.matriz[i][j] = { 255, 255, 255 };
      }
    }
  }
}

void gerar_mascara_xadrez(Img& img) {
  int largura_quadrado = img.qtd_colunas / 8;
  int altura_quadrado = img.qtd_linhas / 8;
  bool imprimir_branco = true;
  for (int i = 0; i < img.qtd_linhas; i++) {
    if (i % altura_quadrado == 2) {
      imprimir_branco = !imprimir_branco;
    }
    for (int j = 0; j < img.qtd_colunas; j++) {
      if (j % largura_quadrado == 0) {
        imprimir_branco = !imprimir_branco;
      }
      if (imprimir_branco) {
        img.matriz[i][j] = { 255, 255, 255 };
      } else {
        img.matriz[i][j] = { 0, 0, 0 };
      }
    }
  }
}

void gerar_mascara_barras(Img& img) {
  int largura_barra = img.qtd_colunas / 8;
  bool imprimir_branco = false;
  for (int i = 0; i < img.qtd_linhas; i++) {
    for (int j = 0; j < img.qtd_colunas; j++) {
      if (j % 30 == 0) {
        imprimir_branco = !imprimir_branco;
      }
      if (imprimir_branco) {
        img.matriz[i][j] = { 255, 255, 255 };
      } else {
        img.matriz[i][j] = { 0, 0, 0 };
      }
    }
  }
}

void gerar_mascara_primeiro_quadrante(Img& img) {
  int metade_altura = img.qtd_linhas / 2;
  int metade_largura = img.qtd_colunas / 2;
  for (int i = 0; i < img.qtd_linhas; i++) {
    for (int j = 0; j < img.qtd_colunas; j++) {
      if (i < metade_altura && j < metade_largura) {
        img.matriz[i][j] = { 0, 0, 0 };
      } else {
        img.matriz[i][j] = { 255, 255, 255 };
      }
    }
  }
}

void aplicar_mascara(Img img_in, Img& img_out, Img mascara) {
  for (int i = 0; i < img_out.qtd_linhas; i++) {
    for (int j = 0; j < img_out.qtd_colunas; j++) {
      if (mascara.matriz[i][j].red == 0) {
        img_out.matriz[i][j] = img_in.matriz[i][j];
      }
    }
  }
}

void gerar_mascara(char nome_mascara[], Img& mascara, char nome_arquivo[], Img imagem) {
  mascara.qtd_linhas = imagem.qtd_linhas;
  mascara.qtd_colunas = imagem.qtd_colunas;
  ler_escolha_mascara(mascara);
  strcpy(nome_mascara, nome_arquivo);
  strcat(nome_mascara, "_mascara");
}
