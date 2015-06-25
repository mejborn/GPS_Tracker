<?php

require("common.php");

ini_set('display_errors', 'On');
error_reporting(E_ALL);
// response json

/**
 * Registering a user device
 * Store reg id in users table
 */
if (isset($_POST["ID"]) && isset($_POST["regID"])) {
    $ID = $_POST["ID"];
    $gcm_regid = $_POST["regID"]; // GCM Registration ID
    // Store user details in db

    $query = "
            UPDATE gcm_users
            SET gcm_regid=:gcm_regid
            WHERE id=:id
            ";

    $query_params = array(
        ':id' => $ID,
        ':gcm_regid' => $gcm_regid
    );


    try {
        $stmt = $db->prepare($query);
        $result = $stmt->execute($query_params);
    } catch (PDOException $ex) {
        die("Failed to run query: " . $ex->getMessage());
    }

} else {
    print($_POST["ID"]);
    print($_POST["regID"]);

    // user details missing
}
?>