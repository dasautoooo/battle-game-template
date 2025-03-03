/******************************************************************************
 * server.c
 *
 * Template for a networked ASCII "Battle Game" server in C.
 *
 * 1. Create a TCP socket, bind to <PORT>, listen for connections.
 * 2. Accept up to 4 client connections.
 * 3. Manage a global game state: ASCII grid + players + obstacles.
 * 4. On receiving commands (MOVE, ATTACK, QUIT, etc.), update the game state
 *    and broadcast it to all clients.
 *
 * Compile:
 *   gcc server.c -o server -pthread
 *
 * Usage:
 *   ./server <PORT>
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// #include <arpa/inet.h> // Optional if you want to display IP addresses

#define MAX_CLIENTS 4
#define BUFFER_SIZE 1024

/* Example grid size */
#define GRID_ROWS 5
#define GRID_COLS 5

/*---------------------------------------------------------------------------*
 * Data Structures
 *---------------------------------------------------------------------------*/

/* Player structure (you may add fields like HP, etc.) */
typedef struct {
    int x, y;       // position on the grid
    int active;     // 1 if active, 0 otherwise
    // TODO: Add more fields if needed (e.g., HP)
} Player;

/* GameState structure, storing the grid and players */
typedef struct {
    char grid[GRID_ROWS][GRID_COLS];
    Player players[MAX_CLIENTS];
    int clientCount;
} GameState;

/* Global game state */
GameState g_gameState;

/* Store each client's socket (index = player ID) */
int g_clientSockets[MAX_CLIENTS];

/* If you want to protect the shared state, use a mutex */
pthread_mutex_t g_stateMutex = PTHREAD_MUTEX_INITIALIZER;

/*---------------------------------------------------------------------------*
 * TODO: Initialize the game state
 *  - Clear the grid
 *  - Place obstacles (#)
 *  - Reset players
 *---------------------------------------------------------------------------*/
void initGameState() {
    // Example: fill with '.'
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            g_gameState.grid[r][c] = '.';
        }
    }

    // TODO: Place some obstacles, e.g. g_gameState.grid[2][2] = '#';

    // Reset players array
    for (int i = 0; i < MAX_CLIENTS; i++) {
        g_gameState.players[i].x = -1;
        g_gameState.players[i].y = -1;
        g_gameState.players[i].active = 0;
        g_clientSockets[i] = -1;
    }

    g_gameState.clientCount = 0;
}

/*---------------------------------------------------------------------------*
 * TODO: Refresh the grid to place each active player
 *  - Clear old positions (except obstacles)
 *  - Mark each player in the grid (e.g. 'A'+i)
 *---------------------------------------------------------------------------*/
void refreshPlayerPositions() {
    // 1. Clear non-obstacle cells (set them to '.')
    // 2. For each active player, place a character on the grid
}

/*---------------------------------------------------------------------------*
 * TODO: Build a string that represents the current game state (ASCII grid),
 *       which you can send to all clients.
 *---------------------------------------------------------------------------*/
void buildStateString(char *outBuffer) {
    // e.g., prefix with "STATE\n", then rows of the grid, then player info
    outBuffer[0] = '\0'; // start empty

    strcat(outBuffer, "STATE\n");

    // Copy the grid
    // ...
    // Optionally append player info
}

/*---------------------------------------------------------------------------*
 * Broadcast the current game state to all connected clients
 *---------------------------------------------------------------------------*/
void broadcastState() {
    char buffer[BUFFER_SIZE];
    buildStateString(buffer);

    // TODO: send buffer to each active client via send() or write()
}

/*---------------------------------------------------------------------------*
 * TODO: Handle a client command: MOVE, ATTACK, QUIT, etc.
 *  - parse the string
 *  - update the player's position or HP
 *  - call refreshPlayerPositions() and broadcastState()
 *---------------------------------------------------------------------------*/
void handleCommand(int playerIndex, const char *cmd) {
    // Lock state if needed
    pthread_mutex_lock(&g_stateMutex);

    // Example:
    // if (strncmp(cmd, "MOVE", 4) == 0) { ... }
    // else if (strncmp(cmd, "ATTACK", 6) == 0) { ... }
    // else if (strncmp(cmd, "QUIT", 4) == 0) { ... }

    // refreshPlayerPositions();
    // broadcastState();

    pthread_mutex_unlock(&g_stateMutex);
}

/*---------------------------------------------------------------------------*
 * Thread function: handle communication with one client
 *---------------------------------------------------------------------------*/
void *clientHandler(void *arg) {
    int playerIndex = *(int *)arg;
    free(arg);

    int clientSocket = g_clientSockets[playerIndex];

    // Example: set player's initial position
    pthread_mutex_lock(&g_stateMutex);
    g_gameState.players[playerIndex].x = playerIndex; // naive approach
    g_gameState.players[playerIndex].y = 0;
    g_gameState.players[playerIndex].active = 1;
    refreshPlayerPositions();
    broadcastState();
    pthread_mutex_unlock(&g_stateMutex);

    char buffer[BUFFER_SIZE];

    // Main recv loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        // TODO: recv from clientSocket
        // ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        // if (bytesRead <= 0) {
        //     // disconnected
        //     break;
        // }
        // handleCommand(playerIndex, buffer);
        // check if QUIT => break?
    }

    // Cleanup after disconnect
    close(clientSocket);

    pthread_mutex_lock(&g_stateMutex);
    g_clientSockets[playerIndex] = -1;
    g_gameState.players[playerIndex].active = 0;
    refreshPlayerPositions();
    broadcastState();
    pthread_mutex_unlock(&g_stateMutex);

    return NULL;
}

/*---------------------------------------------------------------------------*
 * main: set up server socket, accept clients, spawn threads
 *---------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);

    // 1. Initialize game state
    initGameState();

    // 2. Create server socket
    // int serverSock = socket(...);
    // TODO

    // 3. Bind, listen
    // struct sockaddr_in serverAddr;
    // ...
    // bind(...)
    // listen(...)

    printf("Server listening on port %d...\n", port);

    // 4. Accept loop
    while (1) {
        // int newSock = accept(...);
        // if (newSock < 0) { continue; }

        // If we have capacity, find a free index in g_clientSockets
        // create a thread: pthread_t tid;
        // int *arg = malloc(sizeof(int));
        // *arg = freeIndex;
        // pthread_create(&tid, NULL, clientHandler, arg);
        // pthread_detach(tid);
    }

    // close(serverSock);
    return 0;
}