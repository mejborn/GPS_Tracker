<?php

/*
    CREATE TABLE koordinater(
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `koordinater` varchar(65535) NOT NULL,
    PRIMARY KEY (`id`)
    )ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1;
*/
require("common.php");

if(!empty($_POST)) {
    if(!empty($_POST['koordinater'])) {
        if(empty($_POST['ID'])) {
            header('Location: index.php');
        } else {
            $query = "
                    UPDATE IDs
                    SET koordinater=:koordinater
                    WHERE ID=:id
                ";

            $query_params = array(
                ':koordinater' => $_POST['koordinater'],
                ':id' => $_POST['ID']
            );

            try {
                $stmt = $db->prepare($query);
                $result = $stmt->execute($query_params);
            } catch (PDOException $ex) {
                die("Failed to run query: " . $ex->getMessage());
            }
        }
    } else {
        header('Location: index.php');
    }
}


?>
<html>
<head>
    <title>Arduino Projekt</title>
</head>
<body>
<h2>Saved!</h2>
<a href="index.php">Return</a>
</body>
</html>