#include "libraries.h"
#include "memory.h"

#define T(TYPE) &memory, TYPE

int main() {
    INIT_MEMORY(memory);

    Variant a = {.i = 1, .type = Int};
    memory.new(T(Int), "a", a);
    memory.printRegister(T(Int));

    Variant b = {.d = 1.2, .type = Double};
    memory.new(T(Double), "b", b);
    memory.printRegister(T(Double));

    Variant c = {.s = "kektor", .type = String};
    memory.new(T(String), "c", c);
    memory.printRegister(T(String));
}
