package com.example.mobilelibrarydatabase;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.List;

public class BooksDatabaseActivity extends AppCompatActivity {

    ListView listViewBooksDatabase;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_books_database);

        listViewBooksDatabase = findViewById(R.id.lv_books_database);

        DatabaseHelper databaseHelper = new DatabaseHelper(this);
        List<Book> booksList =  databaseHelper.getBooksList();
        ArrayAdapter booksArrayAdapter = new ArrayAdapter<>(BooksDatabaseActivity.this, android.R.layout.simple_list_item_1, booksList);
        listViewBooksDatabase.setAdapter(booksArrayAdapter);
    }
}
