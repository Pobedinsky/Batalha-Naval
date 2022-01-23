/**
 * @autores:
 * Nome: Artur Putyato      Número de Aluno: 47948
 * Nome: Alexandre Nunes    Número de Aluno: 48267
 * Nome: David Quadrado     Número de Aluno: 48188
 **/ 

#include <stdio.h>
#include <string.h>


#define B 6
#define N 8
#define M 8

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



void init_board(int n, int m, Board *b)
{
    b->numBoatsAfloat = B;
    b->numBoats = 0;

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
    b->afloat=typeToSize(type);
    b->tSize=b->afloat;
    b->type=type;
    for(int i=0; i<typeToSize(type); i++){
        b->coord[i].afloat=1;

        if(dir=='V'){
        b->coord[i].pos.x=xy.x+i;
        b->coord[i].pos.y=xy.y;
        }
        else{
        b->coord[i].pos.x=xy.x;
        b->coord[i].pos.y=xy.y+i;

        }
    }
}



int check_free(int n, int m, Boat *boat, char board[n][m])
{
   int k=1;
   int *p=&k;
   if(boat->coord[0].pos.x-boat->coord[1].pos.x==0){
       if((boat->coord[0].pos.y+typeToSize(boat->type))>M){
           *p=0;
       }
       else{
        for(int i=0; i<typeToSize(boat->type); i++){
           if(board[boat->coord[0].pos.x][boat->coord[0].pos.y+i]!=' '){
               *p=0;
               break;
           }
            else{
                *p=1;
            }
       }
       }   

   }
    else if(boat->coord[0].pos.y-boat->coord[1].pos.y==0 && (boat->coord[0].pos.x+typeToSize(boat->type))<N+1){
        if((boat->coord[0].pos.x+typeToSize(boat->type))>N){
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
      
   
    printf("%d\n", k);


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
    int w;
    int a;
    int b=typeToSize(type);
    int *q=&w;

    if(dir=='H'){
        if(y1+b>M+1){
            *q=-2;

        }
    }

    else if(dir=='V'){
        if(x1+b>N+1){
            *q=-2;

        }
        
    }
    
    if(*q!=-2){

    if (dir != 'H' && dir != 'V'){
        *q=-3;
    }
        if(*q!=-3){
        if(type!='P' && type!='C' && type!='N' && type!='S'){
        *q=-4;
    }
    if(*q!=-4){

    a=board->numBoats;

    board->boats[a].coord[0].pos.x=x1;
    board->boats[a].coord[0].pos.y=y1;


    init_boat(board->boats+a, type, board->boats[a].coord->pos, dir);
    if((check_free(N, M, board->boats+(a), board->board)==1)){
        for(int e=0; e<b;e++){
            if(dir=='H'){
            board->board[x1][y1+e]=type;
            }
            else{
                board->board[x1+e][y1]=type;
            }
        }
        board->numBoats++;
        *q=0;
    }
    else{
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
    char d;
    char *l=&d;
    int flag=0;
    for(int m=0; m<N; m++){
            if(x==m){
                *l='K';
                break;
            }
            else{
                *l='I';
            }
    }
    if(*l!='I'){

    for(int n=0; n<M; n++){
        if(y==n){
                *l='K';
                break;
            }
        else{
                *l='I';
            }
    }
    if(*l=='K'){
        for(int i=0; i<B; i++){
            for(int j=0; j<typeToSize(board->boats[i].type); j++){
                if(flag==0){
                if(board->boats[i].afloat==1 && board->boats[i].coord[j].pos.x==x && board->boats[i].coord[j].pos.y==y && board->boats[i].coord[j].afloat==1){
                    *l=board->boats[i].type;
                    flag++;

                        
                }
                else if(board->boats[i].afloat>1 && board->boats[i].coord[j].pos.x==x && board->boats[i].coord[j].pos.y==y && board->boats[i].coord[j].afloat==1){
                    *l='F';
                    flag++;
                }
                else{
                    *l='F';
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
    int a;
    int *z=&a;
    int flag=0;
    if(check_sink(x, y, board)=='I'){
        *z=-2;
    }
    else{

    for(int i=0; i<B; i++){
        for(int j=0; j<typeToSize(board->boats[i].type); j++){
            if(flag==0){
            if(board->boats[i].coord[j].pos.x==x && board->boats[i].coord[j].pos.y==y){
                if((check_sink(x,y, board)=='P' || check_sink(x,y, board)=='N' || check_sink(x,y, board)=='C' || check_sink(x,y, board)=='S')&& board->boats[i].coord[j].afloat==1){
                    board->boats[i].coord[j].afloat=0;
                    for(int k=0; k<typeToSize(board->boats[i].type); k++){
                        board->board[board->boats[i].coord[k].pos.x][board->boats[i].coord[k].pos.y]='A';
                        flag++;
                        *z=typeToSize(board->boats[i].type);
                    }
                    board->numBoatsAfloat--;

                    }
                else if(check_sink(x, y, board)=='F' && board->boats[i].coord[j].afloat==1){
                    board->boats[i].coord[j].afloat=0;
                    board->boats[i].afloat--;
                        board->board[x][y]='*';
                        *z=1;
                        flag++;


                    }
                else{
                    *z=0;
                    flag ++;
                    
                }
            


            }
            else {
                *z=-1;

            }

            }
        }
                    
    }
    }
    if(a==-1){
        board->board[x][y]='F';
    }

   
    return a;
}


int main(void)
{

    Board brd;
    Board jogo;

    int dirnum, x, y;
    int a;
    int verificaquantidade;
    char player1[50];
    char player2[50];
    int jogadas;
    char dir[6];
    int contratorpedeiros;
    int naviotanque;
    int portavioes;
    int submarinos;
    int ataque;
    char quit;
    char rein;
    char ngame='K'; //ok
    int score1=0;
    int score2=0;
    char intermed[50];
    int intermedscore;
    int cheat;
    repeat:
        quit='0';
        rein='S';
        ngame='K';
    
    
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

    printf("Indique o nome do primeiro jogador: ");
    scanf("%s", player1);

    printf("Indique o nome do segundo jogador: ");
    scanf("%s", player2);

    do{
        contratorpedeiros=2;
        naviotanque=1;
        portavioes=1;
        submarinos=2;
        jogadas=0;
        cheat=0;
        if(ngame=='S'){
        strcpy(intermed, player1);
        strcpy(player1, player2);
        strcpy(player2, intermed);
        intermedscore=score1;
        score1=score2;
        score2=intermedscore;
        }
        
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
    print_board(N, M, brd.board, 0);
    for (int i=0; i<B; i++){
    do{
        do{

    printf("\n%s, indique o tipo do %dº navio que pretende colocar (Caso queira abortar o jogo, indique Q):\t",player1, i+1);
    scanf(" %c", &brd.boats[i].type);
    
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
    if (verificaquantidade==0){
        printf("\n\nEscolha outro tipo de barco, pois usou todos os barcos que podia deste tipo!\n\n");
    }
    if(verificaquantidade>0){
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

    if(brd.boats[i].type=='Q'){
        quit='Q';
    }
    

        }while((verificaquantidade==0 || (brd.boats[i].type!='C' && brd.boats[i].type!='P' && brd.boats[i].type!='N' && brd.boats[i].type!='S')) && quit=='0');
    if(quit=='Q'){
        goto ask;
    }

    printf("Indique as coordenadas da primeira posição (para Abortar o jogo imprime -1 a qualquer momento):\n");

    printf("x: ");
    scanf(" %d", &x);
    if(x==-1){
        goto ask;
    }
    printf("y: ");
    scanf(" %d", &y);
    if(y==-1){
        goto ask;
    }
    
    do{
    printf("\n-----------------------------------------------------------------------------------------------------\n\n");
    printf("Prima 1 para orientação Horizontal e 0 para orientação vertical:\t");
    printf("\nPara Abortar o jogo imprime -1 a qualquer momento\n\n");

    scanf(" %d", &dirnum);
    if (dirnum==1){
        dir[i]='H';
    }
    else if (dirnum==0){
        dir[i]='V';
    }
    else if(dirnum==-1){
        goto ask;
    }
    }while(dirnum!=1 && dirnum!=0);

    
    
    a=place_boat(x, y, dir[i], brd.boats[i].type, &brd);

    if(a!=0){
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
    printf(" __________________________________________________________________________________\n");
    printf("|                                                                                  |\n");
    printf("|  %d-Submarinos(S) %d-Contratorpedeiros(C) %d-Navio-tanque(N) %d-Porta-Aviões(P)      |\n", submarinos, contratorpedeiros, naviotanque,portavioes);
    printf("|__________________________________________________________________________________|\n\n");

    }while(a!=0);

    
    
    init_board(N, M, &jogo);

    }
    puts( "\x1b[2J\x1b[1;1H");
    puts( "\x1b[2J\x1b[1;1H");
    puts( "\x1b[2J\x1b[1;1H");
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
    printf("\nIndique as coordenadas a atacar. Caso queira desistir, escreva -1:\n");
    printf("%s tem %d jogadas restantes.\n", player2, 40-jogadas);
    printf("x: ");
    scanf("%d", &x);
    if(x==-1){
        goto ask;
    }
    else if(x==-2){
        printf("%s perdeste :(\n", player2);
        cheat++;
        print_board(N, M, brd.board, 1);
        printf("x: ");
        scanf("%d", &x);
    }
    printf("y: ");
    scanf("%d", &y);
    printf("\n");
    if(y==-1){
        goto ask;
    }
    else if(y==-2){
        printf("%s perdeste :(\n", player2);
        cheat++;
        print_board(N, M, brd.board, 1);
    }
    
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
                    printf("|    AFUNDASTE  UM BARCO!    |\n");
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


    if(brd.numBoatsAfloat==0){
        jogadas=40;
    }

    jogadas++;

    }while(jogadas<40);
    if(brd.numBoatsAfloat==0 && cheat==0){
        printf("\n%s vence!\n", player2);
        score2++;
    }
    else{
        printf("\n%s vence!\n", player1);
        score1++;
    }
    if(score1>score2){
        printf("O resultado é %d-%d em favor do(a) %s \n", score1, score2, player1);
    }
    else if(score2>score1){
        printf("O resultado é %d-%d em favor do(a) %s \n", score1, score2, player2);
    }
    else{
        printf("O resultado é %d-%d! Empate! \n", score1, score2);

    }
    
    printf("Quer jogar mais uma ronda? Sim (S) ou N(não):\n");
    scanf(" %c", &ngame);
    }while(ngame=='S');
    ask:
        printf("Quer recomeçar o jogo? S(sim) ou N(não):\n");
        scanf(" %c", &rein);
        if(rein=='S')goto repeat;
        else goto sair;
    
    sair:
    printf("Adeus!\n");
    

    return 0;
}
