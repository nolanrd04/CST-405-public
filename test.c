int add(int x, int y) {
    int result;
    result = x + y;
    return result;
}

int multiply(int x, int y) {
    int result;
    result = x * y;
    return result;
}

int main() {
    int a;
    int b;
    int sum;
    int product;
    
    a = 5;
    b = 3;
    
    sum = add(a, b);
    product = multiply(a, b);
    
    print(sum);
    print(product);
    
    return 0;
}