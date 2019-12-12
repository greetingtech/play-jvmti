
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarOutputStream;

/**
 * @author greetingtech
 * @date 2019-12-10
 */
public class Encrypter {

    native byte[] encrypt(byte[] _buf);

    static {
        System.loadLibrary("encrypt");
    }

    static Map<String, String> getArgMap(String[] args){
        Map<String, String> map = new HashMap<>();
        String key = null, val=null;
        for(String tmp : args){
            if(tmp.startsWith("-")){
                if(key!=null)
                    map.put(key, val);
                key = tmp;
                val = null;
            }else{
                val = tmp;
            }
        }
        if(key!=null){
            map.put(key, val);
        }

        return map;
    }

    public static void main(String[] args) throws Exception {
        Map<String, String> map = getArgMap(args);

        String src_name = map.get("-src");
        if(src_name==null){
            System.out.println("usage: java Main -src xxx.jar");
            return;
        }

        Encrypter coder = new Encrypter();

        String dst_name = map.get("-dst");
        if( dst_name==null||dst_name.equals(src_name) )
            dst_name = src_name.substring(0, src_name.length()-4) + "_encrypt.jar";

        System.out.printf("encode jar file: [%s ==> %s ]\n", src_name, dst_name);

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        byte[] buf = new byte[1024];

        File dst_file = new File(dst_name);
        File src_file = new File(src_name);

        FileOutputStream dst_fos = new FileOutputStream(dst_file);
        JarOutputStream dst_jar = new JarOutputStream(dst_fos);

        JarFile src_jar = new JarFile(src_file);
        for (Enumeration<JarEntry> enumeration = src_jar.entries(); enumeration.hasMoreElements(); ) {
            JarEntry entry = enumeration.nextElement();

            InputStream is = src_jar.getInputStream(entry);
            int len;
            while ((len = is.read(buf, 0, buf.length)) != -1) {
                baos.write(buf, 0, len);
            }
            byte[] bytes = baos.toByteArray();

            String name = entry.getName();
            String suffix = name.substring(name.lastIndexOf(".") + 1);
            boolean flag = false;
            if (suffix.equals("class")) {
                flag = true;
            }
            suffix = name.substring(name.lastIndexOf("/") + 1);
            if (suffix.equals("class")) {
                flag = true;
            }
            if (!flag) {
                continue;
            }

            System.out.println("try encrypt " + name.replaceAll("/", "."));
            try {
                bytes = coder.encrypt(bytes);
                System.out.printf("..OK\n");
            }catch (Exception e){
                System.out.println("...ERR");
                e.printStackTrace();
            }

            JarEntry ne = new JarEntry(name);
            dst_jar.putNextEntry(ne);
            dst_jar.write(bytes);
            baos.reset();
        }
        src_jar.close();

        dst_jar.close();
        dst_fos.close();
    }
}