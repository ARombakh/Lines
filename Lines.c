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

enum Statetype handle_output_field(char (*pfield)[FIELD_SIDE][FIELD_SIDE])
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
            
            if ((*pfield)[j][i] == 'A')
            {
                printf(" ");
            }
            else
            {
                printf("%c", (*pfield)[j][i]);
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

enum Statetype handle_test_empty(char (*pfield)[FIELD_SIDE][FIELD_SIDE])
{
    enum Statetype state;
    bool empty = false;

    for (int i = 0; i < FIELD_SIDE; i++)
    {
        for (int j = 0; j < FIELD_SIDE; j++)
        {
            if ((*pfield)[i][j] == 'A')
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

void add_balls(char balls_quantity, char (*pfield)[FIELD_SIDE][FIELD_SIDE])
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
            if ((*pfield)[i][j] == 'A')
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
                if ((*pfield)[i][j] == 'A')
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
    
        (*pfield)[x][y] = color;
        new_entries[(i + 1) * 2] = x;
        new_entries[((i + 1) * 2) + 1] = y;
        i++;
        empty--;
    }
}

enum Statetype handle_enter_coordinates(char *pcoord)
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

        strcpy(pcoord, assign_string_from_input());

        if (strcmp(pcoord, "exit") == 0)
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

enum Statetype handle_test_lex(char *pcoord)
{
    enum Statetype state;

    if (
        ! (
            isdigit(pcoord[0]) ||
            (pcoord[1] == '-') ||
            isdigit(pcoord[2]) ||
            (pcoord[3] == '-') ||
            isdigit(pcoord[4]) ||
            (pcoord[5] == '-') ||
            isdigit(pcoord[6])
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

enum Statetype handle_move_fill(char (*pfield)[FIELD_SIDE][FIELD_SIDE])
{
    char color;

    color = (*pfield)[x_src][y_src];
    (*pfield)[x_src][y_src] = 'A';
    (*pfield)[x_dest][y_dest] = color;

    new_entries[0] = x_dest;
    new_entries[1] = y_dest;

    add_balls(3, pfield);

    return CHECK_5_IN_ROW;
}

void test_row(const int x, const int y, const char color,
                char (*pfield)[FIELD_SIDE][FIELD_SIDE])
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
                if ((color_check = (*pfield)[x + m][y + n]) == color)
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
                    (*pfield)[x + m][y + n] = 'A';
                }
                m += x_plus;
                n += y_plus;

            } while ((*pfield)[x + m][y + n] == color &&
                        (x + m) >= 0 && (x + m) <= FIELD_SIDE &&
                        (y + n) >= 0 && (y + n) <= FIELD_SIDE
                    );
        }
    }

    if (pop_ball)
    {
        (*pfield)[x][y] = 'A';
    }
}

enum Statetype handle_check_5_in_row(char (*pfield)[FIELD_SIDE][FIELD_SIDE])
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

        color = (*pfield)[x][y];

        test_row(x, y, color, pfield);
    }

    return OUTPUT_FIELD;
}

bool handle_test_path_exists(char x_src, char y_src,
                            char x_dest, char y_dest,
                            char (*pfield)[FIELD_SIDE][FIELD_SIDE])
{
    bool path_exists = false;

    printf("Source coord. %d %d dest. coord %d %d\n", x_src, y_src,
            x_dest, y_dest);

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
                    if ((*pfield)[x_check][y_check] == 'A')
                    {
                        if (!search_stack(centered_stack,
                                            x_check, y_check) &&
                            !search_stack(tested_stack, x_check, y_check))
                        {
                            push(tested_stack, x_check, y_check);
                        }
                    }
                }
            } 
        }

        push(centered_stack, pnt_check.x, pnt_check.y); // After checking
        // all adjacent cells put center cell in the stack for center
    }
    
    empty_stack(centered_stack);
    empty_stack(tested_stack);

    return path_exists;
}

enum Statetype handle_test_path(char (*pfield)[FIELD_SIDE][FIELD_SIDE],
                                char *pcoord)
{
    x_src = atoi(&pcoord[0]);
    y_src = atoi(&pcoord[2]);

    x_dest = atoi(&pcoord[4]);
    y_dest = atoi(&pcoord[6]);

    // printf("The source field is %c\n", field[x_src][y_src]);

    if ((*pfield)[x_src][y_src] == 'A')
    {
        printf("The field %d %d is empty. ", x_src, y_src);
        printf("Choose another source field\n");
        max_tries_enter += 1;
        return ENTER_COORD;
    }
    else if ((*pfield)[x_dest][y_dest] != 'A')
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
    else if (handle_test_path_exists(x_src, y_src, x_dest,
                                    y_dest, pfield) == false)
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

    char field[FIELD_SIDE][FIELD_SIDE];

    char (*pfield)[FIELD_SIDE][FIELD_SIDE];

    pfield = &field;

    char pcoord[8];

    for (int i = 0; i < FIELD_SIDE; i++)
    {
        for (int j = 0; j < FIELD_SIDE; j++)
        {
            (*pfield)[i][j] = 'A'; // A for "absent", field is empty
        }
    }
    
    add_balls(3, pfield);
    
    while (state != EXIT)
    {
        switch (state)
        {
            case OUTPUT_FIELD:
                state = handle_output_field(pfield);
                break;
            
            case TEST_EMPTY:
                state = handle_test_empty(pfield);
                break;

            case GAME_OVER:
                state = handle_game_over();
                break;
            
            case ENTER_COORD:
                state = handle_enter_coordinates(pcoord);
                break;
            
            case TEST_LEX:
                state = handle_test_lex(pcoord);
                break;

            case TEST_PATH:
                state = handle_test_path(pfield, pcoord);
                break;
            
            case MOVE_FILL:
                state = handle_move_fill(pfield);
                break;

            case CHECK_5_IN_ROW:
                state = handle_check_5_in_row(pfield);
                break;
            
            default:
                break;
        }
    }
}