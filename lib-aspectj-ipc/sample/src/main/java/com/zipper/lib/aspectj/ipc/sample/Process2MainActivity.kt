package com.zipper.lib.aspectj.ipc.sample

import android.content.ContentValues
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import com.zipper.lib.aspectj.ipc.IIpcLog

class Process2MainActivity : AppCompatActivity(), IIpcLog {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_process2_main)
        log("Process2MainActivity#onCreate")
    }

    fun call1(view: View) {
        val time = System.currentTimeMillis()
        Test.callMain()
        le("call1 time = ${System.currentTimeMillis() - time}")
    }

    fun call2(view: View) {
        Test.callMain1(1, 2f, "hello world")
    }

    fun call3(view: View) {
        val time = System.currentTimeMillis()
        log("result = ${Test.callMain2()}  time = ${System.currentTimeMillis() - time}")
    }

    fun call4(view: View) {
        time("call4") {
            val b = TestBeanA()
            b.name = "hello world"
            b.names = arrayListOf("zhangsan", "lisi")
            b.ages = arrayListOf(1, 2, 3)
            Test.callMain3(b)
        }
    }

    fun call5(view: View) {
        time("call5") {
            val b = TestBeanA()
            b.name = "hello world"
            b.names = arrayListOf("zhangsan", "lisi")
            b.ages = arrayListOf(1, 2, 3)
            Test.callMain4(arrayListOf(b, b))
        }
    }

    fun call6(view: View) {
        time("call6") {
            val b = TestBeanA()
            b.name = "hello world"
            b.names = arrayListOf("zhangsan", "lisi")
            b.ages = arrayListOf(1, 2, 3)
            Test.callMain5(
                mutableMapOf(
                    "String" to b,
                    "String2" to b,
                )
            )
        }
    }

    private val handler = Handler(Looper.getMainLooper())

    fun call7(view: View) {
        handler.post {
            time("onCreate2") {
                contentResolver.call(Uri.parse("content://${packageName}.aspectj.ipc.provider"), "c", null, null)
            }

        }
        handler.post {
            time("onCreate") {
                contentResolver.insert(Uri.parse("content://${packageName}.aspectj.ipc.provider"), ContentValues())
            }
        }
    }

    fun call8(view: View) {}


    fun time(name: String, run: () -> Unit) {
        val time = System.currentTimeMillis()
        run()
        le("$name time = ${System.currentTimeMillis() - time}")
    }
}