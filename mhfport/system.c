// MHF-DOS - A Basic Operating System for the MHF CPU
// Written in Mighf C to demonstrate the system capabilities

// System memory layout:
// 0x0000-0x0FFF: Kernel space
// 0x1000-0x1FFF: System variables and buffers
// 0x2000-0x3FFF: User program space
// 0x4000-0x7FFF: Free memory
// 0x8000-0xFEFF: Stack space
// 0xFF00: Memory-mapped output (putchar)

// Global system variables
int system_ticks;
int current_process;
char input_buffer[256];
int buffer_pos;
int shell_running;

// Process structure (simplified)
int process_state[4];  // 4 processes max
int process_pc[4];     // Program counter for each process

// System calls
void sys_print(char* message) {
    int i = 0;
    while (message[i] != 0) {
        putchar(message[i]);
        i++;
    }
}

void sys_println(char* message) {
    sys_print(message);
    putchar(10);  // newline
}

void sys_print_number(int num) {
    if (num == 0) {
        putchar(48);  // '0'
        return;
    }
    
    if (num < 0) {
        putchar(45);  // '-'
        num = -num;
    }
    
    // Convert number to string (simplified)
    char digits[10];
    int digit_count = 0;
    
    while (num > 0) {
        digits[digit_count] = (num % 10) + 48;  // Convert to ASCII
        num = num / 10;
        digit_count++;
    }
    
    // Print digits in reverse order
    int i = digit_count - 1;
    while (i >= 0) {
        putchar(digits[i]);
        i--;
    }
}

// Memory management (very basic)
int next_free_address = 0x2000;

int sys_malloc(int size) {
    int addr = next_free_address;
    next_free_address = next_free_address + size;
    if (next_free_address > 0x7FFF) {
        return 0;  // Out of memory
    }
    return addr;
}

// Timer and scheduling
void timer_tick() {
    system_ticks++;
    
    // Simple round-robin scheduling
    if (system_ticks % 10 == 0) {
        current_process++;
        if (current_process >= 4) {
            current_process = 0;
        }
    }
}

// Process management
void create_process(int process_id, int start_address) {
    if (process_id < 4) {
        process_state[process_id] = 1;  // Running
        process_pc[process_id] = start_address;
        sys_print("Created process ");
        sys_print_number(process_id);
        sys_println("");
    }
}

void kill_process(int process_id) {
    if (process_id < 4) {
        process_state[process_id] = 0;  // Dead
        sys_print("Killed process ");
        sys_print_number(process_id);
        sys_println("");
    }
}

// Simple shell commands
void cmd_help() {
    sys_println("MHF-DOS Commands:");
    sys_println("  help    - Show this help");
    sys_println("  status  - Show system status");
    sys_println("  mem     - Show memory info");
    sys_println("  proc    - Show processes");
    sys_println("  time    - Show system uptime");
    sys_println("  hello   - Say hello");
    sys_println("  calc    - Simple calculator");
    sys_println("  clear   - Clear screen");
    sys_println("  shutdown- Halt system");
}

void cmd_status() {
    sys_println("MHF-DOS Status:");
    sys_print("System ticks: ");
    sys_print_number(system_ticks);
    sys_println("");
    sys_print("Current process: ");
    sys_print_number(current_process);
    sys_println("");
    sys_print("Free memory starts at: 0x");
    sys_print_number(next_free_address);
    sys_println("");
}

void cmd_memory() {
    sys_println("Memory Layout:");
    sys_println("0x0000-0x0FFF: Kernel");
    sys_println("0x1000-0x1FFF: System data");
    sys_println("0x2000-0x3FFF: User programs");
    sys_println("0x4000-0x7FFF: Free memory");
    sys_println("0x8000-0xFEFF: Stack");
    sys_println("0xFF00: I/O port");
    sys_print("Next free: 0x");
    sys_print_number(next_free_address);
    sys_println("");
}

void cmd_processes() {
    sys_println("Process Table:");
    int i = 0;
    while (i < 4) {
        sys_print("PID ");
        sys_print_number(i);
        sys_print(": ");
        if (process_state[i] == 0) {
            sys_print("DEAD");
        } else {
            sys_print("RUNNING at 0x");
            sys_print_number(process_pc[i]);
        }
        sys_println("");
        i++;
    }
}

void cmd_time() {
    sys_print("System uptime: ");
    sys_print_number(system_ticks);
    sys_println(" ticks");
}

void cmd_hello() {
    sys_println("Hello from MHF-DOS!");
    sys_println("Welcome to the MHF CPU operating system.");
}

void cmd_calc() {
    sys_println("Simple Calculator:");
    sys_println("5 + 3 = 8");
    int result = 5 + 3;
    sys_print("Computed: ");
    sys_print_number(result);
    sys_println("");
    
    sys_println("10 - 4 = 6");
    result = 10 - 4;
    sys_print("Computed: ");
    sys_print_number(result);
    sys_println("");
}

