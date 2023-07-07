package com.example.binder.canary.sample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.zipper.develop.binder.canary.NativeLib;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        new NativeLib().stringFromJNI();
    }
}