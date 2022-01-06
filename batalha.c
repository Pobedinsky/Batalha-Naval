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
    b->numBoatsAfloat = 0;
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
        if (flag) //Mostra tudo
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
    int tamanho=-1;
    int *t=&tamanho;
    switch (type)
    {
    case 'P':
        *t=5;
        break;
    case 'N':
        *t=4;
        break;
    case 'C':
        *t=3;
        break;
    case 'S':
        *t=2;
        break;    
    default:
        *t=-1;
        break;
    }

    return *t;
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
    b->afloat=1;
    b->tSize=typeToSize(type);
    for(int i=0; i<typeToSize(type); i++){
        b->coord[i].afloat=1;

        if(dir=='H'){
        b->coord[i].pos.x+=i;
        b->coord[i].pos.y=0;
        }
        else{
        b->coord[i].pos.x=0;
        b->coord[i].pos.y+=i;

        }
    }

}



int check_free(int n, int m, Boat *boat, char board[n][m])
{
   int k=1;
   for (int l=0; l<5; l++){
        for (int i=0; i<n; i++){
            for (int j=0; j<m; m++){
                if(boat->coord[l].pos.x==i && boat->coord[l].pos.y==j && board[i][j]!=' '){
                    k=0;
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
    int w=-1;
    for (int i=0; i<N; i++){
        if (board->boats->coord->pos.x==i){
            break;
        }
        else{
            w=-2;
        }
    }
    for (int j=0; j<M; j++){
        if (board->boats->coord->pos.y==j){
            break;
        }
        else{
            w=-2;
        }
    }
    if (dir != 'H' || dir != 'V'){
        w=-3;
    }
    else if(type!='P' || type!='C' || type!='N' || type!='S'){
        w=-4;
    }
    else if(check_free(x1, y1, board->boats, board->board)==0){
        for(int i=0; i<) //acabar isto
        w=0;
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
    char l;
    for (int i=0; i<N; i++){
        if (board->boats->coord->pos.x==i){
            break;
        }
        else{
            l='I';
        }
    }
    for (int j=0; j<M; j++){
        if (board->boats->coord->pos.y==j){
            break;
        }
        else{
            l='I';
        }
    }
    if(l!='I'){
        for(int i=0; i<B; i++){
            for(int j=0; j<typeToSize(board->boats[i].type); j++){
                if(board->boats[i].coord[j].afloat == 1){
                    l='F';
                    break;
                    }
                else{
                    continue;
                }
            }
    }
    }

    return l;
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
    //Implementar

    return -3;
}

//int colocaNavio()
int main(void)
{

    Board brd;
    init_board(N, M, &brd);
    print_board(N, M, brd.board, 0);
    
    /**Exemplo de uso da print_board e da place_boat**/
    /**Precisa de as implementar primeiro**/
    //print_board(N, M, brd.board, 0);
    //place_boat(1,3, 'H', 'P', &brd);
    //print_board(N, M, brd.board, 1);
    
    
    return 0;
}
