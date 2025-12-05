void Inc(int[] arr)
{
    arr[0] = arr[0] + 100;
}

int main()
{
    int[] nums = {5};
    Inc(nums);
    
    println(nums[0]);
    
    return 0;
}
