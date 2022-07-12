/* Bruna Cristina Goncalves
 * 20.2.4123
 * BCC201-31
 */

//includes/bibliotecas a serem utilizados
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h> /*bool é um tipo de dados.Assim como tem o int, o char e o double, tem o bool.
                      *só aceita dois valores:true ou false. True significa verdadeiro e false significa falso.
                      */  
//definindo valores, para facilitar caso necessite que seja alterado

#define DIMENSAO 8
#define VALOR 80
#define TAM_MAX 100


//tentei mas nao consegui fazer funcionar
//definindo cores do tabuleiro
#define ANSI_RESET            "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_COLOR_WHITE      "\x1b[37m" // branco
#define ANSI_COLOR_BLACK      "\x1b[30m" // preto
#define ANSI_BG_COLOR_WHITE   "\x1b[47m" // branco
#define ANSI_BG_COLOR_BLACK   "\x1b[40m" // preto

//4 estruturas utilizadas
 typedef struct {
	char nome[TAM_MAX];
	char arquivo[TAM_MAX];
	int vez;	
}jogadorInf;

typedef struct {
	bool captura;
}Captura;

typedef struct {
	int linsOrigem;
	int linsDestino;
	int colsOrigem;
	int colsDestino;
}Transforma;

typedef struct {
	int contPecasO;
	int contPecasX;
	int contDamaO;
	int contDamaX;
	int contDamas;
	int contJogadas;
}Elementos;

//prototipos das funcoes a serem utilizadas
void menuInicial (); //menu de opções
char **criarTabuleiro (int, int); //função responsavel por criar a matriz do tabuleiro
void fazendoTabuleiro (char**, char[], char[]); //função resposavel por imprimir o tabuleiro
void menuJogadores (jogadorInf*); //função resposavel por popular os campos relacionados ao(s) jogador(es)
void verificacoes (jogadorInf*, char**, int*); //função principal
void maquina (char**, jogadorInf*, bool*, char*, char*, char[], char[], Transforma*, int); //inteligencia artificial
bool verificaJogada (char*, char[], char[]); //verifica se a jogada está dentro dos limites do tabuleiros
void TransformaCharParaInt (char*, char*, Transforma*); //Transforma char pra int
bool certificaJogada (char**, jogadorInf*, Transforma*, bool*, int); //valida a jogada
bool certificaCaptura (char**, jogadorInf*, Transforma*, bool*, int); //verifica se a pessoa digitou uma jogada de range 2
void mapearCaptura (char**, jogadorInf*, Captura**, int); //funcao que chama as outras funções responsaveis por mapear capturas
void percorreCapturaDama (char**, jogadorInf*, Captura**, int); //percorre o tabuleiro a fim de achar alguma peça que a dama pode capturar
void percorreCapturaPeca (char**, jogadorInf*, Captura**, int); //percorre o tabuleiro a fim de achar alguma peça que deve ser capturada 
bool possibilidadeCaptura (Captura**); //verifica se há a possibilidade de captura
void moveCapturaDama (char**, jogadorInf*, Transforma*, Captura**, bool*, int); //movimento de captura da dama
void moveCapturaPeca (char**, jogadorInf*, Transforma*, Captura**, int); //movimento de captura da peça
bool moveDama (char**, Transforma*, jogadorInf*, int); 
void movePeca (char**, Transforma*, jogadorInf*, int); //movimento normal da peça
void transformaDama (char**, Transforma*, jogadorInf*, int); //transforma a peça em dama
void defineVez (int*); //define a vez do jogador
void arquivoSalvo (char**, jogadorInf*, int); //salvar o arquivo do jogo
void carregarArquivo (jogadorInf*, char**, int*); //ler o arquivo do jogo
bool empate (char** , Elementos*);
bool vitoria (char** , jogadorInf*, int);
void memoriaFree (char**, Captura**, jogadorInf*, char*, char*, Elementos*, Transforma*);//funcao responsavel por liberar a memoria

//  metodo main
int main() {
	//chamadando a funcao do menu de opcoes
	menuInicial();
	return 0;
}

void menuInicial() {

    int i = 0; //variavel que contem o indice do jogador
	char **tabuleiro = criarTabuleiro(DIMENSAO,DIMENSAO);
	jogadorInf *jogador = malloc(2* sizeof (jogadorInf)); 
	char opcao[TAM_MAX]; 
	jogador->arquivo[0] = 0; //inicializa a variavel arquivo com o valor nulo

	

	do{
		//imprimindo menu
	    printf("\t\t\t\t\tBem vindo ao Jogo de Damas\n");

        printf("\n");

        printf("\t\t\tInstrucoes:\n"); 
		printf("\t\t\t -Jogador 1, voce iniciara o jogo com as pecas \"o\" e ira comecar o jogo.\n");
		printf("\t\t\t -Jogador 2, voce iniciara o jogo com as pecas \"x\".\n");
        printf("\t\t\t -Para cada jogada, digite a casa de origem enter e a do destino. Por exemplo,");
		printf("\t\t\t\t\t  para mover da casa na linha D, coluna 6 para a linha E coluna 5, digite: D6, enter E5)\n");

		printf("\n"); 

		printf("\t\t\tBora comecar?\n");

		printf("\n");

		printf("\t\t\tO que deseja fazer:\n");
		printf("\t\t\t 1. Comecar um novo jogo\n");
		printf("\t\t\t 2. Continuar um novo jogo\n");

        printf("\n");

        printf("\t\t\tEscolha a opcao:(digite \"sair\" em qualquer lugar para sair do jogo):\n");
        
        printf("\n");

		fgets(opcao, TAM_MAX-1, stdin);
		//acrescenta o '\0' pra tirar o '\n'
		int tam = strlen(opcao);
		opcao[tam-1] = '\0';
        
        //se digitar 1, comecar o jogo
		if(strcmp(opcao, "1") == 0){
			menuJogadores(jogador);
			verificacoes(jogador, tabuleiro, &i);
        //se digitar 2, para continuar o jogo    
		} else if(strcmp(opcao, "2") == 0){
			carregarArquivo(jogador, tabuleiro, &i);
        //se digitar sair, saira do jogo    
		} else if(strcmp(opcao, "sair") == 0){
			break;
        } else {
			printf("\t\t\tOpcao invalida! Digite novamente\n\n");
		}//caso nao digite o que foi informado, continuara pedindo ate a pessoa digitar opcoes validas
	} while (strcmp(opcao,"1") != 0 && strcmp(opcao, "2") != 0); 
}

char **criarTabuleiro(int lins, int cols){
	//matriz alocada dinamicamente 
	char **t;
    t = malloc(lins * sizeof(char*));
    for(int i = 0; i < lins; i++){
		t[i] = malloc(cols * sizeof (char));
	}

    //preenchendo a matriz
	for(int i = 0; i < lins; i++){
		for(int j = 0; j < cols; j++){
			if(i <= 2 && (i+j) % 2 != 0)
				t[i][j] = 'x';
			else if(i >= 5 && (i+j) % 2 != 0)
				t[i][j] = 'o';
			else
				t[i][j] = ' ';
		}
	}
	//retorna a matriz preenchida
	return t;
}

