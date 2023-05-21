const planString =
    "11 3 1 17:10 60 0 1 1 18:0 45 1 2 1 19:0 45 1 3 2 4:0 20 1 5 2 4:20 20 1 1 3 18:0 45 1 2 3 19:0 45 1 3 5 4:0 20 1 5 5 4:20 20 1 1 5 18:0 45 1 2 5 19:0 45 1";

const relays = [1,2,3,4,5,6]

function getInt(index) {
    return parseInt()
}

function generateTable(planString) {
    const daysOfWeek = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
    const rows = {};

    // Initialize rows object
    for (let i = 0; i < daysOfWeek.length; i++) {
        rows[daysOfWeek[i]] = {};
    }

    const planArray = planString.split(" ");
    const numOfTasks = planArray[0];

    let index = 1;

    for (let i = 0; i < numOfTasks; i++) {
        const relayID = parseInt(planArray[index++]);
        const day = daysOfWeek[parseInt(planArray[index++])-1];
        const time = planArray[index++];
        const duration = parseInt(planArray[index++]);
        const isRecurring = planArray[index++] === "1";

        if (rows[day][relayID]) {
            rows[day][relayID].push({ time, duration, isRecurring });
        } else {
            rows[day][relayID] = [{ time, duration, isRecurring }];
        }
    }


const line= ()=>console.log("-".repeat(15*7));
    // Create table header
    console.log("Relay |"+ daysOfWeek.filter(d=>d !="").map(d=>d+" ".repeat(12)).join("|"));
    line()
    for (const relayID of relays) {
        console.log(relayID);

        for (let i = 0; i < daysOfWeek.length; i++) {
            const day = daysOfWeek[i];
            const prefix = "       "+" ".repeat(i*16);

            if (rows[day][relayID]) {
                const events = rows[day][relayID];

                for (let j = 0; j < events.length; j++) {
                    const event = events[j];
                    const { time, duration, isRecurring } = event;
                    const formattedTime = formatTime(time);
                    const formattedDuration = `(${duration})`;
                    const formattedEvent = isRecurring
                        ? `${formattedTime} ${formattedDuration} (R)`
                        : `${formattedTime} ${formattedDuration}`;

                    
                        console.log(prefix + formattedEvent);
                }
            }

        }
        line()

    }

}


function formatTime(time) {
    const [hours, minutes] = time.split(":");
    const formattedHours = hours.padStart(2, "0");
    const formattedMinutes = minutes.padStart(2, "0");
    return `${formattedHours}:${formattedMinutes}`;
}


generateTable(planString);
