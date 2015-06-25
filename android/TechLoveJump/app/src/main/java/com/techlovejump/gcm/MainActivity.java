package com.techlovejump.gcm;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import com.google.android.gms.gcm.GoogleCloudMessaging;

import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;

import android.util.Log;
import android.view.Menu;
import android.widget.TextView;

public class MainActivity extends Activity {

    public static final String EXTRA_MESSAGE = "message";
    public static final String PROPERTY_REG_ID = "registration_id";

    String SENDER_ID = "538459826415";

    static final String TAG = "GCMDemo";
    GoogleCloudMessaging gcm;

    TextView mDisplay;
    Context context;
    String regid;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mDisplay = (TextView) findViewById(R.id.display);
        context = getApplicationContext();

        gcm = GoogleCloudMessaging.getInstance(this);

        new RegisterBackground().execute();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    @Override
    protected void onResume(){
        super.onResume();

    }
    class RegisterBackground extends AsyncTask&lt;String,String,String&gt;{

        @Override
        protected String doInBackground(String... arg0) {
            // TODO Auto-generated method stub
            String msg = "";
            try {
                if (gcm == null) {
                    gcm = GoogleCloudMessaging.getInstance(context);
                }
                regid = gcm.register(SENDER_ID);
                msg = "Dvice registered, registration ID=" + regid;
                Log.d("111", msg);
                sendRegistrationIdToBackend(regid);

            } catch (IOException ex) {
                msg = "Error :" + ex.getMessage();
            }
            return msg;
        }

        @Override
        protected void onPostExecute(String msg) {
            mDisplay.append(msg + "\n");

        }
    private void sendRegistrationIdToBackend(String regid) {
        // this code will send registration id of a device to our own server.
    }}
}