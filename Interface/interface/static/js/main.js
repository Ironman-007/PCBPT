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

function untoggleCandidate(position){
    let toggle = document.getElementById("candidates-selection-" + position + "-mouse-select");
    if (toggle.checked) {
        toggle.checked = false;
    }
}

let layoutEdges = [];
let layoutPads = [];
let layoutDimensions = [];
let padCandidates = {
    "left": [],
    "right": []
};
let boardRotation = 0;
let pcbDisplayMode = "2D";
let availableNets = [];


let Context2D = new p5(p => {
    let parent = document.getElementById("main-pcb-2D-section");
    let scale = 10;
    let downPressX = 0;
    let downPressY = 0;
    let centerX = 0;
    let centerY = 0;
    let downPress = false;

    p.setup = () => {
        p.width = parent.offsetWidth;
        p.height = parent.offsetHeight;
        p.canvas = p.createCanvas(p.width, p.height);
        p.canvas.parent(parent.id);

        centerX = p.width / 2 + 200;
        centerY = p.height / 2 + 100;

        p.angleMode(p.DEGREES);
        p.rectMode(p.CENTER);

        setTimeout(() => {
            p.windowResized();
        }, 200);
    }

    p.windowResized = () => {
        p.width = parent.offsetWidth;
        p.height = parent.offsetHeight;
        // centerX = p.width / 2 + 100;
        // centerY = p.height / 2 + 100;
        p.resizeCanvas(p.width, p.height);
    }

    p.hide = () => {
        p.canvas.hide();
    }

    p.show = () => {
        p.canvas.show();
    }

    p.draw = () => {
        p.background(50);

        p.translate(centerX - p.width/2, centerY - p.height/2);
        p.scale(scale);

        p.stroke(255, 100, 0);
        p.strokeWeight(1);
        p.fill(255, 100, 0);

        p.circle(0, 0, 2);
        p.line(0, 0, 10, 0);
        p.line(0, 0, 0, 10);


        p.stroke(255);
        p.strokeWeight(0.1);
        p.fill(0);

        if (boardRotation == 90) {
            p.rotate(90);
            p.translate(0, -layoutDimensions.height);
        } else if (boardRotation == 180) {
            p.rotate(180);
            p.translate(-layoutDimensions.width, -layoutDimensions.height);
        } else if (boardRotation == 270) {
            p.rotate(270);
            p.translate(-layoutDimensions.width, 0);
        }

        p.beginShape();

        for (let i = 0; i < layoutEdges.length; i++) {
            let x = layoutEdges[i]["start"]["x"];
            let y = layoutEdges[i]["start"]["y"];

            p.vertex(x, y, 0);
        }
        p.endShape(p.CLOSE);

        let selector1 = document.getElementById("net-selection-first-select")
        let selector2 = document.getElementById("net-selection-second-select")

        for (let i = 0; i < layoutPads.length; i++) {
            for (let j = 0; j < layoutPads[i].length; j++) {
                let id = layoutPads[i][j]["id"];
                let footprintAngle = layoutPads[i][j]["footprint_angle"];
                let footprintX = layoutPads[i][j]["footprint_x"];
                let footprintY = layoutPads[i][j]["footprint_y"];
                let padX = layoutPads[i][j]["pad_x"];
                let padY = layoutPads[i][j]["pad_y"];
                let net = layoutPads[i][j]["net"];
                let padAngle = layoutPads[i][j]["pad_angle"];
                let sizeX = layoutPads[i][j]["size_x"];
                let sizeY = layoutPads[i][j]["size_y"];
                let shape = layoutPads[i][j]["shape"];
                let roundrectRatio = layoutPads[i][j]["roundrect_ratio"];
                let isAllowed = layoutPads[i][j]["is_allowed"];
                let tags = layoutPads[i][j]["tags"];
                let relativeAngle = layoutPads[i][j]["relative_angle"];


                p.push();
                p.translate(footprintX, footprintY);
                p.rotate(-1*footprintAngle);
                p.fill(255);
                // p.circle(0, 0, 0.2);
                p.translate(padX, padY);
                // p.rotate(padAngle);

                if (selector1.value == net || "/" + selector1.value == net) {
                    p.stroke(255, 0, 0);
                    p.fill(255, 0, 0);
                } else if (selector2.value == net || "/" + selector2.value == net) {
                    p.stroke(0, 0, 255);
                    p.fill(0, 0, 255);
                } else {
                    p.stroke(255);
                    p.fill(255);
                }

                let isCandidate = false;

                if (padCandidates["left"].includes(id)) {
                    p.stroke(100, 200, 0);
                    p.strokeWeight(0.3);
                    isCandidate = true;
                } else if (padCandidates["right"].includes(id)) {
                    p.stroke(100, 200, 0);
                    p.strokeWeight(0.3);
                    isCandidate = true;
                }

                if (!isAllowed) {
                    p.stroke(200, 0, 100);
                    p.fill(200, 0, 100);
                }

                if (shape == "circle") {
                    p.circle(0, 0, sizeX);
                } else if (shape == "rect") {
                    p.rect(0, 0, sizeX, sizeY);
                } else if (shape == "roundrect") {
                    p.rect(0, 0, sizeX, sizeY, sizeX * roundrectRatio);
                } else {
                    p.fill(0, 255, 0);
                    p.stroke(0, 255, 0);
                    p.rect(0, 0, 1, 1);
                    console.log("Unknown pad shape: " + shape);
                }

                if (isCandidate) {
                    let x2 = Math.cos(relativeAngle) * sizeX / 3;
                    let y2 = Math.sin(relativeAngle) * sizeY / 3;

                    p.point(x2, y2);
                }
                // p.textSize(1);
                // p.textAlign(p.CENTER, p.CENTER);
                // p.text(tags, 0, 0);
                p.pop();
            }
        }
    }

    p.mousePressed = () => {
        // Return if the mouse is not pressed on the canvas
        if (p.mouseX < 0 || p.mouseX > p.width || p.mouseY < 0 || p.mouseY > p.height) {
            return;
        }

        downPressX = p.mouseX;
        downPressY = p.mouseY;
        downPress = true;
        // Check if the mouse was pressed on a pad
        for (let i = 0; i < layoutPads.length; i++) {
            for (let j = 0; j < layoutPads[i].length; j++) {
                let id = layoutPads[i][j]["id"];
                let footprintAngle = layoutPads[i][j]["footprint_angle"];
                let footprintX = layoutPads[i][j]["footprint_x"];
                let footprintY = layoutPads[i][j]["footprint_y"];
                let padX = layoutPads[i][j]["pad_x"];
                let padY = layoutPads[i][j]["pad_y"];
                let net = layoutPads[i][j]["net"];
                let padAngle = layoutPads[i][j]["pad_angle"];
                let sizeX = layoutPads[i][j]["size_x"] * scale;
                let sizeY = layoutPads[i][j]["size_y"] * scale;
                let shape = layoutPads[i][j]["shape"];
                let roundrectRatio = layoutPads[i][j]["roundrect_ratio"];
                let isAllowed = layoutPads[i][j]["is_allowed"];
                let tags = layoutPads[i][j]["tags"];

                let radianBoardRotation = boardRotation * p.PI / 180;
                let radianFootprintAngle = footprintAngle * p.PI / 180;
                let localPadX = footprintX + padX*Math.cos(-1*radianFootprintAngle) - padY*Math.sin(-1*radianFootprintAngle);
                let localPadY = footprintY + padX*Math.sin(-1*radianFootprintAngle) + padY*Math.cos(-1*radianFootprintAngle);

                let rotatedPadX = localPadX*Math.cos(radianBoardRotation) - localPadY*Math.sin(radianBoardRotation);
                let rotatedPadY = localPadX*Math.sin(radianBoardRotation) + localPadY*Math.cos(radianBoardRotation);

                let translatedPadX = (boardRotation == 90 ? layoutDimensions.height : 0) + (boardRotation == 180 ? layoutDimensions.width : 0) + rotatedPadX;
                let translatedPadY = (boardRotation == 180 ? layoutDimensions.height: 0) + (boardRotation == 270 ? layoutDimensions.width : 0) + rotatedPadY;

                let padCenterX = centerX - p.width/2 + scale * translatedPadX;
                let padCenterY = centerY - p.height/2 + scale * translatedPadY;

                if (i == 0) {
                    // console.log(p.dist(p.mouseX, p.mouseY, padCenterX, padCenterY) + " " + shape + " " + sizeX / 2 + " " + sizeY / 2);
                    // console.log(localPadX + " " + localPadY + " " + rotatedPadX + " " + rotatedPadY + " " + translatedPadX + " " + translatedPadY + " " + padCenterX + " " + padCenterY)
                }

                let selectedNet = ""

                rotatedSizeX = (boardRotation == 90 || boardRotation == 270) ? sizeY : sizeX;
                rotatedSizeY = (boardRotation == 0 || boardRotation == 180) ? sizeX : sizeY;

                sizeX = p.abs(rotatedSizeX * Math.cos(radianFootprintAngle) - rotatedSizeY * Math.sin(radianFootprintAngle));
                sizeY = p.abs(rotatedSizeX * Math.sin(radianFootprintAngle) + rotatedSizeY * Math.cos(radianFootprintAngle));

                if (shape == "circle") {
                    if (p.dist(p.mouseX, p.mouseY, padCenterX, padCenterY) < sizeX / 2) {
                        if (isAllowed) {
                            selectedNet = net;
                        }
                    }
                } else if (shape == "rect") {
                    if (p.abs(p.mouseX - padCenterX) < sizeX / 2 && p.abs(p.mouseY - padCenterY) < sizeY / 2) {
                        if (isAllowed) {
                            selectedNet = net;
                        }
                    }
                } else if (shape == "roundrect") {
                    if (p.abs(p.mouseX - padCenterX) < sizeX / 2 && p.abs(p.mouseY - padCenterY) < sizeY / 2) {
                        if (isAllowed) {
                            selectedNet = net;
                        }
                    }
                }

                if (selectedNet != "" && availableNets.includes(selectedNet)) {
                    console.log("Clicked on net: " + selectedNet);

                    let netCheckBox1 = document.getElementById("net-selection-first-mouse-select");
                    let netCheckBox2 = document.getElementById("net-selection-second-mouse-select");


                    let selector1 = document.getElementById("net-selection-first-select")
                    let selector2 = document.getElementById("net-selection-second-select")

                    let candidatesCheckBox1 = document.getElementById("candidates-selection-first-mouse-select");
                    let candidatesCheckBox2 = document.getElementById("candidates-selection-second-mouse-select");

                    let candidatesLabel1 = document.getElementById("candidates-selection-first-label");
                    let candidatesLabel2 = document.getElementById("candidates-selection-second-label");


                    if (netCheckBox1.checked && selector2.value != selectedNet) {
                        selector1.value = selectedNet;
                        selector1.dispatchEvent(new Event('change'));
                        netCheckBox1.checked = false;
                    }
                    if (netCheckBox2.checked && selector1.value != selectedNet) {
                        selector2.value = selectedNet;
                        selector2.dispatchEvent(new Event('change'));
                        netCheckBox2.checked = false;
                    }
                    if (candidatesCheckBox1.checked) {
                        padCandidates["left"][0] = id;
                        selector1.value = selectedNet;
                        selector1.dispatchEvent(new Event('change'));
                        candidatesCheckBox1.checked = false;
                        candidatesLabel1.innerHTML = "Probe 1: " + selectedNet;

                    }
                    if (candidatesCheckBox2.checked) {
                        padCandidates["right"][0] = id;
                        selector2.value = selectedNet;
                        selector2.dispatchEvent(new Event('change'));
                        candidatesCheckBox2.checked = false;
                        candidatesLabel2.innerHTML = "Probe 2: " + selectedNet;
                    }

                    return
                }
            }
        }
    }

    p.mouseWheel = (event) => {

        if (p.mouseX < 0 || p.mouseX > p.width || p.mouseY < 0 || p.mouseY > p.height) {
            return;
        }

        if (event.delta > 0) {
            if (scale < 20) {
                scale += 0.2;
            }
        } else {
            if (scale > 6) {
                scale -= 0.2;
            }
        }
    }

    p.mouseDragged = () => {

        if (p.mouseX < 0 || p.mouseX > p.width || p.mouseY < 0 || p.mouseY > p.height) {
            return;
        }

        if (downPress) {
            centerX += p.mouseX - downPressX;
            centerY += p.mouseY - downPressY;
            downPressX = p.mouseX;
            downPressY = p.mouseY;
        }
    }
});

