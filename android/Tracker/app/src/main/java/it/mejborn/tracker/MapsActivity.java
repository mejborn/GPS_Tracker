package it.mejborn.tracker;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.location.LocationListener;
import android.location.LocationManager;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.CircleOptions;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;


public class MapsActivity extends FragmentActivity {

    private Marker destinationMarker;
    private GoogleMap mMap; // Might be null if Google Play services APK is not available.
    private ConnectionManager connectionManager = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        //Set orientation to Portrait
        setRequestedOrientation(ActivityInfo
                .SCREEN_ORIENTATION_PORTRAIT);

        setUpMapIfNeeded();
        setUpConnectionManagerIfNeeded();

    }

    @Override
    protected void onResume() {
        super.onResume();
        setUpMapIfNeeded();
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

    private void changeMap(LatLng pos){
        //@TODO:Remove last marker!
        mMap.addMarker(new MarkerOptions().position(pos));
    }

    public void setLocation(double latitude, double longitude){
        LatLng pos = new LatLng(latitude,longitude);
        changeMap(pos);
    }

}