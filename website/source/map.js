/**
 * Created by BusterK on 19-01-2015.
 */

function initialize() {
    getLocation();
}


var gotLocation = false;
var allAreas = [];
var newVectors = [];
var intersectionsArray = [];
var selectedShape;
var TILE_SIZE = 256;
var projection;

function printCoordinates(overlay) {
    if(overlay == null && (allAreas.length == 0)) {
        document.getElementById('coords').innerHTML = "Ingen områder endnu.";
    } else if(overlay === null) {
        document.getElementById('coords').innerHTML = "Intet område er valgt";
    } else {
        var coordinates = "";
        for (var i = 0; i < overlay.getPath().length; i++) {
            coordinates += "( Lat , Lng ) ( y , x ) = " + overlay.getPath().getAt(i) + "<br>";
        }
        document.getElementById('coords').innerHTML = coordinates;
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
    if(selectedShape) {
        if (allAreas.length <= 1) {
            selectedShape.setMap(null);
            allAreas = []
            printCoordinates(null);
        } else {
            selectedShape.setMap(null);
            printCoordinates(null);
        }
    }
}

function deleteAllShapes() {
    for(var i = 0; i < allAreas.length; i++) {
        allAreas[i].overlay.setMap(null);
    }
    allAreas = [];
    printCoordinates(null);
}

function bound(value, opt_min, opt_max) {
    if (opt_min != null) value = Math.max(value,opt_min);
    if (opt_max != null) value = Math.min(value,opt_max);
    return value;
}

function degreesToRadians(deg) {
    return deg * (Math.PI / 180);
}

function radiansToDegrees(rad) {
    return rad / (Math.PI / 180);
}

function MercatorProjection() {
    this.pixelOrigin_ = new google.maps.Point(TILE_SIZE / 2,
        TILE_SIZE / 2);
    this.pixelsPerLonDegree_ = TILE_SIZE / 360;
    this.pixelsPerLonRadian_ = TILE_SIZE / (2 * Math.PI);
}

MercatorProjection.prototype.fromLatLngToPoint = function(latLng,
                                                          opt_point) {
    var me = this;
    var point = opt_point || new google.maps.Point(0, 0);
    var origin = me.pixelOrigin_;

    point.x = origin.x + latLng[0] * me.pixelsPerLonDegree_;

    // Truncating to 0.9999 effectively limits latitude to 89.189. This is
    // about a third of a tile past the edge of the world tile.
    var siny = bound(Math.sin(degreesToRadians(latLng[1])), -0.9999,
        0.9999);
    point.y = origin.y + 0.5 * Math.log((1 + siny) / (1 - siny)) *
    -me.pixelsPerLonRadian_;
    return point;
};

MercatorProjection.prototype.fromPointToLatLng = function(point) {
    var me = this;
    var origin = me.pixelOrigin_;
    var lng = (point[0] - origin.x) / me.pixelsPerLonDegree_;
    var latRadians = (point[1] - origin.y) / -me.pixelsPerLonRadian_;
    var lat = radiansToDegrees(2 * Math.atan(Math.exp(latRadians)) -
    Math.PI / 2);
    return new google.maps.LatLng(lat, lng);
};

/*
 allAreas[].overlay.getPath().getAt().k er Y-koordinat
 -----------------||-----------------.D er X-koordinat

 array.push funktionen sætter element ind sidst i arrayet - YES BABY!
 */
function saveVectors()
{
    var latLngPoint = [2];
    var number = allAreas.length - 1;
    var points = [];
    var vector = [6];

    for (var i = 0; i < allAreas[number].overlay.getPath().length; i++) {
        latLngPoint = [allAreas[number].overlay.getPath().getAt(i).D,
                       allAreas[number].overlay.getPath().getAt(i).k];
        var coordinates = projection.fromLatLngToPoint(latLngPoint);
        points.push(coordinates);
    }

    for (var i = 0; i < points.length; i++) {
        if(i == points.length - 1) {
            var x1 = points[i].x;
            var y1 = points[i].y;
            var x2 = points[0].x;
            var y2 = points[0].y;
            var a = (y2 - y1)/(x2 - x1);
            var b = y1 - (a*x1);
            vector = [a,b,x1,x2,y1,y2];
            newVectors.push(vector);
        } else {
            var x1 = points[i].x;
            var y1 = points[i].y;
            var x2 = points[i+1].x;
            var y2 = points[i+1].y;
            var a = (y2 - y1)/(x2 - x1);
            var b = y1 - (a*x1);
            vector = [a,b,x1,x2,y1,y2];
            newVectors.push(vector);
        }
    }

    calculateIntersections();
}

function calculateIntersections() {
    var left = 1;
    var intersectionss = 0;
    for (var i = 0; i < newVectors.length; i++) {
        for(var j = left; j < newVectors.length; j++) {
            /* y = a1 * x + b1
               y = a2 * x + b2

               a1 * x + b1 = a2 * x + b2
               a1 * x - a2 * x = b2 - b1
               x(a1 - a2) = b2 - b1
               x = b2 - b1 / a1 - a2

             y = a2 * x + b2
             x = (y-b2)/a2

             y = a1*((y-b2)/a2)+b1

             */
            var a1 = newVectors[i][0];
            var a2 = newVectors[j][0];
            var b1 = newVectors[i][1];
            var b2 = newVectors[j][1];

            var x = (b2 - b1)/(a1 - a2);
            var y = (a1 * x) + (b1);

            if((((x > newVectors[i][2] && x < newVectors[i][3]) || (x < newVectors[i][2] && x > newVectors[i][3])) &&
                ((x > newVectors[j][2] && x < newVectors[j][3]) || (x < newVectors[j][2] && x > newVectors[j][3]))) &&
               (((y > newVectors[i][4] && y < newVectors[i][5]) || (y < newVectors[i][4] && y > newVectors[i][5])) &&
                ((y > newVectors[j][4] && y < newVectors[j][5]) || (y < newVectors[j][4] && y > newVectors[j][5])))) {
                intersectionsArray.push(projection.fromPointToLatLng([x,y]));
                console.log(intersectionsArray[intersectionss])
                intersectionss = intersectionss + 1;

            }
        }
        left++;
    }
    newVectors = [];
    console.log("Intersections: " + intersectionss);
    intersectionss = 0;
}





function showMap(position){

    projection = new MercatorProjection();

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
        printCoordinates(event.overlay);
        allAreas.push(event);
        saveVectors();

        drawingManager.setDrawingMode(null);

        var newShape = event.overlay;
        newShape.type = event.type;
        google.maps.event.addListener(newShape, 'click', function (e) {
            setSelection(newShape);
            printCoordinates(newShape);
        })
        setSelection(newShape);
    });

    google.maps.event.addDomListener(document.getElementById('deleteShape'), 'click', deleteSelectShape);
    google.maps.event.addDomListener(document.getElementById('deleteAllShapes'), 'click', deleteAllShapes);
}

function getLocation() {
    if (navigator.geolocation) {
        gotLocation = true;
        navigator.geolocation.getCurrentPosition(showMap);
    } else {
        x.innerHTML = "Geolocation is not supported by this browser.";
    }
}

google.maps.event.addDomListener(window, 'load', initialize);