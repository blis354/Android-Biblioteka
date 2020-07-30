package com.example.mobilelibrarydatabase;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    Button buttonGoToDatabaseActivity;
    Button buttonGoToAddBookActivity;
    Button buttonGoToRemoveBookActivity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonGoToDatabaseActivity = findViewById(R.id.button_show_database);
        buttonGoToAddBookActivity = findViewById(R.id.button_add_book);
        buttonGoToRemoveBookActivity = findViewById(R.id.button_remove_book);

        DatabaseHelper databaseHelper = new DatabaseHelper(this);

        buttonGoToDatabaseActivity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, BooksDatabaseActivity.class);
                startActivity(intent);
            }
        });

        buttonGoToAddBookActivity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, AddBookActivity.class);
                startActivity(intent);
            }
        });

        buttonGoToRemoveBookActivity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RemoveBookActivity.class);
                startActivity(intent);
            }
        });
    }
}
