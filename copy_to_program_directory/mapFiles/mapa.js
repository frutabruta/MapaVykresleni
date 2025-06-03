var redIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-red.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
  });

  var blueIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-blue.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
  });
  
  var yellowIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-yellow.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
  });
  
  var greenIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-green.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
  });





let marker = null;

var converter = new JTSK_Converter();

const socket = new WebSocket("ws://localhost:12345");

socket.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log(data);
    var lat = data.latitude;
    var lon = data.longitude;
    var centerMap=data.center_map;
    const coordinateSystem = data.coordinate_system;

    if(coordinateSystem=="J_STSK")
    {
            var wgs = converter.JTSKtoWGS84(Math.abs(data.longitude), Math.abs(data.latitude)); // returns object {'lat', 'lon'}
            lon=wgs.lon;
            lat=wgs.lat;
           // console.log(wgs);
    }



    if (marker === null) {
        // Create the marker the first time
        marker = L.marker([lat, lon], {icon: greenIcon}).addTo(map);
    } else {
        // Update the marker's position
        marker.setLatLng([lat, lon]);
    }

    if(centerMap)
    {
    map.setView([lat, lon], 18);
    }



};





var map = L.map('map').setView([50.08, 14.41], 13);
// Set up the OSM layer
L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);




  
  
 


var wgs = converter.JTSKtoWGS84(1104335.13, 707849.38); // returns object {'lat', 'lon'}

//konverze souradnic 
//var point = [ -747963.64999999851, -1041493.3299999982 ];
//$("#source").text(point);
//proj4.defs( "EPSG:5514", "+title=Krovak +proj=krovak +lat_0=49.5 +lon_0=24.83333333333333 +alpha=30.28813972222222 +k=0.9999 +x_0=0 +y_0=0 +ellps=bessel +units=m +towgs84=570.8,85.7,462.8,4.998,1.587,5.261,3.56 +no_defs" );

proj4.defs( "EPSG:5514", "+proj=longlat +datum=WGS84 +to +proj=krovak +lat_0=49.5 +lon_0=24.83333333333333  +alpha=30.28813972222222 +k=0.9999 +x_0=0 +y_0=0 +ellps=bessel  +pm=greenwich +units=m +no_defs  +towgs84=570.8,85.7,462.8,4.998,1.587,5.261,3.56" );




/* nachystame dve vrstvy: pro znacky a pro cary */
var vyparsovanyJson = JSON.parse(dataj);
znackyPole = vyparsovanyJson.polozky;

var q = 0;
var bounds = new L.LatLngBounds();
for (var k = 0; k < znackyPole.length; k++) {
   

    var souradnicovySystem = znackyPole[k].souradnicovySystem;




    znacky = znackyPole[k].data;
    //console.log(znacky);
    console.log(znacky.length);
    var lineCoords = [];
    var lineCoordsOsm = [];
    console.log("k=", k);

    if (znacky.length > 0) {
        for (var i = 0; i < znacky.length; i++) {
            console.log("i=", i);


            var cardData = znacky[i];
            var coords2;
            if (souradnicovySystem == "WGS84") {

                coords2osm=[cardData.lat ,cardData.lng];
               
            }
            else {
     
                var testPoint=[cardData.lng,cardData.lat];
               // coords2osm = proj4( "EPSG:5514" ).inverse( testPoint );
               // console.log("krovak:");
               // console.log(coords2osm);
               // coords2osm=[coords2osm[0], coords2osm[1]   ];
                

                var wgs84 = converter.JTSKtoWGS84(Math.abs(cardData.lng), Math.abs(cardData.lat)); // returns object {'x', 'y'}
                console.log("wgs84");
                console.log(wgs84);
                coords2osm =  wgs84; //[wgs84.x, wgs84.y];
                console.log("krovak2:");
                console.log(coords2osm);
            }
            


            lineCoords.push(coords2);

            //OSM
            lineCoordsOsm.push(coords2osm);

            /* vyrobit znacku */

            var iconColor= {};

      

            var cssmod="";
            if(cardData.color=="#ff0000")
                {
                    cssmod='filter: hue-rotate(0deg);';
                   
                }
                else if(cardData.color=="#0000ff")
                {
                    cssmod='filter: hue-rotate(229deg);';
                }
                else if(cardData.color=="#ffff00")
                {
                    cssmod='filter: hue-rotate(66deg);';
                }
                else
                {                  
                    cssmod='filter: hue-rotate(0deg);';
                }

           

            // OSM
            if (znackyPole[k].vykresliBody) {
                var customIcon = L.divIcon({
                    html: '<div class="innerDiv" style="'+cssmod+'">'+cardData.kapka+'</div>',
                    /* html: '<div class="innerDiv" style="color:'+cardData.color+'">'+cardData.kapka+'</div>',*/
                    className: 'markerCss' // Ensure no default class styles affect your custom icon
                });
                var markerosm = L.marker([cardData.lat, cardData.lng],{ icon: customIcon }).addTo(map);
      
                markerosm.bindPopup(cardData.title+cardData.cont);    
                bounds.extend(markerosm.getLatLng());
            }
          

            const polomer = 0.1;

            if (typeof cardData.radius !== 'undefined') {
      

                //OSM
                if (znackyPole[k].vykresliRadius) {
                    var kruznice=L.circle([cardData.lat, cardData.lng], {radius: cardData.radius}).addTo(map);
                    bounds.extend(kruznice.getBounds());
                }               
   


            }
        }
        
            //OSM
        
            if (znackyPole[k].vykresliSpojnici) {
                var polyline = L.polyline(lineCoordsOsm, {color: 'red'}).addTo(map);
              //  map.fitBounds(polyline.getBounds());
            }
     

            q = q + 1;

    }
    else {
        console.log("datova sada je prazdna");
    }



}

map.fitBounds(bounds);

//znackyPole.forEach((znacky) => {vykresli(znacky)});

function closeMe() {
    window.open("", "_self");
    window.close();
}


var box = document.createElement("div");
//box.innerHTML = "<button></button><textarea></textarea><button><strong>Vykreslit</strong></button>";

box.innerHTML = vyparsovanyJson.popis;

// <h1>spoj</h1><table><tr><td>linka</td><td>337</td></tr><tr><td>spoj</td><td>1007</td></tr></table>
box.id = "boxik";
//mapa.getContainer().appendChild(box);

var mapddiv= document.getElementById("map");
mapddiv.style.zIndex = "1";

var wrapperdiv= document.getElementById("wrapper");
wrapperdiv.appendChild(box);
box.style.zIndex = "2";


