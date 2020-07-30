package com.example.mobilelibrarydatabase;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class AddBookActivity extends AppCompatActivity {

    Button addBookButton;
    EditText authorNameEditText;
    EditText bookNameEditText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_book);

        addBookButton = findViewById(R.id.button_add_book);
        bookNameEditText = findViewById(R.id.book_name_edit_text);
        authorNameEditText = findViewById(R.id.author_name_edit_text);

        addBookButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String bookName = bookNameEditText.getText().toString();
                String authorName = authorNameEditText.getText().toString();

                if (bookName.isEmpty() && authorName.isEmpty()){
                    Toast.makeText(AddBookActivity.this, "Prosze wypelnic wszystkie pola!", Toast.LENGTH_SHORT).show();
                } else {
                    addBookToDb(bookName, authorName);
                }
            }
        });
    }

    private void addBookToDb(String bookName, String authorName){
        Book book = new Book(bookName, authorName);
        DatabaseHelper databaseHelper = new DatabaseHelper(this);
        boolean insertResponse = databaseHelper.addBook(book);
        if (insertResponse == true) {
            Toast.makeText(AddBookActivity.this, "Ksiazka " + bookName + " autora " + authorName + " dodana do bazy danych.", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(AddBookActivity.this, "Błąd!", Toast.LENGTH_SHORT).show();
        }
    }
}
