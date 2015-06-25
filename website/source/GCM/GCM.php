<?php

class GCM {

    //put your code here
    // constructor
    function __construct() {

    }

    /*
     * sending lat long coordinates
     */
    public function send_coordinates($regId, $lat, $lng, $msgType) {

        // Set POST variables
        $url = 'https://android.googleapis.com/gcm/send';

        $data = array('lat' => $lat, 'lng' => $lng, 'msgType' => $msgType);

        $fields = array(
            'registration_ids' => $regId,
            'data' => $data,
        );

        $headers = array(
            'Authorization: key=' . AIzaSyAVUaNQNau4e1_IO4bFRWklGa0BwgflV28,
            'Content-Type: application/json'
        );
        // Open connection
        $ch = curl_init();

        // Set the url, number of POST vars, POST data
        curl_setopt($ch, CURLOPT_URL, $url);

        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

        // Disabling SSL Certificate support temporarly
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);

        curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));

        // Execute post
        $result = curl_exec($ch);
        if ($result === FALSE) {
            die('Curl failed: ' . curl_error($ch));
        }

        // Close connection
        curl_close($ch);
        echo $result;
    }

}

?>