package it.mejborn.tracker;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.support.v4.content.WakefulBroadcastReceiver;

import java.io.Serializable;

public class GcmBroadcastReceiverGammel extends WakefulBroadcastReceiver {

    private MapsActivity mapsActivity;

    // Constructor
    /*public GcmBroadcastReceiver(MapsActivity mapsActivity) {
        this.mapsActivity = mapsActivity;
    }*/

    // Receives the broadcast MapsActivitydirectly from GCM service
    @Override
    public void onReceive(Context context, Intent intent) {

        // Explicitly specify that GcmMessageHandler will handle the intent.
        ComponentName comp = new ComponentName(context.getPackageName(),
                GcmMessageHandler.class.getName());

        // Start the service, keeping the device awake while it is executing.
        //startWakefulService(context, (intent.setComponent(comp)));
        // Return successful
        setResultCode(Activity.RESULT_OK);
    }

    public void setPointer(double latitude, double longitude) {
        mapsActivity.setLocation(latitude, longitude);
    }

    public void setMapsActivity(MapsActivity mapsActivity) {
        this.mapsActivity = mapsActivity;
    }
}