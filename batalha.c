/**
 * @autores:
 * Nome: Artur Putyato      Número de Aluno:
 * Nome:                    Número de Aluno:
 * Nome:                    Número de Aluno:
 **/ 

#include <stdio.h>


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



int check_free(int n, int m, Boat *boat, char board[N][M])
{
   int k=1;
   int *p=&k;
   if(boat->coord[0].pos.x-boat->coord[1].pos.x==0){
       if((boat->coord[0].pos.y+typeToSize(boat->type))>M){
           *p=0;
       }
       else{
        for(int i=0; i<typeToSize(boat->type); i++){
           if(board[n][m+i]!=' '){
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
           if(board[n+i][m]!=' '){
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
    if((check_free(x1, y1, board->boats+(a), board->board)==1)){
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
    char type;
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
    int jogadas=0;
    char dir[6];
    int contratorpedeiros=2;
    int naviotanque=1;
    int portavioes=1;
    int submarinos=2;
    int ataque;

    printf("Bem-vindos ao Battleship! \n \n");

    

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
    printf("|     1. INICIALIZAÇÃO DO JOGO                                        |\n");
    printf("|      1.1. Passos iniciais                                           |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          b) Selecionar 1 para continuar                             |\n");
    printf("|      1.2 Disposição dos barcos/navios                               |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|          a) Inserir o nome dos jogadores                            |\n");
    printf("|                                                                     |\n");
    printf("|_____________________________________________________________________|\n");

    printf("Indique o nome do primeiro jogador: ");
    fgets (player1, 50, stdin);
    printf("Indique o nome do segundo jogador: ");
    fgets (player2, 50, stdin);

    init_board(N, M, &brd);
    print_board(N, M, brd.board, 0);
    for (int i=0; i<B; i++){
    do{
        do{

    printf("Indique o tipo do %dº navio que pretende colocar:\t", i+1);
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
        printf("\n Escolha outro tipo de barco, pois usou todos os barcos que podia deste tipo!\n\n");
    }
        }while(verificaquantidade==0);
    printf("Indique as coordenadas da primeira posição:\t");
    scanf(" %d", &x);
    printf("PORRA, MAIS\n");
    scanf(" %d", &y);
    
    do{
    printf("Prima 1 para orientação Horizontal e 0 para orientação vertical:\t");
    scanf(" %d", &dirnum);
    if (dirnum==1){
        dir[i]='H';
    }
    else if (dirnum==0){
        dir[i]='V';
    }
    }while(dirnum!=1 && dirnum!=0);

    
    
    a=place_boat(x, y, dir[i], brd.boats[i].type, &brd);

    if(a!=0){
        printf("\n\nOs dados foram mal introduzidos, por favor repita!\n\n");
    }
    else if(a==0 && brd.boats[i].type=='C'){
        contratorpedeiros--;
    }
    else if(brd.boats[i].type=='P'){

        portavioes--;

    }
    else if(a==0 && brd.boats[i].type=='S'){

        submarinos--;

    }
    else if(a==0 && brd.boats[i].type=='N'){

        naviotanque--;

    }

    }while(a!=0);

    
    print_board(N, M, brd.board, 1);
    init_board(N, M, &jogo);

    }
    puts( "\x1b[2J\x1b[1;1H");
    printf("\n Já adicionou todos os barcos correctamente! Agora o adversário irá tentar afundá-los!\n");
    print_board(N, M, jogo.board, 1);
    do{
        do{
    printf("\nIndique as coordenadas a atacar:\n");
    printf("Tem %d restantes \n", 40-jogadas);
    scanf("%d", &x);
    scanf("%d", &y);
    ataque=target(x,y, &brd);
    if (ataque==-2){
        printf("Coordenada inválida! Repita pfv:\n");
    }
    else if(jogo.board[x][y]!=' '){
        printf("Esta coordenada já foi atacada!\n");
    }
    
    }while( (ataque==-2 || (ataque==0 || jogo.board[x][y]!=' ')));
        
        if(ataque==-1){
            jogo.board[x][y]='F';
        }
        else if(ataque==1){
            jogo.board[x][y]='*';
        }
        else{
            for(int i=0; i<B; i++){
        for(int j=0; j<typeToSize(brd.boats[i].type); j++){
            if(brd.boats[i].coord[j].pos.x==x && brd.boats[i].coord[j].pos.y==y){
                    for(int k=0; k<typeToSize(brd.boats[i].type); k++){
                        jogo.board[brd.boats[i].coord[k].pos.x][brd.boats[i].coord[k].pos.y]='A';
                    }
                    }
               
                    
    }
    }

        }

    
    print_board(N, M, jogo.board, 1);
    if(brd.numBoatsAfloat==0){
        jogadas=40;
    }
    printf("%d\n", brd.numBoatsAfloat);

    jogadas++;
    }while(jogadas<40);
    if(brd.numBoatsAfloat==0){
        printf("\nO jogador 2 vence!\n");
    }
    else{
        printf("\n O jogador 1 vence!\n");
    }


















    
    return 0;
}
