#include "linked_list.h"
#include "structs.h"
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
    char color_check;   // Variable to hold color that is compared
    // with initial color
    int cnt_in_row = 1; // Count number of balls in the row
    // and if they are more than 5, then pop balls
    bool pop_ball = false;  // Variable to pop balls if they are more than 5
    // in a row
    int m = 0;  // Offset of x when balls in a row are compared
    int n = 0;
    int x_plus; // Increase in offset of x (m) when moving to compare
    // balls in a row
    int y_plus; // Increase in offset of y (n) when moving to compare
    // balls in a row
    int z;  // Variable to hold direction in which comparing ball is moved

    color_check = color;

    for (int i = 0; i < 4; i++)
    {
        // There are 4 directions, in which the comparison goes
        switch (i)
        {
        case 0:
            // Diagonal up and right
            x_plus = 1;
            y_plus = 1;
            break;
        
        case 1:
            // Diagonal down and right
            x_plus = 1;
            y_plus = -1;
            break;

        case 2:
            // Vertical
            x_plus = 0;
            y_plus = 1;
            break;
        
        case 3:
            // Horizontal
            x_plus = 1;
            y_plus = 0;
            break;
        }

        // Initializing quantity of balls in a row
        cnt_in_row = 1;

        for (char j = 0; j < 2; j++)
        {
            if (j == 0)
            {
                z = 1;
            }
            else
            {
                z = -1;
            }
            
            color_check = color;

            m = 0;
            n = 0;
    
            m += x_plus * z;
            n += y_plus * z;
    
            while (color_check == color &&
                    // Field to check doesn't go out of the board
                    (x + m) >= 0 &&
                    (x + m) < FIELD_SIDE &&
                    (y + n) >= 0 &&
                    (y + n) < FIELD_SIDE)
            {
                if ((color_check = field[x + m][y + n]) == color)
                {
                    cnt_in_row += 1;
                    m += x_plus * z;
                    n += y_plus * z;
                }
            }
        }

        if (cnt_in_row >= 5)
        {
            pop_ball = true;

            // After all the checks in row have successfully been made
            // offset is out of the row by one position. The code here
            // is to return offset to rows
            m += x_plus;
            n += y_plus;

            // Popping balls
            do
            {
                // The initial ball (that is in new position)
                // is not to be popped until the checks
                // of all 4 rows is all directions are done
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

        test_row(x, y, color);
    }

    return OUTPUT_FIELD;
}

void handle_delete_breadcrumbs()
{
    for (char i = 0; i < FIELD_SIDE; i++)
    {
        for (char j = 0; j < FIELD_SIDE; j++)
        {
            if (field[i][j] == 'O' || field[i][j] == '*')
            {
                field[i][j] = 'A';
            }
        }
    }
}

void clear_breadcrumbs()
{
    
}

bool handle_test_path_exists(char x_src, char y_src,
                            char x_dest, char y_dest)
{
    bool path_exists = false;

    printf("Source coord. %d %d dest. coord %d %d\n", x_src, y_src,
            x_dest, y_dest);

    printf("Variables to be declared\n");

    char x;
    char y;

    char x_offset;
    char y_offset;

    char x_check;
    char y_check;

    char cell;

    struct Stack* centered_stack = create_stack();
    struct Stack* tested_stack = create_stack();

    struct Point pnt_check;

    // Stack of cells to be checked
    push (tested_stack, x_src, y_src);

    while (!is_empty(tested_stack))
    {
        pnt_check = pop(tested_stack);

        for (char i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                x_offset = 0;
                y_offset = -1;
                break;

            case 1:
                x_offset = -1;
                y_offset = 0;
                break;
            
            case 2:
                x_offset = 1;
                y_offset = 0;
                break;

            case 3:
                x_offset = 0;
                y_offset = 1;
                break;
            }

            x_check = pnt_check.x + x_offset;
            y_check = pnt_check.y + y_offset;

            if (x_check >= 0 && x_check <= FIELD_SIDE - 1 &&
                y_check >= 0 && y_check <= FIELD_SIDE - 1)
            {
                if (x_check == x_dest && y_check == y_dest)
                {
                    return true;
                }
                else
                {
                    if (field[x_check][y_check] == 'A')
                    {
                        if (!search_stack(centered_stack,
                                            x_check, y_check) &&
                            !search_stack(tested_stack, x_check, y_check))
                        {
                            push(tested_stack, x_check, y_check);
                            field[x_check][y_check] = '*';
                        }
                    }
                }
            } 
        }

        push(centered_stack, pnt_check.x, pnt_check.y); // After checking
        // all adjacent cells put center cell in the stack for center
        if (x_src != pnt_check.x && y_src != pnt_check.y)
        {
            field[pnt_check.x][pnt_check.y] = 'O';
        };
        
        handle_output_field();
    }
    
    empty_stack(centered_stack);
    empty_stack(tested_stack);

    return path_exists;
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
    else if (x_src == x_dest && y_src == y_dest)
    {
        printf("The coordinates of source and destination are the same.\n");
        printf("Choose other coordinates.\n");
    }
    else if (handle_test_path_exists(x_src, y_src, x_dest, y_dest) == false)
    {
        printf("The path from %d %d to ", x_src, y_src);
        printf("%d %d cannot be reached\n", x_dest, y_dest);
        printf("Choose another destination field\n");
        return ENTER_COORD;
    }
    
    return MOVE_FILL;
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