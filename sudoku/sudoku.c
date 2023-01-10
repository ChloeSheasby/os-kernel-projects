#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_THREADS 27

int results[NUM_THREADS];

int sudoku[9][9] = {
    {1,2,5,3,7,8,9,4,6},
    {3,7,8,9,6,4,2,1,5},
    {4,9,6,1,2,5,8,3,7},
    {2,6,9,4,5,3,1,7,8},
    {8,4,1,7,9,2,6,5,3},
    {5,3,7,8,1,6,4,9,2},
    {9,1,2,5,8,7,3,6,4},
    {6,5,3,2,4,9,7,8,1},
    {7,8,4,6,3,1,5,2,9}
};

struct parameter {
    int task_id;
    int start_x;
    int start_y;
};

void* test_rows(void* arg)
{
    int freq[9] = {0,0,0,0,0,0,0,0,0};
    struct parameter* param;
    param = (struct parameter*) arg;
    int x = param->start_x;
    bool success = true;
    for(int y = 0; y < 9 && success == true; y++)
    {
        freq[sudoku[x][y] - 1]++;
        if(freq[sudoku[x][y] - 1] > 1)
        {
            success = false;
        }
    }
    printf("Task ID: %d\tSuccess: %d\n", param->task_id, success);
    if(success)
    {
        results[param->task_id - 1] = 1;
    }
    else
    {
        results[param->task_id - 1] = 0;
    }
    pthread_exit(NULL);
    return 0;
}

void* test_columns(void* arg)
{
    int freq[9] = {0,0,0,0,0,0,0,0,0};
    struct parameter* param;
    param = (struct parameter*) arg;
    int y = param->start_y;
    bool success = true;
    for(int x = 0; x < 9 && success == true; x++)
    {
        freq[sudoku[x][y] - 1]++;
        if(freq[sudoku[x][y] - 1] > 1)
        {
            success = false;
        }
    }
    printf("Task ID: %d\tSuccess: %d\n", param->task_id, success);
    if(success)
    {
        results[param->task_id - 1] = 1;
    }
    else
    {
        results[param->task_id - 1] = 0;
    }
    pthread_exit(NULL);
    return 0;
}

void* test_cells(void* arg)
{
    int freq[9] = {0,0,0,0,0,0,0,0,0};
    struct parameter* param;
    param = (struct parameter*) arg;
    int x, y;
    bool success = true;
    for(x = param->start_x, y = param->start_y; x < (param->start_x + 3) && y < (param->start_y + 3) && success == true; x++, y++)
    {
        freq[sudoku[x][y] - 1]++;
        if(freq[sudoku[x][y] - 1] > 1)
        {
            success = false;
        }        
    }
    printf("Task ID: %d\tSuccess: %d\n", param->task_id, success);
    if(success)
    {
        results[param->task_id - 1] = 1;
    }
    else
    {
        results[param->task_id - 1] = 0;
    }
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char* argv[])
{
    pthread_t tid[NUM_THREADS];
    int count = 0, i;
    struct parameter params[NUM_THREADS];
    // check all rows
    for(i = 0; i < 9; i++)
    {
        params[count].task_id = count + 1;
        params[count].start_x = i;
        params[count].start_y = 0;
        pthread_create(&tid[count], NULL, test_rows, (void*)&params[count]);
        count++;
    }

    // check all columns
    for(i = 0; i < 9; i++)
    {
        params[count].task_id = count + 1;
        params[count].start_x = 0;
        params[count].start_y = i;
        pthread_create(&tid[count], NULL, test_columns, (void*)&params[count]);
        count++;
    }

    // check all cells
    for(i = 0; i < 9; i+=3)
    {
        for(int j = 0; j < 9; j+=3)
        {
            params[count].task_id = count + 1;
            params[count].start_x = i;
            params[count].start_y = j;
            pthread_create(&tid[count], NULL, test_cells, (void*)&params[count]);
            count++;
        }
    }

    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    int successCounter = 0;
    for(i = 0; i < NUM_THREADS; i++)
    {
        if(results[i] == 1)
        {
            successCounter++;
        }
    }

    if(successCounter == 27)
    {
        printf("\nSudoku puzzle is valid.\n");
    }
    else
    {
        printf("\nOh no! Sudoku puzzle is not valid.\n");
    }

    pthread_exit(NULL);
}