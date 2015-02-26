/**
 * Created by BusterK on 19-01-2015.
 */

var googleMap;
function initialize() {

}

function showMap(position){

    var mapOptions = {
        zoom: 15,
        center: new google.maps.LatLng(position.coords.latitude, position.coords.longitude)
    };

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
            editable: true
        }
    });
    drawingManager.setMap(map);

    google.maps.event.addListener(drawingManager, "overlaycomplete", function(event){
        console.log(event.overlay.getPath().getArray());
        var coordinates = "";
        for(i = 0; i < event.overlay.getPath().length; i++){
            coordinates += event.overlay.getPath().getAt(i) + "<br>";
        }
        document.getElementById('coords').innerHTML = coordinates;
    });
}

function showDefaultMap(position){

    var mapOptions = {
        zoom: 6,
        center: new google.maps.LatLng(55.695003, 10.629682)
    };

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
            editable: true
        }
    });
    drawingManager.setMap(map);

    google.maps.event.addListener(drawingManager, "overlaycomplete", function(event){
        console.log(event.overlay.getPath().getArray());
        var coordinates = "";
        for(i = 0; i < event.overlay.getPath().length; i++){
            coordinates += event.overlay.getPath().getAt(i) + "<br>";
        }
        document.getElementById('coords').innerHTML = coordinates;
    });

}

function loadGoogleMaps() {
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src =
        'https://maps.googleapis.com/maps/api/js?'
        + 'AIzaSyCiqfn23Gqe_IjUaPwXJETVOUZlblXnaLk' +
        '&'
        + 'v=3.exp' +
        '&'
        + 'libraries=drawing' +
        '&'
        + 'callback=initialize';
    document.body.appendChild(script);
}

function getLocation() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(showMap, showDefaultMap);
    } else {
        x.innerHTML = "Geolocation is not supported by this browser.";
    }
}

window.onload = loadGoogleMaps();
window.onload = getLocation();