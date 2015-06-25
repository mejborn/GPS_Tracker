<?php
    require("common.php");

    if(empty($_SESSION['user'])) 
    {
        header("Location: login.php");
        die("Redirecting to login.php"); 
    } 

    if(!empty($_POST)) {

        if(isset($_POST['addID'])) {
            if(!empty($_POST['ID'])) {
                $query = "
                    SELECT
                        1
                    FROM IDs
                    WHERE
                        ID = :id
                ";
                $query_params = array(
                    ':id' => $_POST['ID']
                );

                try
                {
                    $stmt = $db->prepare($query);
                    $result = $stmt->execute($query_params);
                }
                catch(PDOException $ex)
                {
                    die("Failed to run query: " . $ex->getMessage());
                }
                $row = $stmt->fetch();

                if($row) {
                    print("ID'et er allerede i brug!");
                } else {
                    // ID bliver insat i tabel

                    $query = "
                                INSERT INTO IDs (id, koordinater)
                                VALUES (:id, '')
                            ";

                    $query_params = array(
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
                print("Mangler ID!");
            }
        } else if(isset($_POST['delID'])) {
            if(!empty($_POST['ID'])) {
                $query = "
                    SELECT
                        1
                    FROM IDs
                    WHERE
                        ID = :id
                ";
                $query_params = array(
                    ':id' => $_POST['ID']
                );

                try
                {
                    $stmt = $db->prepare($query);
                    $result = $stmt->execute($query_params);
                }
                catch(PDOException $ex)
                {
                    die("Failed to run query: " . $ex->getMessage());
                }
                $row = $stmt->fetch();

                if(!$row) {
                    print("ID'et eksisterer ikke!");
                } else {
                    // ID bliver slettet i tabel

                    $query = "
                                DELETE FROM IDs
                                WHERE id = :id
                            ";

                    $query_params = array(
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
                print("Mangler ID!");
            }
        } else {
            if(!empty($_POST['ID'])) {
                $query = "
                    SELECT
                        1
                    FROM IDs
                    WHERE
                        ID = :id
                ";
                $query_params = array(
                    ':id' => $_POST['ID']
                );

                try
                {
                    $stmt = $db->prepare($query);
                    $result = $stmt->execute($query_params);
                }
                catch(PDOException $ex)
                {
                    die("Failed to run query: " . $ex->getMessage());
                }

                $row = $stmt->fetch();

                if($row) {
                    header("Location: getAdmin.php?ID=".$_POST['ID']);
                } else {
                    print("ID'et findes ikke!");
                }
            } else {
                print("Mangler ID!");
            }
        }
    }

?>
<html>
<head>
    <title>Arduino Projekt</title>
</head>
<body>
<h1>Arduino Projekt "Admin"</h1>
<form action="private.php" method="post">
    <input type="text" name="ID">
    <input type="submit" name="addID" value="Tilføj ID">
    <input type="submit" name="delID" value="Slet ID">
    <input type="submit" name="seeCoords" value="Få koordinater">
</form>
<a href="logout.php">Log ud</a>
</body>
</html>