var plan = {
    input: `53 6 7 19:30 10 1 3 7 20:0 30 1 5 7 20:30 30 1 3 1 5:0 30 1 5 1 5:30 30 1 3 1 13:0 30 1 5 1 13:30 30 1 6 1 19:30 10 1 3 1 20:0 30 1 5 1 20:30 30 1 3 2 5:0 30 1 5 2 5:30 30 1 5 2 7:0 5 0 3 2 9:0 30 0 3 2 15:45 30 1 5 2 16:15 30 1 2 2 19:0 15 1 6 2 19:30 10 1 3 2 20:0 30 1 5 2 20:30 30 1 3 3 5:0 30 1 5 3 5:30 30 1 3 3 13:0 30 1 5 3 13:30 30 1 6 3 19:30 10 1 3 3 20:0 30 1 5 3 20:30 30 1 3 4 5:0 30 1 5 4 5:30 30 1 3 4 13:0 30 1 5 4 13:30 30 1 6 4 19:30 10 1 3 4 20:0 30 1 5 4 20:30 30 1 3 5 5:0 30 1 5 5 5:30 30 1 3 5 13:0 30 1 5 5 13:30 30 1 2 5 19:0 15 1 6 5 19:30 10 1 3 5 20:0 30 1 5 5 20:30 30 1 3 6 5:0 30 1 5 6 5:30 30 1 3 6 13:0 30 1 5 6 13:30 30 1 6 6 19:30 10 1 3 6 20:0 30 1 5 6 20:30 30 1 3 7 5:0 30 1 5 7 5:30 30 1 3 7 13:0 30 1 5 7 13:30 30 1 `,
    pos: 0
}



let numOfPlans = getInt(plan);
console.log(numOfPlans);

let plans = [];
for (var i = 0 ; i < numOfPlans;i++) {
    let onePlan = {}
    onePlan.relay = getInt(plan);
    onePlan.day = getInt(plan);
    onePlan.hour = getInt(plan);
    onePlan.min = getInt(plan);

    onePlan.durationMin = getInt(plan);

    onePlan.recurrent = getInt(plan) == 1? true:false;
    plans.push(onePlan);
}

plans.sort((a, b) => (a.day > b.day) || (a.day == b.day && a.hour > b.hour) ||  (a.day == b.day && a.hour == b.hour && a.min > b.min)  ? 1 : -1)
     
var day = 0;

for (var i = 0 ; i < numOfPlans;i++) {
    if (day != plans[i].day) {
        console.log(" ");
        console.log("day: " + plans[i].day);
        console.log("--------------");
        day = plans[i].day;
    }
    console.log(formatHour(plans[i].hour,plans[i].min) + " - relay: " + plans[i].relay +" - " +plans[i].durationMin+" minutes")

}

function formatHour(h, m) {
    return (h>9?h:("0"+h)) + ":" + (m>9?m:("0"+m));
}


function getInt(p) {
    //skip white space of :
    while(p.input[p.pos] == ' ' || p.input[p.pos] == ':')
        p.pos++;

    var intStr = ""
    var i = 0;
    while(i < 10 && p.input[p.pos] != ' ' && p.input[p.pos] != ':' && p.input[p.pos] != ',') {
        intStr += p.input[p.pos];
        p.pos++;
        i++;
    }

    return parseInt(intStr);
}