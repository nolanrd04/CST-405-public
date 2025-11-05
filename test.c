int Add(int x, int y) {
    int result;
    result = x + y;
    return result;
}

int Multiply(int x, int y) {
    int result;
    result = x * y;
    return result;
}

int main() {
    int x;
    x = 10;

    if (x > 5) {
        print(x);
        int y = Add(3, 4);
        print(y);
    }
    else {
        print(0);
    }

    if (x < 5) {
        print(x);
    }
    else {
        print(0);
    }

    print((2.0 + 1) / (3.0 * 4));

    return 0;
}
