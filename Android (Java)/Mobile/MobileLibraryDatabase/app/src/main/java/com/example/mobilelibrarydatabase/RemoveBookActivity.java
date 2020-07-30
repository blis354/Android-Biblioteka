package com.example.mobilelibrarydatabase;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class RemoveBookActivity extends AppCompatActivity {

    Button buttonRemoveBook;
    EditText bookIdInput;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remove_book);

        buttonRemoveBook = findViewById(R.id.button_remove_book);
        bookIdInput = findViewById(R.id.book_id_input);

        buttonRemoveBook.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String idInputText = bookIdInput.getText().toString();
                if (idInputText.isEmpty()) {
                    Toast.makeText(RemoveBookActivity.this, "Podaj Id", Toast.LENGTH_SHORT).show();
                } else {
                    DatabaseHelper databaseHelper = new DatabaseHelper(RemoveBookActivity.this);
                    boolean help = databaseHelper.removeBook(idInputText);
                    if (!help) {
                        Toast.makeText(RemoveBookActivity.this, "Książka pomyślnie usunięta", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(RemoveBookActivity.this, "Książka nie usunieta, nie znaleziona", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
    }
}

