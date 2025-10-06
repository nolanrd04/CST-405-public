int tacOptimizationTest() {
    int a, b, c;

    a = 5;
    b = 0;
    c = a + b;  // Should optimize c=a+b to c=a
    c = c; // Should eliminate self-assignment

    print(a);
    print(b);
    print(c);
}
