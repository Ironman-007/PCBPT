const socket = io.connect(location.origin, {
    path: "/socket.io",
    cors: {
        origin: "*",
        methods: ["GET", "POST"]
    },
    agent: false,
    upgrade: false,
    rejectUnauthorized: false,
    transports: ["websocket"],
});


socket.on("connect", () => {
    console.log("Connected to socket.io server");
    socket.emit("list_serial_devices");
    socket.emit("list_nets");
    socket.emit("get_schematics_path");
    socket.emit("list_edges");
});


socket.on("serial_devices", (devices) => {
    let select = document.getElementById("connection-port-select");
    let oldSelected = select.value;
    select.innerHTML = "";
    devices.forEach((device) => {
        let port = device.port;
        let connected = device.connected;
        let option = document.createElement("option");
        option.value = port;
        option.text = port;
        if (port == oldSelected) {
            option.selected = true;
        }
        select.appendChild(option);
    });
});



socket.on("devices_connection_status", (status) => {
    let select = document.getElementById("connection-port-select");
    let currentPort = select.value;
    let connectButton = document.getElementById("connection-button");
    let connectForm = document.getElementById("connection-section");
    if (status[currentPort]) {
        connectButton.innerHTML = "Disconnect";
        connectForm.action = "/serial/disconnect_device";
        select.classList.add("connected");
    }
    else {
        connectButton.innerHTML = "Connect";
        connectForm.action = "/serial/connect_device";
        select.classList.remove("connected");
    }
})


socket.on("nets", (nets) => {
    let firstNetSelect = document.getElementById("net-selection-first-select");
    let secondNetSelect = document.getElementById("net-selection-second-select");
    firstNetSelect.innerHTML = "";
    secondNetSelect.innerHTML = "";
    nets.forEach((net) => {
        var option = document.createElement("option");
        option.value = net;
        option.text = net;
        firstNetSelect.appendChild(option);
        option = document.createElement("option");
        option.value = net;
        option.text = net;
        secondNetSelect.appendChild(option);
    });

    if (nets.includes("VCC")) {
        firstNetSelect.value = "VCC";
    } else if (nets.includes("+3V3")) {
        firstNetSelect.value = "+3V3";
    } else if (nets.includes("+5V")) {
        firstNetSelect.value = "+5V";
    } else {
        firstNetSelect.value = nets[0];
    }

    if (nets.includes("GND")) {
        secondNetSelect.value = "GND";
    } else {
        secondNetSelect.value = nets[1];
    }
});


socket.on("net_coordinates", (result) => {
    let data = result["coordinates"]
    let nets = Object.keys(data);

    // console.log(result);
    if (nets == 0) {
        let statusP = document.getElementById("net-selection-status");
        statusP.innerHTML = "No candidates for net(s): " + nets.join(", ");
        statusP.classList.add("error");
        statusP.classList.remove("success");
        return;
    }

    else if (nets.length == 1) {
        let statusP = document.getElementById("net-selection-status");
        statusP.innerHTML = "Only one candidate for net: " + nets[0];
        statusP.classList.add("error");
        statusP.classList.remove("success");
        return;
    }

    let missingCoordinates = [];
    if (data[nets[0]].length == 0) {
        missingCoordinates.push(nets[0]);
    }
    if (data[nets[1]].length == 0) {
        missingCoordinates.push(nets[1]);
    }

    if (missingCoordinates.length > 0) {
        let statusP = document.getElementById("net-selection-status");
        statusP.innerHTML = "No candidates for net(s): " + missingCoordinates.join(", ");
        // console.log(nets);
        statusP.classList.add("error");
        statusP.classList.remove("success");
    } else {
        let statusP = document.getElementById("net-selection-status");
        statusP.innerHTML = "Candidates found for both nets";
        statusP.classList.remove("error");
        statusP.classList.add("success");
    }
});

socket.on("net_selection_error", (error) => {
    console.log(error);
    let statusP = document.getElementById("net-selection-status");
    statusP.innerHTML = error;
    statusP.classList.add("error");
    statusP.classList.remove("success");
});


socket.on("schematics_path", (schematics_path) => {
    let sch_path = schematics_path.split("/").slice(1).join("/");
    // console.log("Schematics file: " + sch_path);
    let parent = document.getElementById("main-schematics-section");
    parent.innerHTML = "";

    // Create kicad-schematic element
    let kicad_schematic = document.createElement("kicad-schematic");
    kicad_schematic.setAttribute("src", sch_path);
    kicad_schematic.id = "schematics";

    // Add kicad-schematic element to main-view
    parent.appendChild(kicad_schematic);
});


socket.on("command_response", (response) => {
    let type = response["type"];
    let data = response["data"];

    let responseDiv = document.getElementById("console-output");
    let timestamp = new Date().toLocaleTimeString();
    let responseP = document.createElement("p");
    if(type == "error") {
        responseP.classList.add("error");
    } else if (type == "received") {
        responseP.classList.add("accent");
    }
    responseP.innerHTML = timestamp + ": " + type.toUpperCase() + ": " + data;
    responseDiv.appendChild(responseP);
});


socket.on("device_connected", (isConnected) => {
    let connectButton = document.getElementById("connection-button");
    let connectForm = document.getElementById("connection-section");
    if (isConnected) {
        connectButton.innerHTML = "Disconnect";
        connectForm.action = "/serial/disconnect_device";
    } else {
        connectButton.innerHTML = "Connect";
        connectForm.action = "/serial/connect_device";
    }
});

socket.on("edges", (edges) => {
    boardEdges = edges;
});



setInterval(() => {
    socket.emit("list_serial_devices");
    socket.emit("get_devices_connection_status");
}, 1000);
