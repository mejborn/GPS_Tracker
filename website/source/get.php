<?php

require("common.php");

if(!empty($_GET['ID'])) {
    $query = "
            SELECT koordinater
            FROM IDs
            WHERE ID=:id
        ";

    $query_params = array(
        ':id' => $_GET['ID']
    );

    try {
        $stmt = $db->prepare($query);
        $result = $stmt->execute($query_params);
    } catch (PDOException $ex) {
        die("Failed to run query: " . $ex->getMessage());
    }

    $row = $stmt->fetch();

    echo $row['koordinater'];
}