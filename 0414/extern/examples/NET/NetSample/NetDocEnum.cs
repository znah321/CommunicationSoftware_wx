namespace NetDocEnum
{
    [System.Flags()]
    public enum MyDays
    {
        None = 0,
        Monday = 1,
        Tuesday = 2,
        Wednesday = 4,
        Thursday = 8,
        Friday = 16,
        Saturday = 32,
        Sunday = 64
    }

    public enum Range : long
    {
        Max = 2147483648L,
        Min = 255L
    }
}
