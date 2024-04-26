const units = {
  Celcius: "°C",
  Fahrenheit: "°F"
};


const config = {
  minTemp: -20,
  maxTemp: 50,
  unit: "Celcius"
};
const configWater = {
  minliter: 0,
  maxliter: 50,
  unit: "liter"
};

// Change min and max temperature values

const tempValueInputs = document.querySelectorAll("input[type='text']");

tempValueInputs.forEach(input => {
  input.addEventListener("change", event => {
    const newValue = event.target.value;

    if (isNaN(newValue)) {
      return input.value = config[input.id];
    } else {
      config[input.id] = input.value;
      range[input.id.slice(0, 3)] = config[input.id]; // Update range
      return setTemperature(); // Update temperature
    }
  });
});

// Switch unit of temperature

const unitP = document.getElementById("unit");

// unitP.addEventListener("click", () => {
//   config.unit = config.unit === "Celcius" ? "Fahrenheit" : "Celcius";
//   unitP.innerHTML = config.unit + ' ' + units[config.unit];
//   return setTemperature();
// });

// Change temperature

const range = document.querySelector("input[type='range']");
const temperature = document.getElementById("temperature");
const graden = document.getElementById("degree");

function setTemperature() {
  temperature.style.height = (range.value - config.minTemp) / (config.maxTemp - config.minTemp) * 100 + "%";
  temperature.dataset.value = range.value + units[config.unit];

  graden.textContent = temperature.dataset.value;
}

range.addEventListener("input", setTemperature);
setTimeout(setTemperature, 1000);


//waterpijl

// const tank_slide = document.getElementById("watertank");
const tank_slide = document.querySelector("input[type='range']");

const liter = document.getElementById("liter");
const water = document.getElementById("water");

function setLiter() {
  liter.style.height = (tank_slide.value - configWater.minTemp) / (configWater.maxTemp - configWater.minTemp) * 100 + "%";
  liter.dataset.value = tank_slide.value + units[configWater.unit];



  graden.textContent = liter.dataset.value;
}

tank_slide.addEventListener("input", setLiter);
setTimeout(setLiter, 1000);







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
  colorStart: '#D4FF19',   // Colors
  colorStop: '#D4FF19',    // just experiment with them
  strokeColor: '#333333',  // to see which ones work best for you
  generateGradient: true,
  highDpiSupport: true,     // High resolution support
  
};

var target = document.getElementById('motor'); // your canvas element
var gauge = new Gauge(target).setOptions(opts); // create sexy gauge!
gauge.maxValue = 100; // set max gauge value
gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
gauge.animationSpeed = 70; // set animation speed (32 is default value)
gauge.set(70); // set actual value
