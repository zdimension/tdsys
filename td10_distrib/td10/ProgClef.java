import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

class ProgClef
{
    public static String MOT_DE_PASSE = "systeme";
    public static final byte[] HASH = {-36, -119, -45, -65, -48, -100, -30, 54, -51, 66, 118, -65, 110, -109, -119, -23};

    public static void main(String[] args) throws NoSuchAlgorithmException
    {
        if (!Arrays.equals(MessageDigest.getInstance("MD5").digest(MOT_DE_PASSE.getBytes(StandardCharsets.UTF_8)),
            HASH))
        {
            System.exit(1);
            return;
        }
        System.out.println("Programme de test");
    }
}
