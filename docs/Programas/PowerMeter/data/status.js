var xmlHttp;

function updateValues() {
	setInterval( update, 1000);
}

function getXmlHttpObject() {
  try {
    return new ActiveXObject("Msxml2.XMLHTTP");
  } catch(e) {} // Internet Explorer
  try {
    return new ActiveXObject("Microsoft.XMLHTTP");
  } catch(e) {} // Internet Explorer
  try {
    return new XMLHttpRequest();
  } catch(e) {} // Firefox, Opera 8.0+, Safari
  alert("XMLHttpRequest not supported");
  return null;
}

function update() {
  xmlHttp = getXmlHttpObject();
  xmlHttp.open("GET", "/status", true);
  xmlHttp.onreadystatechange=updateHandleReply;
  xmlHttp.send(null);
}

function updateHandleReply() {
	if( xmlHttp.readyState === 4 ) {
		var voltage = document.getElementById( "voltage" );
		var current = document.getElementById( "current" );
		var power = document.getElementById( "power" );
		var energy = document.getElementById( "energy" );
		var frequency = document.getElementById( "frequency" );
		var powerFactor = document.getElementById( "powerFactor" );
		var time = document.getElementById( "time" );
		
		const data = JSON.parse( xmlHttp.responseText );
		
		voltage.innerHTML = data.voltage;
		current.innerHTML = data.current;
		power.innerHTML = data.power;
		energy.innerHTML = data.energy;
		frequency.innerHTML = data.frequency;
		powerFactor.innerHTML = data.powerFactor;
		time.innerHTML = data.time;
	}
}
