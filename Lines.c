#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FIELD_SIDE 10
#define COLORS_COUNT 5
#define MAX_STRING 256

int max_tries_enter = 0;

char field[FIELD_SIDE][FIELD_SIDE];

char *coordinates;

int new_entries[8];

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
    CHECK_5_IN_ROW,
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
            
            if (field[j][i] == 'A')
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
            if (field[i][j] == 'A')
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
    int i = 0;
    int empty = 0; // Var to hold number of empty spots
    int which_to_fill;
    char color;
    char color_num;

    // Calculating how many empty spots are left
    for (int i = 0; i < FIELD_SIDE; i++)
    {
        for (int j = 0; j < FIELD_SIDE; j++)
        {
            if (field[i][j] == 'A')
            {
                empty++;
            }
        }
    }

    // srand ( time(NULL) );

    while (i < balls_quantity && empty > 0)
    {
        color_num = rand() % COLORS_COUNT;

        color = colors[color_num][0];

        which_to_fill = rand() % empty;
    
        for (int i = 0; i < FIELD_SIDE; i++)
        {
            for (int j = 0; j < FIELD_SIDE; j++)
            {
                if (field[i][j] == 'A')
                {
                    which_to_fill--;
                    if (which_to_fill == 0)
                    {
                        x = i;
                        y = j;
                        goto loop_exit;
                    }
                }
            }
        }

        loop_exit:
    
        field[x][y] = color;
        new_entries[(i + 1) * 2] = x;
        new_entries[((i + 1) * 2) + 1] = y;
        i++;
        empty--;
    }
}

enum Statetype handle_enter_coordinates()
{
    enum Statetype state;

    if (max_tries_enter >= 5)
    {
        state = EXIT;
    }
    else
    {
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

    x_dest = atoi(&coordinates[4]);
    y_dest = atoi(&coordinates[6]);

    // printf("The source field is %c\n", field[x_src][y_src]);

    if (field[x_src][y_src] == 'A')
    {
        printf("The field %d %d is empty. ", x_src, y_src);
        printf("Choose another source field\n");
        max_tries_enter += 1;
        return ENTER_COORD;
    }
    else if (field[x_dest][y_dest] != 'A')
    {
        printf("The field %d %d is not empty. ", x_dest, y_dest);
        printf("Choose another destination field\n");
        return ENTER_COORD;
    }
    
    return MOVE_FILL;
}

enum Statetype handle_move_fill()
{
    char color;

    color = field[x_src][y_src];
    field[x_src][y_src] = 'A';
    field[x_dest][y_dest] = color;

    new_entries[0] = x_dest;
    new_entries[1] = y_dest;

    add_balls(3);

    return CHECK_5_IN_ROW;
}

void test_row(const int x, const int y, const char color)
{
    char color_check;
    bool pop_ball = false;
    int cnt_in_row = 1;
    int x_plus;
    int y_plus;
    int m = 0;
    int n = 0;

    color_check = color;

    for (int i = 0; i < 4; i++)
    {
        cnt_in_row = 1;
        color_check = color;

        switch (i)
        {
        case 0:
            x_plus = 1;
            y_plus = 1;
            break;
        
        case 1:
            x_plus = 1;
            y_plus = -1;
            break;

        case 2:
            x_plus = 0;
            y_plus = 1;
            break;
        
        case 3:
            x_plus = 1;
            y_plus = 0;
            break;
        
        default:
            break;
        }

        m = 0;
        n = 0;

        m += x_plus;
        n += y_plus;

        while (color_check == color &&
                (x + m) >= 0 &&
                (x + m) < FIELD_SIDE &&
                (y + n) >= 0 &&
                (y + n) < FIELD_SIDE)
        {
            if ((color_check = field[x + m][y + n]) == color)
            {
                cnt_in_row += 1;
                m += x_plus;
                n += y_plus;
            }
        }

        color_check = color;

        m = 0;
        n = 0;

        m -= x_plus;
        n -= y_plus;

        while (color_check == color &&
            (x + m) >= 0 &&
            (x + m) < FIELD_SIDE &&
            (y + n) >= 0 &&
            (y + n) < FIELD_SIDE)
        {
            if ((color_check = field[x + m][y + n]) == color)
            {
                cnt_in_row += 1;
                m -= x_plus;
                n -= y_plus;
            }
        }

        if (cnt_in_row >= 5)
        {
            pop_ball = true;

            m += x_plus;
            n += y_plus;

            do
            {
                if (!(m == 0 && n == 0))
                {
                    field[x + m][y + n] = 'A';
                }
                m += x_plus;
                n += y_plus;

            } while (field[x + m][y + n] == color &&
                        (x + m) >= 0 && (x + m) <= FIELD_SIDE &&
                        (y + n) >= 0 && (y + n) <= FIELD_SIDE
                    );
        }
    }

    if (pop_ball)
    {
        field[x][y] = 'A';
    }
}

enum Statetype handle_check_5_in_row()
{
    int x;
    int y;
    int j; // Offset
    int cnt_in_row = 1;
    bool pop_ball = false;
    char color; // Testing of color
    char color_check;

    for (char i = 0; i < 4; i++)
    {
        x = new_entries[i * 2];
        y = new_entries[(i * 2) + 1];

        color = field[x][y];

        printf("New entry x %d y %d, color %c\n", x, y, color);

        test_row(x, y, color);
        /*
        color_check = color;

        j = 1;

        while (color_check == color && (x - j) >= 0)
        {
            if ((color_check = field[x - j][y]) == color)
            {
                cnt_in_row += 1;
            }
            j++;
        }

        j = 1;

        while (color_check == color && (x + j) <= 9)
        {
            if ((color_check = field[x + j][y]) == color)
            {
                printf("Checking x %d y %d\n", x - j, y);
                cnt_in_row += 1;
                printf("cnt_in_row %d\n", cnt_in_row);
            }
            j++;
        }

        if (cnt_in_row >= 5)
        {
            pop_ball = true;

            do
            {
                if (j != 0)
                {
                    field[x + j][y] = 'A';
                }
                j--;
            } while (field[x + j][y] == color &&
                    (x + j) >= 0 && (x + j) <= 9);
        }

        if (pop_ball)
        {
            field[x][y] = 'A';
        }

        j = 1;

        cnt_in_row = 1;*/
    }

    return OUTPUT_FIELD;
}

int main(void)
{
    enum Statetype state = OUTPUT_FIELD;

    for (int i = 0; i < FIELD_SIDE; i++)
    {
        for (int j = 0; j < FIELD_SIDE; j++)
        {
            field[i][j] = 'A'; // A for "absent", field is empty
        }
    }
    
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

            case CHECK_5_IN_ROW:
                state = handle_check_5_in_row();
                break;
            
            default:
                break;
        }
    }
}