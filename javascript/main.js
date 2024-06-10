

// thermometer

const units = {
  Celcius: "°C",
  Fahrenheit: "°F"
};


const config = {
  minTemp: -20,
  maxTemp: 100,
  unit: "Celcius"
};
const configWater = {
  minliter: 0,
  maxliter: 50,
  unit: "liter"
};

// Change min and max temperature values

const tempValueInputs = document.querySelectorAll("input[type='text']");

// Switch unit of temperature

const unitP = document.getElementById("unit");

// unitP.addEventListener("click", () => {
//   config.unit = config.unit === "Celcius" ? "Fahrenheit" : "Celcius";
//   unitP.innerHTML = config.unit + ' ' + units[config.unit];
//   return setTemperature();
// });

// Change temperature

function setTemperature(nummer) {

  if (nummer == undefined) {
    graden.textContent = "Geen Tempratuur";

  }
  else {
    temperature.style.height = (nummer - config.minTemp) / (config.maxTemp - config.minTemp) * 100 + "%";
    temperature.dataset.value = nummer + units[config.unit];

    graden.textContent = temperature.dataset.value;
  }
}
const range = document.querySelector("input[type='range']");
const temperature = document.getElementById("temperature");
const graden = document.getElementById("degree");
var nummer;
// function setTemperature() {
//   temperature.style.height = (range.value - config.minTemp) / (config.maxTemp - config.minTemp) * 100 + "%";
//   temperature.dataset.value = range.value + units[config.unit];

//   graden.textContent = temperature.dataset.value;
// }



//waterpijl

// const tank_slide = document.getElementById("watertank");
const tank_slide = document.querySelector("input[type='range']");

const liter = document.getElementById("liter");
const water = document.getElementById("water");



setTimeout(setLiter, 1000);



var savedColor = localStorage.getItem('primaryColor');




//Motor


var opts = {
  angle: -0.01, // The span of the gauge arc
  lineWidth: 0.2, // The line thickness
  radiusScale: 1, // Relative radius
  pointer: {
    length: 0.6, // // Relative to gauge radius
    strokeWidth: 0.026, // The thickness
    color: '#ffffff' // Fill color
  },

  limitMax: false,     // If false, max value increases automatically if value > maxValue
  limitMin: false,     // If true, the min value of the gauge will be fixed
  colorStart: savedColor,   // Colors
  colorStop: savedColor,    // just experiment with them
  strokeColor: '#333333',  // to see which ones work best for you
  generateGradient: true,
  highDpiSupport: true,     // High resolution support

};


var motor_text = document.getElementById('usage'); // your canvas element
var target = document.getElementById('MotorGaugeControl'); // your canvas element

var Motorgauge = new Gauge(target).setOptions(opts); // create sexy gauge!
Motorgauge.maxValue = 100; // set max gauge value
Motorgauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
Motorgauge.animationSpeed = 70; // set animation speed (32 is default value)
Motorgauge.set(65); // set actual value
motor_text.textContent = Motorgauge.value + "%";


// water dashboard

var wateropts = {
  angle: 0.35, // The span of the gauge arc
  lineWidth: 0.1, // The line thickness
  radiusScale: 1, // Relative radius
  pointer: {
    length: 0.62, // // Relative to gauge radius
    strokeWidth: 0.049, // The thickness
    color: '#000000' // Fill color
  },
  limitMax: false,     // If false, max value increases automatically if value > maxValue
  limitMin: false,     // If true, the min value of the gauge will be fixed
  colorStart: '#3DCADF',   // Colors
  colorStop: '#3DCADF',    // just experiment with them
  strokeColor: '#EEEEEE',  // to see which ones work best for you
  generateGradient: true,
  highDpiSupport: true,     // High resolution support

};
var Liter_text = document.getElementById('AantalLiter'); // your canvas element

var target = document.getElementById('waterpijl'); // your canvas element
var gauge = new Donut(target).setOptions(wateropts); // create sexy gauge!
gauge.maxValue = 50; // set max gauge value
gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
gauge.animationSpeed = 63; // set animation speed (32 is default value)
gauge.set(30); // set actual value

Liter_text.textContent = gauge.value + " Liter";


// Besturen motor


// database




// import { createClient } from 'https://cdn.skypack.dev/@supabase/supabase-js';

// const supabaseUrl = 'https://ptxsuqkrwagqzbhxektl.supabase.co'
// const supabaseKey = "Turindwa552@" // This assumes that you're using a build tool that supports process.env, such as Node.js with a tool like dotenv.

// const supabase = createClient(supabaseUrl, supabaseKey);