let Context3D = new p5(p => {
    let platformBodyModel;
    let platformClipModel;
    let platformLockModel;
    let parent = document.getElementById("main-pcb-3D-section");

    p.preload = () => {
        platformBodyModel = p.loadModel("/static/models/platform_body.stl");
        platformClipModel = p.loadModel("/static/models/platform_clip.stl");
        platformLockModel = p.loadModel("/static/models/platform_lock.stl");
    }

    p.setup = () => {
        p.width = parent.offsetWidth;
        p.height = parent.offsetHeight;
        p.canvas = p.createCanvas(p.width, p.height, p.WEBGL);
        p.canvas.parent(parent.id);

        setTimeout(() => {
            p.windowResized();
        }, 200);
    }

    p.windowResized = () => {
        p.width = parent.offsetWidth;
        p.height = parent.offsetHeight;
        p.resizeCanvas(p.width, p.height);
    }

    p.hide = () => {
        p.canvas.hide();
    }

    p.show = () => {
        p.canvas.show();
    }

    p.draw = () => {
        p.background(50);
        p.noStroke();
        // push();
        p.scale(5);
        p.scale(-1, 1, 1);
        p.rotateX(p.PI);
        p.rotateZ(p.PI);
        p.ambientLight(50);
        p.directionalLight(255, 255, 255, -1, 0, -1);
        // directionalLight(50, 50, 50, 1, 0, -1);
        // directionalLight(50, 50, 50, 0, 0, 1);

        // spotLight(255, 255, 255, 1000, -1000, 1000, -.6, .5, -1, Math.PI );
        p.translate(-172.5, 20, 100);
        // ambientMaterial(255, 50, 0);
        // textureMode(IMAGE);

        p.fill(50, 50, 50)
        p.model(platformBodyModel);

        p.translate(101.5, -68.5, 4);
        p.rotateX(p.PI);
        p.fill(255, 100, 0)
        p.model(platformClipModel);

        p.fill(100)
        p.translate(-13.5, -15, 0);
        p.model(platformLockModel);
        // pop();

        p.fill(0, 255, 0)
        p.strokeWeight(5);
        p.stroke(0);

        p.translate(-45, -95, 10);

        p.beginShape();

        for (let i = 0; i < layoutEdges.length; i++) {
            let x = layoutEdges[i]["start"]["x"];
            let y = layoutEdges[i]["start"]["y"];

            p.vertex(x, y, 0);
        }
        p.endShape(p.CLOSE);

        p.orbitControl();
    }
});


