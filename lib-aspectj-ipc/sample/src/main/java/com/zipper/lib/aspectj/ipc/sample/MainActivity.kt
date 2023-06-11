package com.zipper.lib.aspectj.ipc.sample

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.zipper.lib.aspectj.ipc.IIpcLog

class MainActivity : AppCompatActivity() , IIpcLog {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        log("MainActivity#onCreate")
    }

    fun dump2(view: View) {
        log("dump 2")
        val intent = Intent(this, Process2MainActivity::class.java)
        startActivity(intent)
    }
}