void fazendoTabuleiro(char **m,char letras[], char nums[]){
	//imprimindo o tabuleiro

    printf("\t\t\t\t   ");
    printf("|");
	//barra da parte dos numeros 

	for(int i = 0; i < DIMENSAO; i++){//incrementando os numeros
        printf(" %c ", nums[i]); // imprimindo os numeros
	}

	printf("|\n"); //fechando a barra dos numeros

	for(int i = 0; i < DIMENSAO; i++){//incrementando as letras
		printf("\t\t\t\t %c |",letras[i]); //imprimindo a barra das letras
        
		//criando o formato do tabuleira e tentando colocar as cores, mas nao deu certo utilizando as cores
		for(int j = 0; j < DIMENSAO; j++){
			if(i <= 2 && (i+j) % 2 != 0)
                printf(" %c ", m[i][j]);
				//printf("%s%s %c %s", ANSI_COLOR_BLACK, ANSI_COLOR_WHITE, m[i][j], ANSI_RESET );
			else if(i >= 5 && (i+j) % 2 != 0)
                printf(" %c ", m[i][j] );
			else if((i+j) % 2 == 0)
                printf(" %c ", m[i][j] );
			else 
                printf(" %c ", m[i][j] );
		}
    
        printf("|%c ", letras[i]); //barra da parte onde ta as letras
		printf("\n");
	}

    printf("\t\t\t\t   |"); //barra da parte onde ta as letras
	for(int i = 0; i < DIMENSAO; i++){
        printf(" %c ",nums[i]); //fazendo o mesmo processo pros numeros da parte de baixo
	}

    printf("|\n");
}

void menuJogadores(jogadorInf *jogador){
	int tam; //variavel criada pra armazenar o tamanho da string aderida pela maquina
	char menuJogadores[TAM_MAX]; //armazena a entrada (quantidade de jogadores que foi
	                         // inicializada com o valor no define)
    
	//o do while ta imprimindo a parte abaixo do menu, que é onde coloca a quantidade de jogares
	//e diz tambem o nome da cada um  
	do{
        printf("\n");
        printf("\t\t\tDigite a quantidade de jogadores < 1 / 2 >:");
		fgets(menuJogadores, TAM_MAX-1, stdin);
		int tam = strlen(menuJogadores);
		menuJogadores[tam-1] = '\0';

		printf("\n");

		if(strcmp(menuJogadores, "1") == 0){
			for(int i = 0; i < 2; i++){
				if(i == 0){
					printf("\t\t\tJogador 1, digite seu nome: ");
					fgets(jogador[i].nome, TAM_MAX-1, stdin);
				}
				strcpy(jogador[1].nome, "computador"); //copia "computador" em jogador[1].nome
				int tam = strlen(jogador[i].nome);
				jogador[i].nome[tam-1] = '\0';
				if(i == 0)
					printf("\t\t\t");
				printf("%s",jogador[i].nome);
				if(i == 0)
					printf(" contra ");
				jogador[i].vez = i+1;
			}
            printf("\n");

		} else if(strcmp(menuJogadores, "2") == 0){
			for(int i = 0; i < 2; i++){
                printf("\t\t\tJogador %d",i+1);
                printf(", digite seu nome: ");
				fgets(jogador[i].nome, TAM_MAX-1, stdin);
				printf("\t\t\tVamos comecar o jogo!");
				int tam = strlen(jogador[i].nome);
		        jogador[i].nome[tam-1] = '\0';
                printf(" %s! \n", jogador[i].nome);
				jogador[i].vez = i+1;	
			}
		} else {
            printf("\t\t\tOpcao invalida! Digite novamente\n\n"); 
			//se o usuario digitar algo diferente do que foi pedido vai pro comando while                 
		}
	} while (strcmp(menuJogadores, "1") != 0 && strcmp(menuJogadores, "2") != 0);
	// repete o processo até a pessoa digitar uma opcao valida
}

