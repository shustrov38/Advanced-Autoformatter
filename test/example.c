void s() {
    f();
}

void g() {
    s();
}

int f() {
    g();
    for (int i = 0; i < 10; ++i) {
        for (int i = 0; i < 10; ++i) {
            for (int i = 0; i < 10; ++i) {
                for (int i = 0; i < 10; ++i) {

                }
            }
        }
    }
    return 54;
}

int main() {
    for (int i = 0; i < 10; ++i) {
        for (int i = 0; i < 10; ++i) {
            for (int i = 0; i < 10; ++i) {
                for (int i = 0; i < 10; ++i) {

                }
            }
        }
        for (int i = 0; i < 10; ++i) {

        }
        for (int i = 0; i < 10; ++i) {
            for (int i = 0; i < 10; ++i) {
                int a = f();
            }
        }
    }
    return 0;
}
