#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FIELD_SIDE 10
#define COLORS_COUNT 5
#define MAX_STRING 256

char field[FIELD_SIDE][FIELD_SIDE];

char *coordinates;

int x_src;
int y_src;
int x_dest;
int y_dest;

enum Statetype {
    OUTPUT_FIELD,
    TEST_EMPTY,
    GAME_OVER,
    ENTER_COORD,
    TEST_LEX,
    TEST_PATH,
    MOVE_FILL,
    EXIT
};

char colors[COLORS_COUNT][7] = {
    "RED",
    "BLUE",
    "GREEN",
    "PURPLE",
    "YELLOW"
};

char *assign_string_from_input ()
{
    static char input[MAX_STRING];
    size_t len;

    if (fgets(input, MAX_STRING, stdin))
    {
        len = strlen(input);
        
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }
    }

    return input;
}

enum Statetype handle_output_field()
{
    enum Statetype state;

    printf("  ");

    for (int i = 0; i < FIELD_SIDE; i++)
    {
        printf("%d ", i);
    }

    printf("\n");

    for (int i = FIELD_SIDE - 1; i >= 0; i--)
    {
        for (int j = 0; j < FIELD_SIDE; j++)
        {
            if (j == 0)
            {
                printf("%d ", i);
            }
            
            if (field[j][i] == '\0')
            {
                printf(" ");
            }
            else
            {
                printf("%c", field[j][i]);
            }

            if (j == FIELD_SIDE - 1)
            {
                printf(" %d\n", i);
            }
            else
            {
                printf(" ");
            }
               
        }
    }
    
    printf("  ");

    for (int i = 0; i < FIELD_SIDE; i++)
    {
        printf("%d ", i);
    }
    
    printf("\n");

    state = TEST_EMPTY;

    return state;
}

enum Statetype handle_test_empty()
{
    enum Statetype state;
    bool empty = false;

    for (int i = 0; i < FIELD_SIDE; i++)
    {
        for (int j = 0; j < FIELD_SIDE; j++)
        {
            if (field[i][j] == '\0')
            {
                empty = true;
            }
        }
    }
    
    if (empty)
    {
        state = ENTER_COORD;
    }
    else
    {
        state = GAME_OVER;
    }

    return state;
}

enum Statetype handle_game_over()
{
    char *enter;
    enum Statetype state;
    printf("GAME OVER!\n");
    printf("Enter \"exit\" to exit the game\n");

    enter = assign_string_from_input();

    if (strcmp(enter, "exit") == 0)
    {
        state = EXIT;
    }

    return EXIT;    
}

void add_balls(char balls_quantity)
{
    int x;
    int y;
    char color;
    char color_num;

    srand ( time(NULL) );

    for (int i = 0; i < balls_quantity; i++)
    {
        color_num = rand() % COLORS_COUNT;

        color = colors[color_num][0];
    
        do
        {
            x = rand() % FIELD_SIDE;
            y = rand() % FIELD_SIDE;
        } while (field[x][y] != '\0');
    
        field[x][y] = color;
    }
}

enum Statetype handle_enter_coordinates()
{
    enum Statetype state;

    printf("Enter the coordinates of the ball you want to move ");
    printf("and destination separated by dash:\n");

    coordinates = assign_string_from_input();

    if (strcmp(coordinates, "exit") == 0)
    {
        state = EXIT;
    }
    else
    {
        state = TEST_LEX;
    }
    
    return state;
}

enum Statetype handle_test_lex()
{
    enum Statetype state;
    if (
        ! (
            isdigit(coordinates[0]) ||
            (coordinates[1] == '-') ||
            isdigit(coordinates[2]) ||
            (coordinates[3] == '-') ||
            isdigit(coordinates[4]) ||
            (coordinates[5] == '-') ||
            isdigit(coordinates[6])
        )
    )
    {
        printf("Coordinates were input incorrectly\n");
        state = ENTER_COORD;
    }
    else
    {
        state = TEST_PATH;
    }

    return state;
}

enum Statetype handle_test_path()
{
    x_src = atoi(&coordinates[0]);
    y_src = atoi(&coordinates[2]);

    printf("The coordinates x_src %d y_src %d\n", x_src, y_src);
    x_dest = atoi(&coordinates[4]);
    y_dest = atoi(&coordinates[6]);

    if (field[x_src][y_src] == '\0')
    {
        printf("The field %d %d is empty. ", x_src, y_src);
        printf("Choose another source field\n");
        return ENTER_COORD;
    }
    else if (field[x_dest][x_src] != '\0')
    {
        printf("The field %d %d is not empty .", x_dest, y_dest);
        printf("Choose another destination field\n");
        return ENTER_COORD;
    }
    
    return MOVE_FILL;
}

enum Statetype handle_move_fill()
{
    field[x_src][y_src] = colors[1][0];
    field[x_dest][y_dest] = colors[1][0];

    return OUTPUT_FIELD;
}

int main(void)
{
    enum Statetype state = OUTPUT_FIELD;

    add_balls(3);
    
    while (state != EXIT)
    {
        switch (state)
        {
            case OUTPUT_FIELD:
                state = handle_output_field();
                break;
            
            case TEST_EMPTY:
                state = handle_test_empty();
                break;

            case GAME_OVER:
                state = handle_game_over();
                break;
            
            case ENTER_COORD:
                state = handle_enter_coordinates();
                break;
            
            case TEST_LEX:
                state = handle_test_lex();
                break;

            case TEST_PATH:
                state = handle_test_path();
                break;
            
            case MOVE_FILL:
                state = handle_move_fill();
                break;
            
            default:
                break;
        }
    }
}