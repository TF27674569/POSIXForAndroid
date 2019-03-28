package com.ubt.posixforandroid;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    private PosixThread p;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        p = new PosixThread();

        p.init();
    }


    public void click(View view) {
        p.pthread();
    }

    @Override
    protected void onDestroy() {
        p.destroy();
        super.onDestroy();
    }

}
