#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GRID_SIZE 20
#define NUM_THREADS 4
#define MAX_GENERATIONS 32

int grid[GRID_SIZE][GRID_SIZE];
pthread_barrier_t barrier;

// Utility to render the grid to the console
void render_grid() {
    system("clear");
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            printf(grid[row][col] ? "# " : "  ");
        }
        printf("\n");
    }
    usleep(500000); // Pause for a short duration for visual effect
}

// thread function to compute the next state of the grid
void* simulate_generation(void* arg) {
    int thread_id = *(int*)arg;
    int start_row = thread_id * (GRID_SIZE / NUM_THREADS);
    int end_row = start_row + (GRID_SIZE / NUM_THREADS);

    for (int gen = 0; gen < MAX_GENERATIONS; ++gen) {
        int temp[GRID_SIZE][GRID_SIZE];

        // Create a copy of the current grid
        for (int r = 0; r < GRID_SIZE; ++r) {
            for (int c = 0; c < GRID_SIZE; ++c) {
                temp[r][c] = grid[r][c];
            }
        }

        // Update cells within the assigned rows
        for (int row = start_row; row < end_row; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                int live_neighbors = 0;

                // Calculate live neighbors
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (dx == 0 && dy == 0) continue; // Skip the current cell

                        int neighbor_row = row + dx;
                        int neighbor_col = col + dy;

                        if (neighbor_row >= 0 && neighbor_row < GRID_SIZE &&
                            neighbor_col >= 0 && neighbor_col < GRID_SIZE) {
                            live_neighbors += grid[neighbor_row][neighbor_col];
                        }
                    }
                }

                // Apply Conway's Game of Life rules
                if (grid[row][col] == 1) {
                    temp[row][col] = (live_neighbors < 2 || live_neighbors > 3) ? 0 : 1;
                } else {
                    temp[row][col] = (live_neighbors == 3) ? 1 : 0;
                }
            }
        }

        pthread_barrier_wait(&barrier); // Synchronize threads

        // Update the shared grid
        for (int row = start_row; row < end_row; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                grid[row][col] = temp[row][col];
            }
        }

        pthread_barrier_wait(&barrier); // Synchronize before rendering

        if (thread_id == 0) {
            render_grid(); // Display the grid from the first thread
        }
    }

    return NULL;
}

// Reset all cells in the grid to 0
void clear_grid(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            grid[row][col] = 0;
        }
    }
}

// Add predefined patterns to the grid
void initialize_patterns(int grid[GRID_SIZE][GRID_SIZE]) {
    clear_grid(grid);

    // Static block pattern
    grid[1][1] = grid[1][2] = grid[2][1] = grid[2][2] = 1;

    // Oscillator: Blinker
    grid[5][6] = grid[6][6] = grid[7][6] = 1;

    // Spaceship: Glider
    grid[10][10] = grid[11][11] = 1;
    grid[12][9] = grid[12][10] = grid[12][11] = 1;
}

int main() {
    initialize_patterns(grid); // Set up initial state

    pthread_barrier_init(&barrier, NULL, NUM_THREADS); // Initialize thread barrier

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, simulate_generation, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier); // Clean up barrier

    return 0;
}

