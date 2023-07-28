let serialDevices = {};

// function subSchematics(obj) {
//     var file = obj.value;
//     var fileName = file.split("\\");
//     document.getElementById("schematics-file-button").innerHTML = fileName[fileName.length - 1];
//     event.preventDefault();
// }

function subFile(obj, filename) {
    var file = obj.value;
    var fileName = file.split("\\");
    document.getElementById(filename + "-file-button").innerHTML = fileName[fileName.length - 1];
    event.preventDefault();
}

function subLayout(obj) {
    var file = obj.value;
    var fileName = file.split("\\");
    document.getElementById("layout-file-button").innerHTML = fileName[fileName.length - 1];
    event.preventDefault();
}

// Untoggle either first or second position
function untoggle(position){
    let toggle = document.getElementById("net-selection-" + position + "-mouse-select");
    if (toggle.checked) {
        toggle.checked = false;
    }
}

let platformBodyModel;
let platformClipModel;
let platformLockModel;
let boardEdges = [];

function preload() {

    platformBodyModel = loadModel("/static/models/platform_body.stl");
    platformClipModel = loadModel("/static/models/platform_clip.stl");
    platformLockModel = loadModel("/static/models/platform_lock.stl");

}

function setup() {
    let parent = document.getElementById("main-pcb-section");
    let width = parent.offsetWidth;
    let height = parent.offsetHeight;
    let canvas = createCanvas(width, height, WEBGL);
    canvas.parent("main-pcb-section");

    setTimeout(() => {
        windowResized();
    }, 200);

    // debugMode();
}

function windowResized() {
    let parent = document.getElementById("main-pcb-section");
    let width = parent.offsetWidth;
    let height = parent.offsetHeight;
    resizeCanvas(width, height);
}

function draw() {
    background(50);
    noStroke();
    // push();
    scale(5);
    scale(-1, 1, 1);
    rotateX(PI);
    rotateZ(PI);
    ambientLight(50);
    directionalLight(255, 255, 255, -1, 0, -1);
    // directionalLight(50, 50, 50, 1, 0, -1);
    // directionalLight(50, 50, 50, 0, 0, 1);


    // spotLight(255, 255, 255, 1000, -1000, 1000, -.6, .5, -1, Math.PI );
    translate(-172.5, 20, 100);
    // ambientMaterial(255, 50, 0);
    // textureMode(IMAGE);

    fill(50, 50, 50)
    model(platformBodyModel);

    translate(101.5, -68.5, 4);
    rotateX(PI);
    fill(255, 100, 0)
    model(platformClipModel);

    fill(100)
    translate(-13.5, -15, 0);
    model(platformLockModel);
    // pop();

    fill(0, 255, 0)

    translate(-45, -95, 10);
    beginShape();

    for (let i = 0; i < boardEdges.length; i++) {
        let x1 = boardEdges[i]["start"]["x"];
        let y1 = boardEdges[i]["start"]["y"];
        let x2 = boardEdges[i]["end"]["x"];
        let y2 = boardEdges[i]["end"]["y"];

        stroke(255, 0, 0);
        strokeWeight(5);
        vertex(x1, y1, 0);
    }
    endShape(CLOSE);


    orbitControl();
}

function switchToSchematics() {
    console.log("Switching to schematics");
    let pcbSection = document.getElementById("main-pcb-section");
    let schematicSection = document.getElementById("main-schematics-section");

    if(schematicSection.classList.contains("main-display-button-selected")) {
        return;
    }

    pcbSection.hidden = true;
    schematicSection.hidden = false;

    let pcbButton = document.getElementById("main-display-pcb-button");
    let schematicButton = document.getElementById("main-display-schematics-button");

    pcbButton.classList.remove("main-display-button-selected");
    schematicButton.classList.add("main-display-button-selected");

    setTimeout(() => {
        windowResized();
    }, 200);
}

function switchToPCB() {
    let pcbSection = document.getElementById("main-pcb-section");
    let schematicSection = document.getElementById("main-schematics-section");

    if(pcbSection.classList.contains("main-display-button-selected")) {
        return;
    }

    pcbSection.hidden = false;
    schematicSection.hidden = true;

    let pcbButton = document.getElementById("main-display-pcb-button");
    let schematicButton = document.getElementById("main-display-schematics-button");

    schematicButton.classList.remove("main-display-button-selected");
    pcbButton.classList.add("main-display-button-selected");

    setTimeout(() => {
        windowResized();
    }, 200);
}
