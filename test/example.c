int f() {
    main();
    return 1;
}

int myfunc(int x) {
    int z = 3;
    if (x < 3)
        return 0;
    z++;
    myfunc(z - 2);
    return f();
}

int main() {
    int a = 5;
}
