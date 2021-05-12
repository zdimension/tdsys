public class exo1
{
    public static void main(String[] args)
    {
        long pid = ProcessHandle.current().pid();
        System.out.println("Exo1 PID = " + pid);
    }
}
