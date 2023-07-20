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
