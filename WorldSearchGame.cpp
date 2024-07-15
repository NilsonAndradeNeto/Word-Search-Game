#include <iostream>
#include <fstream>
#include <time.h>


using namespace std;

const int TAM_MATRIZ = 20;
const int TAM_GRADE = 20 ;
int       TAM_PALAVRA=0;

struct lista_vetor {
    string palavras_lista[10];
};

int linhas_preenchidas(string& arquivo) {
    ifstream arquivoCSV(arquivo);
    string linha;
    int contador = 0;

    while (getline(arquivoCSV, linha)) {
        contador++;
    }

    arquivoCSV.close();

    return contador;
}

void mandar(string arquivo, lista_vetor lista[], int TAM) {
    ifstream meu_arquivo(arquivo);
    string linha;

    for (int i = 0; i < TAM && getline(meu_arquivo, linha); i++) {
        string palavra;
        int palavra_inicio = 0;

        for (int j = 0; j < 10; j++) {
            palavra = "";
            while (palavra_inicio < linha.size() && linha[palavra_inicio] != ';') {
                palavra += linha[palavra_inicio];
                palavra_inicio++;
            }
            lista[i].palavras_lista[j] = palavra;

            palavra_inicio++;
        }
    }

    meu_arquivo.close();
}

void salve_novo(string& arquivo, lista_vetor lista[], int& TAM) {

    ofstream meu_arquivo(arquivo, ios::app);
    string nova_lista;
    int j = 0;

    do {
        cout << "Digite a nova palavra ou 'fim' para finalizar: ";
        cin >> nova_lista;

        if (nova_lista != "fim") {
            lista[TAM].palavras_lista[j] = nova_lista;
            meu_arquivo << nova_lista;
            if (j < 9) {
                meu_arquivo << ";";
            }
            j++;
        }
    } while (nova_lista != "fim" && j < 10);

    TAM++;

    meu_arquivo << endl;
    meu_arquivo.close();
}

void excluir_lista(string& arquivo, int posicao, int& TAM) {

    ifstream arquivo_entrada(arquivo);
    ofstream arquivo_saida("arquivoauxiliar.csv");

    string linha;
    int contador = 0;

    while (getline(arquivo_entrada, linha)) {
        contador++;

        if (contador != posicao) {
            arquivo_saida << linha << endl;
        }
    }

    arquivo_entrada.close();
    arquivo_saida.close();

    ifstream arquivo_auxiliar_entrada("arquivoauxiliar.csv");
    ofstream arquivo_auxiliar_saida(arquivo);

    while (getline(arquivo_auxiliar_entrada, linha)) {
        arquivo_auxiliar_saida << linha << endl;
    }

    arquivo_auxiliar_entrada.close();
    arquivo_auxiliar_saida.close();

    TAM--;
}

void lista_atual(lista_vetor lista[], int TAM) {

    cout << "Lista preenchida ate agora:" << endl;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < 10; j++) {
            cout << " " << lista[i].palavras_lista[j] << "  ";
        }
        cout << endl;
    }
}

void preencher_matriz( char matriz[TAM_MATRIZ][TAM_GRADE]) {

    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_GRADE; j++) {
            if (matriz[i][j] == ' ') {

                matriz[i][j] = 'a' + rand() % 26;
            }
        }
    }
}

void imprimir_matriz_com_marcacoes(char matriz[TAM_MATRIZ][TAM_GRADE]) {

    cout << "   ";
    for (int j = 0; j < TAM_GRADE; j++) {
        cout << char('A' + j) << "   ";
    }
    cout<<endl;

    for (int i = 0; i < TAM_MATRIZ; i++) {
        cout << i + 1 << "  ";
        for (int j = 0; j < TAM_GRADE; j++) {
            cout << matriz[i][j] << "   ";
        }
        cout << endl;
    }
}
bool verificar_palavra( string& palavra, char matriz[TAM_MATRIZ][TAM_GRADE], int linha, int coluna,int x, int y) {

    int tamanho = palavra.length();
    int linhaAtual = linha;
    int colunaAtual = coluna;


    if (linhaAtual + (tamanho - 1) * x >= TAM_MATRIZ || linhaAtual + (tamanho - 1) * x < 0 ||
        colunaAtual + (tamanho - 1) * y >= TAM_GRADE || colunaAtual + (tamanho - 1) * y < 0) {
        return false;
    }


    for (int i = 0; i < tamanho; i++) {
        if (matriz[linhaAtual][colunaAtual] != palavra[i]) {
            return false;
        }
        linhaAtual += x;
        colunaAtual += y;
    }

    return true;
}

