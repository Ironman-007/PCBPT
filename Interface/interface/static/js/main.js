let serialDevices = {};

function subSchematics(obj) {
    var file = obj.value;
    var fileName = file.split("\\");
    document.getElementById("schematics-file-button").innerHTML = fileName[fileName.length - 1];
    event.preventDefault();
}
function subLayout(obj) {
    var file = obj.value;
    var fileName = file.split("\\");
    document.getElementById("layout-file-button").innerHTML = fileName[fileName.length - 1];
    event.preventDefault();
}
