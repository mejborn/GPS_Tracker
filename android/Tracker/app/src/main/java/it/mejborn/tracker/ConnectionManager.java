package it.mejborn.tracker;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class ConnectionManager extends Thread {
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothDevice bluetoothDevice;
    private BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;
    private UUID bluetoothDeviceID = new UUID(123,123);

    public ConnectionManager(MapsActivity map){
        setupBluetoothSocket();
        setupInputOutputStream();
    }

    //Sets up input and output streams.
    private void setupInputOutputStream(){
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try{
            tmpIn = bluetoothSocket.getInputStream();
            tmpOut = bluetoothSocket.getOutputStream();
        }catch(IOException e) {}
        this.inputStream = tmpIn;
        this.outputStream = tmpOut;
    }

    //Initializes bluetooth adapter and create socket
    private void setupBluetoothSocket() {
        this.bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter.isEnabled() != true){
            bluetoothAdapter.enable();
        }
        BluetoothSocket tmpSocket = null;
        try {
            tmpSocket = bluetoothDevice.createRfcommSocketToServiceRecord(bluetoothDeviceID);
        } catch (IOException e) {}
        this.bluetoothSocket = tmpSocket;
    }

    //Read from the stream and send the data to the handler
    public void run(){
        byte[] buffer = new byte[1024];
        int bytes;
        while (true) {
            try{
                bytes = inputStream.read(buffer);
                //Implement handler ex.
                //mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                //.sendToTarget();
            }catch(IOException e) { break; }
        }
    }

    public void write(byte[] bytes){
        try{
            outputStream.write(bytes);
        }catch (IOException e) {}
    }

    public void connect() {
        bluetoothAdapter.cancelDiscovery();
        //Connect to the device via it's socket
        try {
            bluetoothSocket.connect();
        } catch (IOException connectException) {
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
            }
            return;
        }
        //Do work with the socket.
    }

    public void CloseSocket() {
        try{
            bluetoothSocket.close();
        }catch (IOException e) {}
    }
}