bool encontrar_palavra( string& palavra,char matriz[TAM_MATRIZ][TAM_GRADE], int& linha, char& coluna) {

    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_GRADE; j++) {

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x != 0 || y != 0) {

                        if (verificar_palavra(palavra, matriz, i, j, x, y)) {
                            linha = i;

                            coluna = 'a'+j;

                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

void jogar_caca_palavras( lista_vetor &lista, int TAM) {

    char matriz[TAM_MATRIZ][TAM_GRADE];
    srand(time(NULL));

    for (int i = 0; i < TAM_MATRIZ; i++) {
        for (int j = 0; j < TAM_GRADE; j++) {
            matriz[i][j] = ' ';
        }
    }

    for (int j = 0; j < 10; j++) {
        if (!lista.palavras_lista[j].empty()) {
            TAM_PALAVRA++;
        }
    }


    for (int i = 0; i < 10; i++) {
        string palavra = lista.palavras_lista[i];
        int palavraLen = palavra.length();


        int posColuna = rand() % TAM_GRADE;
        int posLinha = rand() % (TAM_MATRIZ - palavraLen + 1);

        bool podeInserirVerticalmente = true;

        for (int j = 0; j < palavraLen; j++) {
            if (matriz[posLinha + j][posColuna] != ' ') {
                podeInserirVerticalmente = false;
                break;
            }
        }


        if (podeInserirVerticalmente) {
            for (int j = 0; j < palavraLen; j++) {
                matriz[posLinha + j][posColuna] = palavra[j];
            }
        } else {

            posColuna = rand() % (TAM_GRADE - palavraLen + 1);
            posLinha = rand() % TAM_MATRIZ;

            bool podeInserirHorizontalmente = true;


            for (int j = 0; j < palavraLen; j++) {
                if (matriz[posLinha][posColuna + j] != ' ') {
                    podeInserirHorizontalmente = false;
                    break;
                }
            }


            if (podeInserirHorizontalmente) {
                for (int j = 0; j < palavraLen; j++) {
                    matriz[posLinha][posColuna + j] = palavra[j];
                }
            }
        }
    }



    // Preenche a matriz com letras aleatórias
    preencher_matriz(matriz);




    int palavrasEncontradas = 0;


    int linha;
    char coluna;
    do {
        string palavra;
        system("cls");
        imprimir_matriz_com_marcacoes(matriz);

        cout << "Digite uma palavra encontrada (ou 'fim' para sair): ";
        cin >> palavra;

        if(palavra=="fim"){
            palavrasEncontradas=TAM_PALAVRA;
        }

        if (encontrar_palavra(palavra, matriz, linha, coluna)) {
            cout << "Palavra encontrada na posicao (" << linha+1 << ", " << coluna <<")"<< endl;

            palavrasEncontradas++;
            cout<<TAM_PALAVRA;
            system("pause");
        } else {
            system("cls");
            cout << "Palavra nao encontrada. Tente novamente." << endl;
        }

        if (palavrasEncontradas == TAM_PALAVRA) {
            system("cls");
            cout << "Parabens todas as palavras foram encontradas." << endl;
            break;
        }
    } while (palavrasEncontradas != TAM_PALAVRA);
    system("cls");
    cout << "Jogo encerrado Voce encontrou " << palavrasEncontradas << " palavra de um total de " << TAM_PALAVRA << " palavra" << endl;
    TAM_PALAVRA=0;

}

int main() {
    srand(time(NULL));

    string arquivo = "trabalhom3.csv";
    int TAM = linhas_preenchidas(arquivo);
    lista_vetor lista[10];
    string lista_aux[10];
    mandar(arquivo, lista, TAM);

    int opcao;
    do {
        system("cls");
        cout<<"\t\t\t\t  _________________________________________________"<<endl;
        cout<<"\t\t\t\t /                                                   "<<endl;
        cout<<"\t\t\t\t|    _________________________________________     |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |        1-DIGITAR NOVO LISTA             |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |        2-EXCLUIR LISTA                  |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |        3-VISUALIZAR LISTA               |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |        4-JOGAR CACA PALAVRA             |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |        0-SAIR                           |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |                                         |    |"<<endl;
        cout<<"\t\t\t\t|   |_________________________________________|    |"<<endl;
        cout<<"\t\t\t\t|                                                  |"<<endl;
        cout<<"\t\t\t\t _________________________________________________/"<<endl;
        cout<<"\t\t\t\t                   \_____________/"<<endl;

        cout<<">>";
        cin >> opcao;

        switch (opcao) {
        case 1:
            system("cls");
            salve_novo(arquivo, lista, TAM);
            break;
        case 2:
            int posicao;
            cout << "Digite a posicao da linha a ser excluida: ";
            cin >> posicao;
            excluir_lista(arquivo, posicao, TAM);
            break;
        case 3:
            system("cls");
            lista_atual(lista, TAM);
            system("pause");
            break;
        case 4:

            system("cls");
            if(TAM>0){
                for (int j = 0; j < 10; j++) {
                    lista_aux[j]=lista[rand()%10].palavras_lista[j];
                }
                jogar_caca_palavras(lista[rand()%TAM],TAM);
            }
            else{
                cout<<" POR FAVOR DIGITE UMA LISTA ANTES DE JOGAR " <<endl;
            }
            system("pause");
            break;
        case 0:
            cout << "Saindo..." << endl;
            break;
        default:
            cout << "Opcao invalida. Tente novamente." << endl;
            break;
        }
    } while (opcao != 0);

    return 0;
}
