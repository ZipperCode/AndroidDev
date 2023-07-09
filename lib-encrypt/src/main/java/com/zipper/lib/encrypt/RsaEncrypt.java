package com.zipper.lib.encrypt;

import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.spec.X509EncodedKeySpec;

import javax.crypto.Cipher;

public final class RsaEncrypt {

    /**
     * 非对称加密密钥算法
     */
    public static final String RSA = "RSA";
    /**
     * 加密填充方式
     */
    public static final String ECB_PKCS1_PADDING = "RSA/ECB/PKCS1Padding";

    public RsaEncrypt() {
        throw new UnsupportedOperationException("not supported yet");
    }

    public static Pair<byte[], byte[]> getSecretKey(String encryptType, int len) throws NoSuchAlgorithmException {
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance(encryptType);
        keyPairGenerator.initialize(len);
        KeyPair keyPair = keyPairGenerator.generateKeyPair();
        byte[] pri = keyPair.getPrivate().getEncoded();
        byte[] pub = keyPair.getPublic().getEncoded();
        return new Pair<>(pri, pub);
    }

    /**
     * 用公钥对数据进行加密
     * @param data                      原文
     * @param publicKey                 密钥
     * @return                          byte[] 解密数据
     */
    public static byte[] encryptByPublicKey(byte[] data, byte[] publicKey) throws Exception {
        // 得到公钥
        X509EncodedKeySpec keySpec = new X509EncodedKeySpec(publicKey);
        KeyFactory kf = KeyFactory.getInstance(RSA);
        PublicKey keyPublic = kf.generatePublic(keySpec);
        // 加密数据
        Cipher cp = Cipher.getInstance(ECB_PKCS1_PADDING);
        cp.init(Cipher.ENCRYPT_MODE, keyPublic);
        return cp.doFinal(data);
    }

    public static class Pair<F,S>{
        public F first;
        public S second;

        public Pair(F first, S second) {
            this.first = first;
            this.second = second;
        }
    }
}
