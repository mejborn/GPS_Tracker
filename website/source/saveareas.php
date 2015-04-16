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

        $query = "
                INSERT INTO koordinater (
                    koordinater
                ) VALUES (
                    :koordinater
                )
            ";

        $query_params = array(
            ':koordinater' => $_POST['koordinater']
        );

        try {
            $stmt = $db->prepare($query);
            $result = $stmt->execute($query_params);
        } catch (PDOException $ex) {
            die("Failed to run query: " . $ex->getMessage());
        }
    }
}


?>
<html>
<body>
<h2>Saved!</h2>
<a href="index.php">Return</a>
</body>
</html>