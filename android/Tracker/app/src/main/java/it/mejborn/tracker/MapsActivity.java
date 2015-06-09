package it.mejborn.tracker;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.support.v4.content.WakefulBroadcastReceiver;
import android.util.Log;
import android.widget.Toast;

import it.mejborn.tracker.ServerUtilities;
import android.content.Context;

import com.google.android.gms.gcm.GoogleCloudMessaging;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.CircleOptions;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import org.apache.http.impl.client.DefaultHttpClient;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import static it.mejborn.tracker.ConnectionManager.*;


public class MapsActivity extends FragmentActivity {

    private Marker destinationMarker;
    private static GoogleMap mMap; // Might be null if Google Play services APK is not available.
    private ConnectionManager connectionManager = null;
    private GCMClientManager pushClientManager;
    String PROJECT_NUMBER = "1059651119431";
    String registrationID = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);

        //Set orientation to Portrait
        setRequestedOrientation(ActivityInfo
                .SCREEN_ORIENTATION_PORTRAIT);

        setUpMapIfNeeded();
        setUpConnectionManagerIfNeeded();
        setUPGCMClientManagerIfNeeded();
        sendGCMIDToServer();
    }

    @Override
    protected void onResume() {
        super.onResume();
        setUpMapIfNeeded();
    }

    public static class GcmBroadcastReceiver extends WakefulBroadcastReceiver {

        // Receives the broadcast MapsActivitydirectly from GCM service
        @Override
        public void onReceive(Context context, Intent intent) {

            String lat = intent.getExtras().getString("lat");
            String lng = intent.getExtras().getString("lng");
            //String body = intent.getExtras().getString("body");
            Log.i("GCM", "Received lat: " + lat);
            Log.i("GCM", "Received lng: " + lng);
            //Log.i("GCM", "Received Message: " + body);

            double latDbl = Double.parseDouble(lat);
            double lngDbl = Double.parseDouble(lng);

            setLocation(latDbl, lngDbl);



            // Explicitly specify that GcmMessageHandler will handle the intent.
            //ComponentName comp = new ComponentName(context.getPackageName(),
            //        GcmMessageHandler.class.getName());

            // Start the service, keeping the device awake while it is executing.
            //startWakefulService(context, (intent.setComponent(comp)));
            // Return successful
            setResultCode(Activity.RESULT_OK);
        }
    }

    private void setUpMapIfNeeded() {
        // Do a null check to confirm that we have not already instantiated the map.
        if (mMap == null) {
            // Try to obtain the map from the SupportMapFragment.
            mMap = ((SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map))
                    .getMap();
            // Check if we were successful in obtaining the map.
            if (mMap != null) {
                setUpMap();
            }
        }
    }

    private void setUpConnectionManagerIfNeeded(){
        if(connectionManager == null){
            connectionManager = new ConnectionManager(this);
        }
    }

    private void setUpMap() {
        mMap.setMyLocationEnabled(true);
    }

    private static void changeMap(LatLng pos){
        //@TODO:Remove last marker!
        mMap.addMarker(new MarkerOptions().position(pos));
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(pos,15));
    }

    public static void setLocation(double latitude, double longitude){
        LatLng pos = new LatLng(latitude,longitude);
        changeMap(pos);
    }

    private void setUPGCMClientManagerIfNeeded() {
        // GCM
        if(pushClientManager == null) {
            pushClientManager = new GCMClientManager(this, PROJECT_NUMBER);
            pushClientManager.registerIfNeeded(new GCMClientManager.RegistrationCompletedHandler() {
                @Override
                public void onSuccess(String registrationId, boolean isNewRegistration) {
                    Toast.makeText(MapsActivity.this, registrationId, Toast.LENGTH_SHORT).show();
                    Log.v("Reg ID:", registrationId);
                    registrationID = registrationId;
                }

                @Override
                public void onFailure(String ex) {
                    super.onFailure(ex);
                }
            });
        }
    }

    private void sendGCMIDToServer() {
        final String name = "Arduino Kursus";
        final String email = "ArduinoTest@Arduino.dk";
        new AsyncTask<String, Void, String>() {
            @Override
            protected String doInBackground(String... params) {
                ServerUtilities.register(name, email, registrationID);
                return null;
            }

        }.execute(null, null, null);
    }
}