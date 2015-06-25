package it.mejborn.tracker;

import android.app.IntentService;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import com.google.android.gms.gcm.GoogleCloudMessaging;

public class GcmMessageHandler extends IntentService {
    public static final int MESSAGE_NOTIFICATION_ID = 435345;
    private MapsActivity mapsActivity;

    public GcmMessageHandler() {
        super("GcmMessageHandler");
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        // Retrieve data extras from push notification
        Bundle extras = intent.getExtras();
        GoogleCloudMessaging gcm = GoogleCloudMessaging.getInstance(this);
        // The getMessageType() intent parameter must be the intent you received
        // in your BroadcastReceiver.
        String messageType = gcm.getMessageType(intent);
        // Keys in the data are shown as extras
        //String title = extras.getString("title");
        //String body = extras.getString("body");
        // Create notification or otherwise manage incoming push
        //createNotification(title, body);
        // Log receiving message
        //Log.i("GCM", "Received : (" + messageType + ")  " + extras.getString("title"));

        String lat = extras.getString("lat");
        String lng = extras.getString("lng");
        Log.i("GCM", "Received : " + messageType);
        Log.i("GCM", "Received lat: " + lat);
        Log.i("GCM", "Received lng: " + lng);

        double latDbl = Double.parseDouble(lat);
        double lngDbl = Double.parseDouble(lng);


        //mapsActivity.setLocation(latDbl, lngDbl);

        // Notify receiver the intent is completed
        GcmBroadcastReceiverGammel.completeWakefulIntent(intent);
    }

    // Creates notification based on title and body received
    private void createNotification(String title, String body) {
        Context context = getBaseContext();
        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(context)
                .setSmallIcon(R.drawable.ic_launcher).setContentTitle(title)
                .setContentText(body);
        NotificationManager mNotificationManager = (NotificationManager) context
                .getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.notify(MESSAGE_NOTIFICATION_ID, mBuilder.build());
    }

    public void setMapsActivity(MapsActivity mapsActivity) {
        this.mapsActivity = mapsActivity;
    }

}
