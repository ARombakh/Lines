#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define FIELD_SIDE 10
#define COLORS_COUNT 5
#define MAX_STRING 256

char field[FIELD_SIDE][FIELD_SIDE];

char colors[COLORS_COUNT][7] = {
    "RED",
    "BLUE",
    "GREEN",
    "PURPLE",
    "YELLOW"
};

char *assign_string_from_input (void)
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

void output_field(void)
{
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

int main(void)
{
    char *coordinates;

    add_balls(3);
    output_field();

    printf("Enter the coordinates of the ball you want to move ");
    printf("and destination separated by dash:\n");
    
    coordinates = assign_string_from_input();

    if (
        ! (
            isdigit(coordinates[0]) ||
            isdigit(coordinates[1]) ||
            (coordinates[2] == '-') ||
            isdigit(coordinates[3]) ||
            isdigit(coordinates[4])
        )
    )
    {
        printf("Coordinates were input incorrectly\n");
    }
    
}