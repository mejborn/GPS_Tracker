function initialize() {
    getLocation();
}

/*
    Booleans
 */
var gotLocation = false;
var firstDrawing = true;

/*
    Variables
 */
var masterPolygon;
var selectedShape;
var coordinatesForDownload;

/*
    Download af koordinater
 */
var downloadFile = document.getElementById("downloadPoints");
downloadFile.addEventListener('click', downloadTxtFile);

function downloadTxtFile() {
    if(masterPolygon == null) {
        window.alert("Intet at gemme!");
    } else {
        function downloadInnerHtml(filename, mimeType) {
            var link = document.createElement('a');
            mimeType = mimeType || 'text/plain';
            link.setAttribute('download', filename);
            link.setAttribute('href', 'data:' + mimeType + ';charset=utf-8,' + encodeURIComponent(coordinatesForDownload));
            link.click();
        }

        var fileName = 'koordinater.txt';
        downloadInnerHtml(fileName, 'text/plain');
        return false;
    }
}


function printCoordinates() {
    if(masterPolygon == null) {
        document.getElementById('coords').innerHTML = "Ingen områder endnu.";
    } else {
        /*
            .k er Latitude og .D er Longitude.
            Longitude = <--->

                         ^
            Latitude =   |
                         v
         */
        var coordinatesForHtml = "";
        coordinatesForDownload = "$";
        for (var i = 0; i < masterPolygon.getPath().length; i++) {
            if(i+1 != masterPolygon.getPath().length) {
                coordinatesForDownload += masterPolygon.getPath().getAt(i).k + "," + masterPolygon.getPath().j[i].D + ",";
                coordinatesForHtml += masterPolygon.getPath().getAt(i) + "<br>";
            } else {
                coordinatesForDownload += masterPolygon.getPath().getAt(i).k + "," + masterPolygon.getPath().j[i].D + "$";
                coordinatesForHtml += masterPolygon.getPath().getAt(i);
            }
        }
        document.getElementById('coords').innerHTML = coordinatesForHtml;
        document.getElementById('DBcoords').value = coordinatesForDownload;
    }
}

function clearSelection() {
    if(selectedShape) {
        selectedShape.setEditable(false);
        selectedShape = null;
    }
}

function setSelection(shape) {
    clearSelection();
    selectedShape = shape;
    shape.setEditable(true);
}

function deleteSelectShape() {
    masterPolygon.setMap(null);
    masterPolygon = null;
    printCoordinates();
    firstDrawing = true;
}

function addListeners() {
    google.maps.event.addListener(masterPolygon, 'click', function (e) {
        setSelection(masterPolygon);
        printCoordinates();
    })

    google.maps.event.addListener(masterPolygon, 'drag', function(e) {
        printCoordinates();
    })

    google.maps.event.addListener(masterPolygon.getPath(), 'set_at', function(e) {
        printCoordinates();
    })

    google.maps.event.addListener(masterPolygon.getPath(), 'insert_at', function(e) {
        printCoordinates();
    })
}

function showMap(position){

    var mapOptions;

    if(gotLocation) {
        mapOptions = {
            zoom: 15,
            center: new google.maps.LatLng(position.coords.latitude, position.coords.longitude)
        };
    } else {
        mapOptions = {
            zoom: 6,
            center: new google.maps.LatLng(55.695003, 10.629682)
        };
    }

    var map = new google.maps.Map(document.getElementById('map-canvas'),
        mapOptions);

    var drawingManager = new google.maps.drawing.DrawingManager({
        drawingMode: google.maps.drawing.OverlayType.POLYGON,
        drawingControl: true,
        drawingControlOptions: {
            position: google.maps.ControlPosition.TOP_CENTER,
            drawingModes: [
                google.maps.drawing.OverlayType.POLYGON
            ]
        },
        markerOptions: {
            icon: 'images/beachflag.png'
        },
        polygonOptions: {
            clickable: true,
            editable: true,
            draggable: true,
            strokeWeight: 0,
            fillOpacity: 0.45
        }
    });

    drawingManager.setMap(map);

    google.maps.event.addListener(drawingManager, "overlaycomplete", function(event) {
        if(firstDrawing) {

            /*
                Sætte masterPolygon til det overordnede polygon.
             */
            masterPolygon = event.overlay;

            /*
            tilføj listeners til overordnet polygon.
             */
            addListeners();

            /*
                Print koordinater til browser vinduet.
             */
            printCoordinates();

            /*
                Reset drawing manager, så der skal klikkes igen for at tegne ny tegning.
             */
            drawingManager.setDrawingMode(null);

            setSelection(masterPolygon);

            firstDrawing = false;

        } else {

            /*
            Se om det tegnet polygon enten dækker hele det gamle polygon eller
            om det gamle polygon dækker hele det nye polygon. Samtidig checkes også
            om polygonerne overlapper hinanden. Hvis ikke, slettes det ny tegnet og det gamle
            gemmes som sædvanlig.
             */
            var pointsToCheckSecond = 0;
            var pointsOutsideToCheck = 0;

            while(pointsToCheckSecond != masterPolygon.getPath().j.length) {
                if(google.maps.geometry.poly.containsLocation(masterPolygon.getPath().getAt(pointsToCheckSecond), event.overlay)) {
                    pointsToCheckSecond++;
                } else {
                    break;
                }
            }
            while(pointsOutsideToCheck != event.overlay.getPath().j.length) {
                if(!(google.maps.geometry.poly.containsLocation(event.overlay.getPath().getAt(pointsOutsideToCheck), masterPolygon))) {
                    pointsOutsideToCheck++;
                } else {
                    break;
                }
            }

            if (pointsToCheckSecond == masterPolygon.getPath().j.length) {
                /*
                 Hvis det nye polygon dækker det gamle, så fjernes alle de gamle punkter, og oldVectors slettes,
                 da disse vektorer ikke skal bruges mere.
                 */
                masterPolygon.setMap(null);
                masterPolygon = event.overlay;

                printCoordinates();

                setSelection(masterPolygon);

                drawingManager.setDrawingMode(null);

                addListeners();

            } else if (pointsOutsideToCheck == event.overlay.getPath().j.length) {
                event.overlay.setMap(null);
                window.alert("Husk at tegne så boksene dækker hinanden!");
            } else {
                event.overlay.setMap(null);
            }
        }
    });

    google.maps.event.addDomListener(document.getElementById('deleteShape'), 'click', deleteSelectShape);
}

function getLocation() {
    if (navigator.geolocation) {
        gotLocation = true;
        navigator.geolocation.getCurrentPosition(showMap, showMap);
    } else {
        x.innerHTML = "Geolocation is not supported by this browser.";
    }
}

google.maps.event.addDomListener(window, 'load', initialize);