void verificacoes(jogadorInf *jogador, char ** tabuleiro, int *i){
	Elementos *pedras = malloc (sizeof (Elementos)); // struct utilizada para verificar empate
	Transforma *inteiros = malloc (sizeof (Transforma));
	//struct contendo o valor da conversao de char pra int de cada jogada
	Captura **possibilidade = malloc (sizeof (Captura*)*DIMENSAO); //struct de boleanos (stdbool.h)

    //logica parecida com a da tabela verdade
	/*dama: peça normal = false, dama = true;
	 *capturaDama: não há possibilidade de captura = false, há possibilidade de captura = true;
	 *computador: é o computador = true, não é o computador = false.
     */ 
	bool dama = false, capturaDama, computador;

	for(int k = 0; k < DIMENSAO; k++){
		possibilidade[k] = malloc (sizeof(Captura)*DIMENSAO);
	}

	int linRepetir, colRepetir; //armazena a linha e coluna da posicao de destino que a captura é true

	/*repetirVez: possibilidade de capturar novamente = true, sem possibilidade de capturar novamente = false;
	 *fim: sair/empate/vitoria = true
	 *flagLeitura: string maior ou igual a 3 = true, string que atenda os requisitos = false
     *flag é um mecanismo que serve para controlar a "lógica" de seu programa.
	 *(interrompendo ou permitindo a execução de alguns comandos),
     *(isto é, valores 1/0, ligado/desligado, ativo/inativo)
     */
	bool repetirVez = false, fim = false, flagLeitura = false; 

	char linsLetras[DIMENSAO+1] = "HGFEDCBA"; //passando como vai ficar no tabuleiro
	char colsNums[DIMENSAO+1] = "12345678";
	char *jogadaComeco = malloc (sizeof(char)*VALOR);
	char *jogadaFinal = malloc (sizeof(char)*VALOR);

	pedras->contDamas = 0;
	pedras->contJogadas = 0;
	fazendoTabuleiro(tabuleiro, linsLetras, colsNums);
	printf("\n");

	do{
		computador = false;

		if(strcmp(jogador[*i].nome, "computador") == 0){
			maquina(tabuleiro, jogador, &dama, jogadaComeco, jogadaFinal, linsLetras, colsNums, inteiros, *i);
			computador = true;
		}
		
		if(jogador[*i].vez == 1 || jogador[*i].vez == 2){

			capturaDama = true;
			printf("\n");
			if(!computador){

				printf("\t\t\t");
				printf("%s",jogador[*i].nome);
				printf(", faca um jogada valida: ");

				fgets(jogadaComeco, VALOR, stdin);
				int tam = strlen(jogadaComeco);
		        jogadaComeco[tam-1] = '\0';
			} 

			if(strlen(jogadaComeco) >= 3){
				flagLeitura = true;
			}

			if(strcmp(jogadaComeco, "sair") == 0){
				printf("\n");
				printf("\t\t\tVolte sempre!\n");
				flagLeitura = false;
				fim = true;
	 		} else if(strcmp(jogadaComeco, "salvar") == 0){
	 			arquivoSalvo(tabuleiro, jogador, *i);
	 			fazendoTabuleiro(tabuleiro, linsLetras, colsNums);
				printf("\t\t\tArquivo salvo com sucesso!\n");
				flagLeitura = false; 
				/*motivo: 'salvar' é uma string com tamanho maior que 3,
				 *portanto o bool flagLeitura virava true e caia na condicao
				 *de string de tamanho excedido
				 */
				continue;
			} else if(strlen(jogadaComeco) == 1){
				/*entra nessa condiçao caso o usuario tenha digitado uma jogada com menos de duas casas
				 *e o fgets irá pegar a proxima entrada apos o espaço para o destino
				 */
                
				fgets(jogadaFinal, VALOR, stdin);
				int tam = strlen(jogadaFinal);
		        jogadaFinal[tam-1] = '\0';

				printf("\t\t\tJogada invalida! \n");
                
			} else if(flagLeitura){
				/*caso o usuario digite mais de 3 caracteres na jogada origem, 
				 *sera atribuido à jogada destino o conteudo da variavel que jogadaComeco aponta!
				 *motivo: antes de utilizar esse metodo, o programa sempre esperava por uma entrada
				 *apos digitar a jogadaComeco
				 */
				*jogadaFinal = *jogadaComeco;
				printf("\t\t\tJogada invalida! Digite uma jogada por vez e aperte enter.\n");
				flagLeitura = false;
                
			} else {
				if(!computador)
                    fgets(jogadaFinal, VALOR, stdin);
					int tam = strlen(jogadaFinal);
		        	jogadaFinal[tam-1] = '\0';
				if((verificaJogada(jogadaComeco, linsLetras, colsNums) == true) 
				&& (verificaJogada(jogadaFinal, linsLetras, colsNums) == true)){

					TransformaCharParaInt(jogadaComeco, jogadaFinal, inteiros);

					if(certificaJogada(tabuleiro, jogador, inteiros, &dama, *i) == true){
						mapearCaptura(tabuleiro, jogador, possibilidade, *i);

						if(certificaCaptura(tabuleiro, jogador, inteiros, &dama, *i) == true && !dama){

							if(possibilidadeCaptura(possibilidade) == true && !repetirVez){
								moveCapturaPeca(tabuleiro, jogador, inteiros, possibilidade, *i);

								pedras->contDamas = 0;

								mapearCaptura(tabuleiro, jogador, possibilidade, *i);

								if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == false){
									repetirVez = false;
									if(vitoria(tabuleiro, jogador, *i) == false){
										transformaDama(tabuleiro, inteiros, jogador, *i);
										defineVez(i);
									}
								} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true && 
								abs(inteiros->linsOrigem - inteiros->linsDestino) == 1 && abs(inteiros->colsOrigem - inteiros->colsDestino) == 1){
									repetirVez = false;
									if(vitoria(tabuleiro, jogador, *i) == false){
										transformaDama(tabuleiro, inteiros, jogador, *i);
										defineVez(i);
									}
								} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true){
									linRepetir = inteiros->linsDestino;
									colRepetir = inteiros->colsDestino;
									repetirVez = true;
								}

							} else if(possibilidadeCaptura(possibilidade) == true && repetirVez){
								if(inteiros->linsOrigem == linRepetir && inteiros->colsOrigem == colRepetir){
									moveCapturaPeca(tabuleiro, jogador, inteiros, possibilidade, *i);

									pedras->contDamas = 0;

									mapearCaptura(tabuleiro, jogador, possibilidade, *i);

									if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == false){
										repetirVez = false;
										if(vitoria(tabuleiro, jogador, *i) == false)
											defineVez(i);
									} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true && 
									abs(inteiros->linsOrigem - inteiros->linsDestino) == 1 && abs(inteiros->colsOrigem - inteiros->colsDestino) == 1){
										repetirVez = false;
										if(vitoria(tabuleiro, jogador, *i) == false)
											defineVez(i);
									} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true){
										linRepetir = inteiros->linsDestino;
										colRepetir = inteiros->colsDestino;
										repetirVez = true;
									}
								} else {
									if(!computador){
                                        printf ("\n");
										printf("\t\t\tVoce deve continuar a captura!\n");
									} 
									else {
										continue;
									}
								}
							} else {
								if(!computador){
									printf("\t\t\tMovimento invalido!\n");
								}
								else {
									continue;
								}
							}

						} else if(certificaCaptura(tabuleiro, jogador, inteiros, &dama, *i) == true && dama){

							if(possibilidadeCaptura(possibilidade) == true && !repetirVez){
								moveCapturaDama(tabuleiro, jogador, inteiros, possibilidade, &capturaDama, *i);
								mapearCaptura(tabuleiro, jogador, possibilidade, *i);

								pedras->contDamas = 0;

								if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == false){
									repetirVez = false;
									if(vitoria(tabuleiro, jogador, *i) == false)
										defineVez(i);
								} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true && 
								abs(inteiros->linsOrigem - inteiros->linsDestino) == 1 && abs(inteiros->colsOrigem - inteiros->colsDestino) == 1){
									repetirVez = false;
									if(vitoria(tabuleiro, jogador, *i) == false)
										defineVez(i);
								} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true){
									linRepetir = inteiros->linsDestino;
									colRepetir = inteiros->colsDestino;
									repetirVez = true;
								} 
							} else if(possibilidadeCaptura(possibilidade) == true && repetirVez){
								if(inteiros->linsOrigem == linRepetir && inteiros->colsOrigem == colRepetir){
									moveCapturaDama(tabuleiro, jogador, inteiros, possibilidade, &capturaDama, *i);
									mapearCaptura(tabuleiro, jogador, possibilidade, *i);

									pedras->contDamas = 0;

									if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == false){
										repetirVez = false;
										if(vitoria(tabuleiro, jogador, *i) == false)
											defineVez(i);
									} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true && 
									abs(inteiros->linsOrigem - inteiros->linsDestino) == 1 && abs(inteiros->colsOrigem - inteiros->colsDestino) == 1){
										repetirVez = false;
										if(vitoria(tabuleiro, jogador, *i) == false)
											defineVez(i);
									} else if(possibilidade[inteiros->linsDestino][inteiros->colsDestino].captura == true){
										linRepetir = inteiros->linsDestino;
										colRepetir = inteiros->colsDestino;
										repetirVez = true;
									}
								} else {
									if(!computador){
										printf("\n");
										printf("\t\t\tVoce deve continuar a captura!\n");
									} else {
										continue;
									}
								}
							} else if(possibilidadeCaptura(possibilidade) == false){
								pedras->contDamas++;
								movePeca(tabuleiro, inteiros, jogador, *i);
								if(vitoria(tabuleiro, jogador, *i) == false)
									defineVez(i);
							}  
						} else if(possibilidadeCaptura(possibilidade) == false && certificaCaptura(tabuleiro, jogador, inteiros, &dama, *i) == false) {
							pedras->contDamas = 0;
							movePeca(tabuleiro, inteiros, jogador, *i);
							if(vitoria(tabuleiro, jogador, *i) == false){
								transformaDama(tabuleiro, inteiros, jogador, *i);
								defineVez(i);
							}			
						} 
						else if(possibilidadeCaptura(possibilidade) == true && certificaCaptura(tabuleiro, jogador, inteiros, &dama, *i) == false){
							if(!computador){
								printf("\t\t\tJogada invalida, e obrigatorio captura a peca adversaria!\n");
							} else {
								continue;
							}
						}
						fazendoTabuleiro(tabuleiro, linsLetras, colsNums);
                        printf("\n");
					} else {
						if(!computador){
							printf("\t\t\tJogada invalida! Digite um valor que atenda os requisitos.\n");
						} else {
							continue;
						}
					}
				} else {
					if(!computador){
						printf("\t\t\tSuas jogadas nao atendem aos requisitos!\n");
					} else {
						continue;
					}
				}
			}

			if(!capturaDama){
				defineVez(i);
				if(!computador){
					printf("\t\t\tJogada invalida, eh obrigatorio captura a peca adversaria!\n");
				} else {
					continue;
				}
			}
			if(vitoria(tabuleiro, jogador, *i) == true){
				if(!computador){
		 			printf("\t\t\tParabens %s", jogador[*i].nome);
                    printf(", voce venceu!\n");
				} else {
					printf("\t\t\tcomputador ganhou!\n");
				}
				fim = true;
			} else if(empate(tabuleiro, pedras) == true){
				printf("\t\t\tEmpate!\n");
				fim = true;
			} 

			if(fim){
				memoriaFree(tabuleiro, possibilidade, jogador, jogadaComeco, jogadaFinal, pedras, inteiros);
				break;
			}
		}

	} while(!fim);
}

