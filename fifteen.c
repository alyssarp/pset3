/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// position of blank
int z_i;
int z_j;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // TODO
    int tile_num = d*d -1;
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
                board[i][j]= tile_num;
                tile_num = tile_num - 1;
            }
    }
    if (d%2 == 0){
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
    z_i = d-1;
    z_j = d-1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            if(board[i][j]>=10){
                printf("%d ", board[i][j]);
            } else {
                printf(" %d ", board[i][j]);
            }  
         }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int t_i, t_j;
    // find which space in the array that tile is currently located
    // check to see if 0 is in any of the bordering array spaces
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            if (board[i][j] == tile){
                t_i = i;
                t_j = j;
            }
         }
    }
    //check to see if the chosen tile is in the same column as the zero tile
    if (t_i == z_i){
        //if it is in the same column, check to see if the chosen tile is to the left or right of the zero column
        if (t_j + 1 == z_j || t_j - 1 == z_j){
            //store the value of the chosen tile in 0's place
            board[z_i][z_j] = tile;
            //store the value of 0 in the chosen tile's place
            board[t_i][t_j] = 0;
            //set the new position of 0 to where the chosen tile was
            z_i = t_i;
            z_j = t_j;
            return true;
            }
     //if not in the same column, check if the chosen tile is in the same row
     } else if(t_j == z_j){
         //if it is, check if the chosen tile is above or below the zero tile
         if (t_i + 1 == z_i || t_i - 1 == z_i){
            //store the value of the chosen tile in 0's place
            board[z_i][z_j] = tile;
            //store the value of 0 in the chosen tile's place
            board[t_i][t_j] = 0;
            //set the new position of 0 to where the chosen tile was
            z_i = t_i;
            z_j = t_j;
            return true;
          }
        
     } 
     //if none of the if statements are met, return false.
     return false;
    
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int count = d*d-1;
    // TODO
    for(int i = d-1; i >= 0; i--){
        for(int j = d-1; j >= 0; j--){
           if(i == d-1 && j == d-1){
               if(board[i][j]!=0){
               return false;
               }
           }else if (board[i][j] == count){
              count--;
           } else {
            return false;
           }
            
        }     
    }
   return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
