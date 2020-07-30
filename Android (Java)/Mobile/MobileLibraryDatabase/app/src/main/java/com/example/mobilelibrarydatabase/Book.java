package com.example.mobilelibrarydatabase;

import androidx.annotation.NonNull;

public class Book {
    int id;
    String name;
    String author;
    boolean available = true;

    public Book(String name, String author) {
        this.name = name;
        this.author = author;
    }

    public Book(int id, String name, String author, boolean available) {
        this.id = id;
        this.name = name;
        this.author = author;
        this.available = available;
    }

    public int getId() {
        return id;
    }

    @NonNull
    @Override
    public String toString() {
        return "ID: " + id + ", Title: " + name + ", Author: " + author + ", isAvailable:" + available;
    }
}