void cmd_clear() {
    // Clear screen by printing newlines
    int i = 0;
    while (i < 25) {
        sys_println("");
        i++;
    }
}

void cmd_shutdown() {
    sys_println("Shutting down MHF-DOS...");
    sys_println("Goodbye!");
    shell_running = 0;
}

// Command parser
void parse_command(char* cmd) {
    // Simple string comparison (since we don't have strcmp)
    if (cmd[0] == 'h' && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'p' && cmd[4] == 0) {
        cmd_help();
    }
    else if (cmd[0] == 's' && cmd[1] == 't' && cmd[2] == 'a' && cmd[3] == 't' && cmd[4] == 'u' && cmd[5] == 's' && cmd[6] == 0) {
        cmd_status();
    }
    else if (cmd[0] == 'm' && cmd[1] == 'e' && cmd[2] == 'm' && cmd[3] == 0) {
        cmd_memory();
    }
    else if (cmd[0] == 'p' && cmd[1] == 'r' && cmd[2] == 'o' && cmd[3] == 'c' && cmd[4] == 0) {
        cmd_processes();
    }
    else if (cmd[0] == 't' && cmd[1] == 'i' && cmd[2] == 'm' && cmd[3] == 'e' && cmd[4] == 0) {
        cmd_time();
    }
    else if (cmd[0] == 'h' && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'l' && cmd[4] == 'o' && cmd[5] == 0) {
        cmd_hello();
    }
    else if (cmd[0] == 'c' && cmd[1] == 'a' && cmd[2] == 'l' && cmd[3] == 'c' && cmd[4] == 0) {
        cmd_calc();
    }
    else if (cmd[0] == 'c' && cmd[1] == 'l' && cmd[2] == 'e' && cmd[3] == 'a' && cmd[4] == 'r' && cmd[5] == 0) {
        cmd_clear();
    }
    else if (cmd[0] == 's' && cmd[1] == 'h' && cmd[2] == 'u' && cmd[3] == 't' && cmd[4] == 'd' && cmd[5] == 'o' && cmd[6] == 'w' && cmd[7] == 'n' && cmd[8] == 0) {
        cmd_shutdown();
    }
    else if (cmd[0] == 0) {
        // Empty command, do nothing
    }
    else {
        sys_print("Unknown command: ");
        sys_println(cmd);
        sys_println("Type 'help' for available commands.");
    }
}

// Simple shell
void shell() {
    char command[64];
    int cmd_pos = 0;
    
    sys_println("MHF-DOS Shell v1.0");
    sys_println("Type 'help' for commands");
    
    shell_running = 1;
    
    while (shell_running) {
        // Simulate timer ticks
        timer_tick();
        
        // Show prompt
        sys_print("C:\\>");
        
        // Simulate getting input (in real system this would be keyboard input)
        // For demo, we'll execute predefined commands
        if (system_ticks == 50) {
            sys_println("help");
            cmd_help();
        }
        else if (system_ticks == 100) {
            sys_println("status");
            cmd_status();
        }
        else if (system_ticks == 150) {
            sys_println("mem");
            cmd_memory();
        }
        else if (system_ticks == 200) {
            sys_println("proc");
            cmd_processes();
        }
        else if (system_ticks == 250) {
            sys_println("hello");
            cmd_hello();
        }
        else if (system_ticks == 300) {
            sys_println("calc");
            cmd_calc();
        }
        else if (system_ticks == 350) {
            sys_println("time");
            cmd_time();
        }
        else if (system_ticks == 400) {
            sys_println("shutdown");
            cmd_shutdown();
        }
        else if (system_ticks > 400) {
            break;
        }
        
        // Simulate some processing delay
        int delay = 0;
        while (delay < 100) {
            delay++;
        }
    }
}

// Kernel initialization
void kernel_init() {
    system_ticks = 0;
    current_process = 0;
    buffer_pos = 0;
    next_free_address = 0x2000;
    
    // Initialize process table
    int i = 0;
    while (i < 4) {
        process_state[i] = 0;  // All processes start dead
        process_pc[i] = 0;
        i++;
    }
    
    sys_println("");
    sys_println("==============================");
    sys_println("    MHF-DOS v1.0 Booting");
    sys_println("==============================");
    sys_println("");
    
    sys_println("Initializing kernel...");
    sys_println("Setting up memory management...");
    sys_println("Initializing process table...");
    sys_println("Starting system timer...");
    
    // Create kernel process
    create_process(0, 0x1000);
    
    sys_println("Boot complete!");
    sys_println("");
}

// Main kernel entry point
int main() {
    kernel_init();
    shell();
    
    sys_println("");
    sys_println("System halted.");
    
    return 0;
}