function switchToSchematics() {
    let pcb2DSection = document.getElementById("main-pcb-2D-section");
    let pcb3DSection = document.getElementById("main-pcb-3D-section");
    let schematicSection = document.getElementById("main-schematics-section");

    if(schematicSection.classList.contains("main-display-button-selected")) {
        return;
    }

    pcb2DSection.hidden = true;
    pcb3DSection.hidden = true;
    schematicSection.hidden = false;

    let pcb2DButton = document.getElementById("main-display-pcb-2D-button");
    let pcb3DButton = document.getElementById("main-display-pcb-3D-button");
    let schematicButton = document.getElementById("main-display-schematics-button");

    pcb2DButton.classList.remove("main-display-button-selected");
    pcb3DButton.classList.remove("main-display-button-selected");
    schematicButton.classList.add("main-display-button-selected");

    setTimeout(() => {
        Context2D.windowResized();
        Context3D.windowResized();
    }, 200);
}

function switchToPCB(mode) {
    let selectedPcbSection = document.getElementById("main-pcb-" + mode + "-section");
    let otherPcbSection = document.getElementById("main-pcb-" + (mode === "3D" ? "2D" : "3D") + "-section");
    let schematicSection = document.getElementById("main-schematics-section");

    if(selectedPcbSection.classList.contains("main-display-button-selected")) {
        return;
    }

    otherPcbSection.hidden = true;
    schematicSection.hidden = true;
    selectedPcbSection.hidden = false;

    let pcbButton = document.getElementById("main-display-pcb-" + mode + "-button");
    let othermode = mode === "3D" ? "2D" : "3D";
    let otherButton = document.getElementById("main-display-pcb-" + othermode + "-button");
    let schematicButton = document.getElementById("main-display-schematics-button");

    schematicButton.classList.remove("main-display-button-selected");
    otherButton.classList.remove("main-display-button-selected");
    pcbButton.classList.add("main-display-button-selected");

    pcbDisplayMode = mode;

    setTimeout(() => {
        Context2D.windowResized();
        Context3D.windowResized();
    }, 200);
}
