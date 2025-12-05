int main()
{
    int x;
    int y;

    x = 0;
    when (x > 10)
    {
        print(x);
    }
    or (x < 4)
    {
        print(x);
        x = x + 1;
    }
    else
    {
        x = x + 2;
    }

    y = 0;
    when (y > 5)
    {
        print(y);
    }
    or (y == 3)
    {
        break when (y > 2);
    }
    else
    {
        y = y + 1;
    }
    print(x);
    print(y);

    return 0;
}
