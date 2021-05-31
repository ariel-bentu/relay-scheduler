

function getItems(calendarName) {
  var now = new Date();
  
  // set time to midnight
  now.setHours(0,0,0,0);

  // Get the begining of the week
  var sunday = new Date();
  sunday.setDate(now.getDate() - now.getDay() - 1);

  
  var offset = 7*24*60*60*1000;
  var inAWeek = new Date(sunday.getTime() + offset);
 
  var cal = CalendarApp.getCalendarsByName(calendarName)[0];
  return cal.getEvents(sunday, inAWeek);
}
  
  
function doGet(e) {
  var eTag = e.parameter ? e.parameter["eTag"]: "";
  var log = e.parameter ? e.parameter["log"] : "";
  var calendarName = e.parameter ? e.parameter["cal"] : "";
  if (log && log.length > 0) {
    console.log(log);
    return ContentService.createTextOutput("Logged: "+log);
  }
  return exec(eTag, calendarName);
}

function exec(eTag, calendarName) {
  var items = getItems(calendarName);
  var itemsCount = items.length;
  
  var result = "";

  for (var i=0;i<items.length;i++) {
    var item = items[i];
    
    var relays = item.getTitle();
    
    var startDay = item.getStartTime().getDay() + 1;
    var startHour = item.getStartTime().getHours();
    var startMin = item.getStartTime().getMinutes();
    var durationMin = Math.round((item.getEndTime() - item.getStartTime()) / 60000);
    var recurrent = item.isRecurringEvent() ? "1" : "0";
    
    //support multiple relays in one event, comma seperated
    var relayIDs = relays.split(",");
    if (relayIDs.length > 1) {
      itemsCount += relayIDs.length - 1;
    }
    
    for (var j=0;j<relayIDs.length;j++) {
      result += relayIDs[j] +" "+ startDay + " " + startHour +":"+ startMin +" " + durationMin +" "+ recurrent +" ";
    }
  }
  
  result =  itemsCount + " " + result;
  
  var eTagNew = MD5(result, true);
  if (eTagNew == eTag) {
    result = "";
    eTagNew = "-";
  }
  
  var timeOfWeek = getTimeOfWeek();
  
  return ContentService.createTextOutput(eTagNew + "|" + timeOfWeek + "|" + result);
}

function getTimeOfWeek() {
  var d = new Date();
  var day = d.getDay()+1;
  
  var hour = d.getHours();
  var min = d.getMinutes();
  var sec = d.getSeconds();
  
  return day + ":" + hour + ":" + min + ":" + sec;
}

function MD5( input, isShortMode )
{
    var txtHash = '';
    var rawHash = Utilities.computeDigest(
                      Utilities.DigestAlgorithm.MD5,
                      input,
                      Utilities.Charset.UTF_8 );

    var isShortMode = ( isShortMode == true ) ? true : false;
 
    if ( ! isShortMode ) {
        for ( i = 0; i < rawHash.length; i++ ) {

            var hashVal = rawHash[i];

            if ( hashVal < 0 ) {
                hashVal += 256;
            };
            if ( hashVal.toString( 16 ).length == 1 ) {
                txtHash += '0';
            };
            txtHash += hashVal.toString( 16 );
        };
    } else {
        for ( j = 0; j < 16; j += 8 ) {

            hashVal = ( rawHash[j]   + rawHash[j+1] + rawHash[j+2] + rawHash[j+3] )
                    ^ ( rawHash[j+4] + rawHash[j+5] + rawHash[j+6] + rawHash[j+7] );

            if ( hashVal < 0 ) {
                hashVal += 1024;
            };
            if ( hashVal.toString( 36 ).length == 1 ) {
                txtHash += "0";
            };

            txtHash += hashVal.toString( 36 );
        };
    };

    // change below to "txtHash.toLowerCase()" for lower case result.
    return txtHash.toUpperCase();

}