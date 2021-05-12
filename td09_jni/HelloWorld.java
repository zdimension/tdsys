import java.io.IOException;

public class HelloWorld
{
    public static native void printCpp();
    public static native void printStringToCpp(String str);
    public native String stringFromCpp();
    public native void callJavaMethod();
    public static native int fib(int n);

    @Override
    public native String toString();

    int entier = 42;

    static void test(String str)
    {
        System.out.println("Fonction test() appelée avec : " + str);
    }

    static
    {
        System.loadLibrary("HelloWorld"); // Chargement de la bibliothèque
    }

    public static void main(String[] args) throws IOException
    {
        System.out.print("Hello "); // Affiche Hello en Java
        HelloWorld.printCpp(); // Affiche World en C/C++
        System.out.println("Appuyez sur une touche");
        System.in.read();

        var obj = new HelloWorld();
        printStringToCpp(obj.stringFromCpp());

        obj.callJavaMethod();

        System.out.println("Original : a = " + obj.entier);
        System.out.println(obj.toString());

        System.out.println(fib(10));
    }
}