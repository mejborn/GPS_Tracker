
<?php
/*if (isset($_GET["lat"]) && isset($_GET["lng"])) {
    $lat = $_GET["lat"];
    $lng = $_GET["lng"];

    include_once './GCM.php';

    $gcm = new GCM();
    $regID = 'APA91bF_9N-yr89HvwYCclHoEtjJZHv-Qm0O1eDOMe_HDr2dMxqwCuxIzC5z45SBznE9t8b3r4qp5uHE6JCnnNi6jIMXKJyuQoH-uh5ZjRxsivVDieQ0UCEezgjC07W-CAx1py441XVG-ojGcN1EUdoroD76Cr6UIg';
    //$message = array("price" => $message);

    $gcm->send_coordinates($regID, $lat, $lng);
}*/


if (isset($_GET["regId"]) && isset($_GET["lat"]) && isset($_GET["lng"])) {
    $regId = $_GET["regId"];
    $lat = $_GET["lat"];
    $lng = $_GET["lng"];

    include_once './GCM.php';

    $gcm = new GCM();

    $registration_ids = array($regId);
    //$message = array("price" => $message);

    $gcm->send_coordinates($registration_ids, $lat, $lng);
}
?>