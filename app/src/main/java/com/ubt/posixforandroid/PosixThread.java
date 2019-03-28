package com.ubt.posixforandroid;

/**
 * description：
 * <p/>
 * Created by TIAN FENG on 2019/3/28
 * QQ：27674569
 * Email: 27674569@qq.com
 * Version：1.0
 */
public class PosixThread {

    static {
        System.loadLibrary("native-lib");
    }

    public native void init();

    public native void pthread();


    public native void destroy();

}
