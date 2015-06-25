
<?php

require("common.php");

if (isset($_GET["ID"]) && isset($_GET["lat"]) && isset($_GET["lng"]) && isset($_GET["msgType"])) {
    $ID = $_GET["ID"];
    $lat = $_GET["lat"];
    $lng = $_GET["lng"];
    $msgType = $_GET["msgType"];

    $query = "
            SELECT gcm_regid
            FROM gcm_users
            WHERE id=:id
            ";

    $query_params = array(
        ':id' => $ID,
    );

    try {
        $stmt = $db->prepare($query);
        $result = $stmt->execute($query_params);
    } catch (PDOException $ex) {
        die("Failed to run query: " . $ex->getMessage());
    }

    $row = $stmt->fetch();

    if($row) {
        $regId = $row['gcm_regid'];

        include_once './GCM.php';

        $gcm = new GCM();

        $registration_ids = array($regId);

        $gcm->send_coordinates($registration_ids, $lat, $lng, $msgType);
    }
} else {
    print($_GET["ID"]);
    print($_GET["lat"]);
    print($_GET["lng"]);
}