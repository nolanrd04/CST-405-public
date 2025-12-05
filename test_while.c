int main()
{
    int a = 10;
    int b = 5;
    int c = 20;
    int d = 10;
    while ((a > b)*(c > d))
    {
        print("r");
        print("=");
        print((a > b)*(c > d));
        println("");
        a = a - 1;
    }

    return 0;
}