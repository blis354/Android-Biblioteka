package com.example.mobilelibrarydatabase;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.BounceInterpolator;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class SplashScreenActivity extends AppCompatActivity {

    Button enterButton;
    TextView mobileLibraryTextView;
    TextView databaseTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash_screen);

        enterButton = findViewById(R.id.enter_button);
        mobileLibraryTextView = findViewById(R.id.mobile_library_text_view);
        databaseTextView = findViewById(R.id.database_text_view);

        mobileLibraryTextSlideIn();

        enterButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(SplashScreenActivity.this, MainActivity.class);
                startActivity(intent);
            }
        });
    }

    private void mobileLibraryTextSlideIn() {
        mobileLibraryTextView.setVisibility(View.VISIBLE);
        Animation animation = AnimationUtils.loadAnimation(this, R.anim.slide_in_left);
        animation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                databaseTextViewSlideIn();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
        mobileLibraryTextView.startAnimation(animation);
    }

    private void databaseTextViewSlideIn() {
        databaseTextView.setVisibility(View.VISIBLE);
        Animation animation = AnimationUtils.loadAnimation(this, R.anim.slide_in_left);
        animation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                buttonBounceAnimation();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
        databaseTextView.startAnimation(animation);
    }

    private void buttonBounceAnimation() {
        enterButton.setVisibility(View.VISIBLE);
        Animation bounceAnimation = AnimationUtils.loadAnimation(this, R.anim.bounce);
        bounceAnimation.setDuration(3000);
        bounceAnimation.setInterpolator(new BounceInterpolator());
        enterButton.startAnimation(bounceAnimation);
    }
}
