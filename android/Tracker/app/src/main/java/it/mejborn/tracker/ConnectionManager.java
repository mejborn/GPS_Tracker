package it.mejborn.tracker;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.util.Set;


public class ConnectionManager extends Thread {
    private MapsActivity map;
    BluetoothAdapter mBluetoothAdapter;
    private BluetoothSocket bluetoothSocket;
    private WorkerThread mWorkerThread;
    static Handler mHandler;

    public ConnectionManager(MapsActivity map){
        //Set up a new handler
        mHandler = new Handler(Looper.getMainLooper()){
            /*
             * handleMessage() defines the operations to perform when
             * the Handler receives a new Message to process.
             */
            @Override
            public void handleMessage(Message inputMessage) {
                // Gets the image task from the incoming Message object.
                setMapPointer(inputMessage.arg1,inputMessage.arg2);

            }
        };

        /*mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        ConnectThread mConnectThread;
        this.map = map;
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                // Add the name and address to an array adapter to show in a ListView
                if (device.getName().contains("Adafruit")){
                    Log.v("Debug", "Initiating bluetooth Connection");
                    mConnectThread = new ConnectThread(device,this);
                    mConnectThread.start();
                }
            }
        }*/
    }

    /*public void setBluetoothSocket(BluetoothSocket socket){
        this.bluetoothSocket = socket;
        this.mWorkerThread = new WorkerThread(socket,this);
        mWorkerThread.start();
    }*/

    public void setMapPointer(double latitude, double longitude){
        map.setLocation(latitude,longitude);
    }

    public MapsActivity getMapsActivity(){
        return this.map;
    }

    public Handler getHandler(){
        return this.mHandler;
    }
}
