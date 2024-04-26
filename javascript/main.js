// //Import the THREE.js library
// import * as THREE from "https://cdn.skypack.dev/three@0.129.0/build/three.module.js";
// // To allow for the camera to move around the scene
// import { OrbitControls } from "https://cdn.skypack.dev/three@0.129.0/examples/jsm/controls/OrbitControls.js";
// // To allow for importing the .gltf file
// import { GLTFLoader } from "https://cdn.skypack.dev/three@0.129.0/examples/jsm/loaders/GLTFLoader.js";

// //Create a Three.JS Scene
// const scene = new THREE.Scene();
// //create a new camera with positions and angles
// const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

// //Keep track of the mouse position, so we can make the eye move
// let mouseX = window.innerWidth / 2;
// let mouseY = window.innerHeight / 2;

// //Keep the 3D object on a global variable so we can access it later
// let object;

// //OrbitControls allow the camera to move around the scene
// let controls;

// //Set which object to render
// let objToRender = 'Perseverance';

// //Instantiate a loader for the .gltf file
// const loader = new GLTFLoader();

// //Load the file
// loader.load(
//   `Model/${objToRender}.glb`,
//   function (gltf) {
//     //If the file is loaded, add it to the scene
//     object = gltf.scene;
//     scene.add(object);
//   },
//   function (xhr) {
//     //While it is loading, log the progress
//     console.log((xhr.loaded / xhr.total * 100) + '% loaded');
//   },
//   function (error) {
//     //If there is an error, log it
//     console.error(error);
//   }
// );

// //Instantiate a new renderer and set its size
// const renderer = new THREE.WebGLRenderer({ alpha: true }); //Alpha: true allows for the transparent background
// renderer.setSize(window.innerWidth, window.innerHeight);

// //Add the renderer to the DOM
// document.getElementById("container3D").appendChild(renderer.domElement);

// //Set how far the camera will be from the 3D model
// camera.position.z = objToRender === "dino" ? 25 : 500;

// //Add lights to the scene, so we can actually see the 3D model
// const topLight = new THREE.DirectionalLight(0xffffff, 1); // (color, intensity)
// topLight.position.set(500, 500, 500) //top-left-ish
// topLight.castShadow = true;
// scene.add(topLight);

// const ambientLight = new THREE.AmbientLight(0x333333, objToRender === "dino" ? 5 : 1);
// scene.add(ambientLight);

// //This adds controls to the camera, so we can rotate / zoom it with the mouse
// if (objToRender === "dino") {
//   controls = new OrbitControls(camera, renderer.domElement);
// }

// //Render the scene
// function animate() {
//   requestAnimationFrame(animate);
//   //Here we could add some code to update the scene, adding some automatic movement

//   //Make the eye move
//   if (object && objToRender === "eye") {
//     //I've played with the constants here until it looked good 
//     object.rotation.y = -3 + mouseX / window.innerWidth * 3;
//     object.rotation.x = -1.2 + mouseY * 2.5 / window.innerHeight;
//   }
//   renderer.render(scene, camera);
// }

// //Add a listener to the window, so we can resize the window and the camera
// window.addEventListener("resize", function () {
//   camera.aspect = window.innerWidth / window.innerHeight;
//   camera.updateProjectionMatrix();
//   renderer.setSize(window.innerWidth, window.innerHeight);
// });

// //add mouse position listener, so we can make the eye move
// document.onmousemove = (e) => {
//   mouseX = e.clientX;
//   mouseY = e.clientY;
// }

// //Start the 3D rendering
// animate();


var dom = document.getElementById('chart-container');
var myChart = echarts.init(dom, null, {
  renderer: 'canvas',
  useDirtyRect: false
});
var app = {};

var option;

option = {
  series: [
    {
      type: 'gauge',
      center: ['50%', '60%'],
      startAngle: 200,
      endAngle: -20,
      min: 0,
      max: 60,
      splitNumber: 12,
      itemStyle: {
        color: '#FFAB91'
      },
      progress: {
        show: true,
        width: 30
      },
      pointer: {
        show: false
      },
      axisLine: {
        lineStyle: {
          width: 30
        }
      },
      axisTick: {
        distance: -45,
        splitNumber: 5,
        lineStyle: {
          width: 2,
          color: '#999'
        }
      },
      splitLine: {
        distance: -52,
        length: 14,
        lineStyle: {
          width: 3,
          color: '#999'
        }
      },
      axisLabel: {
        distance: -20,
        color: '#999',
        fontSize: 20
      },
      anchor: {
        show: false
      },
      title: {
        show: false
      },
      detail: {
        valueAnimation: true,
        width: '60%',
        lineHeight: 40,
        borderRadius: 8,
        offsetCenter: [0, '-15%'],
        fontSize: 60,
        fontWeight: 'bolder',
        formatter: '{value} °C',
        color: 'auto'
      },
      data: [
        {
          value: 20
        }
      ]
    },
    {
      type: 'gauge',
      center: ['50%', '60%'],
      startAngle: 200,
      endAngle: -20,
      min: 0,
      max: 60,
      itemStyle: {
        color: '#FD7347'
      },
      progress: {
        show: true,
        width: 8
      },
      pointer: {
        show: false
      },
      axisLine: {
        show: false
      },
      axisTick: {
        show: false
      },
      splitLine: {
        show: false
      },
      axisLabel: {
        show: false
      },
      detail: {
        show: false
      },
      data: [
        {
          value: 20
        }
      ]
    }
  ]
};
setInterval(function () {
  const random = +(Math.random() * 60).toFixed(2);
  myChart.setOption({
    series: [
      {
        data: [
          {
            value: random
          }
        ]
      },
      {
        data: [
          {
            value: random
          }
        ]
      }
    ]
  });
}, 2000);

if (option && typeof option === 'object') {
  myChart.setOption(option);
}

window.addEventListener('resize', myChart.resize);