void maquina(char** tabuleiro, jogadorInf* jogador, bool* dama, char* jogadaComeco, char* jogadaFinal, 
             char letras[], char numeros[], Transforma* inteiros, int indice){
	bool flag; //armazena o valor do retorno da função certificaJogada

	do{
			//essa funcao da um valor aleatório na faixa entre 0 a 7
			jogadaComeco[0] = rand() % 8 + 'A'; 
			jogadaComeco[1] = rand() % 8 + '1';
			jogadaComeco[2] = '\0';
			jogadaFinal[0] = rand() % 8 + 'A';
			jogadaFinal[1] = rand() % 8 + '1';
			jogadaFinal[2] = '\0';

			if(verificaJogada(jogadaComeco, letras, numeros) == true && verificaJogada(jogadaFinal, letras, numeros) == true){
				TransformaCharParaInt(jogadaComeco, jogadaFinal, inteiros);
				flag = certificaJogada(tabuleiro, jogador, inteiros, dama, indice);
			} 
           
	} while (!flag); /*flag = certificaJogada, ou seja, quando nao validar a jogada vai repetir ate ficar ok(!flag)
	                  *repete o processo até que a maquina/pc gere as jogadas validas
	                  *o intuito era esse
					  */
}

bool verificaJogada(char *jogada, char letras[], char numeros[]){
	int cont = 0;

	/*verifica se a jogada está dentro dos vetores
	 *correspondentes às linhas e colunas
	 */
	for(int i = 0; i < DIMENSAO; i++){
		if(jogada[0] == letras[i]){
			cont++;
		} else if(jogada[1] == numeros[i]){
			cont++;
		}
	}
    
	/*fgets sempre terá um '\0' no final,
	 *logo é necessario a validacao
	 */
	if(jogada[2] == '\0')
		cont++;

	if(cont == 3){
		return true;
	}
	return false;
}
    /*função abs () em C devolve o valor absoluto de um número inteiro.
     *O valor absoluto de um número é sempre positivo. Apenas os valores inteiros são suportados em C.
	 */
void TransformaCharParaInt(char *origem, char *destino, Transforma *inteiros){
	//Transformando char pra int (de acordo com a tabela ASCII)
	inteiros->linsOrigem = abs((int)origem[0] - 72);
	inteiros->colsOrigem = (int)origem[1] - 49;
	inteiros->linsDestino = abs((int)destino[0] - 72);
	inteiros->colsDestino = (int)destino[1] - 49;
}

bool certificaJogada(char **tabuleiro, jogadorInf *jogador, Transforma *inteiros, bool* dama, int i){
	/*as primeiras condições são pra validar as condicoes do tabuleiro;
	 *a terceira condicao faz chamada de uma função booleana e valida apenas se ela retornar true
	 */
	if((tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'o') 
		&& (tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' ')
		&& (jogador[i].vez == 1) && ((inteiros->linsOrigem - 1) == inteiros->linsDestino)
		&& (abs(inteiros->colsOrigem - inteiros->colsDestino) == 1)){
		*dama = false;
		return true;
	} 
	else if ((tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'x') 
		&& (tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' ')
		&& (jogador[i].vez == 2) && ((inteiros->linsOrigem + 1) == inteiros->linsDestino)
		&& (abs(inteiros->colsOrigem - inteiros->colsDestino) == 1)){
		*dama = false;
		return true;
	} 
	else if(certificaCaptura(tabuleiro,jogador, inteiros, dama, i) == true){
		return true;
	}

	return false;
}

bool certificaCaptura(char **tabuleiro, jogadorInf *jogador, Transforma *inteiros, bool* dama, int i){	
	//valida a captura de uma peça normal e o movimento de uma dama
	if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'o' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 1 && inteiros->linsOrigem - inteiros->linsDestino == 2
		&& (tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem - 1] == 'x' || tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem - 1] == 'X')
		&& inteiros->colsOrigem - inteiros->colsDestino == 2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'o' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 1 && inteiros->linsOrigem - inteiros->linsDestino == 2
		&& (tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem + 1] == 'x' || tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem + 1] == 'X')
		&& inteiros->colsOrigem - inteiros->colsDestino == -2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'o' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 1 && inteiros->linsOrigem - inteiros->linsDestino == -2
		&& (tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem - 1] == 'x' || tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem - 1] == 'X')
		&& inteiros->colsOrigem - inteiros->colsDestino == 2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'o' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 1 && inteiros->linsOrigem - inteiros->linsDestino == -2
		&& (tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem + 1] == 'x' || tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem + 1] == 'X')
		&& inteiros->colsOrigem - inteiros->colsDestino == -2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'x' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 2 && inteiros->linsOrigem - inteiros->linsDestino == 2
		&& (tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem - 1] == 'o' || tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem - 1] == 'O')
		&& inteiros->colsOrigem - inteiros->colsDestino == 2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'x' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 2 && inteiros->linsOrigem - inteiros->linsDestino == 2
		&& (tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem + 1] == 'o' || tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem + 1] == 'O')
		&& inteiros->colsOrigem - inteiros->colsDestino == -2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'x' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 2 && inteiros->linsOrigem - inteiros->linsDestino == -2
		&& (tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem - 1] == 'o' || tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem - 1] == 'O')
		&& inteiros->colsOrigem - inteiros->colsDestino == 2) {
		*dama = false;
		return true;
	} 
	else if(tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'x' && tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' '
		&& jogador[i].vez == 2 && inteiros->linsOrigem - inteiros->linsDestino == -2
		&& (tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem + 1] == 'o' || tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem + 1] == 'O')
		&& inteiros->colsOrigem - inteiros->colsDestino == -2) {
		*dama = false;
		return true;
	} 
	else if((tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'O') && (tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' ')
		&& (jogador[i].vez == 1) && (abs(inteiros->linsDestino - inteiros->linsOrigem) == abs(inteiros->colsDestino - inteiros->colsOrigem))){
		*dama = true;
		return moveDama(tabuleiro, inteiros, jogador, i);
	}
	else if((tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] == 'X') && (tabuleiro[inteiros->linsDestino][inteiros->colsDestino] == ' ')
		&& (jogador[i].vez == 2) && (abs(inteiros->linsDestino - inteiros->linsOrigem) == abs(inteiros->colsDestino - inteiros->colsOrigem))){
		*dama = true;
		return moveDama(tabuleiro, inteiros, jogador, i);
	}
	return false;
}

void mapearCaptura(char **tabuleiro, jogadorInf *jogador, Captura **mapeia, int k){
	/*inicializa a matriz com o valor false	
	 *e faz a chamada de duas funções 
	 */
	for(int i = 0; i < DIMENSAO; i++)
		for(int j = 0; j < DIMENSAO; j++)
			mapeia[i][j].captura = false;

	percorreCapturaPeca(tabuleiro, jogador, mapeia, k);

	percorreCapturaDama(tabuleiro, jogador, mapeia, k);
}

