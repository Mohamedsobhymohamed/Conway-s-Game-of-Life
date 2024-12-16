# Conway's Game of Life (Multithreaded)

This project is a multithreaded implementation of Conway's Game of Life, leveraging **POSIX threads (pthreads)** for parallel computation. Each thread processes a portion of the grid independently, synchronized at key steps to maintain a consistent state across generations.

## Features
- **Parallel Processing**: The grid is divided among multiple threads for efficient computation.
- **Predefined Patterns**: Includes static blocks, oscillators, and gliders to demonstrate various behaviors.
- **Console Visualization**: Real-time rendering of the grid for easy observation of the simulation.
- **Customizable**: Easily adjust the grid size (`GRID_SIZE`), number of threads (`NUM_THREADS`), and generations (`MAX_GENERATIONS`).

## Requirements
- POSIX-compatible operating system.
- GCC or a similar compiler with pthread support.

## How to Run
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-folder>
   ```

2. Compile the code using GCC:
   ```bash
   gcc -o game_of_life game_of_life.c -pthread
   ```

3. Run the executable:
   ```bash
   ./game_of_life
   ```

## Code Highlights
- **Multithreading**: Uses `pthread_create` to spawn threads and `pthread_barrier` for synchronization.
- **Grid Initialization**: Includes predefined patterns like the Block, Blinker, and Glider.
- **Simulation Logic**: Implements Conway's rules for cell survival, reproduction, and death.

## Customization
Modify the following macros in the code to customize the simulation:
- `GRID_SIZE`: Change the size of the grid.
- `NUM_THREADS`: Adjust the number of threads.
- `MAX_GENERATIONS`: Set the number of generations to simulate.

## Example Output
A real-time grid rendered in the console:
```
# #          
# #          
              
      #       
    # # #     
              
```

## License
This project is open-source and available under the [MIT License](LICENSE).

