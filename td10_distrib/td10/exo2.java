public class exo2
{
    public static native long getPid();

    static
    {
        System.loadLibrary("exo2");
    }

    public static void main(String[] args)
    {
        long pid = getPid();
        System.out.println("Exo2 PID = " + pid);
    }
}
