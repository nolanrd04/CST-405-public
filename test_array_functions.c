void ProcessArray(int[] arr, int size)
{
    int i = 0;
    while (i < size)
    {
        arr[i] = arr[i] * 2;
        i = i + 1;
    }
}

int main()
{
    int[] nums = {1, 2, 3, 4, 5};
    ProcessArray(nums, 5);
    
    println(nums[0]);
    println(nums[1]);
    println(nums[2]);
    
    return 0;
}
