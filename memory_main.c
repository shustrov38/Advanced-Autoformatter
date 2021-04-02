#include "libraries.h"
#include "memory.h"

// #define T(name, var) &memory, var.type, name, var

int main() {
    INIT_MEMORY(memory);

    MEMORY_NEW(memory, Int, "a", 12);
    memory.printRegister(&memory, Int);

    Variant *p = memory.getValue(&memory, "a");
    p->i++;
    memory.printRegister(&memory, Int);
}
