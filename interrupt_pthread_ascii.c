#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>   // For Sleep()

#define DEVICE_COUNT 3

// Device identifiers
enum Device { KEYBOARD = 0, MOUSE = 1, PRINTER = 2 };

// Human-readable device names
const char* deviceName(int device) {
    switch (device) {
        case KEYBOARD: return "Keyboard";
        case MOUSE:    return "Mouse";
        case PRINTER:  return "Printer";
        default:       return "Unknown";
    }
}

// Shared state
bool maskStatus[DEVICE_COUNT] = { false, false, false };
int pendingInterrupts[DEVICE_COUNT] = {0, 0, 0};
pthread_mutex_t queueLock;

// File pointer for logging
FILE* logFile;

// Function to get current timestamp as string
void getTimestamp(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

// Thread function to simulate interrupts asynchronously
void* deviceInterruptSimulator(void* arg) {
    srand((unsigned int)time(NULL));

    for (int step = 0; step < 20; step++) {
        Sleep((rand() % 2 + 1) * 1000); // Random delay 1-2 seconds

        int deviceTriggered = rand() % DEVICE_COUNT;

        pthread_mutex_lock(&queueLock);

        char timestamp[20];
        getTimestamp(timestamp, sizeof(timestamp));

        if (maskStatus[deviceTriggered]) {
            printf("%s Interrupt Ignored (Masked)\n", deviceName(deviceTriggered));
            fprintf(logFile, "[%s] %s Interrupt Ignored (Masked)\n", timestamp, deviceName(deviceTriggered));
        } else {
            pendingInterrupts[deviceTriggered]++;

            // Handle highest-priority pending interrupt
            for (int i = 0; i < DEVICE_COUNT; i++) {
                if (pendingInterrupts[i] > 0) {
                    printf("%s Interrupt Triggered -> Handling ISR -> Completed\n", deviceName(i));
                    fprintf(logFile, "[%s] %s Interrupt Triggered -> Handling ISR -> Completed\n", timestamp, deviceName(i));
                    pendingInterrupts[i]--;
                    break;
                }
            }
        }

        pthread_mutex_unlock(&queueLock);
    }

    return NULL;
}

int main() {
    pthread_t simulatorThread;

    printf("=== Interrupt Controller Simulation with Logging ===\n");
    printf("Devices: Keyboard (High), Mouse (Medium), Printer (Low)\n\n");

    // Open log file for writing
    logFile = fopen("C:\\Users\\sskan\\nnm24is191\\interrupt_log.txt", "w");
    if (!logFile) {
        perror("Error opening log file");
        return 1;
    }

    pthread_mutex_init(&queueLock, NULL);

    pthread_create(&simulatorThread, NULL, deviceInterruptSimulator, NULL);
    pthread_join(simulatorThread, NULL);

    pthread_mutex_destroy(&queueLock);
    fclose(logFile);

    printf("\nSimulation complete. All interrupts handled.\n");
    printf("Execution log saved to 'C:\\Users\\sskan\\nnm24is191\\interrupt_log.txt'.\n");

    return 0;
}
