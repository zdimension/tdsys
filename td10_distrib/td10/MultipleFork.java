public class MultipleFork
{
    public static native long fork();
    public static native long posix_wait();
    public static native void sleep(int ms);

    static
    {
        System.loadLibrary("MultipleFork");
    }

    public static void main(String[] args)
    {
        var pids = new long[10];
        for (int i = 0; i < 10; i++)
        {
            if ((pids[i] = fork()) < 0)
            {
                System.err.println("fork");
                System.exit(1);
            }
            else if (pids[i] == 0)
            {
                for (int j = 0; j < 10; j++)
                {
                    System.out.println(i);
                    sleep(1);
                }
                System.exit(0);
            }
        }
        while (posix_wait() != -1);
    }
}
