package com.jl.pjdemo;

import android.app.Application;

import org.jetbrains.annotations.Nullable;

import timber.log.Timber;

public class MyApp extends Application {

    @Override
    public void onCreate() {
        super.onCreate();

//        if (BuildConfig.DEBUG) {
//            Timber.plant(new Timber.DebugTree());
//        } else {
//            Timber.plant(new CrashReportingTree());
//        }

        Timber.plant(new Timber.DebugTree(){
            @Override
            protected boolean isLoggable(@Nullable String tag, int priority) {
                return BuildConfig.DEBUG;
            }
        });
    }
}
