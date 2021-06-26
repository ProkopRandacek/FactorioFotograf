ext = ".png" // change this to jpg if you converted the images to jpg

mapInfo = JSON.parse(mapInfo); // mapInfo comes from mapInfo.js

var map = L.map('map', {
	crs: L.CRS.Simple,
	center: [0, 0],
	zoom: 0,
})

L.tileLayer('./images/0/{z}/{x}/{y}' + ext, {
	attribution: '<a href="https://github.com/ProkopRandacek/FactorioFotograf">FactorioFotograf</a>',
	maxZoom: 8,
	minZoom: 0,
	zoomOffset: 0,
	noWrap: true,
	tileSize: mapInfo.block_size * mapInfo.pixels_per_tile,
}).addTo(map)

L.TileLayer.prototype.getTileUrl = function(c) {
	//if (c.x >= mapInfo.minx && c.x <= mapInfo.maxx && c.y >= mapInfo.miny && c.y <= mapInfo.maxy) {
		console.log('./images/0/' + c.z + '/' + c.x + '/' + c.y + ext)
		return './images/0/' + c.z + '/' + c.x + '/' + c.y + ext
	//} else { // tile is definitely out of bounds
		//return ""
	//}
}

