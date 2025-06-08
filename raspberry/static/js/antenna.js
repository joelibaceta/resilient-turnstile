import * as THREE from 'https://unpkg.com/three@0.112/build/three.module.js';

let accel = { x: 0, y: 0, z: 0 };
let gyro = { x: 0, y: 0, z: 0 };
let antennaFallTimeout = null;
let isAntennaDown = false;

let scene, camera, renderer, pivot;

function initScene() {
  const container = document.getElementById('three-container');
  const width = container.clientWidth;
  const height = container.clientHeight;

  scene = new THREE.Scene();
  scene.background = new THREE.Color(0x111111);

  camera = new THREE.PerspectiveCamera(60, width / height, 0.1, 100);
  camera.position.set(3, 6, 5);
  camera.lookAt(0, 0, 0);

  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(width, height);
  container.appendChild(renderer.domElement);

  // Luces y ejes
  const light = new THREE.DirectionalLight(0xffffff, 1);
  light.position.set(5, 5, 5);
  scene.add(light);
  scene.add(new THREE.AmbientLight(0x404040));
  scene.add(new THREE.AxesHelper(2));

  // Antena
  pivot = new THREE.Group();
  const antenna = createAntenna();
  antenna.position.y = 1.5; // Centrar base
  pivot.add(antenna);
  scene.add(pivot);

  animate();
}

function createAntenna() {
  const material = new THREE.MeshStandardMaterial({ color: 0x00ffff });

  const mast = new THREE.Mesh(
    new THREE.CylinderGeometry(0.05, 0.05, 3, 16),
    material
  );

  function createCross(yPosition) {
    const group = new THREE.Group();

    const bar1 = new THREE.Mesh(
      new THREE.CylinderGeometry(0.02, 0.02, 2, 8),
      material
    );
    bar1.rotation.z = Math.PI / 2;

    const bar2 = new THREE.Mesh(
      new THREE.CylinderGeometry(0.02, 0.02, 2, 8),
      material
    );
    bar2.rotation.x = Math.PI / 2;

    group.add(bar1, bar2);
    group.position.y = yPosition;
    return group;
  }

  const antenna = new THREE.Group();
  antenna.add(mast);
  antenna.add(createCross(1));
  antenna.add(createCross(-1));
  return antenna;
}

function initWebSocket() {
  const socket = io();

  socket.on('connect', () => {
    console.log('✅ Conectado al servidor WebSocket');
  });

  socket.on("event_data", handleEvent);
  socket.on("metric_data", handleMetric);
}

function handleEvent(eventStr) {
  if (!eventStr.includes("not_vertical")) return;

  if (!isAntennaDown) {
    updateStatusUI("⚠️ Antena caída", "red");
    isAntennaDown = true;
  }

  if (antennaFallTimeout) clearTimeout(antennaFallTimeout);
  antennaFallTimeout = setTimeout(() => {
    updateStatusUI("✅ Antena en posición vertical", "#00ffff");
    isAntennaDown = false;
    antennaFallTimeout = null;
  }, 3000);
}

function handleMetric(data) {
  const [_, subtype, ...vals] = data.split(",");
  const [x, y, z] = vals.map(parseFloat);

  switch (subtype) {
    case "ACEL":
      accel = { x, y, z };
      break;
    case "GYRO":
      gyro = { x, y, z };
      break;
    case "TEMP":
      document.getElementById("temp").textContent = `Temp: ${x.toFixed(1)} °C`;
      break;
    case "HUMIDITY":
      document.getElementById("hum").textContent = `Humedad: ${x.toFixed(1)} %`;
      break;
  }
}

function updateStatusUI(text, color) {
  document.getElementById("three-container").style.borderColor = color;
  document.getElementById("info-panel").style.borderColor = color;
  const status = document.getElementById("status");
  status.textContent = text;
  status.style.color = color;
}

function animate() {
  requestAnimationFrame(animate);

  const up = new THREE.Vector3(accel.x, accel.y, accel.z).normalize();
  const defaultUp = new THREE.Vector3(1, 0, 0); // eje natural
  const q = new THREE.Quaternion().setFromUnitVectors(defaultUp, up);

  pivot.setRotationFromQuaternion(q);
  renderer.render(scene, camera);
}

// Inicialización
initScene();
initWebSocket();