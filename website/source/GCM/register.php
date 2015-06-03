<?php

require("common.php");

ini_set('display_errors', 'On');
error_reporting(E_ALL);
// response json

/**
 * Registering a user device
 * Store reg id in users table
 */
if (isset($_POST["name"]) && isset($_POST["email"]) && isset($_POST["regId"])) {
    $name = $_POST["name"];
    $email = $_POST["email"];
    $gcm_regid = $_POST["regId"]; // GCM Registration ID
    // Store user details in db

    $query = "
            INSERT
            INTO gcm_users(name, email, gcm_regid, created_at)
            VALUES(:name, :email, :gcm_regid, NOW())
            ";

    $query_params = array(
        ':name' => $name,
        ':email' => $email,
        ':gcm_regid' => $gcm_regid
    );


    try {
        $stmt = $db->prepare($query);
        $result = $stmt->execute($query_params);
    } catch (PDOException $ex) {
        die("Failed to run query: " . $ex->getMessage());
    }

} else {
    print($_GET["name"]);
    print($_GET["email"]);
    print($_GET["regId"]);

    // user details missing
}
?>