package it.mejborn.tracker;

import android.app.Activity;
import android.app.NotificationManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.support.v4.content.WakefulBroadcastReceiver;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;


public class MapsActivity extends FragmentActivity {

    private static GoogleMap mMap; // Might be null if Google Play services APK is not available.
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

        setUpMap();
        setUPGCMClientManagerIfNeeded();
        sendGCMIDToServer();
    }

    @Override
    protected void onResume() {
        super.onResume();
        setUpMap();
    }

    public static class GcmBroadcastReceiver extends WakefulBroadcastReceiver {

        // Receives the broadcast MapsActivitydirectly from GCM service
        @Override
        public void onReceive(Context context, Intent intent) {

            int msgType = Integer.parseInt(intent.getExtras().getString("msgType"));

            if(msgType == 1) {
                String lat = intent.getExtras().getString("lat");

                String lng = intent.getExtras().getString("lng");


                Log.i("GCM", "Received lat: " + lat);
                Log.i("GCM", "Received lng: " + lng);

                double latDbl = Double.parseDouble(lat);
                double lngDbl = Double.parseDouble(lng);

                setLocation(latDbl, lngDbl);
            } else if(msgType == 2) {
                ComponentName comp = new ComponentName(context.getPackageName(),
                        GcmMessageHandler.class.getName());
                startWakefulService(context, (intent.setComponent(comp)));
            }
            setResultCode(Activity.RESULT_OK);
        }

    }

    private void setUpMap() {
        mMap = ((SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map))
                .getMap();
        mMap.setMyLocationEnabled(true);
    }

    private static void changeMap(LatLng pos){
        mMap.addMarker(new MarkerOptions().position(pos));
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(pos, 15));
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
        final String ID ="1";
        new AsyncTask<String, Void, String>() {
            @Override
            protected String doInBackground(String... params) {
                ServerUtilities.register(ID, registrationID);
                return null;
            }

        }.execute(null, null, null);
    }
}