void percorreCapturaDama(char **tabuleiro, jogadorInf* jogador, Captura **mapeia, int indice){
	/*esta funcao tem o intuito de percorrer o tabuleiro e validar a captura da dama na
	 *posição caso atenda todos os requisitos da condição
	 */
	for(int i = 0; i < DIMENSAO; i++){
		for(int j = 0; j < DIMENSAO; j++){
			if(jogador[indice].vez == 1 && tabuleiro[i][j] == 'O'){
				if((i + 2) <= 7 && (j + 2) <= 7){ 
					if(i >= j){
						for(int k = 0; k < DIMENSAO - j; k++){
							if((i + k + 1) <= 7 && (j + k + 1) <= 7 && (i + k - 1) >= 0 && (j + k - 1) >= 0
							&& (tabuleiro[i + k][j + k] == 'x' || tabuleiro[i + k][j + k] == 'X') 
							&& tabuleiro[i + k + 1][j + k + 1] == ' ' && tabuleiro[i + k - 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k < DIMENSAO - i; k++){
							if((i + k + 1) <= 7 && (j + k + 1) <= 7 && (i + k - 1) >= 0 && (j + k - 1) >= 0
							&& (tabuleiro[i + k][j + k] == 'x' || tabuleiro[i + k][j + k] == 'X') 
							&& tabuleiro[i + k + 1][j + k + 1] == ' ' && tabuleiro[i + k - 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
			  				}
						}
					}
				} 
				if((i + 2) <= 7 && (j - 2) >= 0){
					if(i >= j){
						for(int k = 0; k <= j; k++){
							if((i + k + 1) <= 7 && (j - k - 1) >= 0 && (i + k - 1) >= 0 && (j - k + 1) <= 7
							&& (tabuleiro[i + k][j - k] == 'x' || tabuleiro[i + k][j - k] == 'X') 
							&& tabuleiro[i + k + 1][j - k - 1] == ' ' && tabuleiro[i + k - 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k < DIMENSAO - i; k++){
							if((i + k + 1) <= 7 && (j - k - 1) >= 0 && (i + k - 1) >= 0 && (j - k + 1) <= 7
							&& (tabuleiro[i + k][j - k] == 'x' || tabuleiro[i + k][j - k] == 'X') 
							&& tabuleiro[i + k + 1][j - k - 1] == ' ' && tabuleiro[i + k - 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						} 
					}
				} 
				if((i - 2) >= 0 && (j + 2) <= 7){
					if(i >= j){
						for(int k = 0; k < DIMENSAO - j; k++){
							if((i - k - 1) >= 0 && (j + k + 1) <= 7 && (i - k + 1) <= 7 && (j + k - 1) >= 0
							&& (tabuleiro[i - k][j + k] == 'x' || tabuleiro[i - k][j + k] == 'X')
							&& tabuleiro[i - k - 1][j + k + 1] == ' ' && tabuleiro[i - k + 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k <= i; k++){
							if((i - k - 1) >= 0 && (j + k + 1) <= 7 && (i - k + 1) <= 7 && (j + k - 1) >= 0
							&& (tabuleiro[i - k][j + k] == 'x' || tabuleiro[i - k][j + k] == 'X')
							&& tabuleiro[i - k - 1][j + k + 1] == ' ' && tabuleiro[i - k + 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					}
				} 
				if((i - 2) >= 0 && (j - 2) >= 0){
					if(i >= j){
						for(int k = 0; k <= j; k++){
							if((i - k - 1) >= 0 && (j - k - 1) >= 0 && (i - k + 1) <= 7 && (j - k + 1) <= 7
							&& (tabuleiro[i - k][j - k] == 'x' || tabuleiro[i - k][j - k] == 'X')
							&& tabuleiro[i - k - 1][j - k - 1] == ' ' && tabuleiro[i - k + 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k <= i; k++){
							if((i - k - 1) >= 0 && (j - k - 1) >= 0 && (i - k + 1) <= 7 && (j - k + 1) <= 7
							&& (tabuleiro[i - k][j - k] == 'x' || tabuleiro[i - k][j - k] == 'X')
							&& tabuleiro[i - k - 1][j - k - 1] == ' ' && tabuleiro[i - k + 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					}
				}
			} else if(jogador[indice].vez == 2 && tabuleiro[i][j] == 'X'){
				if((i + 2) <= 7 && (j + 2) <= 7){ 
					if(i >= j){
						for(int k = 0; k < DIMENSAO - j; k++){
							if((i + k + 1) <= 7 && (j + k + 1) <= 7 && (i + k - 1) >= 0 && (j + k - 1) >= 0
							&& (tabuleiro[i + k][j + k] == 'o' || tabuleiro[i + k][j + k] == 'O') 
							&& tabuleiro[i + k + 1][j + k + 1] == ' ' && tabuleiro[i + k - 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k < DIMENSAO - i; k++){
							if((i + k + 1) <= 7 && (j + k + 1) <= 7 && (i + k - 1) >= 0 && (j + k - 1) >= 0
							&& (tabuleiro[i + k][j + k] == 'o' || tabuleiro[i + k][j + k] == 'O') 
							&& tabuleiro[i + k + 1][j + k + 1] == ' ' && tabuleiro[i + k - 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					}
				} 
				if((i + 2) <= 7 && (j - 2) >= 0){
					if(i >= j){
						for(int k = 0; k <= j; k++){
							if((i + k + 1) <= 7 && (j - k - 1) >= 0 && (i + k - 1) >= 0 && (j - k + 1) <= 7
							&& (tabuleiro[i + k][j - k] == 'o' || tabuleiro[i + k][j - k] == 'O') 
							&& tabuleiro[i + k + 1][j - k - 1] == ' ' && tabuleiro[i + k - 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k < DIMENSAO - i; k++){
							if((i + k + 1) <= 7 && (j - k - 1) >= 0 && (i + k - 1) >= 0 && (j - k + 1) <= 7
							&& (tabuleiro[i + k][j - k] == 'o' || tabuleiro[i + k][j - k] == 'O') 
							&& tabuleiro[i + k + 1][j - k - 1] == ' ' && tabuleiro[i + k - 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						} 
					}
				} 
				if((i - 2) >= 0 && (j + 2) <= 7){
					if(i >= j){
						for(int k = 0; k < DIMENSAO - j; k++){
							if((i - k - 1) >= 0 && (j + k + 1) <= 7 && (i - k + 1) <= 7 && (j + k - 1) >= 0
							&& (tabuleiro[i - k][j + k] == 'o' || tabuleiro[i - k][j + k] == 'O')
							&& tabuleiro[i - k - 1][j + k + 1] == ' ' && tabuleiro[i - k + 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k <= i; k++){
							if((i - k - 1) >= 0 && (j + k + 1) <= 7  && (i - k + 1) <= 7 && (j + k - 1) >= 0
							&& (tabuleiro[i - k][j + k] == 'o' || tabuleiro[i - k][j + k] == 'O')
							&& tabuleiro[i - k - 1][j + k + 1] == ' ' && tabuleiro[i - k + 1][j + k - 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					}
				} 
				if((i - 2) >= 0 && (j - 2) >= 0){
					if(i >= j){
						for(int k = 0; k <= j; k++){
							if((i - k - 1) >= 0 && (j - k - 1) >= 0  && (i - k + 1) <= 7 && (j - k + 1) <= 7
							&& (tabuleiro[i - k][j - k] == 'o' || tabuleiro[i - k][j - k] == 'O')
							&& tabuleiro[i - k - 1][j - k - 1] == ' ' && tabuleiro[i - k + 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					} else {
						for(int k = 0; k <= i; k++){
							if((i - k - 1) >= 0 && (j - k - 1) >= 0 && (i - k + 1) <= 7 && (j - k + 1) <= 7
							&& (tabuleiro[i - k][j - k] == 'o' || tabuleiro[i - k][j - k] == 'O')
							&& tabuleiro[i - k - 1][j - k - 1] == ' ' && tabuleiro[i - k + 1][j - k + 1] == ' '){
								mapeia[i][j].captura = true;
							}
						}
					}
				}
			}
		}
	}
}

void percorreCapturaPeca(char **tabuleiro, jogadorInf* jogador, Captura **mapeia, int indice){
	 /*funcao que valida a captura da peça normal e da dama.
	  *obs:validei novamente a dama porque a função percorreCapturaDama
	  *não estava validando se tivesse uma pedrinha no lugar do espaço antes da peça a ser capturada
	  *basicemnte fez os passos da funcao acima novamente.
      */
	for(int i = 0; i < DIMENSAO; i++){
		for(int j = 0; j < DIMENSAO; j++){
			if(jogador[indice].vez == 1 && (tabuleiro[i][j] == 'o' || tabuleiro[i][j] == 'O')){
				if(((i - 2) >= 0 && (j - 2) >= 0) 
					&& (tabuleiro[i - 1][j - 1] == 'x' || tabuleiro[i - 1][j - 1] == 'X')
					&& tabuleiro[i - 2][j - 2] == ' '){
						mapeia[i][j].captura = true;
				} 
				else if(((i - 2) >= 0 && (j + 2) <= 7) 
					&& (tabuleiro[i - 1][j + 1] == 'x' || tabuleiro[i - 1][j + 1] == 'X') 
					&&  tabuleiro[i - 2][j + 2] == ' '){
						mapeia[i][j].captura = true;
				} 
				else if(((i + 2) <= 7 && (j - 2) >= 0) 
					&& (tabuleiro[i + 1][j - 1] == 'x' || tabuleiro[i + 1][j - 1] == 'X')
					&& tabuleiro[i + 2][j - 2] == ' '){
						mapeia[i][j].captura = true;
				} 
				else if(((i + 2) <= 7 && (j + 2) <= 7) 
					&& (tabuleiro[i + 1][j + 1] == 'x'|| tabuleiro[i + 1][j + 1] == 'X') 
					&& tabuleiro[i + 2][j + 2] == ' '){
						mapeia[i][j].captura = true;
				}
			} else if(jogador[indice].vez == 2 && (tabuleiro[i][j] == 'x' || tabuleiro[i][j] == 'X')){
				if(((i - 2) >= 0 && (j - 2) >= 0) 
					&& (tabuleiro[i - 1][j - 1] == 'o' || tabuleiro[i - 1][j - 1] == 'O')
					&& tabuleiro[i - 2][j - 2] == ' '){
						mapeia[i][j].captura = true;
				} 
				else if(((i - 2) >= 0 && (j + 2) <= 7) 
					&& (tabuleiro[i - 1][j + 1] == 'o' || tabuleiro[i - 1][j + 1] == 'O') 
					&&  tabuleiro[i - 2][j + 2] == ' '){
						mapeia[i][j].captura = true;
				} 
				else if(((i + 2) <= 7 && (j - 2) >= 0) 
					&& (tabuleiro[i + 1][j - 1] == 'o' || tabuleiro[i + 1][j - 1] == 'O')
					&& tabuleiro[i + 2][j - 2] == ' '){
						mapeia[i][j].captura = true;
				} 
				else if(((i + 2) <= 7 && (j + 2) <= 7) 
					&& (tabuleiro[i + 1][j + 1] == 'o'|| tabuleiro[i + 1][j + 1] == 'O') 
					&& tabuleiro[i + 2][j + 2] == ' '){
						mapeia[i][j].captura = true;
				}
			}
		}
	}	
}

bool possibilidadeCaptura(Captura **mapeia){
	/*percorre todo o tabuleiro verificando se há alguma (flag) de captura com o valor true
     *(flag) é um mecanismo que serve para controlar a "lógica" de seu programa.
	 *(interrompendo ou permitindo a execução de alguns comandos)
	 */
	for(int i = 0; i < DIMENSAO; i++){
		for(int j = 0; j < DIMENSAO; j++){
			if(mapeia[i][j].captura == true){
				return true;
			}
		}
	}
	return false;
}

void moveCapturaDama(char **tabuleiro, jogadorInf *jogador, Transforma *inteiros,
                           Captura **mapeia,bool* captura, int indice){
	int origemLinha, origemColuna, destinoLinha, destinoColuna;
	/*essa funcao foi criada porque mesmo tendo opcao de capturar uma peca, dava erro e passava a vez,
	 *logo, caso nao entre em nenhuma das condicoes validas se ha captura ou nao a variavel vai
	 *retornar a false e ira definir a vez.
	 */ 
	*captura = false; 


	origemLinha = inteiros->linsOrigem;
	origemColuna = inteiros->colsOrigem;
	destinoLinha = inteiros->linsDestino;
	destinoColuna = inteiros->colsDestino;

	//movimento de captura da dama
	if(tabuleiro[origemLinha][origemColuna] == 'O' && jogador[indice].vez == 1){
		if(origemLinha > destinoLinha && origemColuna > destinoColuna){
			if(origemLinha > origemColuna){ 
				for(int i = 0; i <= origemColuna; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i][origemColuna - i] == 'x'
					|| tabuleiro[origemLinha - i][origemColuna - i] == 'X')
					&& tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == ' '){

						tabuleiro[origemLinha - i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			} 
			else {
				for(int i = 0; i <= origemLinha; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i][origemColuna - i] == 'x'
					|| tabuleiro[origemLinha - i][origemColuna - i] == 'X')
					&& tabuleiro[origemLinha - i - 1][origemColuna - i - 1] ==' '){
						tabuleiro[origemLinha - i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
		}
		else if(origemLinha > destinoLinha && origemColuna < destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i][origemColuna + i] == 'x'
					|| tabuleiro[origemLinha - i][origemColuna + i] == 'X')
					&& tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha - i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
			else {
				for(int i = 0; i <= origemLinha; i++){
					if((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha - i][origemColuna + i] == 'x'
					|| tabuleiro[origemLinha - i][origemColuna + i] == 'X')
					&& tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha - i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
		}
		else if(origemLinha < destinoLinha && origemColuna > destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i <= origemColuna; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna - i] == 'x'
					|| tabuleiro[origemLinha + i][origemColuna - i] == 'X')
					&& tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna - i] == 'x'
					|| tabuleiro[origemLinha + i][origemColuna - i] == 'X')
					&& tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}

			}
		}
		else if(origemLinha < destinoLinha && origemColuna < destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna + i] == 'x'
					|| tabuleiro[origemLinha + i][origemColuna + i] == 'X')
					&& tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna + i] == 'x'
					|| tabuleiro[origemLinha + i][origemColuna + i] == 'X')
					&& tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
		}
	}

	if(tabuleiro[origemLinha][origemColuna] == 'X' && jogador[indice].vez == 2){
 		if(origemLinha > destinoLinha && origemColuna > destinoColuna){
			if(origemLinha > origemColuna){ 
				for(int i = 0; i <= origemColuna; i++){
					if((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna
					&& (tabuleiro[origemLinha - i][origemColuna - i] == 'o'
					|| tabuleiro[origemLinha - i][origemColuna - i] == 'O')
					&& tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == ' '){
						tabuleiro[origemLinha - i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			} 
			else {
				for(int i = 0; i <= origemLinha; i++){
					if((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna
					&& (tabuleiro[origemLinha - i][origemColuna - i] == 'o'
					|| tabuleiro[origemLinha - i][origemColuna - i] == 'O')
					&& tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == ' '){
						tabuleiro[origemLinha - i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
		}
		else if(origemLinha > destinoLinha && origemColuna < destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha - i][origemColuna + i] == 'o'
					|| tabuleiro[origemLinha - i][origemColuna + i] == 'O')
					&& tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha - i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
			else {
				for(int i = 0; i <= origemLinha; i++){
					if((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha - i][origemColuna + i] == 'o'
					|| tabuleiro[origemLinha - i][origemColuna + i] == 'O')
					&& tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha - i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
		}
		else if(origemLinha < destinoLinha && origemColuna > destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i <= origemColuna; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna - i] == 'o'
					|| tabuleiro[origemLinha + i][origemColuna - i] == 'O')
					&& tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna - i] == 'o'
					|| tabuleiro[origemLinha + i][origemColuna - i] == 'O')
					&& tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna - i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}

			}
		}
		else if(origemLinha < destinoLinha && origemColuna < destinoColuna){ 
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna + i] == 'o'
					|| tabuleiro[origemLinha + i][origemColuna + i] == 'O')
					&& tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha + i][origemColuna + i] == 'o'
					|| tabuleiro[origemLinha + i][origemColuna + i] == 'O')
					&& tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == ' '){
						tabuleiro[origemLinha + i][origemColuna + i] = ' ';
						movePeca(tabuleiro, inteiros, jogador, indice);
						*captura = true;
					}
				}
			}
		}
	}
}

void moveCapturaPeca(char **tabuleiro, jogadorInf *jogador, Transforma *inteiros,
                           Captura **mapeia, int indice){
	//esta funcao faz os movimentos de uma peça normal, tais como pra cima, pra direita, esquerda, pra baixo..
	if((mapeia[inteiros->linsOrigem][inteiros->colsOrigem].captura == true) && jogador[indice].vez == 1){
			
		movePeca(tabuleiro, inteiros, jogador, indice);

		if((inteiros->linsOrigem - inteiros->linsDestino) == -2){ //movimenta pra cima
			if((inteiros->colsOrigem - inteiros->colsDestino) == -2) //movimenta pra direita
				tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem + 1] = ' ';
			else if((inteiros->colsOrigem - inteiros->colsDestino) == 2) //movimenta pra esquerda
				tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem - 1] = ' ';
		} 
		else if((inteiros->linsOrigem - inteiros->linsDestino) == 2){ //movimenta pra baixo
			if((inteiros->colsOrigem - inteiros->colsDestino) == -2) //movimenta pra direita
				tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem + 1] = ' ';
			else if((inteiros->colsOrigem - inteiros->colsDestino) == 2) //movimenta pra esquerda
				tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem - 1] = ' ';
		}
	} 
	else if((mapeia[inteiros->linsOrigem][inteiros->colsOrigem].captura == true) && jogador[indice].vez == 2){
			
		movePeca(tabuleiro, inteiros, jogador, indice);

		if((inteiros->linsOrigem - inteiros->linsDestino) == -2){ //movimenta pra cima
			if((inteiros->colsOrigem - inteiros->colsDestino) == -2) //movimenta pra direita
				tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem + 1] = ' ';
			else if((inteiros->colsOrigem - inteiros->colsDestino) == 2) //movimenta pra esquerda
				tabuleiro[inteiros->linsOrigem + 1][inteiros->colsOrigem - 1] = ' ';
		} 
		else if((inteiros->linsOrigem - inteiros->linsDestino) == 2){ //movimenta pra baixo
			if((inteiros->colsOrigem - inteiros->colsDestino) == -2) //movimenta pra direita
				tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem + 1] = ' ';
			else if((inteiros->colsOrigem - inteiros->colsDestino) == 2) //movimenta pra esquerda
				tabuleiro[inteiros->linsOrigem - 1][inteiros->colsOrigem - 1] = ' ';
		}
	} 
	else {
		printf("\t\t\tCaptura obrigatoria!\n");
	}
}

bool moveDama(char **tabuleiro, Transforma *inteiros, jogadorInf *jogador, int indice){
	/* essa funcao esta validando o movimento da dama, 
	 *e tambem verificando os quadrantes para saber quais pecas estao nos quadrantes analisados
	 */
	int origemLinha, origemColuna, destinoLinha, destinoColuna;
	//variaveis criadas pra armazenar a quantidade pecas aliadas e a quantidade de inimigas da dama
	int contAliadas = 0, contInimigas = 0; 

	origemLinha = inteiros->linsOrigem;
	origemColuna = inteiros->colsOrigem;
	destinoLinha = inteiros->linsDestino;
	destinoColuna = inteiros->colsDestino;

	//valida o movimento da Dama
	if(jogador[indice].vez == 1 && tabuleiro[origemLinha][origemColuna] == 'O'){
		if(origemLinha > destinoLinha && origemColuna > destinoColuna){ 
            //o if acima verifica se no primeiro quadrante possui pecas aliadas e/ou peças inimigas
			if(origemLinha > origemColuna){ //o menor numero determina quantas casas a dama irá andar
				for(int i = 0; i <= origemColuna; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'O'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'X'))
						contInimigas++;
				}
			} 
			else {
				for(int i = 0; i <= origemLinha; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'O'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'X'))
						contInimigas++;
				}
			}
		}
		else if(origemLinha > destinoLinha && origemColuna < destinoColuna){ 
            //verificando se no segundo quadrante posssio peças aliadas e/ou peças inimigas
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'O'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'X'))
						contInimigas++;
				}
			}
			else {
				for(int i = 0; i <= origemLinha; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'O'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'X'))
						contInimigas++;
				}
			}
		}
		else if(origemLinha < destinoLinha && origemColuna > destinoColuna){ 
            //verificando se no terceiro quadrante possui peças aliadas e/ou peças inimigas
			if(origemLinha > origemColuna){
				for(int i = 0; i <= origemColuna; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'O'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'X'))
						contInimigas++;
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'O'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'X'))
						contInimigas++;
				}

			}
		}
		else if(origemLinha < destinoLinha && origemColuna < destinoColuna){ 
            //verificando se no quarto quadrante possui peças aliadas e/ou peças inimigas
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'O'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'X'))
						contInimigas++;
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'O'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'X'))
						contInimigas++;
				}
			}
		}
	} else if(jogador[indice].vez == 2 && tabuleiro[origemLinha][origemColuna] == 'X'){
		if(origemLinha > destinoLinha && origemColuna > destinoColuna){
			if(origemLinha > origemColuna){ 
				for(int i = 0; i <= origemColuna; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'X'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'O'))
						contInimigas++;
				}
			} 
			else {
				for(int i = 0; i <= origemLinha; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'X'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna - i - 1) >= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna - i - 1] == 'O'))
						contInimigas++;
				}
			}
		}
		else if(origemLinha > destinoLinha && origemColuna < destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'X'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'O'))
						contInimigas++;
				}
			}
			else {
				for(int i = 0; i <= origemLinha; i++){
					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'X'))
						contAliadas++;

					if(((origemLinha - i - 1) >= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha - i - 1][origemColuna + i + 1] == 'O'))
						contInimigas++;
				}
			}
		}
		else if(origemLinha < destinoLinha && origemColuna > destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i <= origemColuna; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'X'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'O'))
						contInimigas++;
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'X'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) >= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna - i - 1] == 'O'))
						contInimigas++;
				}

			}
		}
		else if(origemLinha < destinoLinha && origemColuna < destinoColuna){
			if(origemLinha > origemColuna){
				for(int i = 0; i < DIMENSAO - origemColuna; i++){
					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'X'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'O'))
						contInimigas++;
				}
			}
			else {
				for(int i = 0; i < DIMENSAO - origemLinha; i++){
					if((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'x'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'X'))
						contAliadas++;

					if(((origemLinha + i + 1) <= destinoLinha && (origemColuna + i + 1) <= destinoColuna)
					&& (tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'o'
					|| tabuleiro[origemLinha + i + 1][origemColuna + i + 1] == 'O'))
						contInimigas++;
				}
			}
		}
	}

	if(contAliadas == 0 && contInimigas <= 1){
		return true;
	}
	return false;
}

