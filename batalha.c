/**
 * @autores:
 * Nome: Artur Putyato      Número de Aluno: 47948
 * Nome: Alexandre Nunes    Número de Aluno: 48267
 * Nome: David Quadrado     Número de Aluno: 48188
 **/ 

#include <stdio.h>
#include <string.h>


#define B 6  //número de barcos
#define N 8  //dimensão x
#define M 8  //dimensão y

/**Representa uma coordenada*/
typedef struct
{
    int x;
    int y;
} Position;

/**Associa cada coordenada do barco com um estado**/
typedef struct
{
    int afloat;        //0 indica que está afundada, 1 indica que não
    Position pos;      //Coordenada
} StateCoord;

/**Representa um barco**/
typedef struct
{
    int afloat;        //Posições que ainda não foram afundadas
    int tSize;         //Tamanho do tipo do barco 
    StateCoord coord[5]; //O barco maior tem 5 coordenadas, usando o tSize garantimos que acedemos apenas às existentes
    char type;         //Caracter que representa o tipo do barco
} Boat;

/**Representa um tabuleiro**/
typedef struct
{
    int numBoatsAfloat; //Número de barcos não afundados
    int numBoats; //Número de barcos que foram colocados
    Boat boats[B]; //Array de barcos. Cada barco é do tipo Boat.
    char board[N][M]; //Array que contém a informação de cada posição do tabuleiro
} Board;

/**
 * Function: init_board
 * 
 * Inicializa um Board.
 * 
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * b: apontador para o Board que vai ser inicializado
 * 
 **/

void init_board(int n, int m, Board *b)
{
    b->numBoatsAfloat = B; //inicialmente, o número de barcos não afundados é B (máximo)
    b->numBoats = 0;  //inicialmente, o número de barcos colocados é 0

    //Inicializa o array board
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        { 
            b->board[i][j] = ' ';
        }
    }
}

/**
 * Function: print_board
 * 
 * Mostra no ecrã o estado atual do tabuleiro.
 * Se a flag é 0 (zero) não mostra os barcos, se for 1, mostra. 
 * Quando a flag é 1, para indicar a posição de cada barco, é colocada em todas
 * as posições do barco a letra que representa o seu tipo.
 * 
 * n: número de linhas do tabuleiro
 * m: número de colunas do tabuleiro
 * board: estado do tabuleiro
 * flag: indicador do modo de visualização do tabuleiro -- se for 0 (zero) não 
 *       mostra os barcos; se for diferente de 0 (zero) mostra.
 */

void print_board(int n, int m, char board[n][m], int flag)
{
    printf("+");
    for (int j = 0; j < n; j++)
    {
        printf("---+");
    }
    printf("\n");

    for (int i = 0; i < m; i++)
    {
        printf("|");
        if (flag==1) //Mostra tudo
        {
            for (int j = 0; j < n; j++)
            {
                printf(" %c |", board[i][j]);
            }
            printf("\n");
        }
        else // Esconde os barcos
        {
            for (int j = 0; j < n; j++)
            {
                printf("   |");
            }
            printf("\n");
        }

        printf("+");
        for (int j = 0; j < n; j++)
        {
            printf("---+");
        }
        printf("\n");
    }
}


/** 
 * Function: typeToSize
 * 
 * Dado o tipo do barco devolve o seu tamanho.
 * 
 * type: tipo do barco ('P', 'N', 'C', ou 'S')
 * 
 * return
 *  -1 se o tipo de barco for inválido
 *  caso contrário, devolve o tamanho do barco
 **/
int typeToSize(char type)
{
    int tamanho;
    if(type=='P'){
        tamanho=5;
    }
    else if(type=='N'){
        tamanho=4;
    }
    else if(type=='C'){
        tamanho=3;
    }
    else if(type=='S'){
        tamanho=2;
    }   
    else{
        tamanho=-1;

    }

    return tamanho;
}

/**
 * Function: init_boat
 * 
 * Dado um apontador para um barco, o tipo, a posição inicial e a direcção, 
 * inicializa o barco com todas as sua posições, tamanho e tipo. 
 * Por exemplo, se o tipo do barco é 'C' (contratorpedeiro),
 * a posição inicial (1,1), e a direcção 'H', então o barco fica inicializado 
 * com os seguintes valores
 * 
 * afloat = 3;
 * tSize = 3;
 * coord = [(1,(1,1)),(1,(1,2)),(1,(1,3))]; Onde (s,(x,y)) representa um elemento do tipo StateCoord
 * type = 'C';
 * 
 * b: barco que vai ser inicializado
 * type: tipo do barco
 * xy: posição inicial do barco
 * dir: direcção do barco
 * 
 **/
