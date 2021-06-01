# Relay-Scheduler

This project was done as a hobby, with the challenge to build a sprinklers automation system for my garden, with the aim to make it dirt cheap and open source.
Hopefully other can enjoy it as well.

The Relay Scheduler is a small device which can control a set of relays and activate them based on a schedule, maintained in a Google Calender.
I used it for water sprinklers but of course it can be used to any electrical powered devices.

## Hardware Prerequisite
- 1 - Arduino compliant device with Wifi - I used `WeMos D1 mini` (2-4$  in e.g. Aliexpress) 
- 1 - 6 channel relay module, 5v - approx. 6$
- 8 wires - approx. 1$
- Power adapter (5v)

<img src="media/wemos-d1-mini.png"/>
<img src="media/relay.png"/>
<img src="media/wires.png"/>


## Technical Knowledge/prerequisite 
- You need to be able to flush the program in this repo into the arduino device
- Have a Gmail account - for calendar and Drive/Apps Script access
- You need to be able to wire the arduino device with the relay
- Any connection of high voltage power device is at your own. I suggest hiring a certified electrician for such cases, or stick to low voltage devices (e.g. 24v electric water valve for watering system)

## Setup steps
1. flush the software to the arduino device
2. setup a new, dedicated google calendar in your existing Gmail account
3. deploy the google Apps Script (details below...)
4. wire the device to the relay and your electrical device to the relay channels (details below...)
5. Configure the device via a web interface (details below...)
6. you are good to go!

### 1. flush the software to the arduino device
This project is setup to use PlatformIO. I found it very useful to work with WeMos, I used VSCode's plugin of PlatformIO (PlatformIO IDE). Follow the plugin details page to see how to flush the code into the device.

### 3. deploy the google Apps Script
- Goto `https://script.google.com/`
- Create new project
- Copy and paste the code in `src/code.gs` into your project's `code.gs` file
- Press 'Deploy'-> 'New Deployment'
- Select type: webapp, execute as: Me (you Gmail address), Who has access: Anyone. - press 'Deploy'
<br/>
<img src="media/deploy.png"/>
<br/>
- Authorize -> choose you Gmail account -> click 'advanced' -> click `go to <your project name>` -> Press 'Allow'
<br/>
<img src="media/trust.png"/>
<br/>
- Copy the URL of the webapp - it has the following format: `https://script.google.com/macros/s/<function-name>/exec`
- Test it by adding `?cal=<calendar name>` - the calendar you created in step 2.
- Save the function name for later use in step 5


### 4. wire the device
- relay channels wires: D1 to channel 1 (IN1), D2 to channel 2 (IN2), ...
  - you may set as many relays as you want, simply change the `#define MAX_RELAYS  6` to any number you choose.
- `5v` to the relay's `DC+`
- `G` to relay's `DC-`

### 5. Configure the device via a web interface 
TODO

## My final result
<img src="media/final-product.jpeg"/>