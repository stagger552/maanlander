

function LoadThema() {
    var background = localStorage.getItem('BackgroundColor');
    var seccondbackground = localStorage.getItem('SeccondBackgroundcolor');
    var TextColor = localStorage.getItem('TextColor');

    var root = document.querySelector(':root');
    if (savedColor) {
        root.style.setProperty('--BackgroundColor', background);
        root.style.setProperty('--SeccondBackgroundcolor', seccondbackground);
        root.style.setProperty('--TextColor', TextColor);

    }
}


function loadColor() {
    var savedColor = localStorage.getItem('primaryColor');
    var root = document.querySelector(':root');

    if (savedColor) {
        root.style.setProperty('--primeryColor', savedColor);
    }
}

document.addEventListener('DOMContentLoaded', loadColor);

document.addEventListener('DOMContentLoaded', LoadThema);


var r = document.querySelector(':root');

function ChangeThema(background, seccondbackground, TextColor) {
    // Set the value of variable --blue to another value (in this case "lightblue")
    r.style.setProperty('--BackgroundColor', background);
    r.style.setProperty('--SeccondBackgroundcolor', seccondbackground);
    r.style.setProperty('--TextColor', TextColor);


    // Get the styles (properties and values) for the root
    var rs = getComputedStyle(r);
    // Alert the value of the --blue variable
    console.log(rs.getPropertyValue('--BackgroundColor'))
    console.log(rs.getPropertyValue('--SeccondBackgroundcolor'))
    console.log(rs.getPropertyValue('--TextColor'))

    //local storage
    localStorage.setItem('BackgroundColor', background);  // Save the color to localStorage
    localStorage.setItem('SeccondBackgroundcolor', seccondbackground);  // Save the color to localStorage
    localStorage.setItem('TextColor', TextColor);  // Save the color to localStorage


}

function changeColor(color) {
    // Set the value of variable --blue to another value (in this case "lightblue")
    r.style.setProperty('--primeryColor', color);
    // Get the styles (properties and values) for the root
    var rs = getComputedStyle(r);
    // Alert the value of the --blue variable
    console.log(rs.getPropertyValue('--primeryColor'))
    //local storage
    localStorage.setItem('primaryColor', color);  // Save the color to localStorage

}