void init_boat(Boat *b, char type, Position xy, char dir)
{
    b->afloat=typeToSize(type); //inicialmente, o barco tem todas as posições ñ afundadas
    b->tSize=b->afloat; //o tamanho do barco, inicialmente, é equivalente ao número de posições não afundadas
    b->type=type; //tipo do barco
    for(int i=0; i<typeToSize(type); i++){
        b->coord[i].afloat=1; //inicialmente, todas as coordenadas têem o status 1 (não afundado)

        if(dir=='V'){
        b->coord[i].pos.x=xy.x+i; //se a direção for vertical, então as coordenadas serão do tipo [(1,(x+0,y)),(1,(x+1,y))...]
        b->coord[i].pos.y=xy.y;
        }
        else{
        b->coord[i].pos.x=xy.x; //se a direção for não-vertical(horizontal), então as coordenadas serão do tipo [(1,(x,y+0)),(1,(x,y+1))...]
        b->coord[i].pos.y=xy.y+i;

        }
    }
}



/**
 * Function: check_free
 * 
 * Verifica se estão livres todas as posições que serão ocupadas por um 
 * barco no tabuleiro.
 * 
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * boat: barco a colocar no tabuleiro
 * board: estado atual do tabuleiro
 * 
 * returns:
 *  1 se todas as posições estão livres
 *  0 caso contrário
 * 
 **/

int check_free(int n, int m, Boat *boat, char board[n][m])
{
   int k=1; //inicialmente, pressupõe-se que todas as posições estão livres
   int *p=&k;
   if(boat->coord[0].pos.x-boat->coord[1].pos.x==0){ //se a variação das coordenadas x for nula, então o barco está na horizontal
       if((boat->coord[0].pos.y+typeToSize(boat->type))>m){
           *p=0; //se a coordenada extrema do y estiver fora do tabuleiro, então a posição é inválida
       }
       else{ //se for válida:
        for(int i=0; i<typeToSize(boat->type); i++){ //verifica posição a posição se o tabuleiro estiver livre
           if(board[boat->coord[0].pos.x][boat->coord[0].pos.y+i]!=' '){ 
               *p=0; //quando encontrar uma posição ocupada, dá-se o valor 0 e sai-se do ciclo for
               break;
           }
            else{
                *p=1; //se não encontrar, vai dando a k o valor 1
            }
       }
       }   

   }
    else if(boat->coord[0].pos.y-boat->coord[1].pos.y==0){ //se a variação das coordenadas y for nula, então o barco está na vertical
       //esta parte segue a mesma lógica da parte anterior
        if((boat->coord[0].pos.x+typeToSize(boat->type))>n){
            *p=0;
        }
        else{
        for(int i=0; i<typeToSize(boat->type); i++){
           if(board[boat->coord[0].pos.x+i][boat->coord[0].pos.y]!=' '){
               *p=0;
               break;
           }
            else{
                *p=1;
            }
       }
       }
    }
      
   


   return k;
}

/** 
 * Function: place_boat
 * 
 * Cria e coloca, se tal for possível, um barco no tabuleiro.
 * 
 * x1: coordenada x inicial do barco  
 * y1: coordenada y inicial do barco  
 * dir: dirrecção do barco ('H' ou 'V')
 * type: o tipo de barco 
 * board: estado atual do tabuleiro
 *
 * returns: 
 * 0 se o barco for colocado com sucesso.
 * -1 se alguma das posições já está ocupada.
 * -2 se as coordenadas forem inválidas.
 * -3 se a direcção for inválida.
 * -4 se o tipo de barco for inválido.
 **/
