# IS2101-Interrupt-Controller-NNM24IS191
To simulate the working of an Interrupt Controller and understand how priorities and masking affect device handling in a computer system, using real-world programming logic and event simulation.

## Features
* Simulates three I/O devices:
   * Keyboard – High Priority
   * Mouse – Medium Priority
   * Printer – Low Priority
* Handles interrupts based on priority.
* Supports masking: enables or disables specific device interrupts at runtime.
* Displays clear console output for triggered, handled, or ignored interrupts.
* Optional: logs each Interrupt Service Routine (ISR) execution with timestamps to a text file.

## File structure 
* `interrupt_pthread_ascii.c` - Your main source code with pthread simulation, priority handling, masking, and optional logging.

## Compilation 
```bash
gcc -std=c11 interrupt_pthread_ascii.c -o interrupt_pthread_ascii.exe -lpthread
.\interrupt_pthread_ascii.exe
```

## Output of Interrupt Controller
<img width="878" height="576" alt="Screenshot 2025-10-26 230620" src="https://github.com/user-attachments/assets/a3d7f38e-ecf4-4ba4-83bf-cb88b03adda2" />
