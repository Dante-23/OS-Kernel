#include "../drivers/print_string.c"
#include "../drivers/essentials.c"

#define KERNEL_CS 0x08
#define IDT_ENTRIES 256

long long timer_counter = 0;

typedef struct {
    unsigned short low_offset; /* Lower 16 bits of handler function address */
    unsigned short sel; /* Kernel segment selector */
    unsigned char always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    unsigned char flags; 
    unsigned short high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t ;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */
typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* Struct which aggregates many registers */
typedef struct {
   unsigned int ds; /* Data segment selector */
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   unsigned int int_no, err_code; /* Interrupt number and error code (if applicable) */
   unsigned int eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;


void set_idt_gate(int n, unsigned int handler) {
    idt[n].low_offset = get_low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = get_high_16(handler);
}

void set_idt() {
    idt_reg.base = (unsigned int) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}

void isr_install() {
    set_idt_gate(0, (unsigned int)isr0);
    set_idt_gate(1, (unsigned int)isr1);
    set_idt_gate(2, (unsigned int)isr2);
    set_idt_gate(3, (unsigned int)isr3);
    set_idt_gate(4, (unsigned int)isr4);
    set_idt_gate(5, (unsigned int)isr5);
    set_idt_gate(6, (unsigned int)isr6);
    set_idt_gate(7, (unsigned int)isr7);
    set_idt_gate(8, (unsigned int)isr8);
    set_idt_gate(9, (unsigned int)isr9);
    set_idt_gate(10, (unsigned int)isr10);
    set_idt_gate(11, (unsigned int)isr11);
    set_idt_gate(12, (unsigned int)isr12);
    set_idt_gate(13, (unsigned int)isr13);
    set_idt_gate(14, (unsigned int)isr14);
    set_idt_gate(15, (unsigned int)isr15);
    set_idt_gate(16, (unsigned int)isr16);
    set_idt_gate(17, (unsigned int)isr17);
    set_idt_gate(18, (unsigned int)isr18);
    set_idt_gate(19, (unsigned int)isr19);
    set_idt_gate(20, (unsigned int)isr20);
    set_idt_gate(21, (unsigned int)isr21);
    set_idt_gate(22, (unsigned int)isr22);
    set_idt_gate(23, (unsigned int)isr23);
    set_idt_gate(24, (unsigned int)isr24);
    set_idt_gate(25, (unsigned int)isr25);
    set_idt_gate(26, (unsigned int)isr26);
    set_idt_gate(27, (unsigned int)isr27);
    set_idt_gate(28, (unsigned int)isr28);
    set_idt_gate(29, (unsigned int)isr29);
    set_idt_gate(30, (unsigned int)isr30);
    set_idt_gate(31, (unsigned int)isr31);
    
    // Remap the PIC
    outputbyte(0x20, 0x11);
    outputbyte(0xA0, 0x11);
    outputbyte(0x21, 0x20);
    outputbyte(0xA1, 0x28);
    outputbyte(0x21, 0x04);
    outputbyte(0xA1, 0x02);
    outputbyte(0x21, 0x01);
    outputbyte(0xA1, 0x01);
    outputbyte(0x21, 0x0);
    outputbyte(0xA1, 0x0); 

    // Install the IRQs
    set_idt_gate(32, (unsigned int)irq0);
    set_idt_gate(33, (unsigned int)irq1);
    set_idt_gate(34, (unsigned int)irq2);
    set_idt_gate(35, (unsigned int)irq3);
    set_idt_gate(36, (unsigned int)irq4);
    set_idt_gate(37, (unsigned int)irq5);
    set_idt_gate(38, (unsigned int)irq6);
    set_idt_gate(39, (unsigned int)irq7);
    set_idt_gate(40, (unsigned int)irq8);
    set_idt_gate(41, (unsigned int)irq9);
    set_idt_gate(42, (unsigned int)irq10);
    set_idt_gate(43, (unsigned int)irq11);
    set_idt_gate(44, (unsigned int)irq12);
    set_idt_gate(45, (unsigned int)irq13);
    set_idt_gate(46, (unsigned int)irq14);
    set_idt_gate(47, (unsigned int)irq15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void init_timer(unsigned int freq) {
    /* Install the function we just wrote */
    // register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    unsigned int divisor = 1193180 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)( (divisor >> 8) & 0xFF);
    /* Send the command */
    outputbyte(0x43, 0x36); /* Command port */
    outputbyte(0x40, low);
    outputbyte(0x40, high);
}

void isr_handler(registers_t r) {
    print_string("receieved interrupt");
    // kprint("received interrupt: ");
     char s[3];
     int_to_ascii(r.int_no, s);
     print_string(s);
    // kprint(s);
    // kprint("\n");
    // kprint(exception_messages[r.int_no]);
    print_string(exception_messages[r.int_no]);
    print_string("\n");
    // kprint("\n");
}

void irq_handler(registers_t r){
    if(r.int_no == 32){
        timer_counter++;
    }
    else{
        char s[3];
        int_to_ascii(r.int_no, s);
        print_string(s);
    }
    // print_string("received interrupt irq_handler");
    // char s[3];
    // int_to_ascii(r.int_no, s);
    // print_string(s);
    // print_string("\n");
    outputbyte(0x20, 0x20); // EOI command to primary controller
}
