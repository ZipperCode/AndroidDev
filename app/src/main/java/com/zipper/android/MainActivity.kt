package com.zipper.android

import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.annotation.RequiresApi
import androidx.core.view.OnApplyWindowInsetsListener
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.zipper.android.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setSupportActionBar(null)
        val binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(binding.v, object : OnApplyWindowInsetsListener {
            @RequiresApi(Build.VERSION_CODES.KITKAT_WATCH)
            override fun onApplyWindowInsets(v: View?, insets: WindowInsetsCompat): WindowInsetsCompat {
                Log.d("BAAA", "insets displayCutout = ${insets.displayCutout}")
                Log.d("BAAA", "insets isConsumed = ${insets.isConsumed}")
                Log.d("BAAA", "insets isRound = ${insets.isRound}")
                Log.d("BAAA", "insets hasInsets = ${insets.hasInsets()}")
                Log.d("BAAA", "insets toWindowInsets = ${insets.toWindowInsets()}")

                return insets
            }

        })
    }
}