int place_boat(int x1, int y1, int dir, char type, Board *board)
{
    int w; //valor de retorno
    int a; //corresponde ao board->numBoats
    int b=typeToSize(type); //tamanho do barco
    int *q=&w; //ponteiro para o valor de retorno

    if(dir=='H'){      
        if(y1+b>M+1){  //se a direção for horizontal e a coordenada extrema estiver fora do tabuleiro, então retorna inválido
            *q=-2;

        }
    }

    else if(dir=='V'){
        if(x1+b>N+1){ //se a direção for vertical e a coordenada extrema estiver fora do tabuleiro, então retorna inválido
            *q=-2;

        }
        
    }
    
    if(*q!=-2){ //se a coordenada extrema estiver dentro do tabuleiro:

    if (dir != 'H' && dir != 'V'){ //se a direção for diferente de horizontal e de vertical, então direção inválida
        *q=-3;
    }
        if(*q!=-3){ //se a direção estiver bem:
        if(type!='P' && type!='C' && type!='N' && type!='S'){ //se o tipo não corresponder aos existentes, então tipo inválido
        *q=-4;
    }
    if(*q!=-4){ //se o tipo estiver bem então pode-se prosseguir

    a=board->numBoats;//permite aceder às posições do array de boats

    board->boats[a].coord[0].pos.x=x1; //dá-se à primeira coordenada do board as coordenadas passadas para a função
    board->boats[a].coord[0].pos.y=y1;


    init_boat(board->boats+a, type, board->boats[a].coord->pos, dir); //inicia o boat
    if((check_free(N, M, board->boats+(a), board->board)==1)){ //se as posições estiverem livres, então o tabuleiro irá imprimir o barco
        for(int e=0; e<b;e++){
            if(dir=='H'){
            board->board[x1][y1+e]=type;
            }
            else{
                board->board[x1+e][y1]=type;
            }
        }
        board->numBoats++; //incrementa o valor, para depois aceder à próxima posição do array de boats
        *q=0;
    }
    else{ //caso alguma posição estiver ocupada, então retorna -1. E não ocorre a incrementação do numBoats, permitindo reescrever o boat inválido anterior
        *q=-1;
    }
    }
    }
    }

    return w;
}

/**
 * Function: check_sink
 * 
 * Verifica se ao atacar a posição (x,y) algum barco é afundado. 
 * 
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 * 
 * returns:
 *   'F' se não for afundado nenhum barco.
 *   Se afundar um barco devolve a letra que representa o seu tipo: 'P', 'N', 'C' ou 'S' 
 *   'I' se a coordenada for inválida.
 * 
 **/
char check_sink(int x, int y, Board *board)
{
    char d; //valor de retorno
    char *l=&d; //ponteiro para o valor de retorno
    int flag=0; //flag que será usada como uma espécie de discriminante
    for(int m=0; m<N; m++){ //verifica se a coordenada x for inválida
            if(x==m){
                *l='K'; //valor intermédio
                break;
            }
            else{
                *l='I'; //enquanto não encontrar a coordenada, vai dando o valor I (inválido), quando encontrar dará o valor K e sai do ciclo for
            }
    }
    if(*l!='I'){ //caso a coordenada x esteja inválida, então não vale a pena continuar 

    for(int n=0; n<M; n++){ //segue a mesma lógica da coordenada x
        if(y==n){
                *l='K';
                break;
            }
        else{
                *l='I';
            }
    }
    if(*l=='K'){ //se tudo estiver bem, então o valor de d será K, caso contrário sai do programa
        for(int i=0; i<B; i++){ //permite percorrer o array de boats
            for(int j=0; j<typeToSize(board->boats[i].type); j++){ //permite percorrer o array de coordenadas
                if(flag==0){ //enquanto não verifica que o barco foi afundado ou lesionado, então flag=0, caso contrário acaba a verificação
                if(board->boats[i].afloat==1 && board->boats[i].coord[j].pos.x==x && board->boats[i].coord[j].pos.y==y && board->boats[i].coord[j].afloat==1){ 
                    //se o número de posição não afundadas do barco for 1, se as coordenadas de ataque corresponderem as do boat e se é essa a única coordenada não abatida então:
                    *l=board->boats[i].type; //valor de retorno é o tipo do barco
                    flag++; //flag incrementa e a verificação pára

                        
                }
                else if(board->boats[i].afloat>1 && board->boats[i].coord[j].pos.x==x && board->boats[i].coord[j].pos.y==y && board->boats[i].coord[j].afloat==1){
                    //se o número de posição não afundadas do barco for maior 1, se as coordenadas de ataque corresponderem as do boat e se é essa uma das coordenadas não abatidas então:
                    *l='F'; //não afunda
                    flag++; //flag incrementa e a verificação pára
                }
                else{
                    *l='F'; //caso contrário, vai-se dando o valor F, no sentido que falhou
                }
                }        
    }
        }

    }
    }

    return d;

}

