var xmlHttp;

var xmlHttpServos;

var xmlHttpCrossings;

function GetXmlHttpObject() {
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

function initPage() {
	showSliderValue( 'motionTime' );

	setEnableAccessPoint( 'settingsWF_AccessPoint' );
}
	
function showSliderValue(name) {
	let slider = document.getElementById( name );
	let slideValuePlaceHolder = document.getElementById( name + 'Value' );
	slideValuePlaceHolder.innerHTML = slider.value;
}

function showSliderValueReset(name) {
	let slider = document.getElementById( name );
	let sliderValuePlaceHolder = document.getElementById( name + 'Value' );
	let sliderValueDefaultValue = slider.defaultValue;
	sliderValuePlaceHolder.innerHTML = sliderValueDefaultValue;
}

function showSliderValuesReset(type, servoID) {
	
	let sliderMin = document.getElementById( type + '_min#' + servoID );
	let sliderMax = document.getElementById( type + '_max#' + servoID );
	
	let sliderValuePlaceHolderMin = document.getElementById( type + '_min#' + servoID + 'Value' );
	let sliderValuePlaceHolderMax = document.getElementById( type + '_max#' + servoID + 'Value' );
	
	let sliderMinDefaultValue = sliderMin.defaultValue;
	let sliderMaxDefaultValue = sliderMax.defaultValue;
	
	sliderValuePlaceHolderMin.innerHTML = sliderMinDefaultValue;
	sliderValuePlaceHolderMax.innerHTML = sliderMaxDefaultValue;
}

function servoMovement(position, id) {
	let motionType = document.getElementById( 'smoothID#' + id ).checked;

	let command = -1;

	switch(position) {
		case "min":
			command = 0;
			break;
		
		case "center":
			command = 1;
			break;
		
		case "max":
			command = 2;
			break;
		
		default:
			alert( "Invalid position value (" + position + ") for servo movement" );
			return;
	}

	let args = "";
	if ( motionType==true ) {
		args = "servoID=" + id + "&command=" + command + "&movType=1";
	}
	else {
		args = "servoID=" + id + "&command=" + command + "&movType=0";
	}

	// With HTTP GET method
	xmlHttp = GetXmlHttpObject();
	xmlHttp.open("POST", "/moveServo", true);
	xmlHttp.setRequestHeader( "Content-type", "application/x-www-form-urlencoded" );
	xmlHttp.onreadystatechange = function () {
		if( xmlHttp.readyState === 4 ) {
			//alert( xmlHttp.response );
		}
	}
	xmlHttp.send( args );
}

function crossingMovement(operation, id) {
	let command = -1;

	switch(operation) {
		case "open":
			command = 0;
			break;
		
		case "close":
			command = 1;
			break;
		
		default:
			alert( "Invalid operation value (" + operation + ") for crossing movement" );
			return;
	}

	let args = "crossingID=" + id + "&command=" + command;

	// With HTTP GET method
	xmlHttp = GetXmlHttpObject();
	xmlHttp.open( "POST", "/moveCrossing", true );
	xmlHttp.setRequestHeader( "Content-type", "application/x-www-form-urlencoded" );
	xmlHttp.onreadystatechange = function () {
		if( xmlHttp.readyState === 4 ) {
			//alert( xmlHttp.response );
		}
	}
	xmlHttp.send( args );
}


function setEnableAccessPoint(selID) {
	let isSelected = document.getElementById( selID ).checked;
	
	document.getElementById( 'settingsWF_Password' ).disabled = isSelected;
	
	document.getElementById( 'settingsWF_Address' ).disabled = !isSelected;
	document.getElementById( 'settingsWF_Gateway' ).disabled = !isSelected;
	document.getElementById( 'settingsWF_NetworkMask' ).disabled = !isSelected;
}

function viewPassword(passwordID, passwordToggleID) {
    let passwordInput = document.getElementById( passwordID );
    let passwordInputToggle = document.getElementById( passwordToggleID );
 
    if ( passwordInput.type === "password" ){
        passwordInput.type = "text";
        passwordInputToggle.className = "fa fa-eye-slash";
    }
    else{
        passwordInput.type = "password";
        passwordInputToggle.className= "fa fa-eye";
    }
}

function buildServos() {
	// With HTTP GET method
	xmlHttpServos = GetXmlHttpObject();
	xmlHttpServos.open("GET", "/getServos", true);
	xmlHttpServos.onreadystatechange=ServosDataReply;
	xmlHttpServos.send( null );
}

function ServosDataReply() {
	if( xmlHttpServos.readyState === 4 ) {	
		let servos = JSON.parse( xmlHttpServos.responseText );

		let data = "";
		
		for (i = 0; i < servos.length; ++i) {
			let currentServo = servos[i].servo;

			let name = currentServo.name;
			let description = currentServo.description;

			let checkedFlag = currentServo.smooth === "true";
			
			let rangeMin = currentServo.minPosition;
			let rangeMax = currentServo.maxPosition;
			
			let r1_min		= rangeMin.min;
			let r1_value	= rangeMin.value;
			let r1_max		= rangeMin.max;
			let r1_units	= rangeMin.units;

			let r2_min		= rangeMax.min;
			let r2_value	= rangeMax.value;
			let r2_max		= rangeMax.max;
			let r2_units	= rangeMax.units;
			
			data += buildServo( i, name, description, checkedFlag, r1_min, r1_value, r1_max, r1_units, r2_min, r2_value, r2_max, r2_units );
		}
		
		document.getElementById( "servosPlace").innerHTML = data;
		
		for (i = 0; i < servos.length; ++i) {
			showSliderValue( 'servo_min#' + i );
			showSliderValue( 'servo_max#' + i );
		}
	}
}

function buildServo(servoID, servoName, servoDescription, checkedFlag, range1_Min, range1_Value, range1_Max, range1_Units, range2_Min, range2_Value, range2_Max, range2_Units) {
	
	let response = "";
	
	response += "<!-- " + servoName + " -->\n" ;
	response += "<div class=\"panel panel-default myAlignment myPlacementIn\">\n" ;
	response += "	<div class=\"panel-heading\">\n" ;
	response += "        <h3 class=\"panel-title\">" + servoName + "</h3>\n" ;
	response += "        <h3 class=\"panel-title panel-title-desc\">" + servoDescription + "</h3>\n" ;
	response += "    </div>\n" ;
	response += "    <div class=\"form\">\n" ;
	response += "        <form action=\"/updateServo\" method=\"POST\" onReset=\"showSliderValuesReset( 'servo', '" + servoID + "' )\" >\n" ;
	response += "            <div class=\"panel-body\">\n" ;
	response += "                <div>Tortoise position</div>\n" ;
	response += "				<div class=\"divTable\">\n" ;
	response += "					<div class=\"divTableBody\">\n" ;
	response += "						<div class=\"divTableRow\">\n" ;
	response += "							<div class=\"divTableCell\">\n" ;
	response += "								<input onclick=\"servoMovement( 'min', '" + servoID + "')\" type=\"button\" value=\"Minimum\">\n" ;
	response += "							</div>\n" ;
	response += "							<div class=\"divTableCell\">\n" ;
	response += "								<input onclick=\"servoMovement('center', '" + servoID + "')\" type=\"button\" value=\"Center\">\n" ;
	response += "							</div>\n" ;
	response += "							<div class=\"divTableCell\">\n" ;
	response += "								<input onclick=\"servoMovement( 'max', '" + servoID + "')\" type=\"button\" value=\"Maximum\">\n" ;
	response += "							</div>\n" ;
	response += "						</div>\n" ;				
	response += "					</div>\n" ;
	response += "				</div>\n" ;
	response += "				<div class=\"motionContainer\">\n" ;
	response += "					<input type=\"checkbox\" id=\"smoothID#" + servoID + "\" name=\"smooth#" + servoID + "\" " + (checkedFlag==true ? "checked" : "" ) + " />\n" ;
	response += "					<label for=\"smoothID#" + servoID + "\">Smooth motion</label>\n" ;
	response += "				</div>\n" ;
	
	response += "				<input type=\"hidden\" name=\"servoID\" value=\"" + servoID + "\">\n" ;
	
	response += "				<div>Minimum value settings (Current value: <i><span id=\"servo_min#" + servoID + "Value\"></span></i> " + range1_Units +")</div>\n" ;
	response += "				<div class=\"slideContainer\">\n" ;
	response += "					<input onInput=\"showSliderValue( 'servo_min#" + servoID + "' )\" type=\"range\" min=\""+range1_Min+"\" max=\""+range1_Max+"\" value=\""+range1_Value+"\" class=\"slider\" name=\"servo_min#" + servoID + "\" id=\"servo_min#" + servoID + "\">\n" ;
	response += "				</div>\n" ;
	
	response += "				<div>Maximum value settings (Current value: <i><span id=\"servo_max#" + servoID + "Value\"></span></i> " + range2_Units +")</div>\n" ;
	response += "				<div class=\"slideContainer\">\n" ;
	response += "					<input onInput=\"showSliderValue( 'servo_max#" + servoID + "' )\" type=\"range\" min=\""+range2_Min+"\" max=\""+range2_Max+"\" value=\""+range2_Value+"\" class=\"slider\" name=\"servo_max#" + servoID + "\" id=\"servo_max#" + servoID + "\">\n" ;
	response += "				</div>\n" ;
	response += "			</div>\n" ;
	response += "            <div class=\"form-button\">\n" ;
	response += "                <input type=\"submit\" value=\"Save Values\"> <input type=\"reset\" value=\"Reset\">\n" ;
	response += "            </div>\n" ;
	response += "        </form>\n" ;
	response += "	</div>\n" ;
	response += "</div>\n" ;
	
	return response;
}

function buildCrossings() {
	// With HTTP GET method
	xmlHttpCrossings = GetXmlHttpObject();
	xmlHttpCrossings.open("GET", "/getCrossings", true);
	xmlHttpCrossings.onreadystatechange=CrossingsDataReply;
	xmlHttpCrossings.send( null );
}

function CrossingsDataReply() {
	if( xmlHttpCrossings.readyState === 4 ) {	
		let crossings = JSON.parse( xmlHttpCrossings.responseText );

		let data = "";
		
		for (i = 0; i < crossings.length; ++i) {
			let currentCrossing = crossings[i].crossing;

			let name = currentCrossing.name;
			let description = currentCrossing.description;
			
			let rangeMin = currentCrossing.minVolume;
			let rangeMax = currentCrossing.maxVolume;
			
			let r1_min		= rangeMin.min;
			let r1_value	= rangeMin.value;
			let r1_max		= rangeMin.max;
			let r1_units	= rangeMin.units;

			let r2_min		= rangeMax.min;
			let r2_value	= rangeMax.value;
			let r2_max		= rangeMax.max;
			let r2_units	= rangeMax.units;
			
			data += buildCrossing( i, name, description, r1_min, r1_value, r1_max, r1_units, r2_min, r2_value, r2_max, r2_units );
		}
		
		document.getElementById( "crossingsPlace").innerHTML = data;
		
		for (i = 0; i < crossings.length; ++i) {
			showSliderValue( 'crossing_min#' + i );
			showSliderValue( 'crossing_max#' + i );
		}
	}
}

function buildCrossing(crossingID, crossingName, crossingDescription, range1_Min, range1_Value, range1_Max, range1_Units, range2_Min, range2_Value, range2_Max, range2_Units) {
	
	let response = "";
	
	response += "<!-- " + crossingName +" -->\n" ;
	response += "<div class=\"panel panel-default myAlignment myPlacementIn\">\n" ;
	response += "	<div class=\"panel-heading\">\n" ;
	response += "        <h3 class=\"panel-title\">" + crossingName + "</h3>\n" ;
	response += "        <h3 class=\"panel-title panel-title-desc\">" + crossingDescription + "</h3>\n" ;
	response += "    </div>\n" ;
	response += "    <div class=\"form\">\n" ;
	response += "        <form action=\"/updateCrossing\" method=\"POST\" onReset=\"showSliderValuesReset( 'crossing', '" + crossingID + "' )\" >\n" ;
	response += "            <div class=\"panel-body\">\n" ;
	response += "                <div>Crossing operations</div>\n" ;
	response += "				<div class=\"divTable\">\n" ;
	response += "					<div class=\"divTableBody\">\n" ;
	response += "						<div class=\"divTableRow\">\n" ;
	response += "							<div class=\"divTableCell\">\n" ;
	response += "								<input onclick=\"crossingMovement( 'close', '" + crossingID + "')\" type=\"button\" value=\"Close\">\n" ;
	response += "							</div>\n" ;
	response += "							<div class=\"divTableCell\">\n" ;
	response += "								<input onclick=\"crossingMovement( 'open', '" + crossingID + "')\" type=\"button\" value=\"Open\">\n" ;
	response += "							</div>\n" ;
	response += "						</div>\n" ;				
	response += "					</div>\n" ;
	response += "				</div>\n" ;
	response += "				<input type=\"hidden\" name=\"crossingID\" value=\"" + crossingID + "\">\n" ;

	response += "				<div>Minimum volume settings (Current value: <i><span id=\"crossing_min#" + crossingID + "Value\"></span></i> " + range1_Units +")</div>\n" ;

	response += "				<div class=\"slideContainer\">\n" ;
	response += "					<input onInput=\"showSliderValue( 'crossing_min#" + crossingID + "' )\" type=\"range\" min=\""+range1_Min+"\" max=\""+range1_Max+"\" value=\""+range1_Value+"\" class=\"slider\" name=\"crossing_min#" + crossingID + "\" id=\"crossing_min#" + crossingID + "\">\n" ;
	response += "				</div>\n" ;

	response += "				<div>Maximum volume settings (Current value: <i><span id=\"crossing_max#" + crossingID + "Value\"></span></i> " + range2_Units +")</div>\n" ;
	response += "				<div class=\"slideContainer\">\n" ;
	response += "					<input onInput=\"showSliderValue( 'crossing_max#" + crossingID + "' )\" type=\"range\" min=\""+range2_Min+"\" max=\""+range2_Max+"\" value=\""+range2_Value+"\" class=\"slider\" name=\"crossing_max#" + crossingID + "\" id=\"crossing_max#" + crossingID + "\">\n" ;
	response += "				</div>\n" ;
	response += "			</div>\n" ;
	response += "            <div class=\"form-button\">\n" ;
	response += "                <input type=\"submit\" value=\"Save Values\"> <input type=\"reset\" value=\"Reset\">\n" ;
	response += "            </div>\n" ;
	response += "        </form>\n" ;
	response += "	</div>\n" ;
	response += "</div>\n" ;
	
	return response;
}
