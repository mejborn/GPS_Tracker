package it.mejborn.tracker;

import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by BusterK on 04-04-2015.
 */
public class WorkerThread extends Thread{
    private boolean stopWorker;
    private final InputStream mmInStream;
    private final BluetoothSocket mmSocket;
    private ConnectionManager mConnectionManager;

    public WorkerThread(BluetoothSocket socket, ConnectionManager mConnectionManager){
        this.mConnectionManager = mConnectionManager;
        stopWorker = false;
        mmSocket = socket;
        InputStream tmpIn = null;
        // Get the input and output streams, using temp objects because
        // member streams are final
        try {
            tmpIn = socket.getInputStream();
        } catch (IOException e) { }

        mmInStream = tmpIn;

    }

    @Override
    public void run() {
        byte[] readBuffer = new byte[1024];
        final byte delimiter = 10;
        int readBufferPosition = 0;
        int counter = 0;
        double tmp = 0;

        while(!Thread.currentThread().isInterrupted() && !stopWorker) {
            try {
                int bytesAvailable = mmInStream.available();
                if(bytesAvailable > 0) {
                    byte[] packetBytes = new byte[bytesAvailable];
                    mmInStream.read(packetBytes);
                    for(int i=0;i<bytesAvailable;i++) {
                        //Check that the bytes recieved is within the range of accepted characters. Dont accept letters null etc.
                        byte b = packetBytes[i];
                        Log.v("Debug", "Recieved a byte from Bluetooth");
                        if( (b >= 48 && b <= 57 ) //Valid byte codes for numbers 0-9
                        || (b == 46 || b == 10)){ //Valid byte codes for . and NL
                            if(b == delimiter) {
                                byte[] encodedBytes = new byte[readBufferPosition];
                                System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                final String data = new String(encodedBytes, "UTF-8");
                                readBufferPosition = 0;

                                if(counter == 0){
                                    Log.v("Debug",data);
                                    tmp = Double.parseDouble(data);
                                    counter = 1;
                                }else{
                                    PostingThread mPostingThread = new PostingThread(tmp,Double.parseDouble(data),mConnectionManager);
                                    Log.v("Debug","Posting the bytes!");
                                    mPostingThread.start();
                                    counter = 0;
                                }

                            }
                            else {
                                readBuffer[readBufferPosition++] = b;
                            }
                        }
                    }
                }
            }
            catch (IOException ex) {
                stopWorker = true;
            }
        }
    }
}

class PostingThread extends Thread{
    double latitude, longitude;
    ConnectionManager mConnectionManager;
    public PostingThread(double latitude, double longitude, ConnectionManager mConnectionManager){
        this.latitude = latitude;
        this.longitude = longitude;
        this.mConnectionManager = mConnectionManager;
    }
    @Override
    public void run(){
        Handler handler = mConnectionManager.getHandler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                mConnectionManager.setMapPointer(latitude,longitude);
            }
        });
    }
}

