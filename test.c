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
        int y = Add(3, 5);
        print(y);

        if (x < 5) {
            print(x);
        }
        else {
            print(x + 5);
        }
    }
    else {
        print(0);
    }

    x = 10;
    switch (x) {
        case 5:
            print(5);
            break;
        case 10:
            print(10);
            break;
        default:
            print(0);
            break;
    }

    print((2.0 + 1) / (3.0 * 4));

    bool a = true;
    bool b = false;
    bool c = a && b;
    bool d = a || b;
    bool e = !a;

    print(a);
    print(b);
    print(c);
    print(d);
    print(e);

    return 0;
}
