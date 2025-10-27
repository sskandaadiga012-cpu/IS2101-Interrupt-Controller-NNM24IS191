
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


// 1 = enabled (unmasked), 0 = disabled (masked)
int keyboard_enabled = 1;
int mouse_enabled = 0;   // Masked as per requirement
int printer_enabled = 1;

pthread_mutex_t lock;

// Function to log interrupt activity with timestamps
void log_event(const char *device, const char *status) {
    FILE *file = fopen("interrupt_log.txt", "a");
    if (file == NULL) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(file, "[%02d-%02d-%04d %02d:%02d:%02d] %s Interrupt %s\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec,
            device, status);

    fclose(file);
}

// ISR (Interrupt Service Routine) for Keyboard
void *keyboard_isr(void *arg) {
    while (1) {
        sleep(rand() % 3 + 1);
        pthread_mutex_lock(&lock);
        if (keyboard_enabled) {
            printf("Keyboard Interrupt Triggered -> Handling ISR -> Completed\n");
            log_event("Keyboard", "Handled -> Completed");
        } else {
            printf("Keyboard Interrupt Ignored (Masked)\n");
            log_event("Keyboard", "Ignored (Masked)");
        }
        pthread_mutex_unlock(&lock);
    }
}

// ISR for Mouse (Masked)
void *mouse_isr(void *arg) {
    while (1) {
        sleep(rand() % 3 + 1);
        pthread_mutex_lock(&lock);
        if (mouse_enabled) {
            printf("Mouse Interrupt Triggered -> Handling ISR -> Completed\n");
            log_event("Mouse", "Handled -> Completed");
        } else {
            printf("Mouse Interrupt Ignored (Masked)\n");
            log_event("Mouse", "Ignored (Masked)");
        }
        pthread_mutex_unlock(&lock);
    }
}

// ISR for Printer
void *printer_isr(void *arg) {
    while (1) {
        sleep(rand() % 4 + 1);
        pthread_mutex_lock(&lock);
        if (printer_enabled) {
            printf("Printer Interrupt Triggered -> Handling ISR -> Completed\n");
            log_event("Printer", "Handled -> Completed");
        } else {
            printf("Printer Interrupt Ignored (Masked)\n");
            log_event("Printer", "Ignored (Masked)");
        }
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    srand(time(0));

    pthread_t t1, t2, t3;
    pthread_mutex_init(&lock, NULL);

    // Clear the previous log file contents
    FILE *clear = fopen("interrupt_log.txt", "w");
    if (clear != NULL) {
        fprintf(clear, "==== Interrupt Controller Simulation Log ====\n");
        fclose(clear);
    }

    printf("Interrupt Controller Simulation Started\n");
    printf("Keyboard = Enabled | Mouse = Masked | Printer = Enabled\n\n");

    pthread_create(&t1, NULL, keyboard_isr, NULL);
    pthread_create(&t2, NULL, mouse_isr, NULL);
    pthread_create(&t3, NULL, printer_isr, NULL);

    // Run for 20 seconds before stopping
    sleep(20);

    printf("\nSimulation Completed. Check 'interrupt_log.txt' for details.\n");

    // Clean up
    pthread_cancel(t1);
    pthread_cancel(t2);
    pthread_cancel(t3);
    pthread_mutex_destroy(&lock);

    return 0;
}
