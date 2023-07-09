package com.zipper.lib.encrypt;

import java.util.Arrays;

public final class StringUtils {

    private static final String HEX_CHARS = "0123456789abcdef";

    private static final char[] HEX_DIGITS =
            {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    private StringUtils() {
        throw new UnsupportedOperationException("not supported yet");
    }

    public static boolean isEmpty(String text) {
        return text == null || text.length() == 0;
    }

    public static boolean isNotEmpty(String text) {
        return !isEmpty(text);
    }

    public static byte[] hex(String hexString) {
        if (hexString == null || hexString.length() == 0) {
            return new byte[0];
        }
        if (hexString.length() % 2 != 0){
            throw new IllegalArgumentException("Invalid hex string");
        }
        byte[] result = new byte[hexString.length() >> 1];
        int j = 0;
        // 10
        for (int i = 0; i < result.length; i++) {
            int h = Character.digit(hexString.charAt(j++), 16);
            int l = Character.digit(hexString.charAt(j++), 16);
            result[i] = (byte) ((h << 4 & 0xFF) + l & 0xFF);
        }
        return result;
    }

    public static String hex(byte[] bytes) {
        if (bytes == null || bytes.length == 0) {
            return "";
        }
        char[] result = new char[bytes.length << 1];
        for (int i = 0, j = 0; i < bytes.length; i++) {
            result[j++] = HEX_DIGITS[bytes[i] >>> 4 & 0x0f];
            result[j++] = HEX_DIGITS[bytes[i] & 0x0f];
        }
        return new String(result);
    }

    public static void main(String[] args) {
        byte[] data = new byte[]{0,10, 15};
        String s = hex(data);
        byte[] data2 = hex(s);
        System.out.println(Arrays.toString(data2));
    }
}
