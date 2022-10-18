#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void intHandler() {
    // Make the cursor visible
    printf("\e[?25h");
    exit(0);
}

void print_logo(char **dvd_logo, int color, int x, int y){
    printf("\033[1;9%dm\033[2J", (color%7)+1);
    for( int i=0; i<7; i++)
        printf("\033[%d;%dH%s",y++,x, *(dvd_logo+i));
}

void alloc_logo(char **dvd_logo){
    for( int i=0; i<7; i++)
        *(dvd_logo+i) = (char*)malloc(sizeof(char)*30);
    *(dvd_logo+0) = "  ⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⡀";
    *(dvd_logo+1) = "⠀⢠⣿⣿⡿⠀⠀⠈⢹⣿⣿⡿⣿⣿⣇⠀⣠⣿⣿⠟⣽⣿⣿⠇⠀⠀⢹⣿⣿⣿";
    *(dvd_logo+2) = "⠀⢸⣿⣿⡇⠀⢀⣠⣾⣿⡿⠃⢹⣿⣿⣶⣿⡿⠋⢰⣿⣿⡿⠀⠀⣠⣼⣿⣿⠏"; 
    *(dvd_logo+3) = "⠀⣿⣿⣿⣿⣿⣿⠿⠟⠋⠁⠀⠀⢿⣿⣿⠏⠀⠀⢸⣿⣿⣿⣿⣿⡿⠟⠋⠁⠀";
    *(dvd_logo+4) = "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣸⣟⣁⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    *(dvd_logo+5) = "⣠⣴⣶⣾⣿⣿⣻⡟⣻⣿⢻⣿⡟⣛⢻⣿⡟⣛⣿⡿⣛⣛⢻⣿⣿⣶⣦⣄⡀⠀";
    *(dvd_logo+6) = "⠉⠛⠻⠿⠿⠿⠷⣼⣿⣿⣼⣿⣧⣭⣼⣿⣧⣭⣿⣿⣬⡭⠾⠿⠿⠿⠛⠉⠀ ";
}

int main(){

    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int xmax = w.ws_col - 30;
    int ymax = w.ws_row - 7;

    char **dvd_logo = NULL;
    dvd_logo = (char**)malloc(sizeof(char*)*7);
    alloc_logo(dvd_logo);

    srand(time(NULL));
    int x = rand()%xmax;
    int y = rand()%ymax;
    int direction = 4;
    int color = rand()%8;

    signal(SIGINT, intHandler);
    // Make the cursor invisible
    printf("\e[?25l");

    int loop = 0;

    while(1){
        
        if( loop%10 == 0){
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            xmax = w.ws_col - 30;
            ymax = w.ws_row - 7;
        }
        loop++;

        if( x >= xmax){
            if( direction == 2 )
                direction = 1;
            else if(direction == 4)
                direction = 3;
            color++;
        }
        else if( x <= 1){
            if( direction == 1)
                direction = 2;
            else if( direction == 3)
                direction = 4;
            color++;
        }
        else if( y >= ymax){
            if( direction == 3)
                direction = 1;
            else if( direction == 4)
                direction = 2;
            color++;
        }
        else if(y <= 1 ){
            if( direction == 1)
                direction = 3;
            else if(direction == 2)
                direction = 4;
            color++;
        }

        int mov_x=2;
        int mov_y=1;

        switch(direction){
            case 1:
                y-=mov_y;
                x-=mov_x;
            break;
            
            case 2:
                x+=mov_x;
                y-=mov_y;
            break;

            case 3:
                y+=mov_y;
                x-=mov_x;
            break;

            case 4:
                y+=mov_y;
                x+=mov_x;
            break;
        }

        print_logo(dvd_logo, color, x, y); 
        usleep(100000);
        fflush(stdout);

    }

    return 0;
}
