/**
 * Created by BusterK on 19-01-2015.
 */

function initialize() {
    getLocation();
}

/*
    Booleans
 */
var gotLocation = false;
var firstDrawing = true;

/*
    Arrays
 */
var allPoints = [];
var newVectors = [];
var oldVectors = [];
var intersectionsArray = [];

/*
    Variables
 */
var selectedShape;
var TILE_SIZE = 256;
var projection;
var masterPolygon;

var tempOverlay;

function printCoordinates() {
    if(masterPolygon === null) {
        document.getElementById('coords').innerHTML = "Ingen områder endnu.";
    } else {
        var coordinates = "";
        for (var i = 0; i < masterPolygon.getPath().length; i++) {
            coordinates += "( Lat , Lng ) ( y , x ) = " + masterPolygon.getPath().getAt(i) + "<br>";
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
        selectedShape.setMap(null);
        printCoordinates();
        firstDrawing = true;
    } else {
        selectedShape.setMap(null);
        printCoordinates(null);
    }

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
function saveVectors(tempPolygon)
{
    var latLngPoint = [2];
    var points = [];
    var vector = [6];

    for (var i = 0; i < tempPolygon.getPath().length; i++) {
        latLngPoint = [tempPolygon.getPath().getAt(i).D,
                       tempPolygon.getPath().getAt(i).k];
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

    if(firstDrawing) {
        oldVectors = newVectors;
        newVectors = [];
    }
}

function calculateIntersections() {

    var tempPoints = [];
    var newPoints = [];
    var x, y, a1, a2, b1, b2;
    var foundIntersection = false;
    var firstVectorIntersection = false;
    var currentNewVector = 0;

    for (var i = 0; i < oldVectors.length; i++) {
        firstVectorIntersection = false;
        for (var j = 0; j < newVectors.length; j++) {
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

            a2 = newVectors[j][0];
            a1 = oldVectors[i][0];
            b2 = newVectors[j][1];
            b1 = oldVectors[i][1];

            x = (b2 - b1) / (a1 - a2);
            y = (a1 * x) + (b1);

            /*
             vector = [a,b,x1,x2,y1,y2];
             */

            if ((((x > newVectors[j][2] && x < newVectors[j][3]) || (x < newVectors[j][2] && x > newVectors[j][3])) &&
                ((x > oldVectors[i][2] && x < oldVectors[i][3]) || (x < oldVectors[i][2] && x > oldVectors[i][3]))) &&
                (((y > newVectors[j][4] && y < newVectors[j][5]) || (y < newVectors[j][4] && y > newVectors[j][5])) &&
                ((y > oldVectors[i][4] && y < oldVectors[i][5]) || (y < oldVectors[i][4] && y > oldVectors[i][5])))) {
                intersectionsArray.push(projection.fromPointToLatLng([x, y]));
                tempPoints.push([x, y]);
                foundIntersection = true;

                if(!firstVectorIntersection) {
                    currentNewVector = j;
                    firstVectorIntersection = true;
                }
            }
        }

        var difference = 0;
        var choice = 0;
        for(var k = 0; k < tempPoints.length; k++) {
            if(k == 0) {
                difference = x - tempPoints[k][0];
            } else {
                var distance = x - tempPoints[k][0];
            }
            if(distance < difference) {
                difference = distance;
                choice = k;
            }
        }

        if(foundIntersection) {
            newPoints.push(projection.fromPointToLatLng([oldVectors[i][2],oldVectors[i][4]]));
            newPoints.push(projection.fromPointToLatLng([tempPoints[choice][0], tempPoints[choice][1]]));
            if(google.maps.geometry.poly.containsLocation(projection.fromPointToLatLng([newVectors[currentNewVector][2],newVectors[currentNewVector][4]]), masterPolygon)) {
                newPoints.push(projection.fromPointToLatLng([newVectors[currentNewVector][3],newVectors[currentNewVector][5]]));
            } else {
                newPoints.push(projection.fromPointToLatLng([newVectors[currentNewVector][2],newVectors[currentNewVector][4]]));
            }

            /*
            var l = currentNewVector + 1;
            while(l < newVectors.length && !google.maps.geometry.poly.containsLocation(projection.fromPointToLatLng([newVectors[l][3],newVectors[l][5]]),masterPolygon)) {
                newPoints.push(projection.fromPointToLatLng([newVectors[l][3],newVectors[l][5]]));
                l++;
            }
            */
            newPoints.push(projection.fromPointToLatLng([newVectors[2][2],newVectors[2][4]]));
            newPoints.push(projection.fromPointToLatLng([tempPoints[choice+1][0], tempPoints[choice+1][1]]));

            foundIntersection = false;
        } else {
            newPoints.push(projection.fromPointToLatLng([oldVectors[i][2],oldVectors[i][4]]));
        }
    }

    allPoints = newPoints;

    masterPolygon.setMap(null);
}

function dotInsidePolygon(overlay) {
    for(var i = 0; i < overlay.getPath().length; i++) {
        if(google.maps.geometry.poly.containsLocation(overlay.getPath().getAt(i), masterPolygon)) {

        }
    }

}

function drawNewPolygon() {

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
        if(firstDrawing) {

            /*
                Sætte masterPolygon til det overordnede polygon.
             */
            masterPolygon = event.overlay;
            /*
                Print koordinater til browser vinduet.
             */
            printCoordinates();

            /*
                Reset drawing manager, så der skal klikkes igen for at tegne ny tegning.
             */
            drawingManager.setDrawingMode(null);

            /*
                Tilføj listener til hvert overlay, så at det vælges hvis man trykker på det.
             */
            var newShape = event.overlay;
            newShape.type = event.type;
            google.maps.event.addListener(newShape, 'click', function (e) {
                setSelection(newShape);
                printCoordinates(newShape);
            })

            /*
                Når overlay er tegnet, er det valgt fra starten af.
             */
            setSelection(newShape);

            /*
                Gem alle vektorer fra tegnet overlay.
             */
            saveVectors(event.overlay);

            firstDrawing = false;

        } else {
            /*
                Gem alle vektorer fra tegnet overlay.
             */
            saveVectors(event.overlay);

            /*
                Beregn skæringer imellem vektorerne.
             */
            calculateIntersections();

            /*
                Tegn ny polygon
             */

            event.overlay.setMap(null);

            new google.maps.Polygon({
                paths: allPoints,
                clickable: true,
                editable: true,
                draggable: true,
                strokeWeight: 0,
                fillOpacity: 0.45,
                map: map
            });

            /*
                Print koordinater til browser vinduet.
             */
                printCoordinates();

            /*
                Reset drawing manager, så der skal klikkes igen for at tegne ny tegning.
             */
            drawingManager.setDrawingMode(null);

            for(var i = 0; i < intersectionsArray.length; i++) {
                var marker = new google.maps.Marker({
                    position: intersectionsArray[i],
                    map: map
                });
                console.log("(Lat, Lng) : (" + intersectionsArray[i].k + ", " + intersectionsArray[i].D + ")");
            }
        }
    });

    google.maps.event.addDomListener(document.getElementById('deleteShape'), 'click', deleteSelectShape);
    google.maps.event.addDomListener(document.getElementById('deleteAllShapes'), 'click', deleteAllShapes);
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