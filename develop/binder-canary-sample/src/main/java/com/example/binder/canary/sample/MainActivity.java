package com.example.binder.canary.sample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.zipper.develop.binder.canary.BinderCanary;
import com.zipper.develop.binder.canary.MonitorConfig;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        BinderCanary.init(new MonitorConfig());
        BinderCanary.monitor();
    }
}