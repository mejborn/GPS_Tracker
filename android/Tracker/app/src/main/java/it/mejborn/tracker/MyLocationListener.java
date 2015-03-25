package it.mejborn.tracker;

import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.util.Log;

/**
 * Created by BusterK on 19-03-2015.
 */
public class MyLocationListener implements LocationListener {
    private MapsActivity map;

    public MyLocationListener(MapsActivity map) {
        this.map = map;
    }

    @Override
    public void onLocationChanged(Location loc) {
        map.setLocation(loc.getLatitude(),loc.getLongitude());
        loc.getLatitude();
        loc.getLongitude();
        String longitude = "Longitude: " +loc.getLongitude();
        Log.v("Debug", longitude);
        String latitude = "Latitude: " +loc.getLatitude();
        Log.v("Debug", latitude);
    }

    @Override
    public void onProviderDisabled(String provider) {
        // TODO Auto-generated method stub
        Log.v("Debug","GPS Disabled!");
    }

    @Override
    public void onProviderEnabled(String provider) {
        // TODO Auto-generated method stub
        Log.v("Debug","GPS Enabled!");
    }

    @Override
    public void onStatusChanged(String provider,
                                int status, Bundle extras) {
        // TODO Auto-generated method stub
    }
}