/**
 * Function: target
 * 
 * Ataca a coordenada (x,y) no tabuleiro.
 * Se a coordenada (x,y) é parte de um barco, então coloca um '*' nessa 
 * posição '*'; caso contrário coloca a letra 'F' nessa posição; caso afunde 
 * um barco coloca em todas as posições desse barco a letra 'A'.
 * 
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 * 
 * returns:
 *   0 se a posicao já foi atacada anteriormente
 *   1 se acerta numa coordenada de um barco mas sem o afundar
 *   -1 se nao tiver sucesso
 *   -2 se a coordenada for inválida. 
 *   caso afunde um barco, devolve o número correspondente ao tamanho do barco 
 *   que afundou (valor de 2 a 5)
 * 
 **/
int target(int x, int y, Board *board)
{
    int a; //valor de retorno
    int *z=&a; //ponteiro para o valor
    int flag=0; //flag discriminante tal como na função anterior
    if(check_sink(x, y, board)=='I'){ //se as coordenadas forem inválidas, então retorna -2
        *z=-2;
    }
    else{ //caso contrário:

    for(int i=0; i<B; i++){ //permite percorrer o array de boats
        for(int j=0; j<typeToSize(board->boats[i].type); j++){ //permite percorrer o array de coordenadas
            if(flag==0){ //se a flag discriminante estiver a 0, então vai verificar
            if(board->boats[i].coord[j].pos.x==x && board->boats[i].coord[j].pos.y==y){ //se ao percorrer os dois arrays encontrar um determinado barco i com as coordenadas j iguais às passadas pela função:
                if((check_sink(x,y, board)=='P' || check_sink(x,y, board)=='N' || check_sink(x,y, board)=='C' || check_sink(x,y, board)=='S')&& board->boats[i].coord[j].afloat==1){
                    //se ao verificar afundamento, a função checksink retornar um char correspondente ao tipo (barco afundado) e se essa coordenada não estiver afundada, então:
                    board->boats[i].coord[j].afloat=0; //afunda-se e:
                    for(int k=0; k<typeToSize(board->boats[i].type); k++){
                        board->board[board->boats[i].coord[k].pos.x][board->boats[i].coord[k].pos.y]='A'; //imprime A em todas as posições do barco
                        flag++; //incrementa o flag para parar a verificação
                        *z=typeToSize(board->boats[i].type); //retorna o tamanho
                    }
                    board->numBoatsAfloat--; //decrementa o número de barcos não afundados

                    }
                else if(check_sink(x, y, board)=='F' && board->boats[i].coord[j].afloat==1){ //se o barco não afundar e for essa a coordenada não afundada a ser atingida:
                    board->boats[i].coord[j].afloat=0; //coordenada afundada
                    board->boats[i].afloat--; //número de posições do barco não afundadas decrementa
                        board->board[x][y]='*'; //imprime no tabuleiro *
                        *z=1; //retorna 1 (acertar, sem afundar)
                        flag++; //pára a verificação


                    }
                else{ // caso contrário, a posição já foi atacada
                    *z=0;
                    flag ++;
                    
                }
            


            }
            else { //se não forem encontradas as coordenadas correspondentes no array de barcos, então nenhum barco foi atingido
                *z=-1;

            }

            }
        }
                    
    }
    }
    if(a==-1){ //se não for atingido, então imprime F
        board->board[x][y]='F';
    }

   
    return a;
}


