package com.zipper.lib.encrypt;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.crypto.KeyGenerator;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

public final class HashEncrypt {

    private HashEncrypt(){
        throw new UnsupportedOperationException("Not supported yet");
    }

    public static byte[] md5(final byte[] data) {
        return hash(data, "MD5");
    }

    public static String md5Hex(final byte[] data) {
        return StringUtils.hex(hash(data, "MD5"));
    }

    public static byte[] sha1(final byte[] data) {
        return hash(data, "SHA1");
    }

    public static byte[] sha256(final byte[] data) {
        return hash(data, "SHA-256");
    }

    public static byte[] sha512(final byte[] data) {
        return hash(data, "SHA-512");
    }

    public static byte[] hMac(final byte[] key, final byte[] data) {
        try {
            String algorithm = "HmacMD5";
            Mac mac = Mac.getInstance(algorithm);
            SecretKeySpec keySpec = new SecretKeySpec(key, algorithm);
            mac.init(keySpec);
            mac.update(data);
            return mac.doFinal();
        } catch (Exception e){
            e.printStackTrace();
        }
        return new byte[0];
    }

    public static byte[] hash(final byte[] data, final String algorithm) {
        if (data == null || data.length <= 0) {
            return new byte[0];
        }
        try {
            MessageDigest md = MessageDigest.getInstance(algorithm);
            md.update(data);
            return md.digest();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
            return new byte[0];
        }
    }

    public static byte[] fileHash(final File file){
        return fileHash(file, "MD5");
    }

    public static byte[] fileHash(final File file, final String algorithm){
        if (file == null || !file.exists()){
            return new byte[0];
        }
        DigestInputStream digestInputStream;
        try (FileInputStream fis = new FileInputStream(file)) {
            MessageDigest md = MessageDigest.getInstance(algorithm);
            digestInputStream = new DigestInputStream(fis, md);
            byte[] buffer = new byte[256 * 1024];
            while (true) {
                int read = digestInputStream.read(buffer);
                if (!(read > 0)) {
                    break;
                }
            }
            md = digestInputStream.getMessageDigest();
            return md.digest();
        } catch (NoSuchAlgorithmException | IOException e) {
            e.printStackTrace();
            return new byte[0];
        }
    }

    public static void main(String[] args) {
        System.out.println(StringUtils.hex(sha512(new byte[]{0,1})));;
    }
}
