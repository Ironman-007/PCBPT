/*
    Copyright (c) 2022 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

import * as parser from "./parser.js";
import * as items from "./items.js";
import * as render from "./render.js";
import { $make, $draw, $on, $event, $q } from "./utils.js";

class KicadSchematicElement extends HTMLElement {
    constructor() {
        super();
        this.selected = [];
    }

    async connectedCallback() {
        await this.load();
        await document.fonts.ready;
        this.create_canvas();
        this.gather_selectable_items();
        this.draw();
        this.setup_events();
        this.select_from_location_hash();
    }

    async load() {
        const src = this.getAttribute("src");

        try {
            const sch_text = await (await window.fetch(src)).text();
            const sch_parsed = parser.parse(sch_text);
            this.sch = new items.KicadSch(sch_parsed);
            this.sch = labelWires(this.sch);
        } catch (e) {
            // console.log(e);
            throw new Error(`Couldn't load schematic ${src}`);
        }
    }

    create_canvas() {
        this.canvas = $make("canvas");
        this.canvas.width = this.offsetWidth;
        this.canvas.height = this.offsetHeight;
        // Remove any existing children.
        while (this.firstChild) {
            this.removeChild(this.firstChild);
        }
        this.prepend(this.canvas);
        this.renderer = new render.Renderer(this.canvas);

        const sch_bb = this.renderer.bbox(this.sch);
        sch_bb.grow(2);
        this.renderer.fit_to_bbox(sch_bb);

        this.setAttribute("loaded", true);
    }

    gather_selectable_items() {
        this.selectable_items = this.renderer.interactive_bboxes(this.sch);
        for (const bb of this.selectable_items) {
            bb.grow(1);
        }
    }

    setup_click_event() {
        $on(this.canvas, "mousedown", (e) => {
            const p = this.renderer.screen_space_to_world_space(
                e.clientX,
                e.clientY
            );

            this.select();
            // console.log(p.x, p.y);
            for (const b of this.selectable_items) {
                if (b.contains_point(p.x, p.y)) {
                    this.select([b]);
                    break;
                }
            }
        });
    }

    setup_events() {
        this.setup_click_event();

        $on(window, "popstate", (e) => {
            this.select_from_location_hash();
        });

        let selector1 = document.getElementById("net-selection-first-select")
        let selector2 = document.getElementById("net-selection-second-select")

        $on(selector1, "change", (e) => {
            // Set all wires labeled selector1.value to red, the rest to black and redraw
            this.setWiresStyle("black", 0.1, "all_wires_lbls");
            this.setWiresStyle("red", 1, selector1.value);
            this.setWiresStyle("red", 1, selector1.value.substring(1));
            this.setWiresStyle("blue", 1, selector2.value);
            this.setWiresStyle("blue", 1, selector2.value.substring(1));
            this.draw();
        });

        $on(selector2, "change", (e) => {
            // Set all wires labeled selector1.value to red, the rest to black and redraw
            this.setWiresStyle("black", 0.1, "all_wires_lbls");
            this.setWiresStyle("red", 1, selector1.value);
            this.setWiresStyle("red", 1, selector1.value.substring(1));
            this.setWiresStyle("blue", 1, selector2.value);
            this.setWiresStyle("blue", 1, selector2.value.substring(1));
            this.draw();
        });

        let schematicButton = document.getElementById("main-display-schematics-button");
        this.prev_window_width = window.innerWidth;
        this.prev_window_height = window.innerHeight;

        $on(schematicButton, "click", (e) => {
            setTimeout(() => {
                this.updateDisplay()
            }, 200)
        });

        selector1.dispatchEvent(new Event("change"));
        selector2.dispatchEvent(new Event("change"));


        setInterval(() => {
            if (window.innerWidth != this.prev_window_width || window.innerHeight != this.prev_window_height) {
                this.updateDisplay()
            }
        }, 200);
    }

    updateDisplay() {
        this.create_canvas();
        this.gather_selectable_items();
        this.draw();
        this.setup_click_event();

        this.prev_window_width = window.innerWidth;
        this.prev_window_height = window.innerHeight;
    }

    setWiresStyle(color, width, label) {
        let wires = this.sch.wires;
        let ids = Object.keys(wires);
        for (let i = 0; i < ids.length; i++) {
            let wire = wires[ids[i]];
            if (wire.label == label || "/" + wire.label == label || label == "all_wires_lbls") {
                wire.stroke.color = color;
                wire.stroke.width = width;
            }
            wires[ids[i]] = wire;
        }
        this.sch.wires = wires;
    }


    draw() {
        $draw(() => {
            this.renderer.clear();
            this.renderer.draw(this.sch);
            this.renderer.ctx.shadowColor = this.renderer.style.highlight;
            this.renderer.ctx.shadowBlur = 10;
            for (const selected of this.selected) {
                this.renderer.draw(selected.context);
            }
            this.renderer.ctx.shadowColor = "transparent";
        });
    }

    select(items = [], event = true) {
        this.selected = items;

        // if (this.hasAttribute("id") && this.selected.length == 1) {
        //     const ref = this.selected[0].context.properties.Reference.value;
        //     const url = new URL(window.location);
        //     url.hash = `#${this.getAttribute("id")}:${ref}`;
        //     window.history.pushState({}, "", url);
        // }


        let checkbox1 = document.getElementById("net-selection-first-mouse-select");
        let checkbox2 = document.getElementById("net-selection-second-mouse-select");

        let selector1 = document.getElementById("net-selection-first-select")
        let selector2 = document.getElementById("net-selection-second-select")

        if (event && this.selected.length == 1) {
            $event(this, "kicad-schematic:item-selected", this.selected[0].context);

            let label = this.selected[0].context.label;
            if (label != null) {

                if (checkbox1.checked && selector2.value != label) {
                    selector1.value = label;
                    selector1.dispatchEvent(new Event('change'));
                    checkbox1.checked = false;
                }
                if (checkbox2.checked && selector1.value != label) {
                    selector2.value = label;
                    selector2.dispatchEvent(new Event('change'));
                    checkbox2.checked = false;
                }
            }
        }

        this.draw();
    }

    select_all() {
        this.select(this.selectable_items);
    }

    select_by_reference(ref, event = true) {
        this.select();
        for (const i of this.selectable_items) {
            if (i.context?.properties?.Reference?.value == ref) {
                this.select([i], event);
                return;
            }
        }
    }

    select_from_location_hash() {
        const [id, ref] = window.location.hash.slice(1).split(":");
        if (id != this.getAttribute("id")) {
            return;
        }

        this.select_by_reference(ref, false);

        this.scrollIntoView({ block: "nearest" });
    }
}

window.customElements.define("kicad-schematic", KicadSchematicElement);

// Each wire has two points, and each label has a point. For each wire, find if
// any label has the same point. If so, label the wire with the label's text and
// check if the other end of the wire is connected to an unlabelled wire. If so,
// label that wire with the same text.
function labelWires(sch) {
    let wires = sch.wires; // Dict with id: wire
    const labels = sch.labels; // Dict with id: label
    let symbols = sch.symbols; // Dict with id: symbol
    // filter out symbols with lib_id not starting with "power"
    symbols = Object.keys(symbols).reduce((obj, key) => {
        if (symbols[key].lib_id.startsWith("power")) {
            obj[key] = symbols[key];
        }
        return obj;
    }, {});

    // Loop through all wires with id
    let i = 0
    let ids = Object.keys(wires);
    while (i < ids.length) {
        let id = ids[i];
        let wire = wires[id];
        if (wire.label != null) {
            i++;
            continue;
        }
        var label = findLabel(labels, symbols, wire.pts[0]);

        if (label != null) {
            wires = checkNextWire(wires, wire, label, 1);
        }

        label = findLabel(labels, symbols, wire.pts[1]);

        if (label != null) {
            wires = checkNextWire(wires, wire, label, 0);
        }
        i++;
    }

    sch.wires = wires;
    // console.log(sch);
    return sch;
}


function checkNextWire(wires, wire, label, position = 1) {

    // Find the next wire connected to the end of this wire
    for (const [id, other_wire] of Object.entries(wires)) {
        if (other_wire.pts[0].x == wire.pts[position].x && other_wire.pts[0].y == wire.pts[position].y &&
            id != wire.id) {
            let next_wire = other_wire;
            wires = checkNextWire(wires, next_wire, label, 1);
        }

        if (other_wire.pts[1].x == wire.pts[position].x && other_wire.pts[1].y == wire.pts[position].y &&
            id != wire.id) {
            let next_wire = other_wire;
            wires = checkNextWire(wires, next_wire, label, 0);
        }
    }

    // Label the next wire
    wire.label = label;
    wire.stroke.color = "green";
    wires[wire.id] = wire;

    return wires;
}

function findLabel(labels, symbols, point) {
    for (const [id, label] of Object.entries(labels)) {
        const label_point = label.at;

        if (point.x == label_point.x && point.y == label_point.y) {
            return label.name;
        }
    }

    for (const [id, symbol] of Object.entries(symbols)) {
        const symbol_point = symbol.at;

        if (point.x == symbol_point.x && point.y == symbol_point.y) {
            // Only the section after power: is needed
            return symbol.lib_id.split(":")[1];
        }
    }

    return null;
}

class KicadSchematicDialogElement extends HTMLElement {
    constructor() {
        super();
    }

    get dialog() {
        return $q(this, "dialog");
    }

    async connectedCallback() {
        this.render();
        $on(document, "kicad-schematic:item-selected", (e) => {
            this.on_item_selected(e.target, e.detail);
        });
    }

    on_item_selected(sch, detail) {
        // console.log(sch, detail);
        this.render_properties(detail.properties);
        this.dialog.showModal();
    }

    render() {
        const template = $make("template", {
            innerHTML: `
                <dialog>
                    <form method="dialog">
                        <div class="properties"></div>
                        <button>Close</button>
                    </form>
                </dialog>`,
        });

        this.append(template.content.cloneNode(true));
    }

    render_properties(properties) {
        const parent = $q(this, ".properties");
        parent.innerHTML = "";

        for (const [_, prop] of Object.entries(properties)) {
            const template = $make("template", {
                innerHTML: `
                    <div class="property">
                        <label for="${prop.key}">${prop.key}</label>
                        <input type="text" readonly id="${prop.key}" name="${prop.key}" value="${prop.value}" />
                    </div>`,
            });
            parent.append(template.content.cloneNode(true));
        }
    }
}

window.customElements.define(
    "kicad-schematic-dialog",
    KicadSchematicDialogElement
);
