package com.example.mybusfinal;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    CardView bus1,bus2,bus3,bus4,bus5;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        bus1 = findViewById(R.id.Bus1);
        bus2 = findViewById(R.id.Bus2);
        bus3 = findViewById(R.id.Bus3);
        bus4 = findViewById(R.id.Bus4);
        bus5 = findViewById(R.id.Bus5);

        bus1.setOnClickListener(this);
        bus2.setOnClickListener(this);
        bus3.setOnClickListener(this);
        bus4.setOnClickListener(this);
        bus5.setOnClickListener(this);


    }

    @Override
    public void onClick(View v) {

        switch (v.getId()){
            case R.id.Bus1:
                startActivity(new Intent(MainActivity.this, bus1_gmap.class));
                break;
            case R.id.Bus2:
                startActivity(new Intent(MainActivity.this, bus2_gmap.class));
                break;
            case R.id.Bus3:
                startActivity(new Intent(MainActivity.this, bus3_gmap.class));
                break;
            case R.id.Bus4:
                startActivity(new Intent(MainActivity.this, bus4_gmap.class));
                break;
            case R.id.Bus5:
                startActivity(new Intent(MainActivity.this, bus5_gmap.class));
                break;

        }
    }
}