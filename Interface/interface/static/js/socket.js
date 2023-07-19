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

    firstNetSelect.dispatchEvent(new Event("change"));
    secondNetSelect.dispatchEvent(new Event("change"));
});


socket.on("net_coordinates", (result) => {
    let data = result["coordinates"]
    let nets = Object.keys(data);

    console.log(result);
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
        statusP.classList.add("error");
        statusP.classList.remove("success");
    } else {
        let statusP = document.getElementById("net-selection-status");
        statusP.innerHTML = "Candidates found for both nets";
        statusP.classList.remove("error");
        statusP.classList.add("success");
    }
});


socket.on("schematics_file", (schematics_path) => {
    let sch_path = schematics_path.split("/").slice(1).join("/");
    console.log("Schematics file: " + sch_path);
    let main_view = document.getElementById("main-view");
    main_view.innerHTML = "";
    // Create kicad-schematic element
    let kicad_schematic = document.createElement("kicad-schematic");
    kicad_schematic.src = sch_path;
    kicad_schematic.id = "schematics";
    // Add kicad-schematic element to main-view
    main_view.appendChild(kicad_schematic);
});


socket.on("command_response", (response) => {
    let responseDiv = document.getElementById("console-output");
    let timestamp = new Date().toLocaleTimeString();
    let responseP = document.createElement("p");
    responseP.innerHTML = timestamp + ": " + response;
    responseDiv.appendChild(responseP);
});


socket.on("device_connected", (isConnected) => {
    let connectButton = document.getElementById("connection-button");
    let connectForm = document.getElementById("connection-section");
    if (isConnected) {
        connectButton.innerHTML = "Disconnect";
        connectForm.action = "/api/disconnect_device";
    } else {
        connectButton.innerHTML = "Connect";
        connectForm.action = "/api/connect_device";
    }
});


setInterval(() => {
    socket.emit("list_serial_devices");
}, 1000);
