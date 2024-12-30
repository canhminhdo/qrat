const { nqubits } = require("./global.js");

let init = () => {
    let prog = "\t// initialize superposition qubits\n";
    for (i = 0; i < nqubits; i++) {
        prog += "\tq" + i + " := H[q" + i + "];\n";
    }
    return prog;
}

module.exports = {init};