package com.zipper.lib.encrypt;

import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Arrays;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import sun.net.www.content.text.plain;

public final class SymmetricEncrypt {

    private SymmetricEncrypt() {
        throw new UnsupportedOperationException("not supported yet");
    }

    private static final String TYPE_DES = "DES";
    private static final String TYPE_AES = "AES";
    private static final String TYPE_DES3 = "DESEDE";

    public static byte[] getSecretKey(String encryptType) throws NoSuchAlgorithmException {
        KeyGenerator keyGenerator = KeyGenerator.getInstance(encryptType);
        keyGenerator.init(new SecureRandom());
        SecretKey generateKey = keyGenerator.generateKey();

        return generateKey.getEncoded();
    }

    public static byte[] encrypt(String plain, String algorithm, byte[] key) {
        return encrypt(plain, algorithm, key, null);
    }

    public static byte[] encrypt(String plain, String algorithm, byte[] key, byte[] iv) {
        if (plain == null || plain.length() == 0) {
            return new byte[0];
        }

        if (StringUtils.isEmpty(algorithm)) {
            throw new IllegalArgumentException("algorithm must not none");
        }
        if (key == null || key.length < 8) {
            throw new IllegalArgumentException("key length must grater then 8");
        }

        SecretKey secretKey = new SecretKeySpec(key, algorithm);
        try {
            Cipher cipher = Cipher.getInstance(algorithm);
            if (iv == null || iv.length == 0) {
                cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            } else {
                IvParameterSpec ivParameterSpec = new IvParameterSpec(iv);
                cipher.init(Cipher.ENCRYPT_MODE, secretKey, ivParameterSpec);
            }
            return cipher.doFinal(plain.getBytes());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new byte[0];
    }

    public static byte[] decrypt(String text, String algorithm, byte[] key) {
        return decrypt(text, algorithm, key, null);
    }

    public static byte[] decrypt(String text, String algorithm, byte[] key, byte[] iv) {
        if (text == null || text.length() == 0) {
            return new byte[0];
        }

        if (StringUtils.isEmpty(algorithm)) {
            throw new IllegalArgumentException("algorithm must not none");
        }
        if (key == null || key.length < 8) {
            throw new IllegalArgumentException("key length must grater then 8");
        }

        SecretKey secretKey = new SecretKeySpec(key, algorithm);
        try {
            Cipher cipher = Cipher.getInstance(algorithm);
            if (iv == null || iv.length == 0) {
                cipher.init(Cipher.DECRYPT_MODE, secretKey);
            } else {
                IvParameterSpec ivParameterSpec = new IvParameterSpec(iv);
                cipher.init(Cipher.DECRYPT_MODE, secretKey, ivParameterSpec);
            }
            return cipher.doFinal(text.getBytes());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new byte[0];
    }

    public static String desEncrypt(byte[] key, String plain) {
        return StringUtils.hex(encrypt(plain, TYPE_DES, key));
    }

    public static String desDecrypt(byte[] key, String text) {
        return StringUtils.hex(decrypt(text, TYPE_DES, key));
    }

    public static String des3Encrypt(byte[] key, String plain) {
        return StringUtils.hex(encrypt(plain, TYPE_DES3, key));
    }

    public static String des3Decrypt(byte[] key, String text) {
        return StringUtils.hex(decrypt(text, TYPE_DES3, key));
    }

    public static String aesEncrypt(byte[] key, String plain) {
        return StringUtils.hex(encrypt(plain, TYPE_AES, key));
    }

    public static String aesDecrypt(byte[] key, String text) {
        return StringUtils.hex(decrypt(text, TYPE_AES, key));
    }

}