void movePeca(char **tabuleiro, Transforma *inteiros, jogadorInf *jogador, int i){
	 /*essa funcao foi feita para o movimento da peça, ou seja para passar um espaco em branco do
	  *lugar que ela sair e colocar a sua peca na posicao de destino
      */
	tabuleiro[inteiros->linsDestino][inteiros->colsDestino] = tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem];
	tabuleiro[inteiros->linsOrigem][inteiros->colsOrigem] = ' ';
}

void transformaDama(char **tabuleiro, Transforma *inteiros, jogadorInf *jogador, int indice){
	/* essa funcao transforma a peça em dama se ela atingir a ultima linha do lado oposto
	 *que ela foi inicializada e imprime a mensagem para o usuario. 
	 */
	for(int i = 0; i < DIMENSAO; i++){
		if(tabuleiro[0][i] == 'o'){
			printf("\t\t\tParabens %s",jogador[indice].nome);
            printf(", voce conseguiu uma dama!\n");
			tabuleiro[0][i] = 'O';
		} 
		if(tabuleiro[7][i] == 'x'){
			if(strcmp(jogador[i].nome, "computador") != 0){
                printf("\t\t\tParabens %s",jogador[indice].nome);
                printf(", voce conseguiu uma dama!\n");
			}else if(strcmp(jogador[i].nome, "computador") == 0){
                printf("\t\t\tcomputador conquistou uma dama!\n");
			} 
			tabuleiro[7][i] = 'X';
		}
	}
}

