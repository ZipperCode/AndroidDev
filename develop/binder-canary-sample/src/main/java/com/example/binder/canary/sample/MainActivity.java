package com.example.binder.canary.sample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;

import com.zipper.develop.binder.canary.BinderCanary;
import com.zipper.develop.binder.canary.MonitorConfig;
import com.zipper.hiddenapibypass.unsafe.HiddenApiBypass;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            HiddenApiBypass.setHiddenApiExemptions("L");
        }
        setContentView(R.layout.activity_main);
        BinderCanary.init(new MonitorConfig());
    }
}