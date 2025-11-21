int[] Swap(int a, int b, int[] arr)
{
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;

    return arr;
}

int main()
{
    int[] num = {5, 10, 15, 20, 25};
    int numLength = 5;

    int x = 0;
    while (x < numLength - 1)
    {
        if (num[x] < num[x+1])
        {
            Swap(x, x+1, num);
        }
        x = x + 1;
    }

    println(num[0]);
    println(num[1]);
    println(num[2]);
    println(num[3]);
    println(num[4]);

    return 0;
}