void defineVez(int *i){
	//essa funcao troca a vez
	if(*i == 0){
		*i = 1;
	} 
	else if(*i == 1){
		*i = 0;
	}
}

void arquivoSalvo(char **tabuleiro,jogadorInf *jogador, int num){
	//a funcao como o nome ja diz tem a funcao de salvar os arquivos do jogo
	FILE* saida; //meu apontador de arquivo
    
	//Pede o nome do arquivo apenas na primeira vez que for salva-lo.
	if(jogador->arquivo[0] == 0){
        printf("\t\t\tDigite o nome do arquivo: ");
        scanf("%s", jogador->arquivo);
	}
	//abre no arquivo com o intuito de gravar o conteudo nele
    saida = fopen (jogador->arquivo, "w");

	//conteudo a ser salvo no arquivo
	fprintf (saida, "%s\n", jogador[0].nome);
	fprintf (saida, "%s\n", jogador[1].nome);
    
	for(int i = DIMENSAO - 1; i >= 0; i--){
		for(int j = 0; j < DIMENSAO; j++){
			if((i+j) % 2 != 0){
				if(tabuleiro[i][j] == 'x' || tabuleiro[i][j] == 'o' || tabuleiro[i][j] == 'X' || tabuleiro[i][j] == 'O'){
					fprintf(saida, "%c ", tabuleiro[i][j]);
				} 
				else {
					fprintf(saida, "- ");
				}
			}
		}
		fprintf (saida, "\n");
	}
    fprintf (saida, "%d", num+1);
	//fecha o arquivo
	fclose (saida);
}

