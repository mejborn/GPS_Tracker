<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Arduino Projekt</title>
    <style type="text/css">
        html, body { height: 100%; margin: 0; padding: 0;}
        #map-canvas { height: 50%; width: 50%; top: 0%;
            left: 25%;}

        #buttons {
            text-align: center;
        }
    </style>
</head>
<body>
<h1 style="text-align: center">Arduino Projekt</h1>
<h4 style="text-align: center">Husk at indtaste ID, ellers går tegningen tabt!</h4>
<div id="map-canvas"></div>
<div id="buttons">
    <button id="deleteShape">Slet område</button>
    <button id="downloadPoints">Hent område</button>
    <form action="saveareas.php" method="post">
        Indtast ID: <input type="text" name="ID">
        <input type="submit" value="Gem område"/>
        <input type="hidden" id="DBcoords" name="koordinater"/>
    </form>
    <form>
        <input type="button" value="Login" onclick="window.location.href='login.php'">
    </form>
    <script src="https://maps.googleapis.com/maps/api/js?AIzaSyCiqfn23Gqe_IjUaPwXJETVOUZlblXnaLk&v=3.exp&libraries=drawing"></script>
    <script src="map.js"></script>
</div>
<h3 style="text-align: center">Koordinater:</h3>
<p id="coords", style="text-align: center">Intet område endnu.</p>
</body>
</html>