package com.example.mobilelibrarydatabase;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.List;

public class DatabaseHelper extends SQLiteOpenHelper {

    private static final String BOOK_TABLE = "BOOK_TABLE";
    private static final String COLUMN_ID = "ID";
    private static final String COLUMN_BOOK_NAME = "BOOK_NAME";
    private static final String COLUMN_BOOK_AUTHOR = "BOOK_AUTHOR";
    private static final String COLUMN_IS_AVAILABLE = "IS_AVAILABLE";

    DatabaseHelper(@Nullable Context context) {
        super(context, "library.db", null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createTableStatement = "CREATE TABLE " + BOOK_TABLE + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, " + COLUMN_BOOK_NAME + " TEXT, " + COLUMN_BOOK_AUTHOR + " TEXT, " + COLUMN_IS_AVAILABLE + " BOOL)";
        db.execSQL(createTableStatement);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }

    public boolean addBook(Book book){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues cv = new ContentValues();

        cv.put(COLUMN_BOOK_NAME, book.name);
        cv.put(COLUMN_BOOK_AUTHOR, book.author);
        cv.put(COLUMN_IS_AVAILABLE, book.available);

        long insert = db.insert(BOOK_TABLE, null, cv);

        if (insert == -1) return false;
        else return true;
    }

    public List<Book> getBooksList() {
        List<Book> returnList = new ArrayList<>();
        String queryString = "SELECT * FROM " + BOOK_TABLE;

        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(queryString, null);

        if (cursor.moveToFirst()) {
            do {
                int bookId = cursor.getInt(0);
                String bookName = cursor.getString(1);
                String bookAuthor = cursor.getString(2);
                boolean isAvailable = cursor.getInt(3) == 1 ? true: false;

                Book book = new Book(bookId, bookName, bookAuthor, isAvailable);
                returnList.add(book);
            } while (cursor.moveToNext());
        } else { }

        cursor.close();
        db.close();
        return returnList;
    }

    public boolean removeBook(String id) {
        SQLiteDatabase db = this.getWritableDatabase();
        String queryString = "DELETE FROM " + BOOK_TABLE + " WHERE " + COLUMN_ID + " = " + id;
        Cursor cursor = db.rawQuery(queryString, null);
        if (cursor.moveToFirst()) {
            cursor.close();
            db.close();
            return true;
        } else {
            cursor.close();
            db.close();
            return false;
        }
    }
}