void carregarArquivo(jogadorInf *jogador, char** tabuleiro, int *num){
	FILE* entrada;

	do{		
		printf("\t\t\tDigite o nome do arquivo que voce deseja abrir: ");
        scanf("%s", jogador->arquivo);
		getchar();
		entrada = fopen (jogador->arquivo, "r"); 

		if(entrada == NULL){
			printf("\t\t\tFalha ao abrir o arquivo\n");
		} else {
			fgets(jogador[0].nome, TAM_MAX-1, entrada);
			fgets(jogador[1].nome, TAM_MAX-1,entrada);

			for(int i = DIMENSAO - 1; i >= 0; i--){
				for(int j = 0; j < DIMENSAO; j++){
					if((i+j) % 2 != 0)
						fscanf(entrada, "%s", &tabuleiro[i][j]);
					else
						tabuleiro[i][j] = ' ';
				}
			}
            
			fscanf(entrada, "%d", num);
			*num -= 1;
			jogador[0].vez = 1;
			jogador[1].vez = 2;

			for(int i = 0; i < DIMENSAO; i++){
				for(int j = 0; j < DIMENSAO; j++){
					if(tabuleiro[i][j] == '-'){
						tabuleiro[i][j] = ' ';
					}
				}
			}

			
		}
	}while(feof(entrada));
	//enquanto houver a falha e nao chegar ao final do arquivo
	fclose(entrada);
	verificacoes (jogador, tabuleiro, num);
}

bool empate(char** m, Elementos *pedras){
	//essa funcao tem o intuito de percorrer o tabuleiro e verificar se ha empate
	pedras->contPecasX = 0;
	pedras->contPecasO = 0;
	pedras->contDamaX = 0;
	pedras->contDamaO = 0; 

	for(int i = 0; i < DIMENSAO; i++){
		for(int j = 0; j < DIMENSAO; j++){
			if(m[i][j] == 'o')
				pedras->contPecasO++;
			else if(m[i][j] == 'O')
				pedras->contDamaO++;
			else if(m[i][j] == 'x')
				pedras->contPecasX++;
			else if(m[i][j] == 'X')
				pedras->contDamaX++;
		}
	}

	if(pedras->contDamaO == 2 && pedras->contDamaX == 2 
	&& pedras->contPecasX == 0 && pedras->contPecasO == 0)
		pedras->contJogadas++;
	else if((pedras->contDamaO == 2 && pedras->contDamaX == 1 && pedras->contPecasX == 0 && pedras->contPecasO == 0)
	|| (pedras->contDamaX == 2 && pedras->contDamaO == 1 && pedras->contPecasX == 0 && pedras->contPecasO == 0))
		pedras->contJogadas++;
	else if((pedras->contDamaO == 2 && pedras->contDamaX == 1 && pedras->contPecasX == 1 && pedras->contPecasO == 0)
	|| (pedras->contDamaX == 2 && pedras->contDamaO == 1 && pedras->contPecasO == 1 && pedras->contPecasX == 0))
		pedras->contJogadas++;
	else if((pedras->contDamaO == 1 && pedras->contDamaX == 1 &&  pedras->contPecasX == 0 && pedras->contPecasO == 0) 
	|| (pedras->contDamaX == 1 && pedras->contDamaO == 1 &&  pedras->contPecasX == 0 && pedras->contPecasX == 0))
		pedras->contJogadas++;
	else if((pedras->contDamaO == 1 && pedras->contDamaX == 1 &&  pedras->contPecasX == 1 && pedras->contPecasO == 0) 
	|| (pedras->contDamaX == 1 && pedras->contDamaO == 1 &&  pedras->contPecasX == 0 && pedras->contPecasO == 1))
		pedras->contJogadas++;

	if(pedras->contJogadas == 10)
		return true;
	else if(pedras->contDamas == 40)
		return true;
	return false;
}

bool vitoria(char** tabuleiro, jogadorInf* jogador, int indice){
	//essa funcao verifica a vitoria, como o nome ja diz
	for(int i = 0; i < DIMENSAO; i++){
		for(int j = 0; j < DIMENSAO; j++){
			if(jogador[indice].vez == 1 && (tabuleiro[i][j] == 'x' || tabuleiro[i][j] == 'X')){
				return false;
			} else if(jogador[indice].vez == 2 && (tabuleiro[i][j] == 'o' || tabuleiro[i][j] == 'O')){
				return false;
			}
		}
	}
	return true;
}

void memoriaFree(char **M, Captura **N, jogadorInf *jogador, char* jogadaComeco, char* jogadaFinal, Elementos* pecinhas, Transforma* inteiros){
	//libera os vetores e matrizes alocados dinamicamente
	for(int i = 0; i < DIMENSAO; i++){ 
		free (M[i]);
		free (N[i]);
	}
    free (M);
	free (N);
	free (jogador);
	free (jogadaComeco);
	free (jogadaFinal);
	free (pecinhas);
	free (inteiros);
}