int main(void)
{

    Board brd; //tabuleiro com toda a informaçãp
    Board jogo; //tabuleiro que será mostrado ao jogador atacante aquando do ataque

    int dirnum, x, y;
    int a;
    int verificaquantidade; //variável associada ao número de navios disponíveis
    char player1[50]; //nome do jogador 1
    char player2[50]; //nome do jogador 2
    int jogadas;
    char dir[6]; //array que permite guardar as direções dos barcos aquando da disposição dos mesmos no tabuleiro

    //--------------------Quantidade de navios disponíveis--------------------------------
    int contratorpedeiros; 
    int naviotanque;
    int portavioes;
    int submarinos;
    //-------------------------------------------------------------------------------------
    int ataque; //quantidade de ataques
    char quit; //variável relacionada com o saír do jogo
    char rein; //variável relacionada com o reínicio do jogo
    char ngame='K'; //variável relacionada com uma nova ronda 
    int score1=0; //pontuação inicial=0
    int score2=0;
    char intermed[50]; //string intermédia usada ,aquando de uma nova ronda, de trocar os nomes
    int intermedscore; //valor que permite a troca de valores da pontuação aquando de uma nova ronda
    int cheat; //flag que discrimina batota
    repeat: //lable que permite reíniciar o jogo

    //-------------------valores iniciais que devem ser 'resetados' antes de um novo jogo----------------
        quit='0';
        rein='S';
        ngame='K';
    //----------------------------------------------------------------------------------------------------

    //logótipo
    printf("\n");
    printf("     ***** **                                  ***                            *                          \n");
    printf("  ******  ***                  *         *      ***                         **          *                \n");
    printf(" **   *  * **                 **        **       **                         **         ***               \n");
    printf("*    *  *  **                 **        **       **                         **          *                \n");
    printf("    *  *   *                ********  ********   **                 ****    **                   ****    \n");
    printf("   ** **  *        ****    ********  ********    **       ***      * **** * **  ***   ***       * ***  * \n");
    printf("   ** ** *        * ***  *    **        **       **      * ***    **  ****  ** * ***   ***     *   ****  \n");
    printf("   ** ***        *   ****     **        **       **     *   ***  ****       ***   ***   **    **    **   \n");
    printf("   ** ** ***    **    **      **        **       **    **    ***   ***      **     **   **    **    **   \n");
    printf("   ** **   ***  **    **      **        **       **    ********      ***    **     **   **    **    **   \n");
    printf("   *  **     ** **    **      **        **       **    *******         ***  **     **   **    **    **   \n");
    printf("      *      ** **    **      **        **       **    **         ****  **  **     **   **    **    **   \n");
    printf("  ****     ***  **    **      **        **       **    ****    * * **** *   **     **   **    *******    \n");
    printf(" *  ********     ***** **      **        **      *** *  *******     ****    **     **   *** * ******     \n");
    printf("*     ****        ***   **                        ***    *****               **    **    ***  **         \n");
    printf("*     ****        ***   **                        ***    *****               **    **    ***  **         \n");
    printf("*                                                                                  *          **         \n");
    printf(" **                                                                               *           **         \n");
    printf("                                                                                 *            **        \n");
    printf("                                                                                *              **        \n");
    printf("                                                                               *                        \n");
    printf("\n");
    //indicação dos nomes dos jogadores
    printf("Indique o nome do primeiro jogador: ");
    scanf("%s", player1);

    printf("Indique o nome do segundo jogador: ");
    scanf("%s", player2);

    do{ //este 'do-while' corresponde a uma nova ronda e permite resetar os valores aquando de um novo jogo (devido às propriedades deste loop)
        contratorpedeiros=2;
        naviotanque=1;
        portavioes=1;
        submarinos=2;
        jogadas=0;
        cheat=0;
        if(ngame=='S'){ //se corresponder a um novo jogo, então a informação é trocada, pois aquele que foi atacado irá atacar
        strcpy(intermed, player1);
        strcpy(player1, player2);
        strcpy(player2, intermed);
        intermedscore=score1;
        score1=score2;
        score2=intermedscore;
        }
    //manual inicial    
    printf(" _____________________________________________________________________\n");
    printf("|                                                                     |\n");
    printf("|         __   __    _____   _     _  _     _   _____   _             |\n");
    printf("|        (__)_(__)  (_____) (_)   (_)(_)   (_) (_____) (_)            |\n");
    printf("|       (_) (_) (_)(_)___(_)(__)_ (_)(_)   (_)(_)___(_)(_)            |\n");
    printf("|       (_) (_) (_)(_______)(_)(_)(_)(_)   (_)(_______)(_)            |\n");    
    printf("|       (_)     (_)(_)   (_)(_)  (__)(_)___(_)(_)   (_)(_)____        |\n");
    printf("|       (_)     (_)(_)   (_)(_)   (_) (_____) (_)   (_)(______)       |\n");
    printf("|                                                                     |\n");
    printf("|                                                                     |\n");
    printf("|    1. INICIALIZAÇÃO DO JOGO                                         |\n");
    printf("|     1.1. Passos iniciais                                            |\n");
    printf("|         Inserir o nome dos jogadores; (já feito)                    |\n");
    printf("|         O jogador a defender irá posicionar os seus barcos;         |\n");
    printf("|     1.2 Disposição dos barcos/navios                                |\n");
    printf("|         a) A coordenada x tem uma orientação vertical,              |\n");
    printf("|       de cima para baixo (começa com x=0);                          |\n");
    printf("|         b) A coordenada y tem uma orientação horizontal,            |\n");
    printf("|       da esquerda para a direita (começa com y=0);                  |\n");
    printf("|         C) Existem 4 tipos de barcos:                               |\n");
    printf("|           1) S-Submarino (quantidade: 2, tamanho: 2 casas);         |\n");
    printf("|           2) C-Contratorpedeiro (quantidade: 2, tamanho: 3 casas);  |\n");
    printf("|           3) N-Navio-tanque (quantidade: 1, tamanho: 4 casas);      |\n");
    printf("|           4) P-Porta-aviões (quantidade: 1, tamanho: 5 casas);      |\n");
    printf("|    2. ABORTAGEM DO JOGO                                             |\n");
    printf("|      2.1. Durante todas as etapas será disponibilizada              |\n");
    printf("|       essa informação;                                              |\n");
    printf("|    3. SITUAÇÃO DE VITÓRIA                                           |\n");
    printf("|      3.1. Caso o atacante não tenha conseguido afundar todos        |\n");
    printf("|       os barcos, então o defensor ganha;                            |\n");
    printf("|      3.2. Caso o atacante peça para visualizar o tabuleiro          |\n");
    printf("|       com a informação dos barcos do adversário, perde;             |\n");
    printf("|      3.3. A única maneira de o atacante ganhar, é afundar todos     |\n");
    printf("|       os barcos;                                                    |\n");
    printf("|                                                                     |\n");
    printf("|                                                                     |\n");
    printf("|_____________________________________________________________________|\n");
    



    

    init_board(N, M, &brd); 
    print_board(N, M, brd.board, 0); //imprime o board com os barcos escondidos
    for (int i=0; i<B; i++){ //ciclo que permite colocar todos os barcos
    do{
        do{

    printf("\n%s, indique o tipo do %dº navio que pretende colocar (Caso queira abortar o jogo, indique Q):\t",player1, i+1);
    scanf(" %c", &brd.boats[i].type);
    //--------------------------------passa a quantidade disponível para a variáve verificaquantidade-------------------------------
    if(brd.boats[i].type=='C'){
        verificaquantidade=contratorpedeiros;
    }
    else if(brd.boats[i].type=='S'){
        verificaquantidade=submarinos;
    }
    else if (brd.boats[i].type=='N')
    {
        verificaquantidade=naviotanque;
    }
    else if (brd.boats[i].type=='P')
    {
        verificaquantidade=portavioes;
    }
    //-------------------------------------------------------------------------------------------------------------------------------
    if (verificaquantidade==0){// se a quantidade estiver indisponível, será dado aviso
        printf("\n\nEscolha outro tipo de barco, pois usou todos os barcos que podia deste tipo (%c)!\n\n", brd.boats[i].type);
    }
    else if(verificaquantidade>0){ //se houver quantidade, então a quantidade será decrementada
    if(brd.boats[i].type=='C'){
        contratorpedeiros--;
    }
    else if(brd.boats[i].type=='P'){

        portavioes--;

    }
    else if(brd.boats[i].type=='S'){

        submarinos--;

    }
    else if(brd.boats[i].type=='N'){

        naviotanque--;

    }
    }

    if(brd.boats[i].type=='Q'){ //se for escolhido tipo Q
        quit='Q';
    }
    

        }while((verificaquantidade==0 || (brd.boats[i].type!='C' && brd.boats[i].type!='P' && brd.boats[i].type!='N' && brd.boats[i].type!='S')) && quit=='0'); //o ciclo só repetirá se o tipo for inválido, quantidade nula ou se o jogador quer sair
    if(quit=='Q'){ //se for escolhido sair então a lable ask irá perguntar se o jogador quer reiniciar ou mesmo acabar o jogo
        goto ask;
    }

    printf("Indique as coordenadas da primeira posição (para abortar o jogo imprime -1 a qualquer momento):\n");

    printf("x: ");
    scanf(" %d", &x);
    if(x==-1){ //será perguntado se quiser sair ou reiniciar o jogo
        goto ask;
    }
    printf("y: ");
    scanf(" %d", &y);
    if(y==-1){
        goto ask;
    }
    
    do{
    printf("\n");
    printf("Prima 1 para orientação Horizontal e 0 para orientação vertical:\t");
    printf("\nPara abortar o jogo imprime -1 a qualquer momento\n\n");

    scanf(" %d", &dirnum);
    if (dirnum==1){
        dir[i]='H';
    }
    else if (dirnum==0){
        dir[i]='V';
    }
    else if(dirnum==-1){ //caso se quiser saír
        goto ask;
    }
    }while(dirnum!=1 && dirnum!=0);

    
    
    a=place_boat(x, y, dir[i], brd.boats[i].type, &brd);

    if(a!=0){ //se o barco de um determinado tipo for mal-posto, então o contador do seu tipo será incrementado (pois foi previamente decrementado)
        printf("\n\nOs dados foram mal introduzidos, por favor repita!\n\n");
        if(brd.boats[i].type=='C'){
        contratorpedeiros++;
    }
        else if(brd.boats[i].type=='P'){

        portavioes++;

    }
        else if(brd.boats[i].type=='S'){

        submarinos++;

    }
    else if(brd.boats[i].type=='N'){

        naviotanque++;

    }
    }
    printf("\n-----------------------------------------------------------------------------------------------------\n\n");
    print_board(N, M, brd.board, 1);
    //indica a quantidade de barcos disponíveis
    printf(" __________________________________________________________________________________\n");
    printf("|                                                                                  |\n");
    printf("|  %d-Submarinos(S) %d-Contratorpedeiros(C) %d-Navio-tanque(N) %d-Porta-Aviões(P)      |\n", submarinos, contratorpedeiros, naviotanque,portavioes);
    printf("|__________________________________________________________________________________|\n\n");

    }while(a!=0); //Se o barco for mal posto, então será re-pedido

    
    
    init_board(N, M, &jogo); //inicializa o board de impressão para o outro jogador

    }

    printf("\e[1;1H\e[2J"); //permite apagar o terminal para o atacante não ver as posições do defensor
    
    printf("\n%s já adicionou todos os barcos correctamente! Agora o %s irá tentar afundá-los!\n\n", player1, player2); 
    printf(" ___________________________________________________\n");
    printf("|                                                   |\n");
    printf("|                MENÚ DE OPÇÕES                     |\n");
    printf("|                                                   |\n");
    printf("|    1) Imprima -1 para saír do jogo;               |\n");
    printf("|    2) Para imprimir o tabuleiro com os barcos,    |\n");
    printf("|    imprima -2. AVISO, CASO SEJA UTILIZADO-PERDE   |\n");
    printf("|    , contudo o jogo continua;                     |\n");
    printf("|___________________________________________________|\n\n");
    printf("%s, o terminal pertence agora a %s!\n\n", player1, player2);
    print_board(N, M, jogo.board, 1);
    do{
        do{
    printf("\nIndique as coordenadas a atacar. Caso queira acabar o jogo, escreva -1:\n");
    printf("%s tem %d jogadas restantes.\n", player2, 40-jogadas); //jogadas inicialmente são 0
    printf("x: "); //pede-se o x
    scanf("%d", &x);
    if(x==-1){ //verifica se o utilizador quiser saír
        goto ask;
    }
    else if(x==-2){ //se for pedido o tabuleiro, então o jogador atacante perde, mas jogo continua
        printf("%s perdeste :(\n", player2);
        printf("Mas podes continuar a jogar.\n");
        cheat++; //flag cheat incrementa
        print_board(N, M, brd.board, 1);
        printf("x: ");
        scanf("%d", &x);
    }
    printf("y: ");
    scanf("%d", &y);
    printf("\n");
    if(y==-1){ //mesma lógica para o x
        goto ask;
    }
    else if(y==-2){
        printf("%s perdeste :(\n", player2);
        printf("Mas podes continuar a jogar.\n");
        cheat++;
        print_board(N, M, brd.board, 1);
    }

    printf("\n-----------------------------------------------------------------------------------------------------\n\n");
    ataque=target(x,y, &brd);
    if (ataque==-2){
        printf(" ____________________________\n");
        printf("|                            |\n");
        printf("|     COORDENADA INVÁLIDA    |\n");
        printf("|____________________________|\n");
        print_board(N, M, jogo.board, 1);
        
    }
    else if(jogo.board[x][y]!=' '){
        printf(" ____________________________\n");
        printf("|                            |\n");
        printf("|       JÁ FOI ATACADO       |\n");
        printf("|____________________________|\n");
        print_board(N, M, jogo.board, 1);
    }
    
    }while( (ataque==-2 || (ataque==0 || jogo.board[x][y]!=' ')));
        
        if(ataque==-1){
            jogo.board[x][y]='F';
            printf(" ____________________________\n");
            printf("|                            |\n");
            printf("|       NÃO ACERTASTE!       |\n");
            printf("|____________________________|\n");
        }
        else if(ataque==1){
            jogo.board[x][y]='*';
            printf(" ____________________________\n");
            printf("|                            |\n");
            printf("|    ACERTASTE NUM BARCO!    |\n");
            printf("|____________________________|\n");
        }
        else{
            for(int i=0; i<B; i++){
        for(int j=0; j<typeToSize(brd.boats[i].type); j++){
            if(brd.boats[i].coord[j].pos.x==x && brd.boats[i].coord[j].pos.y==y){
                    for(int k=0; k<typeToSize(brd.boats[i].type); k++){
                        jogo.board[brd.boats[i].coord[k].pos.x][brd.boats[i].coord[k].pos.y]='A';
                    }
                    printf(" ____________________________\n");
                    printf("|                            |\n");
                    printf("|     AFUNDASTE UM BARCO!    |\n");
                    printf("|____________________________|\n");
                    if(brd.boats[i].type=='S'){
                            submarinos++;
                        }
                    else if(brd.boats[i].type=='N'){
                            naviotanque++;
                        }
                    else if(brd.boats[i].type=='P'){
                            portavioes++;
                        }
                    else if(brd.boats[i].type=='C'){
                            contratorpedeiros++;
                        }
                    }
               
                    
    }
    }

        }

    
    print_board(N, M, jogo.board, 1);
    printf(" ______________________________________________________________________\n");
    printf("|                                                                      |\n");
    printf("|  NAVIOS POR AFUNDAR:                                                 |\n");
    printf("|                                                                      |\n");
    printf("|  %d-Submarinos %d-Contratorpedeiros %d-Navio-tanque %d-Porta-Aviões      |\n", 2-submarinos, 2-contratorpedeiros, 1-naviotanque, 1-portavioes);
    printf("|______________________________________________________________________|\n");


    if(brd.numBoatsAfloat==0){ //se todos os barcos afundarem, as jogadas passam diretamente para 40
        jogadas=40;
    }

    jogadas++; //incrementa a jogada

    }while(jogadas<40); //o ataque ocorre quando houverem menos de 40 jogadas
    if(brd.numBoatsAfloat==0 && cheat==0){//se afundar todos os barcos e se a flag cheat for 0 (não fez batota), então ganha
        printf("\n%s vence!\n", player2);
        score2++;
    }
    else{ //caso contrário o jogador1 ganha
        printf("\n%s vence!\n", player1);
        score1++;
    }
    if(score1>score2){
        printf("O resultado é %d-%d em favor do(a) %s \n", score1, score2, player1);
    }
    else if(score2>score1){
        printf("O resultado é %d-%d em favor do(a) %s \n", score1, score2, player2);
    }
    else{ //se a pontuação for igual, então é empate
        printf("O resultado é %d-%d! Empate! \n", score1, score2);

    }
    
    printf("Quer jogar mais uma ronda? Sim (S) ou N(não):\n");
    scanf(" %c", &ngame);
    }while(ngame=='S');
    ask:
        printf("Quer recomeçar o jogo? S(sim) ou N(não):\n");
        scanf(" %c", &rein);
        if(rein=='S')goto repeat; //reínicia o jogo
        else goto sair;
    
    sair: //se for usada esta lable, então o programa acaba de executar
    printf("Adeus!\n");
    

    return 0;
}
