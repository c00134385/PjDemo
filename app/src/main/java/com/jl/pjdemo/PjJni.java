package com.jl.pjdemo;

public class PjJni {
    private static final PjJni ourInstance = new PjJni();

    public static PjJni getInstance() {
        return ourInstance;
    }

    private PjJni() {
    }

    static {
        System.loadLibrary("native-lib");
    }

    public native int init(PjCb cb);

    public native int uninit();

    public native int start(String ip, int port, PjCb cb);

    public native int stop();

    public native String version();

}
