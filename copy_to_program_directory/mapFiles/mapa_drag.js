// Initialize map
const map = L.map('map').setView([49.8209, 18.2625], 13); // Ostrava as example center

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

// Create a draggable marker
const marker = L.marker([49.8209, 18.2625], {
    draggable: true,
    autoPan: true // pans the map if you drag near the edge
}).addTo(map);

// Set up the WebSocket connection
const ws = new WebSocket('ws://localhost:12347');

ws.onopen = () => console.log('WebSocket connected');
ws.onerror = (err) => console.error('WebSocket error:', err);
ws.onclose = () => console.log('WebSocket closed');

// Fire when the user drops the marker
marker.on('dragend', (event) => {
    const position = event.target.getLatLng();

    const payload = {
        center_map: true,
        coordinate_system: "WGS84",
        latitude: position.lat,
        longitude: position.lng
    };

    if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify(payload));
    } else {
        console.warn('WebSocket not open, could not send position');
    }
});